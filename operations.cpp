#include "operations.h"
#include <iostream>
#include "libs/Qic_database.h"
#include <boost/algorithm/string.hpp>

// we declare all the important variables here.
qic::DataBase db("books.catalogue");
std::vector<Book> books;
std::vector<std::string> tables;
std::string input;

void borrowBook()
{
	std::cout << "which book would you like to borrow (insert the ID)" << std::endl;
	std::cin  >> input;
	// to avoid errors caused by the user inserting a string instead of an int we add a try statment
	try {
		// for each book we check if the id matches until we get one match
		for (int i = 0; i < books.size(); i++) {
			Book *book = &books.at(i);
			if (std::stoi(input) == book->id) {
				std::cout << "please insert when to expire the book borrow. (day/year/month)" << std::endl;
				std::cin  >> book->landedDate.day >> book->landedDate.month >> book->landedDate.year;
				book->landed = true;
				std::cout << "who will have the book?" << std::endl;
				std::cin  >> book->person_who_has_the_book;
				break;
			} 
      		}
		db.open();
		db.dropTable("books");
		db.createTable("books",std::vector<qic::dataType> 
		{
			qic::dataType::String, // author
			qic::dataType::String, // title
			qic::dataType::Integer, // book id
			qic::dataType::Boolean, // landed 
			qic::dataType::Integer, // landed day
			qic::dataType::Integer, // landed month
			qic::dataType::Integer, // landed year
			qic::dataType::String,  // the name of the person who landed the book.
		});
		// now we update the database.
		for (auto book : books) {
		    db.addValueToTable("books",std::vector<qic::Value>
		    {
			    qic::Value(qic::String,book.author),
			    qic::Value(qic::String,book.title),
			    qic::Value(qic::Integer,book.id),
			    qic::Value(qic::dataType::Boolean, book.landed),
			    qic::Value(qic::dataType::Integer, book.landedDate.day),
			    qic::Value(qic::dataType::Integer, book.landedDate.month),
			    qic::Value(qic::dataType::Integer, book.landedDate.year),
			    qic::Value(qic::dataType::String, book.person_who_has_the_book)
		    });	
		}
		db.close();
	} catch (std::exception) {}
}

Book::Book(int pId, std::string pAuthor, std::string pTitle, bool pLanded)
{
	this->id = pId;
	this->author = pAuthor;
	this->title = pTitle;
	this->landed = pLanded;
}

void loadTables() 
{

	db.open();
	tables = db.getAllTables();

	// If the table "book" of the database does not exists we print a warning and we create the table
	// Else if the table exists we can scan it to read all the books values.
	if (!tableExists("books",tables))
	{
		
		db.createTable("books",std::vector<qic::dataType> 
		{
			qic::dataType::String, // author
			qic::dataType::String, // title
			qic::dataType::Integer, // book id
			qic::dataType::Boolean, // landed 
			qic::dataType::Integer, // landed day
			qic::dataType::Integer, // landed month
			qic::dataType::Integer, // landed year
			qic::dataType::String,  // the name of the person who landed the book.
		});
	
		std::cout << "It seems like the books data is either corrupted or non existing.\n creating a new database table for the data..."  << std::endl;

	}
	else
	{
		// if the table exists it means that we can try to gather books from it!
		std::vector<std::vector<qic::Value>> booksFromTable = db.getAllValuesFromTable("books");
		// each book in the raw data books vector "booksFromTable" gets converted into a Book and added to the "books" vector
		for (auto book : booksFromTable) {
			Book newInstance(book.at(2).get_int_value(),book.at(0).get_string_value(),book.at(1).get_string_value(),book.at(3).get_bool_value());
			newInstance.landedDate = date{
				.day = book.at(4).get_int_value(),
				.month = book.at(5).get_int_value(),
				.year = book.at(6).get_int_value(),
			};
			newInstance.person_who_has_the_book = book.at(7).get_string_value();
			books.push_back(newInstance);
		}
		// now that we added all the books found in the database we can close it and open the menu.

	}
	db.close();

}

// simplifies a string (example: Harry_Potter_and_the_chalice_of_fire turn into harrypotterandthechaliceoffire
std::string simplifyString(std::string input)
{
    // we declare a variable for the new string.
    std::string simplifiedString;
    // we declare an array containing all the characters which are allowed into the string
    char allowedChars[] = {
            'q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k',
            'l','z','x','c','v','b','n','m','1','2','3','4','5','6','7','8','9','0'
    };
    // we lower the string
    boost::to_lower(input);
    // for each character of the string we check if it is allowed and if it is we add it into the new string
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
        std::cout << tableInVec << std::endl;
        if (tableInVec == table) {
            return true;
        }
    }
    return false;

}


