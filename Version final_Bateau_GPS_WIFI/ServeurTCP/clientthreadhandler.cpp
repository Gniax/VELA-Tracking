#include "clientthreadhandler.h"
#include <QTcpSocket>
#include <QThread>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QTextStream>
#include <QFile>
#include <QDataStream>
#include <QEventLoop>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

// Functions prototypes
static inline QString toIPv4Address(QHostAddress source);
static inline map<string, string> string_to_map(string data);

ClientThreadHandler::ClientThreadHandler(qintptr ID, QObject* parent) : QThread(parent)
{
    this->socketDescriptor = ID;
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
    int enableKeepAlive = 1;
    int fd = socket->socketDescriptor();
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enableKeepAlive, sizeof(enableKeepAlive));

    int maxIdle = 10; // secondes
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &maxIdle, sizeof(maxIdle));

    int count = 3;  // envoie 3 keepalive paquets, puis déconnecte si aucune réponse
    setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &count, sizeof(count));

    int interval = 2;   // envoie un paquet keepalive chaque 2 secondes
    setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));

    // Indique au coureur qu'il est connecté
    socket->write("From server: connecté");
    socket->flush();
    socket->waitForBytesWritten();

    qDebug() << "Coureur" << toIPv4Address(socket->peerAddress()) << ':' << socket->peerPort() << "connecté !";

    connect(socket, SIGNAL(readyRead()), this, SLOT(incomingData()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));

    exec();
}

// Reçoit les données
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
    string dLatitude = gpsInfos["dLatitude"];
    string dLongitude = gpsInfos["dLongitude"];
    string timestamp = gpsInfos["Timestamp"];

    // Si le timestamp reçu est différent du précédent, c'est que les coordonnées ont bien été actualisées
    // Note : le précédent timestamp à été stocké au préalable dans la variable...
    if(_timestamp != timestamp)
    {
        _timestamp = timestamp;
        // Ici il faut gérer l'ajout des données dans la BDD
        // en attente que les aures finissent la partie BDD
        if(mode == "COUREUR")
        {
            printf("[DATA] Coureur %s => Vitesse: %s ; Latitude: %s ; Longitude: %s ; dLatitude: %s ; dLongitude: %s ; Timestamp: %s [FIN]",
                toIPv4Address(socket->peerAddress()).toStdString().c_str(), vitesse.c_str(), latitude.c_str(),
                longitude.c_str(), dLatitude.c_str(), dLongitude.c_str(), timestamp.c_str());
        }
        else if(mode == "BALISE")
        {
            printf("[DATA] Balise %s => Latitude: %s ; Longitude: %s ; dLatitude: %s ; dLongitude: %s ; Timestamp: %s [FIN]",
                toIPv4Address(socket->peerAddress()).toStdString().c_str(), latitude.c_str(),
                longitude.c_str(), dLatitude.c_str(), dLongitude.c_str(), timestamp.c_str());
        }
    }
}

// Ferme le thread lorsque le client est déconnecté
void ClientThreadHandler::connectionClosed()
{
    qDebug() << "Coureur déconnecté !";

    socket->deleteLater();
    exit(0);
}

// Permet de récupérer l'ipv4 depuis une QHostAddress
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
