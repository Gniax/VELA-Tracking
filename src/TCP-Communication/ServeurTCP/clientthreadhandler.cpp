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

// Functions prototypes
static inline QString toIPv4Address(QHostAddress source);


ClientThreadHandler::ClientThreadHandler(qintptr ID, QObject* parent) : QThread(parent)
{
    this->socketDescriptor = ID;
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
    qDebug() << data;
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
