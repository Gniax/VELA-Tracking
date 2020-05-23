#ifndef VELAWINDOW_H
#define VELAWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QMainWindow>
#include <QPushButton>
#include <QList>
#include <QMessageBox>
#include <QScroller>
#include <QListWidgetItem>
#include <string>
#include<iostream>
#include <QTimer>


#include <coureur.h>
#include <balise.h>
#include <optionwindow.h>
#include <graphicmanager.h>
#include <testecourse.h>
#include <serverwifi.h>
#include <sqlmanager.h>

using namespace std;
Q_DECLARE_METATYPE(string)

namespace Ui {
class VelaWindow;
}

class OptionWindow;
class GraphicManager;
class ServerWifi;

class VelaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VelaWindow(QWidget *parent = 0);
    void ResetCourse();
    bool ExportBDD();

    ~VelaWindow();

private:
    Ui::VelaWindow *ui;
    GraphicManager* graphicMng; //Classe qui gère l'affiche graphique
    ServerWifi* serveurWifi;    //Serveur qui gère la communication avec les coureurs et balise
    SQLManager* sqlManager;     //Classe qui gère les bases de données
    QFont itemTexteStyle;
    QList<QListWidgetItem*> allWidgetlist;
    QList<Coureur*> lesCoureurs; //Liste des coureurs
    QList<Balise*> lesBalises; //Liste des balises
    OptionWindow* option_win;     // Fenetre des paramètres
    int CurrentViewItemId;
    int TranslateIpToNumber(QString ip); //Translation de l'ip en identifiant

public slots:
    void SelecteCoureurView(Coureur* selectedcoureur);
    void SelecteBaliseView(Balise* selectedbalise);
    void ServeurInformation(string ip, string longitude, string latitude, string timestamp, string vitesse);

private slots:
    void optionButton_Clicked(); //bouton acces aux options


};

#endif // VELAWINDOW_H
