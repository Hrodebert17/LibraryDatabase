#include "operations.h"
#include <iostream>
#include "libs/Qic_database.h"
#include <boost/algorithm/string.hpp>
#include <string>
#include <limits>


// Here we declare all the important variables.
qic::DataBase db("books.catalogue");
std::vector<Book> books;
std::vector<std::string> tables;
std::string input;

void returnBook() {
	std::cout << "Which book would you like to return? Please write its ID." << std::endl;
	std::cin  >> input;
	// Instead of an int we add a try statement to avoid any errors caused by the user inserting a string.
	try {
		// We check if the ID given corresponds to any of the books in the library until we get a match.
		for (int i = 0; i < books.size(); i++) {
			Book *book = &books.at(i);
			if (std::stoi(input) == book->id) {
				std::cout << "Returning the book" << std::endl;
				book->lent = false;
				book->lentDate = date();
				book->person_who_has_the_book = "";
				break;
			} 
      		}
		db.open();
		db.dropTable("books");
		db.createTable("books",std::vector<qic::dataType> 
		{
			qic::dataType::String, // Author
			qic::dataType::String, // Title
			qic::dataType::Integer, // Book ID
			qic::dataType::Boolean, // Lent 
			qic::dataType::Integer, // Lent day
			qic::dataType::Integer, // Lent month
			qic::dataType::Integer, // Lent year
			qic::dataType::String,  // The name of the person who borrowed the book.
		});
		// Now we update the database.
		for (auto book : books) {
		    db.addValueToTable("books",std::vector<qic::Value>
		    {
			    qic::Value(qic::String,book.author),
			    qic::Value(qic::String,book.title),
			    qic::Value(qic::Integer,book.id),
			    qic::Value(qic::dataType::Boolean, book.lent),
			    qic::Value(qic::dataType::Integer, book.lentDate.day),
			    qic::Value(qic::dataType::Integer, book.lentDate.month),
			    qic::Value(qic::dataType::Integer, book.lentDate.year),
			    qic::Value(qic::dataType::String, book.person_who_has_the_book)
		    });	
		}
		db.close();
	} catch (std::exception) {}
	
}

void borrowBook()
{
	std::cout << "Which book would you like to borrow? Please insert its ID." << std::endl;
	std::cin  >> input;
	// To avoid errors caused by the user inserting a string instead of an int we try to add a statement.
	try {
		// We check if the ID given corresponds to any of the books in the library until we gat a match.
		for (int i = 0; i < books.size(); i++) {
			Book *book = &books.at(i);
			if (std::stoi(input) == book->id) {
				std::cout << "Please enter the date the book shold be returned (day/year/month)." << std::endl;
				std::cin  >> book->lentDate.day >> book->lentDate.month >> book->lentDate.year;
				book->lent = true;
				std::cout << "Who is the borrower?" << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
				std::getline(std::cin,book->person_who_has_the_book);
				break;
			} 
      		}
		db.open();
		db.dropTable("books");
		db.createTable("books",std::vector<qic::dataType> 
		{
			qic::dataType::String, // Author
			qic::dataType::String, // Title
			qic::dataType::Integer, // Book ID
			qic::dataType::Boolean, // lent 
			qic::dataType::Integer, // lent day
			qic::dataType::Integer, // lent month
			qic::dataType::Integer, // lent year
			qic::dataType::String,  // The name of the person who borrowed the book.
		});
		// Now we update the database.
		for (auto book : books) {
		    db.addValueToTable("books",std::vector<qic::Value>
		    {
			    qic::Value(qic::String,book.author),
			    qic::Value(qic::String,book.title),
			    qic::Value(qic::Integer,book.id),
			    qic::Value(qic::dataType::Boolean, book.lent),
			    qic::Value(qic::dataType::Integer, book.lentDate.day),
			    qic::Value(qic::dataType::Integer, book.lentDate.month),
			    qic::Value(qic::dataType::Integer, book.lentDate.year),
			    qic::Value(qic::dataType::String, book.person_who_has_the_book)
		    });	
		}
		db.close();
	} catch (std::exception) {}
}

Book::Book(int pId, std::string pAuthor, std::string pTitle, bool pLent)
{
	this-> id = pId;
	this->author = pAuthor;
	this->title = pTitle;
	this->lent = pLent;
}

void loadTables() 
{

	db.open();
	tables = db.getAllTables();

	// If the table "book" of the database does not exists we print a warning and we create the table.
	// Else if the table exists we can scan it to read all the books values.
	if (!tableExists("books",tables))
	{
		
		db.createTable("books",std::vector<qic::dataType> 
		{
			qic::dataType::String, // Author
			qic::dataType::String, // Title
			qic::dataType::Integer, // Book ID
			qic::dataType::Boolean, // Lent
			qic::dataType::Integer, // Lent day
			qic::dataType::Integer, // Lent month
			qic::dataType::Integer, // Lent year
			qic::dataType::String,  // The name of the person who borrowed the book.
		});
	
		std::cout << "It seems like the books data is either corrupted or non existing.\n creating a new database table for the data..."  << std::endl;

	}
	else
	{
		// If the table exists it means that we can try to gather books from it!
		std::vector<std::vector<qic::Value>> booksFromTable = db.getAllValuesFromTable("books");
		// Each book in the raw data books vector "booksFromTable" gets converted into a Book and added to the "books" vector.
		for (auto book : booksFromTable) {
			Book newInstance(book.at(2).get_int_value(),book.at(0).get_string_value(),book.at(1).get_string_value(),book.at(3).get_bool_value());
			newInstance.lentDate = date{
				.day = book.at(4).get_int_value(),
				.month = book.at(5).get_int_value(),
				.year = book.at(6).get_int_value(),
			};
			newInstance.person_who_has_the_book = book.at(7).get_string_value();
			books.push_back(newInstance);
		}
		// Now that we added all the books found in the database we can close it and open the menu.

	}
	db.close();

}

