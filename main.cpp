#include <iostream>
#include "operations.h"
//TODO add a way to lend a book and to unlend a book.


int main()
{
    std::cout << "Reading the database." << std::endl;
    loadTables();
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
                  "Menu:\n" <<
                  "Press N to add a new book\n" <<
                  "Press L to list all the books\n" <<
                  "Press R to remove a book\n" <<
                  "Press S to search\n" <<
                  "Press Q to quit" <<
                  std::endl;
        // we get the input and then we handle it
	std::string input;
        std::cin  >> input;
	if (input == "S" || input == "s") 
	{
		search();
	}
	else if (input == "Q" || input == "q")
        {
        	return 0;
        }
	else if (input == "R" || input == "r")
        {
		remove();
        }
	else if (input == "N" || input == "n")
        {
		newBook();
        }
	else if (input == "L" || input == "l") 
	{
		list();
	}
     }
    return 0;

}
