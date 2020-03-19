// Qt headers
#include <QCoreApplication>
#include <QThread>

// Own headers
#include <client.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client* client = new Client();

    return a.exec();
}
