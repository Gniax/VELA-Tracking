#include "coureur.h"

//Constructeur par défaut d'un coureur
Coureur::Coureur(QString name,int id,float longitude, float latitude , int vitesse)
{
    //Enregistre les informations du coureur
    Name = name;
    Identifiant = id;
    CoordLongitude = longitude;
    CoordLatitude = latitude;
    Vitesse = vitesse;

    //Ajout a la liste de point
    allCoordLongitude.append(longitude);
    allCoordLatitude.append(latitude);

    itemButton = new QPushButton(name);
    connect(itemButton, SIGNAL(clicked()), this, SLOT(itemButton_clicked()));

    //Gestion du timer pour connaitre la durée qui sépare le dernier point reçue
    _timerLastPoint = new QTimer(this);
    _timeLastPoint = 0;
    connect(_timerLastPoint , SIGNAL(timeout()) , this , SLOT(UpdateTimer()));
    _timerLastPoint->start(1000);
}

//Incrémente la durée qui sépare du dernier point reçue
void Coureur::UpdateTimer()
{
    _timeLastPoint++;
}

//Met à jour les informations du coureur
void Coureur::UpdateInformation(float longitude, float latitude , int vitesse)
{
    CoordLongitude = longitude;
    CoordLatitude = latitude;
    Vitesse = vitesse;
    allCoordLongitude.append(longitude); //Stock info pour l'affichage du tracer de la course
    allCoordLatitude.append(latitude);  //Stock info pour l'affichage du tracer de la course
    _timeLastPoint = 0;                 //Le temps qui sépare du dernier point revient a 0
}

//Evènement a l'appuie sur le bouton du coureur
void Coureur::itemButton_clicked()
{
    //Modifie le style du bouton pour la bordure blanche
    itemButton->setStyleSheet("background-color: #282828; color: white;border: 4px solid #FFFFFF;");

    //Emet un signal que le coureur a été sélectionner
    emit CoureurSelected(this);
}

//Désactive la séléction du coureur
void Coureur::DisableSelection()
{
   itemButton->setStyleSheet("background-color: #282828; color: white;border: 2px solid #000000;");
}

//Récupère identifiant du coureur
int Coureur::GetId()
{
    return Identifiant;
}

//Récupère le nom du coureur
QString Coureur::GetName()
{
    return Name;
}

//Récupère la dernière position longitude du coureur
float Coureur::GetLastLongitude()
{
    return CoordLongitude;
}

//Récupère la dernière position latitude du coureur
float Coureur::GetLastLatitude()
{
    return CoordLatitude;
}

//Récupère la dernière vitesse du coureur
int Coureur::GetLastVitesse()
{
    return Vitesse;
}

