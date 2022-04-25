#include <iostream>
#include <list>
#include <sqlite3.h>
#include <memory>

class Record
{
public:
    Record(std::string title, std::string date, std::string description, int id = -1)
      : title(std::move(title)), date(std::move(date)), description(std::move(description)), id(id)
    {}
    void print() const
    {
        std::cout << "Data: " << date << " Zadanie: " << title  << " Opis: " << description << std::endl;
    }
private:
    int id;
    std::string title;
    std::string date;
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
    DbManager(std::string path);
    std::list<Record> getRecords() const;
    void insert(Record record);
private:
    std::string path;
    std::unique_ptr<sqlite3, sqlite3_deleter> db;
};

DbManager::DbManager(std::string path)
  : path(std::move(path))
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

std::list<Record> DbManager::getRecords() const
{
    // auto patients = db.executeQuery<Patient>("select * from patients", {sizeof(Patient::id), 20, 20, 12});
    std::list<Record> records;
    records.emplace_back("Domyslny record", "2000-11-11", "Blabla");
    return records;
}

void DbManager::insert(Record record)
{
    // std::string komenda = "INSERT INTO Patients (name, surname, pesel) VALUES ('"+patient.name+ "','" +patient.surname+"','"+patient.pesel+"');";
    // db.executeQuery<>(komenda);
}

Record createRecordFromUser()
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
            Record record = createRecordFromUser();
            db.insert(record);
            std::cout << "Dodano rekord: " << std::endl;
            record.print();
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
