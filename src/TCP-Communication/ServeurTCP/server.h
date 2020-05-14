#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor);

signals:

public slots:

private:

};

#endif // MYSERVER_H
