#ifndef CLIENTTHREADHANDLER_H
#define CLIENTTHREADHANDLER_H

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>

class ClientThreadHandler : public QThread
{
    Q_OBJECT

public:
    explicit ClientThreadHandler(qintptr ID, QObject *parent = 0);
    void run();

signals:
    void error(QAbstractSocket::SocketError error);

public slots:
    void connectionClosed();
    void incomingData();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QMessageLogger logger;
};

#endif // CLIENTTHREADHANDLER_H
