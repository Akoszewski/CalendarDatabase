#include "DbManager.h"

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
    DbManager& db = DbManager::Get();
    db.init("database.db");

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
