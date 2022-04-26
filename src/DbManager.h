#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "Record.h"

struct sqlite3_deleter
{
    void operator()(sqlite3* sql) {
        sqlite3_close_v2(sql);
    }
};

class DbManager
{
public:
    static DbManager& Get() { return instance; }
    DbManager(const DbManager&) = delete;
    void init(const std::string& path);
    std::list<Record> getRecords();
    void insert(const Record& record);
private:
    std::string path;
    std::unique_ptr<sqlite3, sqlite3_deleter> db;
    std::list<Record> executeQuery(const std::string& query);

    DbManager() {}
    static DbManager instance;
};

#endif