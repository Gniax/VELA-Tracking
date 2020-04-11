#include "server.h"
#include "clientthreadhandler.h"

#define HOST_ADDRESS "127.0.0.1"
#define PORT 4242

// Constructor
Server::Server(QObject *parent) : QTcpServer(parent) { }

// Méthode qui démarre le serveur
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

// On attribue un thread à chaque nouveau client qui se connecte
void Server::incomingConnection(qintptr socketDescriptor)
{
    ClientThreadHandler *thread = new ClientThreadHandler(socketDescriptor, this);

    // On connecte le signal et le slot
    // Lorsque le thread n'est plus utilisé, il sera supprimé
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // On démarre le thread, qui va appelé la fonction run de la classe ClientThreadHandler
    thread->start();
}

// Méthode qui implémente le pointeur vers la base de données
void Server::setDatabase(Database* db)
{
    database = db;
}

// Méthode qui retourne la base de données
Database* Server::getDatabase()
{
    return database;
}
