/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 4: Book Module
 * ============================================================
 *  Module      : book.h
 *  Description : Book class (attributes + methods) and the
 *                entry function of the Book Management module.
 *
 *  OOP concepts used here:
 *    - Class & Object        : class Book
 *    - Encapsulation         : private attributes, public
 *                              getters / setters
 *    - Data structure        : vector<Book> holds all records
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>

class Book
{
private:
    // ---------- Attributes (encapsulated) ----------
    std::string bookId;      // Unique ID, e.g. "B101"
    std::string title;
    std::string author;
    std::string category;
    double       price;      // Rs.
    int          copies;     // Available copies in library

public:
    // ---------- Constructors ----------
    Book();
    Book(const std::string& id,
         const std::string& title,
         const std::string& author,
         const std::string& category,
         double price,
         int copies);

    // ---------- Getters ----------
    std::string getBookId()    const;
    std::string getTitle()     const;
    std::string getAuthor()    const;
    std::string getCategory()  const;
    double       getPrice()    const;
    int          getCopies()   const;

    // ---------- Setters ----------
    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setCategory(const std::string& category);
    void setPrice(double price);
    void setCopies(int copies);
};

// ---------- Module entry point (called from main menu) ----------
void runBookModule(std::vector<Book>& books);

#endif // BOOK_H
