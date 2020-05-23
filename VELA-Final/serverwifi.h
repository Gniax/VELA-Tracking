#ifndef SERVERWIFI_H
#define SERVERWIFI_H
#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>
#include "sqlmanager.h"
#include "velawindow.h"

class VelaWindow;

class ServerWifi : public QTcpServer
{
    Q_OBJECT
public:
    ServerWifi(QObject *parent = 0);
    void startServer();
    void setSQL(SQLManager* sql);
    SQLManager* getSQL();

protected:
    void incomingConnection(qintptr socketDescriptor);

private slots:
    void ItemData(string ip, string longitude, string latitude, string vitesse, string timestamp);

signals:
     void ReceptionInformations(string,string,string,string,string);

private:
    SQLManager* _sql;
};

#endif // SERVERWIFI_H