void search() 
{
    std::string mode;
    std::vector<Book> search;
    // if the user wants to search a book we give them some options and then we collect the input
    std::cout << "What you want to search? (T = tittle, A = author, i = Id)" << std::endl;
    std::cin  >> input;
    // we cheek which option was chosen and print some instructions
    if (input == "t" || input == "T") {
	std::cout << "Please insert the title you want to search" << std::endl;
    }
    if (input == "a" || input == "A") {
	std::cout << "Please insert the author you want to search" << std::endl;
    }
    if (input == "i" || input == "I") {
	std::cout << "Please insert the ID you want to search" << std::endl;
    }
    mode = input;
    // we gather the input and save it.
    std::cin  >> input;

    // we simplify the input so that we can find better results
    input = simplifyString(input);
    // for each book we cheeck if they match the search and if they do we add them to a vector containing the search results.
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
    // for each book in the search result we print the author,title and Id.
    for (auto bookSession : search) {
	std::cout << "--------------------------------------------" << std::endl;
	std::string landed_warning;
	if (bookSession.landed) {
	    landed_warning = "[Borrowed] until: " + std::to_string(bookSession.landedDate.day)
				+ "/" + std::to_string(bookSession.landedDate.month) 
				+ "/" + std::to_string(bookSession.landedDate.year)
				+ " by: " + bookSession.person_who_has_the_book;
	}
	std::cout << "Title  : " << bookSession.title << landed_warning << std::endl;
	std::cout << "Author : " << bookSession.author << std::endl;
	std::cout << "ID     : " << bookSession.id << std::endl;
    }
    // then we print the options again
    std::cout <<"\n\n\n\n" << "-------options-------" << std::endl;

}

void remove() 
{
    // if the user wants to remove a book then we ask for the ID
    std::cout << "insert the book ID" << std::endl;
    std::cin  >> input;
    try
    {
	// we save
	int IdToDelte = std::stoi(input);
	// now for each book we check if the ID matches.
	for (auto book : books)
	{
	    if (book.id == IdToDelte)
	    {
		// if the ID matches we open the database
		// we remove the book from the vector.
		db.open();
		books.erase(std::find(books.begin(),books.end(),book));
		db.close();
		db.open();
		db.dropTable("books");
		db.createTable("books",std::vector<qic::dataType>
			{
				qic::dataType::String, // author
				qic::dataType::String, // title
				qic::dataType::Integer, // book id
				qic::dataType::Boolean, // landed
				qic::dataType::Integer, // landed day
				qic::dataType::Integer, // landed month
				qic::dataType::Integer, // landed year
				qic::dataType::String,  // the name of the person who landed the book.
			});
		db.close();
		db.open();
		for (auto instance : books) {
		    db.addValueToTable("books",std::vector<qic::Value>
			    {
				    qic::Value(qic::String,instance.author),
				    qic::Value(qic::String,instance.title),
				    qic::Value(qic::Integer,instance.id),
				    qic::Value(qic::dataType::Boolean, instance.landed),
				    qic::Value(qic::dataType::Integer, instance.landedDate.day),
				    qic::Value(qic::dataType::Integer, instance.landedDate.month),
				    qic::Value(qic::dataType::Integer, instance.landedDate.year),
				    qic::Value(qic::dataType::String, instance.person_who_has_the_book)
			    });
		}
		db.close();
	    }
	}
    }
    catch (std::exception e)
    {
	// In the case the ID is not a number we ask the user to insert a number.
	std::cout << "ID needs to be a number." << std::endl;
    }

}

void newBook() 
{
	// if the user wants to add another book we create a new book instance and we ask the user to insert the parameters of it
	Book newBook;
	std::cout << "Insert the book title"  << std::endl;
	std::cin  >> input;
	newBook.title = input;
	std::cout << "Insert the book author" << std::endl;
	std::cin  >> input;
	newBook.author = input;
	std::cout << "Insert the book ID"    << std::endl;
	std::cin  >> input;
	// if the id is not a number we ask to insert a number and we just skip this loop itineration.
	try
	{
	newBook.id = std::stoi(input);
	}
	catch (std::exception e)
	{
	std::cout << "bad ID. retry" << std::endl;
	return;
	}

	newBook.landed = false;

	// we open the database and we add a new book inside of it
	db.open();

	db.addValueToTable("books",std::vector<qic::Value>
	{
	    qic::Value(qic::String,newBook.author),
	    qic::Value(qic::String,newBook.title),
	    qic::Value(qic::Integer,newBook.id),
	    qic::Value(qic::dataType::Boolean, false),
	    qic::Value(qic::dataType::Integer, 0),
	    qic::Value(qic::dataType::Integer, 0),
	    qic::Value(qic::dataType::Integer, 0),
	    qic::Value(qic::dataType::String, "")
	});
	db.close();
	// we close the database and we add the book to the vector
	std::cout << "Added a book to the catalogue" << std::endl;
	books.push_back(newBook);
}

void list() 
{
   // for each book in the "books" vector we print author, title and ID of it.
    for (auto bookSession : books)
    {
	std::cout << "--------------------------------------------" << std::endl;
	std::string landed_warning;
	if (bookSession.landed) {
	    landed_warning = "[Borrowed] until: " + std::to_string(bookSession.landedDate.day)
				+ "/" + std::to_string(bookSession.landedDate.month) 
				+ "/" + std::to_string(bookSession.landedDate.year)
				+ " by: " + bookSession.person_who_has_the_book;
	}
	std::cout << "Title  : " << bookSession.title << landed_warning << std::endl;
	std::cout << "Author : " << bookSession.author << std::endl;
	std::cout << "ID     : " << bookSession.id << std::endl;
	}
	// then we print the options again.
	std::cout <<"\n\n\n\n" << "-------options-------" << std::endl;
}

