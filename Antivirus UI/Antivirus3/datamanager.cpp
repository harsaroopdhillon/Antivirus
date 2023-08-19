#include "datamanager.h"
#include <QDebug>
#include <QList>

/* IN THIS INTERFACE , RETURNING 1 REPRESENTS FUNCTION FAILURE
 * WHILE RETURNING 0 REPRESENTS FUNCTION SUCCESS
 * */

dataManager::dataManager(
        const QString& dbName,
        QObject *parent)
    : QObject(parent)
{
    this->dbName = dbName;
    state = sqlite3_initialize();
    if(state)
        qDebug() << "Database Init failed .... ";
}

bool dataManager :: openDatabase(const QString& dNam , bool create){
    if(create == true){
        state = sqlite3_open_v2(dNam.toStdString().c_str() ,
                                &db
                                , SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE
                                , NULL);
    }else{
        state = sqlite3_open_v2(dNam.toStdString().c_str() ,
                                &db
                                , SQLITE_OPEN_READWRITE , NULL);
    }
    if(state){
        qDebug() << "Database open failed .. ";
        return 1;
    }
    return 0;
}

bool dataManager :: closeDatabase()
{
    state = sqlite3_close(db);
    error = nullptr; a = b = 0; dbName.clear();
    return state;
}

bool dataManager :: createTable(const QString& dNam ,
                                const QString &columnDefs,
                                const QString& tableName)
{
    QString query = "CREATE TABLE " + tableName + "(" +
            columnDefs + ");";
    if(!dNam.isNull())
    {
        state = openDatabase(dNam);
        if(state)
        { qDebug() << "Database open failure"; return 1; }
    }
    state = sqlite3_exec(db , query.toStdString().c_str() , NULL , NULL , &error);
    if(state){
        qDebug() << "Unable to create table ....  sqlite_exec failure ....";
        qDebug() << error; error = nullptr;
        return 1;
    }else{
        qDebug() << "Table create successful .......";
        return 0;
    }
}

bool dataManager :: checkDatabase(const QString &table ,const QString &condition)
{
    if(table.isEmpty())
        return 1;
    QString query;
    if(condition.isEmpty())
        query = "SELECT * FROM " + table + ";";
    else
        query = "SELECT * FROM " + table + " WHERE " +
                condition + ";";
    state = sqlite3_get_table(db , query.toStdString().c_str()
                              , &output , &a , &b , &error);
    if(error || (a == 0 && b == 0)){
        sqlite3_free_table(output);
        error = nullptr; return 1; }
    sqlite3_free_table(output); error = nullptr;
    return 0;
}

dataManager::~dataManager()
{
    delete error;
}

QList<QStringList>* dataManager :: getData(const QString &table ,
                                  const QString &columns, const QString &condition)
{
    QList<QStringList> *result = new QList<QStringList>();
    if(table.isEmpty())
        return result;
    QString query;
    if(columns.isEmpty())
    {
       if(condition.isEmpty())
           query = "SELECT * FROM " + table + ";";
       else
           query = "SELECT * FROM " + table + " WHERE " +
                   condition + ";";
    }else{
        if(condition.isEmpty())
            query = "SELECT " + columns + " FROM " + table + ";";
        else
            query = "SELECT " + columns + " FROM " + table + " WHERE " +
                    condition + ";";
    }
    state = sqlite3_get_table(db , query.toStdString().c_str()
                              , &output , &a , &b , &error);
    if(error)
        qDebug() << error;
    if(a == 0 && b == 0)
        return result;
    int jj{0}; QStringList &rresult = QStringList();
    for(int i{0}; i <= a; i++){
        for(int j{0}; j < b; j++)
        {
            rresult.append(output[jj]);
            jj++;
        }
        result->append(rresult);
        rresult.clear();
    }
    //qDebug() << "a = " << a << " and b = " << b;
    //qDebug() << "length b4 going out " << result->length();
    //qDebug() << *result;
    return result;
}

bool dataManager :: insertData(const QString &table, const QString &columns,
                               QStringList &data)
{
    if(table.isEmpty() || data.isEmpty() || columns.isEmpty())
        return 1;
    QString query = "INSERT INTO " + table + "( " + columns + " ) VALUES( ";
    int sstate{0};
    for(int i{0}; i < data.length(); i++){
        if(i != data.length()-1){
          query.append(" '" + data.at(i) + "' , ");
        }
        else{
            query.append(" '" + data.at(i) + "' ");
        }
    }  query.append(" );");
    state = sqlite3_exec(db , query.toStdString().c_str() , NULL , NULL , &error);
    if(state || error){
        qDebug() << error; sstate = 1;
    } query.clear(); error = nullptr;
    if(sstate == 1)
        return 1;
    return 0;
}


