#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "libs/Qic_database.h"

class Book 
{
public:
	int id;
	std::string author;
	std::string title;
	Book() {}
	Book(int pId, std::string pAuthor, std::string pTitle)
	{
		this->id = pId;
		this->author = pAuthor;
		this->title = pTitle;
	}
	bool operator==(const Book &other) {
		if (other.id == this->id && other.title == this->title && other.author == this->author) {
			return true;
		}
		return false;
	}
};
// simplifies a string (example: Harry_Potter_and_the_chalice_of_fire turn into harrypotterandthechaliceoffire
std::string simplifyString(std::string input)
{
	// we declare a variable for the new string.
	std::string simplifiedString;
	// we declare an array containing all the characters which are allowed into the string
	char allowedChars[] = {
		'q','e','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k',
		'l','z','x','c','v','b','n','m','1','2','3','4','5','6','7','8','9','0'
	};
	// we lower the string
	boost::to_lower(input);
	// for each character of the string we cheeck if its allowed and if it is we add it into the new string
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

bool tableExists(std::string table, std::vector<std::string> vec)
{

	for (std::string tableInVec : vec) {
		if (tableInVec == table) {
			return true;
		}
	}
	return false;

}

int main() 
{

	// we declare all the important variables here.
	qic::DataBase db("books.catalogue");
	std::vector<Book> books;
	std::vector<std::string> tables;
	std::string input;

	std::cout << "reading the database." << std::endl;
	
	db.open();
	tables = db.getAllTables();

	// If the table "book" of the database do not exists we print a warning and we create the table
	// Else if the table exists we can scan it to read all the books values.
	if (!tableExists("books",tables))
	{
		
		db.createTable("books",std::vector<qic::dataType> 
		{
			qic::dataType::String, // author
			qic::dataType::String, // title
			qic::dataType::Integer // book id
		});
	
		std::cout << "seems like the books data is either corrupted or non existing.\n creating a new database table for the data..." << std::endl;

	}
	else
	{
		// if the table exists it means that we can try to gather books from it!
		std::vector<std::vector<qic::Value>> booksFromTable = db.getAllValuesFromTable("books");
		// each book in the raw data books vector "booksFromTable" gets converted into a Book and added to the "books" vector
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
		// we print all the options avaible to the user
		std::cout <<
			"menu:\n" << 
			"press N to add a new book\n" <<
			"press L to list all the books\n" <<
			"press R to remove a book\n" <<
			"press S to search\n" <<
			"press Q to quit" <<
		std::endl;
		// we get the input and then we handle it
		std::cin  >> input;
		if (input == "S" || input == "s") {
			std::string mode;
			std::vector<Book> search;
			// if the user wants to search a book we give him some options and then we collect the input
			std::cout << "What you want to search? (T = tittle, A = author, i = Id)" << std::endl;
			std::cin  >> input;
			// we cheek which option was chosen and print some instructions
			if (input == "t" || input == "T") {
				std::cout << "insert the title you want to search" << std::endl;
			}
			if (input == "a" || input == "A") {
				std::cout << "insert the author you want to search" << std::endl;
			}
			if (input == "i" || input == "I") {
				std::cout << "insert the id you want to search" << std::endl;
			}
			mode = input;
			// we gather the input and save it.
			std::cin  >> input;

			// we simplify the input so that we can find better results
			input = simplifyString(input);
			// for each book we cheeck if they match the search and if they do we add them to a vector containing the search results.
			for (int i = 0; i < books.size(); i++) {
				if (mode == "t" || mode == "T") {
					std::cout << mode;
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
			// for each book in the search result we print the author,title and id.
			for (auto bookSession : search) {
				std::cout << "--------------------------------------------" << std::endl;
				std::cout << "author : " << bookSession.author << std::endl;
				std::cout << "title  : " << bookSession.title << std::endl;
				std::cout << "id     : " << bookSession.id << std::endl;
			}
			// then we print the options again
			std::cout <<"\n\n\n\n" << "-------options-------" << std::endl;
		}
		else if (input == "Q" || input == "q") 
		{
		return 0;
		}
		else if (input == "R" || input == "r")
		{
			// if the user whants to remove a book then weask for the id 
			std::cout << "insert the book id" << std::endl;
			std::cin  >> input;
			try 
			{
				// we save 
				int IdToDelte = std::stoi(input);
				// now for each book we cheeck if the id matches.
				for (auto book : books)
				{
					if (book.id == IdToDelte)
					{
						// if the id matches we open the database
						db.open();
						// then we remvoe that book from the database.
						db.eraseValuesFromTable("books",std::vector<qic::Value>
							{
								qic::Value(qic::String, book.author),
								qic::Value(qic::String, book.title),
								qic::Value(qic::Integer, book.id)
							});
						// we close the database
						db.close();
						// we remove the book from the vector.
						books.erase(std::find(books.begin(),books.end(),book));
					}
				}
			}
			catch (std::exception e)
			{
				// In the case the id is not a number we ask the user to insert a number.
				std::cout << "id needs to be a number." << std::endl;
			}
		}
		if (input == "N" || input == "n")
		{
			// if the user wants to add an other book we create a new book instance and we ask the user to insert the parameters of it
			Book newBook;
			std::cout << "insert the book title"  << std::endl;
			std::cin  >> input;
			newBook.title = input;
			std::cout << "insert the book author" << std::endl;
			std::cin  >> input;
			newBook.author = input;
			std::cout << "insert the book id"    << std::endl;
			std::cin  >> input;
			// if the id is not a number we ask to insert a number and we just skip this loop itineration.
			try
			{
				newBook.id = std::stoi(input);
			}
			catch (std::exception e)
			{
				std::cout << "bad id. retry" << std::endl;	
				continue;
			}
			// we open the database and we add a new book inside of it
			db.open();
			db.addValueToTable("books",std::vector<qic::Value>
				{
					qic::Value(qic::String,newBook.author),
					qic::Value(qic::String,newBook.title),
					qic::Value(qic::Integer,newBook.id)
				});
			db.close();
			// we close the database and we add the book to the vector
			std::cout << "added a book to the catalogue" << std::endl;
			books.push_back(newBook);
		}
		else if (input == "l" || input == "L")
		{
			// for each book in the "books" vector we print author, title and id of it.
			for (auto bookSession : books)
			{
				std::cout << "--------------------------------------------" << std::endl;
				std::cout << "author : " << bookSession.author << std::endl;
				std::cout << "title  : " << bookSession.title << std::endl;
				std::cout << "id     : " << bookSession.id << std::endl;
			}
			// then we print the options again.
			std::cout <<"\n\n\n\n" << "-------options-------" << std::endl;
		}
	}
	return 0;

}
