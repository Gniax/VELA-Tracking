#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QTcpSocket
{
    Q_OBJECT

public:
    explicit Client();
    ~Client();

public slots:
    void onReadyRead();

private:
    QTcpSocket  _socket;
};

#endif // CLIENT_H
