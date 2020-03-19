#include "client.h"
#include <QDebug>
#include <QHostAddress>

Client::Client() : _socket(this)
{
    _socket.connectToHost(QHostAddress("127.0.0.1"), 4242);
    connect(&_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

Client::~Client() { }

// Reçoit le msg du serveur puis envoie
void Client::onReadyRead()
{
    QByteArray datas = _socket.readAll();
    qDebug() << datas;
    _socket.write(QByteArray("ok !\n"));
}
