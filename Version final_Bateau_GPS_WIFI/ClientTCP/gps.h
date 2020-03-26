#ifndef GPS_H
#define GPS_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QThread>
#include <string>

class GPS : public QThread
{
    Q_OBJECT

public:
    explicit GPS(QObject *parent = 0);
    ~GPS();
    void run();
    std::string getLatitude();
    std::string getLongitude();
    std::string getDlatitude();
    std::string getDlongitude();
    std::string getVitesse();


private:
    QMessageLogger logger;
    std::string _latitude;
    std::string _longitude;
    std::string _dLatitude;
    std::string _dLongitude;
    std::string _vitesse;
};

#endif // GPS_H
