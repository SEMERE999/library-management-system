/*
    Library Management System (C++)

    This program is a simple library management system developed using C++.
    It allows management of books and users, including storing book details,
    tracking available copies, and handling user activities such as renting,
    buying, and borrowing books.

    The system is designed using structures (Book and User) and utility
    functions for input handling and file operations.

    Developed by:
    - Semere Brhane

    Course Project: Library Management System
*/
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

struct Book
{
    int id;
    std::string title;
    std::string author;
    std::string genre;
    int total_copies;
    int available_copies;
    int selling_price;
    int renting_price;
    int bought_price;
};

struct User
{
    int id;
    std::string fname;
    std::string lname;
    int rented_amount;
    int bought_amount;
    int borrowed_amount;
};

// Utility
int getIntInput(const std::string &prompt);
std::string getStringInput(const std::string &prompt);
bool isEmpty(const std::string &filename);

// File/Record management
void header(const std::string &filename);
void addBook(const std::string &filename);
void displayAll(const std::string &filename);
void displayBookForUser(const Book &book);
void updateRecord(const std::string &filename, int line_no = 0);
void deleteRecord(const std::string &filename, int line_no);

// Search/Recommendation
int searchBook(const std::string &filename, const std::string &mode);
void recommendBooks(const std::string &filename);

// Workflows
void runAsAdministrator(const std::string &filename);
void runAsUser(const std::string &filename);

int main()
{
    const std::string filename = "books1.txt";

    if (isEmpty(filename))
        header(filename);

    int role = promptUser();

    switch (role)
    {
    case 1:
        runAsAdministrator(filename);
        break;
    case 2:
        runAsUser(filename);
        break;
    case 3:
        std::cout << "Exiting program.\n";
        exit(0);
    }

    return 0;
}

int promptUser()
{
    const int passcode = 1234;
    int choice = 0;
    while (true)
    {
        std::cout << "1. Enter as Administrator\n";
        std::cout << "2. Enter as User\n";
        std::cout << "3. Exit\n";
        choice = getIntInput("Choice: ");
        if (choice >= 1 && choice <= 3)
            break;
        std::cout << "Invalid choice. Try again.\n";
    }
    return choice;
}

int getIntInput(const std::string &prompt)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        else
        {
            std::cout << "Invalid input! Enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string getStringInput(const std::string &prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

bool isEmpty(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return true;
    return file.peek() == std::ifstream::traits_type::eof();
}

void header(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Error creating file: " << filename << "\n";
        return;
    }
    file << std::left
         << std::setw(10) << "ID"
         << std::setw(30) << "TITLE"
         << std::setw(30) << "AUTHOR"
         << std::setw(20) << "GENRE"
         << std::setw(15) << "TOTAL"
         << std::setw(15) << "AVAILABLE"
         << std::setw(15) << "SELL"
         << std::setw(15) << "RENT"
         << std::setw(15) << "BOUGHT" << "\n";
    file << std::string(165, '-') << "\n";
}

void addBook(const std::string &filename)
{
    std::ofstream file(filename, std::ios::app);
    if (!file)
    {
        std::cerr << "Cannot open file.\n";
        return;
    }

    Book book;
    book.id = getIntInput("Enter book ID: ");
    book.title = getStringInput("Enter title: ");
    book.author = getStringInput("Enter author: ");
    book.genre = getStringInput("Enter genre: ");
    book.total_copies = getIntInput("Enter total copies: ");
    book.available_copies = book.total_copies;
    book.selling_price = getIntInput("Enter selling price: ");
    book.renting_price = getIntInput("Enter renting price: ");
    book.bought_price = getIntInput("Enter bought price: ");

    file << std::left
         << std::setw(10) << book.id
         << std::setw(30) << book.title
         << std::setw(30) << book.author
         << std::setw(20) << book.genre
         << std::setw(15) << book.total_copies
         << std::setw(15) << book.available_copies
         << std::setw(15) << book.selling_price
         << std::setw(15) << book.renting_price
         << std::setw(15) << book.bought_price
         << "\n";

    std::cout << "Book added successfully.\n";
}

void displayAll(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
        std::cout << line << "\n";
    file.close();
}

