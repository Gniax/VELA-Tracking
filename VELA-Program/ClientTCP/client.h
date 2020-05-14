#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "gps.h"

class Client : public QTcpSocket
{
    Q_OBJECT

public:
    explicit Client();
    ~Client();
    void setGPS(GPS* gps);

public slots:
    void readMessage();
    void socketDisconnected();
    void socketConnected();
    void sendData();
    void connectionError_Handler(QAbstractSocket::SocketError error);

private:
    QTcpSocket  _socket;
    QTimer _databeatTimer;
    QMessageLogger logger;
    GPS* _gps;
};

#endif // CLIENT_H
