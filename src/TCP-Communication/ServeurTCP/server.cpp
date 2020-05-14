#include "server.h"
#include "clientthreadhandler.h"

#define HOST_ADDRESS "127.0.0.1"
#define PORT 4242

Server::Server(QObject *parent) :
    QTcpServer(parent)
{

}

void Server::startServer()
{
    if(this->listen(QHostAddress::Any, PORT))
    {
        qDebug() << "Le serveur à démarré !";
    }
    else
    {
        qDebug() << "Le serveur n'a pas pu démarré ...";
    }
}

// On attribue un thread à chaque nouveau client
void Server::incomingConnection(qintptr socketDescriptor)
{
    ClientThreadHandler *thread = new ClientThreadHandler(socketDescriptor, this);

    // connect signal/slot
    // Lorsque le thread n'est plus utilisé, il sera supprimé
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
