#include "DataManager.h"
#include <QVariant>
#include <QDebug>

#define CREATE_BIM "CREATE TABLE bim_info (\
                        id  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
                        num  INTEGER,\
                        name  TEXT,\
                        time  TEXT,\
                        url  TEXT,\
                        square  REAL,\
                        position  TEXT,\
                        struct  TEXT,\
                        bim  TEXT,\
                        type  TEXT,\
                        height  TEXT,\
                        nature  TEXT,\
                        key  TEXT,\
                        key_weight  TEXT);"


DataManager* DataManager::GetInstance(){
    static DataManager* instance;
    if (instance == nullptr)
        instance = new DataManager();
    return instance;
}
DataManager::DataManager(){
    InitDataBase();
}
DataManager::~DataManager(){

}
int DataManager::ConnectDataBase(){
    if (QSqlDatabase::contains(DATABASE_ALIAS))
    {
        dataBase = QSqlDatabase::database(DATABASE_ALIAS);
    }
    else
    {
        dataBase = QSqlDatabase::addDatabase("QSQLITE", DATABASE_ALIAS);
        dataBase.setDatabaseName(DATABASE_NAME);
        dataBase.setUserName(DATABASE_USER);
        dataBase.setPassword(DATABASE_PASS);
    }
    if (!dataBase.open(DATABASE_USER, DATABASE_PASS)){
        auto a = dataBase.lastError().text();
        return SQL_OPEN_ERROR;
    }
    QSqlQuery sql_query(dataBase);
    sql_query.prepare("PRAGMA foreign_keys=ON");
    if (!sql_query.exec()) {
        auto a = sql_query.lastError().text();
        return SQL_EXEC_ERROR;
    }
    return SQL_OK;
}
int DataManager::InitDataBase(){
    if (ConnectDataBase() != SQL_OK) return SQL_OPEN_ERROR;
    QSqlQuery sql_query(dataBase);
    sql_query.prepare("PRAGMA foreign_keys = ON;");
    if (!sql_query.exec()) {
        auto a = sql_query.lastError().text();
        dataBase.close();
        return SQL_EXEC_ERROR;
    }
    //sql_query.clear();
    sql_query.prepare(CREATE_BIM);
    if (!sql_query.exec()) {
        auto a = sql_query.lastError().text();
        dataBase.close();
        return SQL_EXEC_ERROR;
    }

    dataBase.close();
    return SQL_OK;
}

//table shop 增删改查
int DataManager::AddBim(BimInfo& bim_info){
    if (ConnectDataBase() != SQL_OK) return SQL_OPEN_ERROR;
    QSqlQuery sql_query(dataBase);
    sql_query.prepare("insert into bim_info(num,name,time,url,square,position,struct,bim,type,height,nature,key,key_weight) values(?,?,?,?,?,?,?,?,?,?,?,?,?)");
    sql_query.addBindValue(bim_info.num);
    sql_query.addBindValue(bim_info.name);
    sql_query.addBindValue(bim_info.time);
    sql_query.addBindValue(bim_info.url);
    sql_query.addBindValue(bim_info.square);
    sql_query.addBindValue(bim_info.position);
    sql_query.addBindValue(bim_info.build_struct);
    sql_query.addBindValue(bim_info.bim);
    sql_query.addBindValue(bim_info.type);
    sql_query.addBindValue(bim_info.height);
    sql_query.addBindValue(bim_info.nature);
    sql_query.addBindValue(bim_info.key);
    sql_query.addBindValue(bim_info.key_weight);
    if (!sql_query.exec()) {
        auto a = sql_query.lastError().text();
        dataBase.close();
        qDebug() << a;
        return SQL_EXEC_ERROR;
    }

    dataBase.close();
    m_bim_info_map[bim_info.id] = bim_info;
    if (min_square > bim_info.square) min_square = bim_info.square;
    if (max_square < bim_info.square) max_square = bim_info.square;
    return SQL_OK;
}
int DataManager::DelBim(int id){
    if (ConnectDataBase() != SQL_OK) return SQL_OPEN_ERROR;
    QSqlQuery sql_query(dataBase);
    sql_query.prepare("delete from bim_info where id=?");
    sql_query.addBindValue(id);
    if (!sql_query.exec()) {
        auto a = sql_query.lastError().text();
        return SQL_EXEC_ERROR;
    }

    dataBase.close();
    double square = -1;
    auto iter = m_bim_info_map.find(id);
    if (iter != m_bim_info_map.end()){
        square = iter->second.square;
        m_bim_info_map.erase(iter);
    }

    if (square == max_square || square == min_square){
        min_square = 99999;
        max_square = -1;
        for (auto iter = m_bim_info_map.begin(); iter != m_bim_info_map.end(); ++iter){
            if (min_square > iter->second.square) min_square = iter->second.square;
            if (max_square < iter->second.square) max_square = iter->second.square;
        }
    }

    return SQL_OK;
}

int DataManager::DelBimByNum(int num){
    for (auto iter = m_bim_info_map.begin(); iter != m_bim_info_map.end(); ++iter){
        if (iter->second.num == num){
            return DelBim(iter->first);
        }
    }
    return SQL_OK;
}

int DataManager::GetBimsPrivate(){
    if (ConnectDataBase() != SQL_OK) return SQL_OPEN_ERROR;
    QSqlQuery sql_query(dataBase);
    QString str = "select id,num,name,time,url,square,position,struct,bim,type,height,nature,key,key_weight from bim_info";
    sql_query.prepare(str);
    if (!sql_query.exec()) {
        auto a = sql_query.lastError().text();
        return SQL_EXEC_ERROR;
    }
    while (sql_query.next()){
        BimInfo info;
        info.id = sql_query.value(0).toInt();
        info.num = sql_query.value(1).toInt();
        info.name = sql_query.value(2).toString();
        info.time = sql_query.value(3).toString();
        info.url = sql_query.value(4).toString();
        info.square = sql_query.value(5).toReal();
        info.position = sql_query.value(6).toString();
        info.build_struct = sql_query.value(7).toString();
        info.bim = sql_query.value(8).toString();
        info.type = sql_query.value(9).toString();
        info.height = sql_query.value(10).toString();
        info.nature = sql_query.value(11).toString();
        info.key = sql_query.value(12).toString();
        info.key_weight = sql_query.value(13).toString();
        if (min_square > info.square) min_square = info.square;
        if (max_square < info.square) max_square = info.square;
        m_bim_info_map[info.id] = info;
    }
    dataBase.close();
    return SQL_OK;
}

