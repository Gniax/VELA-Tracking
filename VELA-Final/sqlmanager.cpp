#include "sqlmanager.h"
#include <QtDebug>
#include <QtSql>
#include <iostream>
#include <cstring>

#define LOCAL_HOST 127.0.0.1
#define LOCAL_DB_NAME "bddmer"

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
    const QString localName = "LOCAL";
    _sql_local = QSqlDatabase::addDatabase("QMYSQL",localName);

    // Définition des paramètres de connexion à la base de données local
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

// Démarre la connexion à la base de données distante
bool SQLManager::startDistantDatabase(const char *host, const char *user, const char *password, const char *db)
{
    // Instanciation de la variable membre sous QMYSQL
    // Si il manque le driver : faire ============>    $sudo apt-get install libqt5sql5-mysql
    const QString distantName = "DISTANT";
    _sql_distant = QSqlDatabase::addDatabase("QMYSQL",distantName);

    // Définition des paramètres de connexion à la base de données distante
    _sql_distant.setHostName(host); // @ip serveur MySQL
    _sql_distant.setDatabaseName(db); // Nom de la base
    _sql_distant.setUserName(user); // Nom utilisateur
    _sql_distant.setPassword(password); // Mot de passe

    if(_distantConnected == true)
        return true;

    if (_sql_distant.open())
    {
        qDebug() << "Connexion à la base de données" << db << "réussi !";
        _distantConnected = true;
        return true;
    }
    else
    {
        qDebug() << "Impossible de se connecter au serveur SQL distant...";
        qDebug() << _sql_distant.lastError();
        _distantConnected = false;
        return false;
    }
}

// Méthode qui permet l'insertion dans la base de données
void SQLManager::execQuery(QString target, QString query)
{
    if (target == "local" && _localConnected)
    {
        //QSqlDatabase defaultDB = QSqlDatabase::database();
        QSqlQuery requete;
        requete = _sql_local.exec(query);
        if(!requete.isActive())
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
        _sql_local.removeDatabase("LOCAL");
        _localConnected = false;
    }
    else if (target == "distant" && _distantConnected)
    {
        _sql_distant.close();
        _sql_distant.removeDatabase("DISTANT");
        _distantConnected = false;
    }
}

// Permet de stocker les informations de la course
void SQLManager::saveDataInformations(string ip, string longitude, string latitude, string vitesse, string timestamp)
{
    if(_localConnected)
    {
        string query = "INSERT INTO course (participant, vitesse, latitude, longitude, temps) VALUES (\""+ip+"\", \""+vitesse+
                       "\", \""+latitude+"\", \""+longitude+"\", "+timestamp+");";

        execQuery("local", QString::fromStdString(query));
    }
}

// Permet de stocker les informations de la course
bool SQLManager::exportDatabaseLocalToDistant()
{
    if(!_localConnected)
        startLocalDatabase("localhost", "admin", "marius", "bddmer");

    if(_localConnected)
    {
        QList<QString> allRequest;
        QSqlQuery query;
        query = _sql_local.exec("SELECT course.participant, course.vitesse, course.latitude, course.longitude, course.temps FROM bddmer.course;");
        while (query.next()) {
            allRequest.append("INSERT INTO courses (participant, vitesse, latitude, longitude, temps) VALUES (\""+query.value("participant").toString()+"\", \""+query.value("vitesse").toString()+
                              "\", \""+query.value("latitude").toString()+"\", \""+query.value("longitude").toString()+"\", "+query.value("temps").toString()+");");
            //A voir -> enlever la liste en insérant directement les infos
        }

        //closeDatabase("local");

        if(!_distantConnected)
            startDistantDatabase("192.168.0.253", "comiter", "marius", "bddterre");

        if(_distantConnected)
        {
            for(int i = 0, c = allRequest.count(); i < c;i++)
            {
                execExportQuery(QString::fromStdString(allRequest[i].toStdString()));
            }

           closeDatabase("distant");
           // startLocalDatabase("localhost", "admin", "marius", "bddmer");

            string query2 ="DELETE FROM course;";
            execQuery("local",QString::fromStdString(query2));

            return true;
        }
    }

    return false;
}

// Méthode qui permet l'exportation de la base de données
void SQLManager::execExportQuery(QString query)
{
       // QSqlDatabase defaultDB = QSqlDatabase::database();
        QSqlQuery requete;
        requete = _sql_distant.exec(query);
        if(!requete.isActive())
        {
            qDebug() << "Echec de la requête" << requete.lastError();
        }
        requete.clear();
}

bool SQLManager::isLocalConnected()
{
    return _localConnected;
}

bool SQLManager::isDistantConnected()
{
    return _distantConnected;
}
