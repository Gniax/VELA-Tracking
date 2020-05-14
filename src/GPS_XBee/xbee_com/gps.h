#ifndef GPS_H
#define GPS_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <string>

class GPS
{
    Q_OBJECT

public:
    explicit GPS(QObject *parent = 0);
    ~GPS();
    void run();
    void setMode(std::string mode);
    std::string getLatitude();
    std::string getLongitude();
    std::string getDlatitude();
    std::string getDlongitude();
    std::string getVitesse();
    std::string getMode();
    time_t getTimestamp();

private:
    QMessageLogger logger;
    std::string _latitude;
    std::string _longitude;
    std::string _dLatitude;
    std::string _dLongitude;
    std::string _vitesse;
    std::string _mode;
    time_t _timestamp;
};

#endif // GPS_H
