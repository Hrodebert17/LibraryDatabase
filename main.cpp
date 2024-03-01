#include <iostream>
#include <string>
#include "libs/Qic_database.h"
#include <boost/algorithm/string.hpp>

class Book {
public:
	int id;
	std::string author;
	std::string title;
	Book() {}
	Book(int pId, std::string pAuthor, std::string pTitle) {
		this->id = pId;
		this->author = pAuthor;
		this->title = pTitle;
	}
};

std::string simplifyString(std::string input) {
	boost::to_lower(input);
	std::string simplifiedString;
	char allowedChars[] = {
		'q','e','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k',
		'l','z','x','c','v','b','n','m','1','2','3','4','5','6','7','8','9','0'
	};
	for (int i = 0; i < input.size(); i++) {
		bool isAllowed = false;
		char character = input[i];
		for (char allowedChar : allowedChars){ 
			if (character == allowedChar) {
				isAllowed = true;
			}
		}
		if (isAllowed) {
			simplifiedString += character;
		}
	}
	return simplifiedString;

} 

bool tableExists(std::string table, std::vector<std::string> vec) {
	for (std::string tableInVec : vec) {
		if (tableInVec == table) {
			return true;
		}
	}
	return false;
}

int main() {
	std::cout << "reading the database." << std::endl;
	qic::DataBase db("books.catalogue");
	std::vector<Book> books;
	std::vector<std::string> tables;
	std::string input;
	db.open();
	tables = db.getAllTables();
	if (!tableExists("books",tables)) {
		db.createTable("books",std::vector<qic::dataType> {
			qic::dataType::String, // author
			qic::dataType::String, // title
			qic::dataType::Integer // book id
		});
		std::cout << "seems like the books data is either corrupted or non existing.\n creating a new database table for the data..." << std::endl;
	} else {
		// if the table exists it means that we can try to gather books from it!
		std::vector<std::vector<qic::Value>> booksFromTable = db.getAllValuesFromTable("books");
		for (auto book : booksFromTable) {
			books.push_back(Book(book.at(2).get_int_value(),book.at(0).get_string_value(),book.at(1).get_string_value()));
		}
		// now that we added all the books found in the database we can close it and open the menu.
		
	}
	db.close();

	std::cout 
		<< "############################\n" 
		<< "#                          #\n"     
		<< "#    Library catalogue.    #\n"     
		<< "#                          #\n"     
		<< "############################\n"
		<<
	std::endl;
	while (true) {
		std::cout << "menu:\n" << "press N to add a new book\n" << "press L to list all the books\n" << "press R to remove a book\n" << "press S to search\n" << "press Q to quit" << std::endl;
		std::cin  >> input;
			if (input == "S" || input == "s") {
			std::cout << "What you want to search? (T = tittle, A = author, i = Id)" << std::endl;
			std::cin  >> input;
			if (input == "t" || input == "T") {
				std::cout << "insert the title you want to search" << std::endl;
			}
			if (input == "a" || input == "A") {
				std::cout << "insert the author you want to search" << std::endl;
			}
			if (input == "i" || input == "I") {
				std::cout << "insert the id you want to search" << std::endl;
			}
				std::string mode = input;
				std::cin  >> input;
				input = simplifyString(input);
				std::vector<Book> search;
				for (int i = 0; i < books.size(); i++) {
					if (mode == "t" || mode == "T") {
						if (boost::contains(simplifyString( books.at(i).title ) ,input) ) { 
							search.push_back(books.at(i));
						}
					}
					if (mode == "a" || mode == "A") {
						if (boost::contains(simplifyString( books.at(i).author ) ,input) ) { 
							search.push_back(books.at(i));
						}
					}
					if (mode == "i" || mode == "I") {
						if (boost::contains(simplifyString( std::to_string(books.at(i).id) ) ,input) ) { 
							search.push_back(books.at(i));
						}
					}

				}
			for (auto bookSession : search) {
				std::cout << "--------------------------------------------" << std::endl;
				std::cout << "author : " << bookSession.author << std::endl;
				std::cout << "title  : " << bookSession.title << std::endl;
				std::cout << "id     : " << bookSession.id << std::endl;
			}
			std::cout <<"\n\n\n\n" << "-------options-------" << std::endl;
			} else if (input == "Q" || input == "q") { return 0;}
			else if (input == "R" || input == "r")  {
				std::cout << "insert the book id" << std::endl;
				std::cin  >> input;
				try {
					int IdToDelte = std::stoi(input);
					for (auto book : books) {
						if (book.id == IdToDelte) {
							db.open();
							db.eraseValuesFromTable("books",std::vector<qic::Value>{
								qic::Value(qic::String, book.author),
								qic::Value(qic::String, book.title),
								qic::Value(qic::Integer, book.id)
							});
							db.close();
							books.clear();
							std::vector<std::vector<qic::Value>> booksFromTable = db.getAllValuesFromTable("books");
							for (auto book : booksFromTable) {
								books.push_back(Book(book.at(2).get_int_value(),book.at(0).get_string_value(),book.at(1).get_string_value()));
							}
						}
					}
				} catch (std::exception e) {

					std::cout << "id needs to be a number." << std::endl;
				}
			}
			if (input == "N" || input == "n") {
				Book newBook;
				std::cout << "insert the book title"  << std::endl;
				std::cin  >> input;
				newBook.title = input;
				std::cout << "insert the book author" << std::endl;
				std::cin  >> input;
				newBook.author = input;
				std::cout << "insert the book id"    << std::endl;
				std::cin  >> input;
				try {
					newBook.id = std::stoi(input);
				} catch (std::exception e) {
					std::cout << "bad id. retry" << std::endl;	
					continue;
				}
				db.open();
				db.addValueToTable("books",std::vector<qic::Value> {
					qic::Value(qic::String,newBook.author),
					qic::Value(qic::String,newBook.title),
					qic::Value(qic::Integer,newBook.id)
				});
				db.close();
				std::cout << "added a book to the catalogue" << std::endl;
				books.push_back(newBook);
			} else if (input == "l" || input == "L") {
				for (auto bookSession : books) {
					std::cout << "--------------------------------------------" << std::endl;
					std::cout << "author : " << bookSession.author << std::endl;
					std::cout << "title  : " << bookSession.title << std::endl;
					std::cout << "id     : " << bookSession.id << std::endl;
				}
				std::cout <<"\n\n\n\n" << "-------options-------" << std::endl;
			}
	}
	return 0;
}
