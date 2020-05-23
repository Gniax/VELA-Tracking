#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <cstring>


class SQLManager : public QObject
{
    Q_OBJECT

public:
    explicit SQLManager(QObject *parent = 0);
    ~SQLManager();
    void startLocalDatabase(const char *host, const char *user, const char *password, const char *db);
    bool startDistantDatabase(const char *host, const char *user, const char *password, const char *db);
    void closeDatabase(std::string target);
    bool isLocalConnected();
    bool isDistantConnected();
    void execQuery(QString target, QString query);
    void execExportQuery(QString query);
    bool exportDatabaseLocalToDistant();
    void saveDataInformations(std::string ip, std::string longitude, std::string latitude, std::string vitesse, std::string timestamp);

//public slots:
  //  void slotExecQuery(QString target, QString query);

private:
    QSqlDatabase _sql_local;
    QSqlDatabase _sql_distant;
    bool _localConnected;
    bool _distantConnected;
};

#endif // SQLMANAGER_H
