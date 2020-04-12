#include "sqlmanager.h"
#include <QtDebug>
#include <QtSql>
#include <iostream>
#include <cstring>

using namespace std;
SQLManager::SQLManager(QObject *parent) : QObject(parent)
{
    _localConnected = false;
    _distantConnected = false;
}

SQLManager::~SQLManager()
{
    closeDatabase("local");
    closeDatabase("distant");
}

// Démarre la connexion à la base de données local
void SQLManager::startLocalDatabase(const char *host, const char *user, const char *password, const char *db)
{
    // Instanciation de la variable membre sous QMYSQL
    // Si il manque le driver : faire ============>    $sudo apt-get install libqt5sql5-mysql
    _sql_local = QSqlDatabase::addDatabase("QMYSQL");

    // Définition des paramètres de connexion à la base de données
    _sql_local.setHostName(host); // @ip serveur MySQL
    _sql_local.setDatabaseName(db); // Nom de la base
    _sql_local.setUserName(user); // Nom utilisateur
    _sql_local.setPassword(password); // Mot de passe

    if (_sql_local.open())
    {
        qDebug() << "Connexion à la base de données" << db << "réussi !";
        _localConnected = true;
    }
    else
    {
        qDebug() << "Impossible de se connecter au serveur SQL local...";
        qDebug() << _sql_local.lastError();
        _localConnected = false;
    }
}

// Méthode qui permet l'insertion dans la base de données
void SQLManager::slotExecQuery(QString target, QString query)
{
    if (target == "local" && _localConnected)
    {
        QSqlDatabase defaultDB = QSqlDatabase::database();
        QSqlQuery requete;
        if(!requete.exec(query))
        {
            qDebug() << "Echec de la requête" << requete.lastError();
        }
    }
}

// Permet de fermer le sqlmanager (censée être appelée à la fermeture du programe...)
void SQLManager::closeDatabase(string target)
{
    if (target == "local" && _localConnected)
    {
        _sql_local.close();
        _localConnected = false;
    }
    else if (target == "distant" && _distantConnected)
    {
        _sql_distant.close();
        _distantConnected = false;
    }
}

bool SQLManager::isLocalConnected()
{
    return _localConnected;
}

bool SQLManager::isDistantConnected()
{
    return _distantConnected;
}
