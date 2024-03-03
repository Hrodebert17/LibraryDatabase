# Library Database.

### Overview 
Hi dev, you may wonder what this project is, this is a simple and light user interface for my database library.
this ui allows the user to insert, remove and search books in a matter of seconds making it an easy task.

if you want you can also edit this project and turn it into a todo list or pretty much anything fits your needs.

this project also contains a simple search algorithm which you could use in your own project.

This project was coded by Hrodebert17 for his school library in order to make the operations to add, remove and search book way easier. This project was also made to test the strenght of the
[Qic database](https://github.com/Hrodebert17/QIC-database).

### Getting started.
In this section we will learn how to compile and use this project.
Before putting our hands on this ui we need to compile it.
here some instructions on how to do that:

##### Pre requisites:
- [Cmake](https://cmake.org/)
- [Boost](https://www.boost.org/)

#### Compiling
Compiling this project is pretty easy. 
you will just need to run the following command which will generate the build files.

```
make .
```

then this command will generate the executable:

```
cmake --build ./
```

### Executing the ui
to open the ui you can either open it like a normal executable or just go into a terminal, navigate into the folder containing the executable and running it.

### Using the ui
now that you are inside the softwere this will be the first thing you see:
```
Reading the database.
It seems like the books data is either corrupted or non existing.
 creating a new database table for the data...
############################
#                          #
#    Library catalogue.    #
#                          #
############################

Menu:
Press N to add a new book
Press L to list all the books
Press R to remove a book
Press S to search
Press Q to quit

```

the second and third line are just warnings complaining because there is not a database avaible which means that the softwere will create one for you.
it is important to know that in the case you are getting this warning when its not the first time using this ui it means that your data got either lost, corrupted or it is unavaible.

we can jump straight to the menu!

#### The keys:
in total we have 5 possible options each of them has his own reason to be there.

the keys are:
- N (this key allows you to add a new book)
- L (this key allows you to list all the books)
- R (this key allows you to remove a book)
- S (this key allows you to filter books and list them)
- Q (this key allows you to quit the program)


### Documentation

#### How to add a new book.
in order to add a new book you will have to press the "N" key, this will show you a dialogue asking you the book title, author and id.
remember to have an unique id.

#### How to remove a book.
in order to remove a book you will have to know his id and then you can just insert the key "R" wich will ask you the id of the book to remove.
if the id contains letters it will give you an error if it does not contain letters then it will remove it.

#### How to list all the books.
in order to list all the books you will just need to type the key "L" and it will give you a list of all the books.

#### How to search for books.
if you would like to search for a book or all the books of an author you will just have to write the key "S" and then after that the program will ask you if you would like to search a book by his author, title or id.

like this:
```
What you want to search? (T = tittle, A = author, i = Id)
```
just type the key corrisponding to your choice and then you can insert the parameter
to search it.
example:
```
Menu:
Press N to add a new book
Press L to list all the books
Press R to remove a book
Press S to search
Press Q to quit
s
What you want to search? (T = tittle, A = author, i = Id)
a
Please insert the author you want to search
hrodebert
--------------------------------------------
Author : Hrodebert
Title  : Hrodebert_Programming_life
ID     : 1
```

#### How to quit.
in order to quit the program you will just have to type the key "Q"

