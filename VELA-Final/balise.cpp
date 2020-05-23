#include "balise.h"

//Création de la balise avec les données reçues
Balise::Balise(QString name, int id, float longitude, float latitude)
{
    //Enregistre les informations de la balise
    Name = name;
    Identifiant = id;
    CoordLongitude = longitude;
    CoordLatitude = latitude;

    //Définie le bouton et le connect a sont évènement
    itemButton = new QPushButton(name);
    connect(itemButton, SIGNAL(clicked()), this, SLOT(itemButton_clicked()));
}

//Update information de la balise
void Balise::UpdateInformation(float longitude, float latitude)
{
    CoordLongitude = longitude;
    CoordLatitude = latitude;
}

//Evenement a l'appuie sur le bouton de la balise
void Balise::itemButton_clicked()
{
    //Modifie le style du bouton
    itemButton->setStyleSheet("background-color: #282828; color: white;border: 4px solid #FFFFFF;");

    //Emet un signal pour informer que la balise est sélectionner
     emit BaliseSelected(this);
}

//Désactive la séléction de la balise
void Balise::DisableSelection()
{
   itemButton->setStyleSheet("background-color: #282828; color: white;border: 2px solid #000000;");
}

//Récupère identifiant de la balise
int Balise::GetId()
{
    return Identifiant;
}

//Récupère le nom du coureur
QString Balise::GetName()
{
    return Name;
}

//Récupère la dernière position longitude de la balise
float Balise::GetLastLongitude()
{
    return CoordLongitude;
}

//Récupère la dernière position latitude de la balise
float Balise::GetLastLatitude()
{
    return CoordLatitude;
}

