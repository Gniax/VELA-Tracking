#include "client.h"
#include <QDebug>
#include <QHostAddress>
#include <QTimer>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <QThread>
#include <QMessageLogger>
#include <QNetworkConfiguration>
#include <QNetworkProxy>
#include <string>
#include <sstream>

#define HOSTADDRESS "127.0.0.1"
#define PORT 4242
using namespace std;

static inline std::string map_to_string(map<string, string>  &m);

Client::Client() : _socket(this)
{
    _databeatTimer.start(5000);
    _socket.connectToHost(QHostAddress(HOSTADDRESS), PORT);

    connect(&_databeatTimer, SIGNAL(timeout()), this, SLOT(sendData()));
    connect(&_socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(&_socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(&_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    // Permet de donner un timeout au socket afin de savoir si le socket est déconnecté en cas de coupure
    int enableKeepAlive = 1;
    int fd = _socket.socketDescriptor();
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enableKeepAlive, sizeof(enableKeepAlive));

    int maxIdle = 10; // secondes
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &maxIdle, sizeof(maxIdle));

    int count = 3;  // envoie 3 keepalive paquets, puis déconnecte si aucune réponse
    setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &count, sizeof(count));

    int interval = 2;   // envoie un paquet keepalive chaque 2 secondes
    setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
}

Client::~Client() { }

void Client::setGPS(GPS* gps)
{
    _gps = gps;
}

// Reçoit le message initial du serveur
void Client::readMessage()
{
    QByteArray datas = _socket.readAll();
    logger.info(datas);
}

void Client::sendData()
{
    if(_socket.state() == QAbstractSocket::ConnectedState)
    {
        _databeatTimer.start(5000);
        //logger.info("Timeout socket connected");
        if(_gps != nullptr)
        {
            string tempVit = _gps->getVitesse(),
                   tempLat = _gps->getLatitude(),
                   tempLong = _gps->getLongitude(),
                   tempDlat = _gps->getDlatitude(),
                   tempDlong = _gps->getDlongitude();
            time_t tempTimestamp =_gps->getTimestamp();
            // Here we can send GPS DATA
            if(tempVit != "\r" && tempLat != "\r" && tempLong != "\r" && tempDlat != "\r" && tempDlong != "\r" && tempTimestamp != 0)
            {
                // Convert timestamp to string
                std::stringstream ss;
                ss << tempTimestamp;
                std::string s_tempTimestamp = ss.str();

                std::map<string, string> gpsData;

                gpsData["Mode"] = _gps->getMode();
                gpsData["Vitesse"] = tempVit;
                gpsData["Latitude"] = tempLat;
                gpsData["Longitude"] = tempLong;
                gpsData["dLatitude"] = tempDlat;
                gpsData["dLongitude"] = tempDlong;
                gpsData["Timestamp"] = s_tempTimestamp;

                string s_gpsData = map_to_string(gpsData);
                //logger.info("infos avant envoie: ", s_gpsData.c_str());

                _socket.write(s_gpsData.c_str());
                _socket.flush();
                _socket.waitForBytesWritten(1000);
            }
        }
    }
    else
    {
        //logger.info("Timeout socket disconnected");
        _databeatTimer.start(3000);
        _socket.connectToHost(QHostAddress(HOSTADDRESS), PORT);
    }
}

void Client::socketConnected ()
{
    _databeatTimer.start(5000);
    logger.info("Socket connecté");
}

void Client::socketDisconnected ()
{
    logger.info("Socket déconnecté");
    logger.info("En attente de reconnexion...");
}

void Client::connectionError_Handler(QAbstractSocket::SocketError error)
{
    switch (error)
    {
    case QAbstractSocket::AddressInUseError:
        logger.warning("SOCKET ERROR: Address is already in use");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        logger.warning("SOCKET ERROR: Connection refused");
        break;
    case QAbstractSocket::HostNotFoundError:
        logger.warning("SOCKET ERROR: Host not found");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        logger.warning("SOCKET ERROR: Remote host closed");
        break;
    }

    _socket.abort();
    _socket.close();
    logger.info("Erreut socket : reconnexion en cours...");
    QThread::currentThread()->sleep(1000);
    _socket.connectToHost(QHostAddress(HOSTADDRESS), PORT);
}

std::string map_to_string(map<string, string>  &m) {
string output = "";
string result = "";

    for (auto it = m.cbegin(); it != m.cend(); it++) {
        output += (it->first) + ":" + it->second + ", ";
    }

    result = output.substr(0, output.size() - 2 );

  return result;
}

