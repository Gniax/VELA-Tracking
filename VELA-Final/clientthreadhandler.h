#ifndef CLIENTTHREADHANDLER_H
#define CLIENTTHREADHANDLER_H

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include <serverwifi.h>

class ClientThreadHandler : public QThread
{
    Q_OBJECT

public:
    explicit ClientThreadHandler(qintptr ID, ServerWifi* parent = 0);
    void run();

signals:
    void execQuery(QString target, QString query);
    void error(QAbstractSocket::SocketError error);
    void sendToDB(string target, string table, string participant, string vitesse, string latitude, string longitude, string timestamp);
    void sendItemInformations(string,string,string,string,string);

public slots:
    void connectionClosed();
    void incomingData();

private:
    ServerWifi* server;
    SQLManager* sql;
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QMessageLogger logger;
    string _timestamp;
};

#endif // CLIENTTHREADHANDLER_H