// Simplifies a string (example: Harry_Potter_and_the_chalice_of_fire turn into harrypotterandthechaliceoffire .
std::string simplifyString(std::string input)
{
    // We declare a variable for the new string.
    std::string simplifiedString;
    // We declare an array containing all the characters which are allowed into the string
    char allowedChars[] = {
            'q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k',
            'l','z','x','c','v','b','n','m','1','2','3','4','5','6','7','8','9','0'
    };
    // We lower the string
    boost::to_lower(input);
    // For each character of the string we check if it is allowed; if it is we add it into the new string.
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
    // If the user wants to search a book we give them some options and then we collect the input.
    std::cout << "What do you want to search? Press T for title, A for author, i for ID)" << std::endl;
    std::cin  >> input;
    // We chek which option was chosen and print some instructions.
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
    // We gather the input and save it.
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::getline(std::cin,input);

    // We simplify the input so that we can find better results.
    input = simplifyString(input);
    // For each book we check if they match the search and if they do we add them to a vector containing the search results.
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
    // For each book in the search result we print the author,title and ID.
    for (auto bookSession : search) {
	std::cout << "--------------------------------------------" << std::endl;
	std::string lent_warning;
	if (bookSession.lent) {
	    lent_warning = "[Borrowed] until: " + std::to_string(bookSession.lentDate.day)
				+ "/" + std::to_string(bookSession.lentDate.month) 
				+ "/" + std::to_string(bookSession.lentDate.year)
				+ " by: " + bookSession.person_who_has_the_book;
	}
	std::cout << "Title  : " << bookSession.title << lent_warning << std::endl;
	std::cout << "Author : " << bookSession.author << std::endl;
	std::cout << "ID     : " << bookSession.id << std::endl;
    }
    // Then we print the options again
    std::cout <<"\n\n\n\n" << "-------options-------" << std::endl;

}

void remove() 
{
    // If the user requests to remove a book we ask for its ID
    std::cout << "Insert the book ID" << std::endl;
    std::cin  >> input;
    try
    {
	// We save
	int IdToDelte = std::stoi(input);
	// Now for each book we check if the ID matches.
	for (auto book : books)
	{
	    if (book.id == IdToDelte)
	    {
		// If the ID matches we open the database.
		// We remove the book from the vector.
		db.open();
		books.erase(std::find(books.begin(),books.end(),book));
		db.close();
		db.open();
		db.dropTable("books");
		db.createTable("books",std::vector<qic::dataType>
			{
				qic::dataType::String, // Author
				qic::dataType::String, // Title
				qic::dataType::Integer, // Book ID
				qic::dataType::Boolean, // Lent
				qic::dataType::Integer, // Lent day
				qic::dataType::Integer, // Lent month
				qic::dataType::Integer, // Lent year
				qic::dataType::String,  // The name of the person who lent the book.
			});
		db.close();
		db.open();
		for (auto instance : books) {
		    db.addValueToTable("books",std::vector<qic::Value>
			    {
				    qic::Value(qic::String,instance.author),
				    qic::Value(qic::String,instance.title),
				    qic::Value(qic::Integer,instance.id),
				    qic::Value(qic::dataType::Boolean, instance.lent),
				    qic::Value(qic::dataType::Integer, instance.lentDate.day),
				    qic::Value(qic::dataType::Integer, instance.lentDate.month),
				    qic::Value(qic::dataType::Integer, instance.lentDate.year),
				    qic::Value(qic::dataType::String, instance.person_who_has_the_book)
			    });
		}
		db.close();
	    }
	}
    }
    catch (std::exception e)
    {
	// In the case the ID is not a number we ask the user to enter the correct ID.
	std::cout << "Error. The ID needs to be a number. Try again." << std::endl;
    }

}

void newBook() 
{
	// In case the user wants to add another book we create a new book instance and we ask the user to insert the parameters of it.
	Book newBook;
	std::cout << "Insert the book title"  << std::endl;
	
	std::cin.clear();
	std::cin.ignore();
	std::getline(std::cin,input);
	newBook.title = input;
	std::cout << "Insert the book author" << std::endl;
	std::getline(std::cin,input);
	newBook.author = input;
	std::cout << "Insert the book ID"    << std::endl;
	std::getline(std::cin,input);
	// In the case the ID is not a number we ask the user to the correct ID and we just skip this loop itineration.
	try
	{
	newBook.id = std::stoi(input);
	}
	catch (std::exception e)
	{
	std::cout << "Error. The ID needs to be a number. Try again." << std::endl;
	return;
	}

	newBook.lent = false;

	// We open the database and we add a new book inside of it
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
	// We close the database and we add the book to the vector.
	std::cout << "Added a book to the catalogue" << std::endl;
	books.push_back(newBook);
}

void list() 
{
   // For each book in the "books" vector we print its author, title and ID.
    for (auto bookSession : books)
    {
	std::cout << "--------------------------------------------" << std::endl;
	std::string lent_warning;
	if (bookSession.lent) {
	    lent_warning = "[Borrowed] until: " + std::to_string(bookSession.lentDate.day)
				+ "/" + std::to_string(bookSession.lentDate.month) 
				+ "/" + std::to_string(bookSession.lentDate.year)
				+ " by: " + bookSession.person_who_has_the_book;
	}
	std::cout << "Title  : " << bookSession.title << lent_warning << std::endl;
	std::cout << "Author : " << bookSession.author << std::endl;
	std::cout << "ID     : " << bookSession.id << std::endl;
	}
	// Next we print the options again.
	std::cout <<"\n\n\n\n" << "-------options-------" << std::endl;
}
