// Qt headers
#include <QCoreApplication>
#include <QThread>
#include <QHostAddress>
#include <QAbstractSocket>

// Own headers
#include <server.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server Serveur;
    Serveur.startServer();

    return a.exec();
}
