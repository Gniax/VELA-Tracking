#include "client.h"
#include <QDebug>
#include <QHostAddress>
#include <QTimer>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <QThread>
#include <QMessageLogger>
#include <QNetworkConfiguration>
#include <QNetworkProxy>
#include <string>
#include <sstream>

#define HOSTADDRESS "127.0.0.1"
#define PORT 4242
using namespace std;

static inline string map_to_string(map<string, string>  &m);

Client::Client() : _socket(this)
{
    // Ici, on initialise un compteur chaque 5 secondes qui va vérifier si les données du GPS ont changées
    // => si oui on les envoies au serveur si on est connecté
    // => sinon on les stockes en attendant d'être connecté puis les envoyer
    _databeatTimer.start(5000);
    _socket.connectToHost(QHostAddress(HOSTADDRESS), PORT);

    // On relie chaque signal à son slot:
    // => Un signal de timeout du compteur qui appelle l'envoie des données
    // => Trois signaux par rapport au socket : prêt à lire ; connecté ; déconnecté
    connect(&_databeatTimer, SIGNAL(timeout()), this, SLOT(sendData()));
    connect(&_socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(&_socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(&_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    // Permet de donner un timeout au socket afin de savoir si le socket est déconnecté en cas de coupure
    int enableKeepAlive = 1;
    int fd = _socket.socketDescriptor();
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enableKeepAlive, sizeof(enableKeepAlive));

    int maxIdle = 10; // secondes
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &maxIdle, sizeof(maxIdle));

    int count = 3;  // envoie 3 paquets keepalive, puis déconnecte si aucune réponse
    setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &count, sizeof(count));

    int interval = 2;   // envoie un paquet keepalive chaque 2 secondes
    setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
}

Client::~Client() { }

// Cette méthode permet de faire pointer l'instance de la classe gps afin d'y récupérer ses données
void Client::setGPS(GPS* gps)
{
    _gps = gps;
}

// Reçoit des messages du serveur (en l'occurence on n'en recevra qu'un à la connexion)
void Client::readMessage()
{
    QByteArray datas = _socket.readAll();
    logger.info(datas);
}

