/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 4: Book Module
 * ============================================================
 *  Module      : book.cpp
 *  Description : Implementation of all Book Management
 *                operations:
 *                  1. Add Book        (with duplicate-ID check)
 *                  2. Display All Books (formatted table)
 *                  3. Search Book     (ID / title / author)
 *                  4. Update Book     (field by field)
 *                  5. Delete Book     (with confirmation)
 *
 *  NOTE: Records live in a vector<Book> in memory.
 *        Saving to file comes in Phase 9.
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>   // setw, left  -> table formatting
#include <cctype>    // tolower    -> case-insensitive search

#include "book.h"
#include "menu.h"
#include "validation.h"

using namespace std;

// ==========================================================
//  Book class - Constructors
// ==========================================================
Book::Book()
{
    bookId = "";
    title = "";
    author = "";
    category = "";
    price = 0.0;
    copies = 0;
}

Book::Book(const std::string& id,
           const std::string& t,
           const std::string& a,
           const std::string& c,
           double p,
           int cp)
{
    bookId   = id;
    title    = t;
    author   = a;
    category = c;
    price    = p;
    copies   = cp;
}

// ==========================================================
//  Getters
// ==========================================================
string Book::getBookId()   const { return bookId;   }
string Book::getTitle()    const { return title;    }
string Book::getAuthor()   const { return author;   }
string Book::getCategory() const { return category; }
double Book::getPrice()    const { return price;    }
int    Book::getCopies()   const { return copies;   }

// ==========================================================
//  Setters
// ==========================================================
void Book::setTitle(const std::string& t)    { title    = t; }
void Book::setAuthor(const std::string& a)   { author   = a; }
void Book::setCategory(const std::string& c) { category = c; }
void Book::setPrice(double p)                { price    = p; }
void Book::setCopies(int cp)                 { copies   = cp; }


// ==========================================================
//  Internal helpers (private to this module)
// ==========================================================

// Finds a book by exact ID. Returns index or -1 if not found.
static int findIndexById(const vector<Book>& books, const string& id)
{
    for (size_t i = 0; i < books.size(); i++)
        if (books[i].getBookId() == id)
            return (int)i;
    return -1;
}

