#include "serverwifi.h"
#include "clientthreadhandler.h"
#include <QString>

#define HOST_ADDRESS "192.168.0.254"
#define PORT 4242

// Constructor
ServerWifi::ServerWifi(QObject *parent) : QTcpServer(parent) { }

// Méthode qui démarre le serveur
void ServerWifi::startServer()
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
void ServerWifi::incomingConnection(qintptr socketDescriptor)
{
    ClientThreadHandler *thread = new ClientThreadHandler(socketDescriptor, this);

    // On connecte le signal et le slot
    // Lorsque le thread n'est plus utilisé, il sera supprimé
    // + possibilité d'executer une query depuis chaue client à la bdd
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(execQuery(QString, QString)), _sql, SLOT( slotExecQuery(QString, QString)));
    connect(thread, SIGNAL(sendItemInformations(string,string,string,string,string)), this, SLOT(ItemData(string,string,string,string,string)));
    // On démarre le thread, qui va appelé la fonction run de la classe ClientThreadHandler
    thread->start();
}

// Méthode qui implémente le pointeur vers la base de données
void ServerWifi::setSQL(SQLManager* sql)
{
    _sql = sql;
}

// Méthode qui retourne la base de données
SQLManager* ServerWifi::getSQL()
{
    return _sql;
}

// On attribue un thread à chaque nouveau client qui se connecte
void ServerWifi::ItemData(string ip, string longitude, string latitude, string vitesse, string timestamp)
{
    emit ReceptionInformations(ip,longitude,latitude,vitesse,timestamp);
}