// Slot appelé lorsque le timeout se termine
// Si le socket est connecté, on vérifie que le gps est bien instancié
// puis si les données stockés (en cas de coupure) ne sont pas vides on les envoies
// sinon on envoie les données actuelles
// Sinon si le socket est déconnecté, on stocke les données dans un vector pour les envoyer par la suite
void Client::sendData()
{
    if(_socket.state() == QAbstractSocket::ConnectedState)
    {
        _databeatTimer.start(5000);
        //logger.info("Timeout socket connected");

        // Au cas ou on verifie que le gps est initialisé
        if(_gps != nullptr)
        {
            // Si on a perdu la connexion et qu'on a sauvegardé des données, on les envoies.. sinon on envoie normalement
            if(!_gps->disconnectedData.empty())
            {
                // On parcourt le vecteur et on envoie chaque map convertie en string
                for (auto it = begin (_gps->disconnectedData); it != end (_gps->disconnectedData); ++it) {
                    map<string, string> tmp = *it;
                    string tmp_gpsData = map_to_string(tmp);
                    _socket.write(tmp_gpsData.c_str());
                    _socket.flush();
                    _socket.waitForBytesWritten(1000);
                }
                // On vide les données après avoir tout envoyé
                _gps->disconnectedData.clear();
            }
            else // Si le vector est vide (données sous déconnexion sont vides)
            {
                string tempVit = _gps->getVitesse(),
                       tempLat = _gps->getLatitude() + _gps->getDlatitude(),
                       tempLong = _gps->getLongitude() + _gps->getDlongitude();
                time_t tempTimestamp =_gps->getTimestamp();

                if(tempVit != "\r" && tempLat != "\r" && tempLong != "\r" && tempTimestamp != 0)
                {
                    // Convertion du timestamp en string pour la map
                    stringstream ss;
                    ss << tempTimestamp;
                    string s_tempTimestamp = ss.str();

                    map<string, string> gpsData;

                    gpsData["Mode"] = _gps->getMode();
                    gpsData["Vitesse"] = tempVit;
                    gpsData["Latitude"] = tempLat;
                    gpsData["Longitude"] = tempLong;
                    gpsData["Timestamp"] = s_tempTimestamp;

                    string s_gpsData = map_to_string(gpsData);
                    //qDebug() << s_gpsData.c_str();

                    _socket.write(s_gpsData.c_str());
                    _socket.flush();
                    _socket.waitForBytesWritten(1000);
                }
            }
        }
    }
    else // Si on est déconnecté
    {
        //logger.info("Timeout socket disconnected");
        _databeatTimer.start(5000);

        // Au cas ou on vérifie que le gps n'est pas nulle
        if(_gps != nullptr)
        {
            string tempVit = _gps->getVitesse(),
                   tempLat = _gps->getLatitude() + _gps->getDlatitude(),
                   tempLong = _gps->getLongitude() + _gps->getDlongitude();
            time_t tempTimestamp =_gps->getTimestamp();

            if(tempVit != "\r" && tempLat != "\r" && tempLong != "\r" && tempTimestamp != 0)
            {
                // Convertion du timestamp en string pour la map
                stringstream ss;
                ss << tempTimestamp;
                string s_tempTimestamp = ss.str();

                map<string, string> gpsData;

                gpsData["Mode"] = _gps->getMode();
                gpsData["Vitesse"] = tempVit;
                gpsData["Latitude"] = tempLat;
                gpsData["Longitude"] = tempLong;
                gpsData["Timestamp"] = s_tempTimestamp;

                // Si les données déconnectés ne sont pas vides on ajoute les données à condition que c'est de nouvelle données
                // Le timestamp indique si ce sont des nouvelles données, donc on va pouvoir vérifier grâce à lui
                if(!_gps->disconnectedData.empty())
                {
                    bool resultIterate = false;
                    for (auto it = begin (_gps->disconnectedData); it != end (_gps->disconnectedData); ++it) {
                        map<string, string> tmp = *it;
                        if(tmp["Timestamp"] == s_tempTimestamp)
                            resultIterate = true;
                    }
                    if(!resultIterate)
                        _gps->disconnectedData.push_back(gpsData);
                }
                else
                {
                    // sinon la liste est vide et on ajoute pour la première fois
                    _gps->disconnectedData.push_back(gpsData);
                }
            }
        }
        _socket.connectToHost(QHostAddress(HOSTADDRESS), PORT);
    }
}

// Socket connecté
void Client::socketConnected ()
{
    _databeatTimer.start(5000);
    logger.info("Socket connecté");
}

// Socket déconnecté
void Client::socketDisconnected ()
{
    logger.info("Socket déconnecté");
    logger.info("En attente de reconnexion...");
}

// Erreur du socket
void Client::connectionError_Handler(QAbstractSocket::SocketError error)
{
    switch (error)
    {
    case QAbstractSocket::AddressInUseError:
        logger.warning("SOCKET ERROR: Address is already in use");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        logger.warning("SOCKET ERROR: Connection refused");
        break;
    case QAbstractSocket::HostNotFoundError:
        logger.warning("SOCKET ERROR: Host not found");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        logger.warning("SOCKET ERROR: Remote host closed");
        break;
    }

    _socket.abort();
    _socket.close();
    logger.info("Erreur socket : reconnexion en cours...");
    QThread::currentThread()->sleep(1000);
    _socket.connectToHost(QHostAddress(HOSTADDRESS), PORT);
}

// Fonction qui va nous permettre de convertir une map de string en string afin de l'envoyer au serveur
string map_to_string(map<string, string>  &m) {
string output = "";
string result = "";

    for (auto it = m.cbegin(); it != m.cend(); it++) {
        output += (it->first) + ":" + it->second + ", ";
    }

    result = output.substr(0, output.size() - 2 );

  return result;
}

