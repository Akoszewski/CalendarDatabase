#ifndef RECORD_H
#define RECORD_H

#include <sqlite3.h>
#include <iostream>
#include <list>
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

#endif