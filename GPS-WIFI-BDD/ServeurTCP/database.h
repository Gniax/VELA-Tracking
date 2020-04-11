#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <cstring>
class Database
{

public:
    explicit Database();
    ~Database();
    void startLocal(const char *host, const char *user, const char *password, const char *db);
    void startDistant(); // TO DO
    void closeDatabase(std::string target);
    void insertIntoDB(std::string target, std::string table, std::string mode, std::string vitesse, std::string latitude, std::string longitude, std::string timestamp);
    bool localConnected;
    bool distantConnected;

private:
    QSqlDatabase sql_local;
    QSqlDatabase sql_distant;
};

#endif // DATABASE_H
