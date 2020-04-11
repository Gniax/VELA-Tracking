#include "database.h"
#include <QtDebug>
#include <QtSql>
#include <iostream>
#include <cstring>

using namespace std;
Database::Database()
{
    localConnected = false;
    distantConnected = false;
}

Database::~Database(){}

// Démarre la connexion à la base de données local
void Database::startLocal(const char *host, const char *user, const char *password, const char *db)
{
    // Instanciation de la variable membre sous SQLITE
    sql_local = QSqlDatabase::addDatabase("QSQLITE");

    // Définition des paramètres de connexion à la base de données
    sql_local.setHostName(host); // @ip serveur MySQL
    sql_local.setDatabaseName(db); // Nom de la base
    sql_local.setUserName(user); // Nom utilisateur
    sql_local.setPassword(password); // Mot de passe

    if (sql_distant.open())
    {
        qDebug() << "Connexion à la base de données " << db << "réussi !";
        localConnected = true;
    }
    else
    {
        qDebug() << "Impossible de se connecter au serveur SQL local...";
        localConnected = false;
    }
}

// Méthode qui permet l'insertion dans la base de données voulue
void Database::insertIntoDB(string target, string table, string participant, string vitesse, string latitude, string longitude, string timestamp)
{
    if (target == "local" && localConnected)
    {
        QSqlQuery requete;
        string query = "INSERT INTO "+table+" (participant, vitesse, latitude, longitude, temps) VALUES (\""+participant+"\", \""+vitesse+"\", \""+latitude+"\", \""+longitude+"\", "+timestamp+");";
        if(!requete.exec(query.c_str()))
        {
            qDebug() << "Echec de la requête" << requete.lastError();
        }
    }
}

// Permet de fermer la base de données (censée être appelée à la fermeture du programe...)
void Database::closeDatabase(string target)
{
    if (target == "local" && localConnected)
    {
        sql_local.close();
        localConnected = false;
    }
    else if (target == "distant" && distantConnected)
    {
        sql_distant.close();
        distantConnected = false;
    }
}
