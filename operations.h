#ifndef OPERATIONS
#define OPERATIONS

#include <iostream>
#include "libs/Qic_database.h"

struct date {
    int day,month,year;
};

class Book
{
public:
    bool lent;
    int id;
    std::string author;
    std::string title;
    std::string person_who_has_the_book;
    date lentDate;
    Book() {}
    Book(int pId, std::string pAuthor, std::string pTitle, bool pLent);
    bool operator==(const Book &other) {
        if (other.id == this->id && other.title == this->title && other.author == this->author && other.lent == this->lent) {
            return true;
        }
        return false;
    }
};

std::string simplifyString(std::string input);

bool tableExists(std::string table, std::vector<std::string> vec);

void search();

void loadTables();

void remove();

void newBook();

void list();

void borrowBook();

void returnBook();

#endif
