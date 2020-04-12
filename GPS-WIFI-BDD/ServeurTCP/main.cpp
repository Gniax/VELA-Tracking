// Qt headers
#include <QCoreApplication>
#include <QThread>
#include <QHostAddress>
#include <QAbstractSocket>

// Own headers
#include <server.h>
#include <sqlmanager.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SQLManager* sql = new SQLManager(0);
    sql->startLocalDatabase("localhost", "root", "pi", "bdd_mer");

    Server Serveur;
    Serveur.startServer();

    Serveur.setSQL(sql);

    return a.exec();
}
