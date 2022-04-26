#include "DbManager.h"

DbManager DbManager::instance;

void DbManager::init(const std::string& path)
{
    this->path = path;
    sqlite3* ptr = nullptr;
    int ret = sqlite3_open(path.c_str(), &ptr);
    db = std::unique_ptr<sqlite3, sqlite3_deleter>(ptr);
    if (ret)
    {
        std::cout << "Nie udalo sie otworzyc bazy danych" << sqlite3_errmsg(db.get()) << std::endl;
        exit(0);
    }
}

std::list<Record> DbManager::getRecords()
{
    std::list<Record> records = executeQuery("select * from Records");
    return records;
}

void DbManager::insert(const Record& record)
{
    std::string query = "INSERT INTO Records (date, title, description) VALUES ('"
        + record.getDate() + "','" + record.getTitle() + "','" + record.getDescription() + "');";
    executeQuery(query);
}

int queryCallback(void* data, int argc, char **argv, char **azColName)
{
   std::list<Record>* dataVect = static_cast<std::list<Record>*>(data);
   dataVect->emplace_back(argv[1], argv[2], argv[3], atoi(argv[0]));
   return 0;
}

std::list<Record> DbManager::executeQuery(const std::string& query)
{
    char* zErrMsg = nullptr;
    std::list<Record> results;
    int res = sqlite3_exec(db.get(), query.c_str(), queryCallback, &results, &zErrMsg);
    if (res != SQLITE_OK) {
        std::cout << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    return results;
}
