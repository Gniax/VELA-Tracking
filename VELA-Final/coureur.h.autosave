#ifndef COUREUR_H
#define COUREUR_H

#include <QString>
#include <QPushButton>
#include <QDebug>
#include <QList>
#include <velawindow.h>


class Coureur : public QObject
{
    Q_OBJECT
public:
    Coureur(QString name,int id,float longitude, float latitude , int vitesse);
    void UpdateInformation(float longitude, float latitude , int vitesse);
    int GetId();
    QString GetName();
    float GetLastLongitude();
    float GetLastLatitude();
    int GetLastVitesse();
    void DisableSelection();
    QPushButton* itemButton;
    QList<float> allCoordLongitude;
    QList<float> allCoordLatitude;
    int _timeLastPoint;
    QTimer* _timerLastPoint;

private:
    QString Name;
    int Identifiant;
    float CoordLongitude;
    float CoordLatitude;
    int Vitesse;

signals:
    void CoureurSelected(Coureur*);

private slots:
    void itemButton_clicked();
    void UpdateTimer();

};

#endif // COUREUR_H
