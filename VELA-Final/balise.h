#ifndef BALISE_H
#define BALISE_H

#include <QString>
#include <QPushButton>
#include <QDebug>

class Balise : public QObject
{
    Q_OBJECT
public:
    Balise(QString name, int id, float longitude, float latitude);
    void UpdateInformation(float longitude, float latitude);
    void DisableSelection();
    int GetId();
    QString GetName();
    float GetLastLongitude();
    float GetLastLatitude();
    QPushButton* itemButton;

private:
    QString Name;
    int Identifiant;
    float CoordLongitude;
    float CoordLatitude;

signals:
    void BaliseSelected(Balise*);

private slots:
    void itemButton_clicked();

};

#endif // BALISE_H
