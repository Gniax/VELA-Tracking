#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>
#include "sqlmanager.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void startServer();
    void setSQL(SQLManager* sql);
    SQLManager* getSQL();
protected:
    void incomingConnection(qintptr socketDescriptor);

signals:

public slots:

private:
    SQLManager* _sql;
};

#endif // MYSERVER_H
