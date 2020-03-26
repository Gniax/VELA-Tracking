// Qt headers
#include <QCoreApplication>
#include <QThread>
#include <QObject>
// Own headers
#include <client.h>
#include <gps.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GPS* gps = new GPS();
    QObject::connect(gps, SIGNAL(finished()), gps, SLOT(deleteLater()));
    gps->start();
    gps->setMode("BALISE"); // COUREUR OU BALISE

    Client* client = new Client();
    client->setGPS(gps);
    return a.exec();
}
