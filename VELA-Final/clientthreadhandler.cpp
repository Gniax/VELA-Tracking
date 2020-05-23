#include "clientthreadhandler.h"
#include <QTcpSocket>
#include <QThread>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QDataStream>
#include <QEventLoop>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netinet/tcp.h>
#include <string>
#include <iostream>
#include <QSqlDatabase>
#include <sstream>
#include <serverwifi.h>
#include <sqlmanager.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Functions prototypes
static inline QString toIPv4Address(QHostAddress source);
static inline map<string, string> string_to_map(string data);

// Constructor
ClientThreadHandler::ClientThreadHandler(qintptr ID, ServerWifi* parent) : QThread(parent)
{
    server = parent;
    socketDescriptor = ID;
    _timestamp = "\r";
}

void ClientThreadHandler::run()
{
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(this->socketDescriptor)) {
        qDebug() << "Impossible d'attribuer le descripteur socket";
        emit error(socket->error());
        return;
    }

    // Permet de donner un timeout au socket afin de savoir si le socket est déconnecté en cas de coupure
  /*  int enableKeepAlive = 1;
    int fd = socket->socketDescriptor();
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enableKeepAlive, sizeof(enableKeepAlive));

    int maxIdle = 10; // secondes
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &maxIdle, sizeof(maxIdle));

    int count = 3;  // envoie 3 paquets keepalive, puis déconnecte si aucune réponse
    setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &count, sizeof(count));

    int interval = 2;  // envoie un paquet keepalive chaque 2 secondes
    setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
*/
    // Indique au coureur qu'il est connecté => Seul message envoyé au client
    socket->write("From server: connecté");
    socket->flush();
    socket->waitForBytesWritten();

    // On affiche dans la sortie la connexion du coureur
    qDebug() << "Coureur" << toIPv4Address(socket->peerAddress()) << ':' << socket->peerPort() << "connecté !";

    // On relie deux signaux du socket:
        // prêt à lire avec les données reçu
        // déconnexion du socket
    connect(socket, SIGNAL(readyRead()), this, SLOT(incomingData()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));

    exec();
}

// À chaque données du client qui sont forcément des maps convertie en string, on effectue l'opération inverse
// On reconvertie le string en map afin de récupérer chaque valeur
// On pourra ensuite traiter et différencier les données
void ClientThreadHandler::incomingData()
{
    if(!socket)
         return;

    QByteArray data = socket->readAll();
    string sData = data.toStdString();
    map<string, string> gpsInfos = string_to_map(sData);

    string mode = gpsInfos["Mode"];
    string vitesse = gpsInfos["Vitesse"];
    string latitude = gpsInfos["Latitude"] ;
    string longitude = gpsInfos["Longitude"];
    string timestamp = gpsInfos["Timestamp"];
    string participant = toIPv4Address(socket->peerAddress()).toStdString().c_str();

    // Si le timestamp reçu est différent du précédent, c'est que les coordonnées ont bien été actualisées
    // Note : le précédent timestamp à été stocké au préalable dans la variable...
    if (_timestamp != timestamp)
    {
        _timestamp = timestamp;

        //Envoie des informations a l'IHM
        emit sendItemInformations(participant,longitude,latitude,vitesse,timestamp);

        SQLManager* sql = server->getSQL();



        // Ici il faut gérer le signal vers l'IHM
        if (mode == "COUREUR")
        {
            printf("[DATA] Coureur %s => Vitesse: %s ; Latitude: %s ; Longitude: %s ; Timestamp: %s [FIN]\n",
                toIPv4Address(socket->peerAddress()).toStdString().c_str(), vitesse.c_str(), latitude.c_str(),
                longitude.c_str(), timestamp.c_str());
            fflush(stdout);

        }
        else if (mode == "BALISE")
        {
            printf("[DATA] Balise %s => Latitude: %s ; Longitude: %s ; Timestamp: %s [FIN]\n",
                toIPv4Address(socket->peerAddress()).toStdString().c_str(), latitude.c_str(),
                longitude.c_str(), timestamp.c_str());
            fflush(stdout);
        }
    }
}

// Signal déconnecté : Ferme le thread lorsque le client est déconnecté
void ClientThreadHandler::connectionClosed()
{
    qDebug() << "Coureur déconnecté !";

    socket->deleteLater();
    exit(0);
}

// Fonction qui permet de récupérer l'ipv4 depuis une QHostAddress
QString toIPv4Address(QHostAddress qhostaddr)
{
    bool conversionOK = false;
    QHostAddress ip4Address(qhostaddr.toIPv4Address(&conversionOK));
    QString ip4String;
    if (conversionOK)
    {
        return ip4String = ip4Address.toString();
    }
    return "bad ip";
}

// Fonction qui permet la convertion du string en map
map<string, string> string_to_map(string input)
{
    istringstream iss(input.c_str());
    map<string, string> flags;
    string token;
    bool first = true;
    while (getline(iss, token, ',')) {
        size_t pos = token.find(':');
        if(first == true)
            flags[token.substr(0, pos)] = token.substr(pos + 1);
        else
            flags[token.substr(1, pos-1)] = token.substr(pos + 1);

        first = false;
    }
    return flags;
}
