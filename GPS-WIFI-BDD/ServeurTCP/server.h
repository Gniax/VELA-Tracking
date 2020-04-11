#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>
#include "database.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void startServer();
    void setDatabase(Database* bdd);
    Database* getDatabase();
protected:
    void incomingConnection(qintptr socketDescriptor);

signals:

public slots:

private:
    Database* database;
};

#endif // MYSERVER_H
