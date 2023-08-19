#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include "sqlite3.h"
#include <QObject>

class dataManager
        : QObject
{
    Q_OBJECT

    sqlite3 *db;
    char **output;
    char *error;
    int a , b;
    bool state;
    QString dbName;
public:
    dataManager(
            const QString& dbName = 0,
            QObject *parent = 0);
    bool openDatabase(const QString& db , bool create = true);
    bool checkDatabase(
            const QString& table = 0,
            const QString& condition = 0);
    bool closeDatabase();
    bool createTable(
            const QString& db = 0,
            const QString& columnDefs = 0,
            const QString& tableName = 0);
    bool insertData(
            const QString& table ,
            const QString& columns ,
            QStringList &data);
    QList<QStringList>* getData(
            const QString& table ,
            const QString& columns  ,
            const QString& condition
            );
    ~dataManager();
};

#endif // DATAMANAGER_H