// Converts a string to lower case (for case-insensitive search)
static string toLower(string s)
{
    for (size_t i = 0; i < s.length(); i++)
        s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

// True if 'text' contains 'pattern' (ignoring case)
static bool containsIgnoreCase(const string& text, const string& pattern)
{
    return toLower(text).find(toLower(pattern)) != string::npos;
}

// Prints the table header used by display / search
static void printBookTableHeader()
{
    cout << "\n"
         << left
         << setw(10) << "ID"
         << setw(28) << "Title"
         << setw(20) << "Author"
         << setw(16) << "Category"
         << setw(10) << "Price"
         << "Copies" << "\n";
    cout << string(92, '-') << "\n";
}

// Prints one row of the table
static void printBookRow(const Book& b)
{
    cout << left
         << setw(10) << b.getBookId()
         << setw(28) << b.getTitle()
         << setw(20) << b.getAuthor()
         << setw(16) << b.getCategory()
         << setw(10) << fixed << setprecision(2) << b.getPrice()
         << b.getCopies() << "\n";
}


// ==========================================================
//  1. ADD BOOK
// ==========================================================
static void addBook(vector<Book>& books)
{
    cout << "\n---------------- ADD NEW BOOK ----------------\n";

    string id = readField("Enter Book ID    : ");

    if (findIndexById(books, id) != -1)
    {
        cout << "\n>> ERROR: A book with ID '" << id
             << "' already exists. Duplicate IDs are not allowed!\n";
        pauseScreen();
        return;
    }

    string title    = readField("Enter Title      : ");
    string author   = readField("Enter Author     : ");
    string category = readField("Enter Category   : ");
    double price    = readDouble("Enter Price (Rs.): ", 0, 1000000);
    int    copies   = readInt("Enter Copies     : ", 1, 10000);

    books.push_back(Book(id, title, author, category, price, copies));

    cout << "\n>> Book added successfully!\n";
    pauseScreen();
}


// ==========================================================
//  2. DISPLAY ALL BOOKS
// ==========================================================
static void displayBooks(const vector<Book>& books)
{
    cout << "\n-------------- ALL BOOKS ---------------------\n";

    if (books.empty())
    {
        cout << "No books in the library yet.\n";
        pauseScreen();
        return;
    }

    printBookTableHeader();
    for (size_t i = 0; i < books.size(); i++)
        printBookRow(books[i]);

    cout << string(92, '-') << "\n";
    cout << "Total books: " << books.size() << "\n";
    pauseScreen();
}


// ==========================================================
//  3. SEARCH BOOK
// ==========================================================
static void searchBook(const vector<Book>& books)
{
    cout << "\n---------------- SEARCH BOOK -----------------\n";

    if (books.empty())
    {
        cout << "No books to search.\n";
        pauseScreen();
        return;
    }

    cout << "   1. Search by Book ID\n";
    cout << "   2. Search by Title\n";
    cout << "   3. Search by Author\n";
    cout << "   0. Cancel\n";

    int choice = readInt("Enter your choice (0-3): ", 0, 3);
    if (choice == 0) return;

    string keyword;
    bool found = false;

    if (choice == 1)
    {
        keyword = readField("Enter Book ID: ");
        int idx = findIndexById(books, keyword);
        if (idx != -1)
        {
            printBookTableHeader();
            printBookRow(books[idx]);
            cout << string(92, '-') << "\n";
            found = true;
        }
    }
    else if (choice == 2)
    {
        keyword = readField("Enter Title keyword: ");
        printBookTableHeader();
        for (size_t i = 0; i < books.size(); i++)
        {
            if (containsIgnoreCase(books[i].getTitle(), keyword))
            {
                printBookRow(books[i]);
                found = true;
            }
        }
        if (found) cout << string(92, '-') << "\n";
    }
    else // choice == 3
    {
        keyword = readField("Enter Author keyword: ");
        printBookTableHeader();
        for (size_t i = 0; i < books.size(); i++)
        {
            if (containsIgnoreCase(books[i].getAuthor(), keyword))
            {
                printBookRow(books[i]);
                found = true;
            }
        }
        if (found) cout << string(92, '-') << "\n";
    }

    if (!found)
        cout << ">> No book found matching '" << keyword << "'.\n";

    pauseScreen();
}


// ==========================================================
//  4. UPDATE BOOK
// ==========================================================
static void updateBook(vector<Book>& books)
{
    cout << "\n---------------- UPDATE BOOK -----------------\n";

    string id = readField("Enter Book ID of the book to update: ");
    int idx = findIndexById(books, id);

    if (idx == -1)
    {
        cout << ">> ERROR: No book found with ID '" << id << "'.\n";
        pauseScreen();
        return;
    }

    cout << "\nCurrent record:\n";
    printBookTableHeader();
    printBookRow(books[idx]);
    cout << string(92, '-') << "\n";

    cout << "   What do you want to update?\n";
    cout << "   1. Title\n";
    cout << "   2. Author\n";
    cout << "   3. Category\n";
    cout << "   4. Price\n";
    cout << "   5. Copies\n";
    cout << "   0. Cancel\n";

    int field = readInt("Enter your choice (0-5): ", 0, 5);

    switch (field)
    {
        case 0:
            cout << "Update cancelled.\n";
            break;
        case 1:
            books[idx].setTitle(readField("Enter new Title      : "));
            break;
        case 2:
            books[idx].setAuthor(readField("Enter new Author     : "));
            break;
        case 3:
            books[idx].setCategory(readField("Enter new Category   : "));
            break;
        case 4:
            books[idx].setPrice(readDouble("Enter new Price (Rs.): ", 0, 1000000));
            break;
        case 5:
            books[idx].setCopies(readInt("Enter new Copies     : ", 1, 10000));
            break;
    }

    if (field != 0)
    {
        cout << "\n>> Updated record:\n";
        printBookTableHeader();
        printBookRow(books[idx]);
        cout << string(92, '-') << "\n";
        cout << ">> Book updated successfully!\n";
    }

    pauseScreen();
}


// ==========================================================
//  5. DELETE BOOK
// ==========================================================
static void deleteBook(vector<Book>& books)
{
    cout << "\n---------------- DELETE BOOK -----------------\n";

    string id = readField("Enter Book ID of the book to delete: ");
    int idx = findIndexById(books, id);

    if (idx == -1)
    {
        cout << ">> ERROR: No book found with ID '" << id << "'.\n";
        pauseScreen();
        return;
    }

    cout << "\nYou are about to delete:\n";
    printBookTableHeader();
    printBookRow(books[idx]);
    cout << string(92, '-') << "\n";

    if (confirm("Are you sure you want to delete this book?"))
    {
        books.erase(books.begin() + idx);
        cout << ">> Book deleted successfully!\n";
    }
    else
    {
        cout << ">> Deletion cancelled.\n";
    }

    pauseScreen();
}


// ==========================================================
//  MODULE ENTRY POINT - Book Management menu loop
// ==========================================================
void runBookModule(vector<Book>& books)
{
    int choice;
    do
    {
        showBookMenu();
        choice = readInt("Enter your choice (0-5): ", 0, 5);

        switch (choice)
        {
            case 1: addBook(books);        break;
            case 2: displayBooks(books);   break;
            case 3: searchBook(books);     break;
            case 4: updateBook(books);     break;
            case 5: deleteBook(books);     break;
            case 0: cout << "\nReturning to Main Menu...\n"; break;
            default: break; // unreachable - readInt validates
        }
    } while (choice != 0);
}
