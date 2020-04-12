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
    void execQuery(QString target, QString query);
    void error(QAbstractSocket::SocketError error);
    void sendToDB(std::string target, std::string table, std::string participant, std::string vitesse, std::string latitude, std::string longitude, std::string timestamp);

public slots:
    void connectionClosed();
    void incomingData();

private:
    Server* server;
    SQLManager* sql;
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QMessageLogger logger;
    std::string _timestamp;
};

#endif // CLIENTTHREADHANDLER_H
