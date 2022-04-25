#include <iostream>
#include <list>
#include <sqlite3.h>
#include <memory>

class Record
{
public:
    Record(std::string date, std::string title, std::string description, int id = -1)
      : date(std::move(date)), title(std::move(title)), description(std::move(description)), id(id)
    {}
    void print(bool shouldShowId = true) const
    {
        if (shouldShowId) { std::cout << "ID: " << id << " "; }
        std::cout << "Data: " << date << " Zadanie: "
            << title  << " Opis: " << description << std::endl;
    }
    std::string getDate() const {
        return date;
    }
    std::string getTitle() const {
        return title;
    }
    std::string getDescription() const {
        return description;
    }
private:
    int id;
    std::string date;
    std::string title;
    std::string description;
};

struct sqlite3_deleter
{
    void operator()(sqlite3* sql) {
        sqlite3_close_v2(sql);
    }
};

class DbManager
{
public:
    DbManager(const std::string& path);
    std::list<Record> getRecords();
    void insert(const Record& record);
private:
    std::string path;
    std::unique_ptr<sqlite3, sqlite3_deleter> db;
    std::list<Record> executeQuery(const std::string& query);
};

DbManager::DbManager(const std::string& path)
  : path(path)
{
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


Record createRecordFromInput()
{
    std::string date, title, desc;
    std::cout << "Podaj date: ";
    std::cin >> date;
    std::cout << "Podaj tytul zadania: ";
    std::cin >> title;
    std::cout << "Dokladniejszy opis: ";
    std::cin >> desc;
    return Record(date, title, desc);
}

int main()
{
    DbManager db("database.db");

    while (true) {
        char option;
        std::cout << "\nCo chcesz zrobic?" << std::endl
                << "1 - Insert\n2 - Select\n3 - Wyjscie" << std::endl;
        std::cin >> option;
        if (option == '1') {
            Record record = createRecordFromInput();
            db.insert(record);
            std::cout << "Dodano rekord: " << std::endl;
            record.print(false);
        }
        else if (option == '2') {
            std::list<Record> records = db.getRecords();
            for (const auto& record : records) {
                record.print();
            }
        }
        else {
            break;
        }
    }
    return 0;
}
