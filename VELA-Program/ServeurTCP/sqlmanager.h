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
    void startDistantDatabase(); // TO DO
    void closeDatabase(std::string target);
    bool isLocalConnected();
    bool isDistantConnected();

public slots:
    void slotExecQuery(QString target, QString query);

private:
    QSqlDatabase _sql_local;
    QSqlDatabase _sql_distant;
    bool _localConnected;
    bool _distantConnected;
};

#endif // SQLMANAGER_H
