# Library Database.

### Overview 
Hi dev, you may wonder what this project is, this is a simple and light user interface for my database library.
This UI allows the user to insert, remove and categorize books in a matter of seconds making it an easy task.
In addition, this project contains a simple search algorithm which you could use in your personal one.

If you want you can also edit this project and turn it into a to-do list or pretty much anything that fits your needs.

This project was coded by Hrodebert17 for his school library in order to make the operations to add, remove and search book way easier. This project was also made to test the strenght of the
[Qic database](https://github.com/Hrodebert17/QIC-database).

### Getting started.
In this section we will learn how to compile and use this project.
Before putting our hands on this UI we need to compile it.
Here are some instructions on how to do that:

##### Pre requisites:
- [Cmake](https://cmake.org/)
- [Boost](https://www.boost.org/)
- [Qic database](https://github.com/Hrodebert17/QIC-database) (put the source files in the libs folder)

#### Compiling
Compiling this project is pretty easy. 
You will just need to run the following command which will generate the build files.

```
make .
```

Afterwards this command will generate the executable:

```
cmake --build ./
```

or you can build it using mingw
```
g++ -std=c++2b -I/path_to_boost -o program main.cpp libs/Qic_database.cpp operations.cpp
```

### Executing the UI
To open the UI you can either open it like a normal executable or just go into a terminal, navigate into the folder containing the executable and running it.

### Using the UI
Upon entering the software this will be the first thing you see:
```
Reading the database.
It seems like the books data is either corrupted or non existing.
Creating a new database table for the data...
############################
#                          #
#    Library catalogue.    #
#                          #
############################

Menu:
Press N to add a new book
Press L to list all the books
Press R to remove a book
Press S to search a book
Press Q to quit

```

The second and third line are just warnings: the software is complaining because there is not a database avaible which means that it will create one for you.
It is important to know that in the case that you are getting this warning (when its not the first time that you are using this UI), it means that your data got either lost, corrupted or it is unavailable.

Let's jump straight to the menu!

#### The keys:
in total we have 5 possible options: each one of them has their own reason to be there.

The keys are:
- N (this key allows you to add a new book)
- L (this key allows you to list all the books)
- R (this key allows you to remove a book)
- S (this key allows you to filter books and list them, i.e. to search)
- Q (this key allows you to quit the program)


### Documentation

#### How to add a new book.
In order to add a new book you have to press the "N" key. This will show you a dialogue asking you the book title, author and ID.
Remember to have an unique ID.

#### How to remove a book.
In order to remove a book you have to insert the key "R". Afterwards the program will ask you the ID of the book to remove.
If the ID contains letters it will give you an error, otherwise it will remove the book selected.

#### How to list all the books.
In order to get a list of all the books you just have to type the key "L".

#### How to search for books.
If you would like to search for a book or all the books of a certain author you have to write the key "S"; following this, the program will ask you if you would like to search a book by his author, title or ID.

Like this:
```
What you want to search? (T = tittle, A = author, i = ID)
```
Just type the key according to your chioce and then you can insert the parameter to search it.
For example:
```
Menu:
Press N to add a new book
Press L to list all the books
Press R to remove a book
Press S to search a book
Press Q to quit
s
What you want to search? (T = tittle, A = author, i = ID)
a
Please insert the author you want to search
hrodebert
--------------------------------------------
Author : Hrodebert
Title  : Hrodebert_Programming_life
ID     : 1
```

#### How to quit.
In order to quit the program you have to type the key "Q".