void updateRecord(const std::string &filename, int line_no)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Cannot open file.\n";
        return;
    }

    std::ofstream temp("temp.txt");
    if (!temp)
    {
        std::cerr << "Cannot create temporary file.\n";
        return;
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line))
    {
        count++;
        if (count != line_no)
        {
            temp << line << "\n";
            continue;
        }

        Book book;
        std::stringstream ss(line);
        ss >> book.id;
        ss.ignore();
        std::getline(ss, book.title, '\t');
        std::getline(ss, book.author, '\t');
        std::getline(ss, book.genre, '\t');
        ss >> book.total_copies >> book.available_copies >> book.selling_price >> book.renting_price >> book.bought_price;

        int choice = getIntInput(
            "Update: 1:ID 2:Title 3:Author 4:Genre 5:Total 6:Available 7:Sell 8:Rent 9:Buy\nChoice: ");
        switch (choice)
        {
        case 1:
            book.id = getIntInput("Enter new ID: ");
            break;
        case 2:
            book.title = getStringInput("Enter new title: ");
            break;
        case 3:
            book.author = getStringInput("Enter new author: ");
            break;
        case 4:
            book.genre = getStringInput("Enter new genre: ");
            break;
        case 5:
            book.total_copies = getIntInput("Enter new total copies: ");
            break;
        case 6:
            book.available_copies = getIntInput("Enter new available copies: ");
            break;
        case 7:
            book.selling_price = getIntInput("Enter new selling price: ");
            break;
        case 8:
            book.renting_price = getIntInput("Enter new renting price: ");
            break;
        case 9:
            book.bought_price = getIntInput("Enter new bought price: ");
            break;
        default:
            std::cout << "Invalid choice\n";
            break;
        }

        temp << std::left
             << std::setw(10) << book.id
             << std::setw(30) << book.title
             << std::setw(30) << book.author
             << std::setw(20) << book.genre
             << std::setw(15) << book.total_copies
             << std::setw(15) << book.available_copies
             << std::setw(15) << book.selling_price
             << std::setw(15) << book.renting_price
             << std::setw(15) << book.bought_price
             << "\n";
    }

    file.close();
    temp.close();
    std::remove(filename.c_str());
    std::rename("temp.txt", filename.c_str());
    std::cout << "Record updated successfully.\n";
}

void deleteRecord(const std::string &filename, int line_no)
{
    std::ifstream file(filename);
    std::ofstream temp("temp.txt");
    std::string line;
    int count = 0;
    while (std::getline(file, line))
    {
        count++;
        if (count != line_no)
            temp << line << "\n";
    }
    file.close();
    temp.close();
    std::remove(filename.c_str());
    std::rename("temp.txt", filename.c_str());
    std::cout << "Record deleted successfully.\n";
}

int searchBook(const std::string &filename, const std::string &searchBy)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Cannot open file.\n";
        return -1;
    }

    std::string line;
    int line_no = 0;
    while (std::getline(file, line))
    {
        line_no++;
        if (line_no <= 2)
            continue; // skip header

        std::stringstream ss(line);
        Book book;
        ss >> book.id;
        ss.ignore(); // skip spacing

        std::getline(ss, book.title, '\t');  // title can contain spaces
        std::getline(ss, book.author, '\t'); // author can contain spaces
        std::getline(ss, book.genre, '\t');  // genre

        if (searchBy == "id")
        {
            int id = getIntInput("Enter book ID: ");
            if (book.id == id)
                return line_no;
        }
        else if (searchBy == "title")
        {
            std::string title = getStringInput("Enter book title: ");
            if (book.title == title)
                return line_no;
        }
        else if (searchBy == "genre")
        {
            std::string genre = getStringInput("Enter genre: ");
            if (book.genre == genre)
                return line_no;
        }
    }

    return -1;
}

void recommendBooks(const std::string &filename)
{
    std::string userGenre = getStringInput("Enter your preferred genre: ");
    std::ifstream file(filename);
    if (!file)
        return;

    Book b;
    bool found = false;
    std::cout << "Recommended books:\n";

    // file format per line:
    // id title author genre availableCopies sellingPrice rentingPrice
    while (file >> b.id >> b.title >> b.author >> b.genre >> b.available_copies >> b.selling_price >> b.renting_price)
    {
        if (b.genre == userGenre && b.available_copies > 0)
        {
            found = true;
            std::cout << "Book ID: " << b.id << '\n';
            std::cout << "Title: " << b.title << '\n';
            std::cout << "Author: " << b.author << '\n';
            std::cout << "Available Copies: " << b.available_copies << '\n';
            std::cout << "Selling Price: " << b.selling_price << '\n';
            std::cout << "Renting Price: " << b.renting_price << '\n';
            std::cout << "-----------------------------\n";
        }
    }
    if (!found)
    {
        std::cout << "No books found for this genre.\n";
    }
    file.close();
}

void saveUserTransaction(const User &user)
{
    std::ofstream file("users.txt", std::ios::app);
    if (!file)
        return;
    file << user.id << "\t" << user.fname << "\t" << user.lname
         << "\tBought: " << user.bought_amount
         << "\tBorrowed: " << user.borrowed_amount
         << "\tRented: " << user.rented_amount << "\n";
}

