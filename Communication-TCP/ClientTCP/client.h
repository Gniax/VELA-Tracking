#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class Client : public QTcpSocket
{
    Q_OBJECT

public:
    explicit Client();
    ~Client();

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
};

#endif // CLIENT_H
