#ifndef CLIENTTHREADHANDLER_H
#define CLIENTTHREADHANDLER_H

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include <server.h>

class ClientThreadHandler : public QThread
{
    Q_OBJECT

public:
    explicit ClientThreadHandler(qintptr ID, Server* parent = 0);
    void run();

signals:
    void error(QAbstractSocket::SocketError error);

public slots:
    void connectionClosed();
    void incomingData();

private:
    Server* server;
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QMessageLogger logger;
    std::string _timestamp;
};

#endif // CLIENTTHREADHANDLER_H
