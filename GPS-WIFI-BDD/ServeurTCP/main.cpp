// Qt headers
#include <QCoreApplication>
#include <QThread>
#include <QHostAddress>
#include <QAbstractSocket>

// Own headers
#include <server.h>
#include <database.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server Serveur;
    Serveur.startServer();

    Database* bddLocal = new Database();
    bddLocal->startLocal("127.0.0.1", "root", "pi", "bdd_mer");

    Serveur.setDatabase(bddLocal);

    return a.exec();
}
