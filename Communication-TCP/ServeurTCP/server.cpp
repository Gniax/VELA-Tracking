#include "server.h"
#include <QDebug>
#include <QHostAddress>
#include <QAbstractSocket>
#include <iostream>

#define MAX_CLIENTS 100

Server::Server() : _server(this)
{
    _server.listen(QHostAddress("127.0.0.1"), 4242);
    connect(&_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    qDebug() << "Server is ready ! :)";
}

Server::~Server() { }

void Server::onNewConnection()
{
   QTcpSocket *clientSocket = _server.nextPendingConnection();
   connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
   connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));

    _sockets.push_back(clientSocket);
    for (QTcpSocket* socket : _sockets) {
        socket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !"));
    }
}

void Server::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        _sockets.removeOne(sender);
    }
}

// Appelé dès que le serveur peut lire...
// redirige les messages aux autres senders
void Server::onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray datas = sender->readAll();

    for (QTcpSocket* socket : _sockets) {
        socket->peerAddress();
        if (socket != sender)
            socket->write(QByteArray::fromStdString(sender->peerAddress().toString().toStdString() + ": " + datas.toStdString()));
    }
}


