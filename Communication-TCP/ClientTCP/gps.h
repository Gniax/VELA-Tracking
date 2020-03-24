#ifndef GPS_H
#define GPS_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QThread>

class GPS : public QThread
{
    Q_OBJECT

public:
    explicit GPS(QObject *parent = 0);
    void run();
    char* latitude;
    char* longitude;
    char* dLatitude;
    char* dLongitude;
    char* vitesse;

private:
    QMessageLogger logger;
};

#endif // GPS_H
