#pragma once
#include <vector>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <map>
#define SQL_OK 0
#define SQL_NOT_FOUND 1
#define SQL_OPEN_ERROR 2
#define SQL_EXEC_ERROR 3
#define SQL_ATTR_ERROR 4

#define DATABASE_NAME "sqlite3.db"
#define DATABASE_USER "echizen"
#define DATABASE_PASS "ryoma"
#define DATABASE_ALIAS "global"

typedef struct{
   int id;
   int num;
   QString name;
   QString time;
   QString url;
   double square;
   QString position;
   QString build_struct;
   QString bim;
   QString type;
   QString height;
   QString nature;
   QString key;
   QString key_weight;
} BimInfo ;

class DataManager{
public:
    static DataManager* GetInstance();
    DataManager();
    ~DataManager();

    int ConnectDataBase();
    int InitDataBase();

    //table bim
    int AddBim(BimInfo&);
    int DelBim(int id);
    int DelBimByNum(int num);
    std::map<int, BimInfo> GetBims(){
        if (m_bim_info_map.empty())
            GetBimsPrivate();
        return m_bim_info_map;
    }
    double getMaxSquare(){
        return max_square;
    }
    double getMinSquare(){
        return min_square;
    }

private:
    int GetBimsPrivate();
    QSqlDatabase dataBase;
    std::map<int, BimInfo> m_bim_info_map;
    double max_square = -1;
    double min_square = 99999;
};