void runAsAdministrator(const std::string &filename)
{
    while (true)
    {
        int pass;
        pass = getIntInput("Enter administrator passcode: ");
        if (pass != 1234)
        {
            std::cout << "Wrong passcode. Try again.\n";
            continue;
        }

        std::cout << "Welcome, Administrator!\n";
        std::cout << "1. Add a book\n";
        std::cout << "2. Add multiple books\n";
        std::cout << "3. Update a book\n";
        std::cout << "4. Delete a book\n";
        std::cout << "5. Display all books\n";
        std::cout << "6. Exit\n";

        int choice = getIntInput("Choice: ");

        switch (choice)
        {
        case 1:
            addBook(filename);
            break;
        case 2:
        {
            int n = getIntInput("How many books? ");
            for (int i = 0; i < n; i++)
                addBook(filename);
            break;
        }
        case 3:
        {
            int line_no = searchBook(filename, "id");
            if (line_no != -1)
                updateRecord(filename, line_no);
            else
                std::cout << "Book not found.\n";
            break;
        }
        case 4:
        {
            int line_no = searchBook(filename, "id");
            if (line_no != -1)
                deleteRecord(filename, line_no);
            else
                std::cout << "Book not found.\n";
            break;
        }
        case 5:
            displayAll(filename);
            break;
        case 6:
            return;
        default:
            std::cout << "Invalid choice.\n";
        }
    }
}

void runAsUser(const std::string &filename)
{
    User user;
    user.id = getIntInput("Enter your user ID: ");
    user.fname = getStringInput("Enter your first name: ");
    user.lname = getStringInput("Enter your last name: ");
    user.bought_amount = 0;
    user.borrowed_amount = 0;
    user.rented_amount = 0;

    while (true)
    {
        std::cout << "User Menu:\n";
        std::cout << "1. Buy a book\n";
        std::cout << "2. Borrow a book\n";
        std::cout << "3. Return a book\n";
        std::cout << "4. Get book recommendations\n";
        std::cout << "5. Search for a book\n";
        std::cout << "6. Exit\n";

        int choice = getIntInput("Choice: ");

        int line_no;
        Book book;
        switch (choice)
        {
        case 1: // Buy
            line_no = searchBook(filename, "id");
            if (line_no != -1)
            {
                std::ifstream file(filename);
                std::string line;
                int count = 0;
                while (std::getline(file, line))
                {
                    count++;
                    if (count == line_no)
                    {
                        std::stringstream ss(line);
                        ss >> book.id;
                        ss.ignore();
                        std::getline(ss, book.title, '\t');
                        std::getline(ss, book.author, '\t');
                        std::getline(ss, book.genre, '\t');
                        ss >> book.total_copies >> book.available_copies >> book.selling_price >> book.renting_price >> book.bought_price;
                        break;
                    }
                }
                file.close();

                if (book.available_copies > 0)
                {
                    book.available_copies--;
                    std::cout << "You bought: " << book.title << "\n";
                    user.bought_amount++;

                    updateRecord(filename, line_no);
                }
                else
                    std::cout << "Book not available.\n";
            }
            else
                std::cout << "Book not found.\n";
            break;

        case 2: // Borrow
            line_no = searchBook(filename, "id");
            if (line_no != -1)
            {
                std::ifstream file(filename);
                std::string line;
                int count = 0;
                while (std::getline(file, line))
                {
                    count++;
                    if (count == line_no)
                    {
                        std::stringstream ss(line);
                        ss >> book.id;
                        ss.ignore();
                        std::getline(ss, book.title, '\t');
                        std::getline(ss, book.author, '\t');
                        std::getline(ss, book.genre, '\t');
                        ss >> book.total_copies >> book.available_copies >> book.selling_price >> book.renting_price >> book.bought_price;
                        break;
                    }
                }
                file.close();

                if (book.available_copies > 0)
                {
                    book.available_copies--;
                    std::cout << "You borrowed: " << book.title << "\n";
                    user.borrowed_amount++;

                    updateRecord(filename, line_no);
                }
                else
                    std::cout << "Book not available.\n";
            }
            else
                std::cout << "Book not found.\n";
            break;

        case 3: // Return
            line_no = searchBook(filename, "id");
            if (line_no != -1)
            {
                std::ifstream file(filename);
                std::string line;
                int count = 0;
                while (std::getline(file, line))
                {
                    count++;
                    if (count == line_no)
                    {
                        std::stringstream ss(line);
                        ss >> book.id;
                        ss.ignore();
                        std::getline(ss, book.title, '\t');
                        std::getline(ss, book.author, '\t');
                        std::getline(ss, book.genre, '\t');
                        ss >> book.total_copies >> book.available_copies >> book.selling_price >> book.renting_price >> book.bought_price;
                        break;
                    }
                }
                file.close();

                book.available_copies++;
                std::cout << "You returned: " << book.title << "\n";
                user.rented_amount++;

                updateRecord(filename, line_no);
            }
            else
                std::cout << "Book not found.\n";
            break;

        case 4:
            recommendBooks(filename);
            break;

        case 5:
        {
            line_no = searchBook(filename, "title");
            if (line_no != -1)
            {
                std::ifstream file(filename);
                std::string line;
                int count = 0;
                while (std::getline(file, line))
                {
                    count++;
                    if (count == line_no)
                    {
                        std::cout << "Book found: \n"
                                  << line << "\n";
                        break;
                    }
                }
                file.close();
            }
            else
                std::cout << "Book not found.\n";
            break;
        }

        case 6:
            saveUserTransaction(user);
            return;

        default:
            std::cout << "Invalid choice.\n";
        }
    }
}
