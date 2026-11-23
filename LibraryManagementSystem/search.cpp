/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 8: Global Search
 * ============================================================
 *  Module      : search.cpp
 *  Description : Implementation of the global search menu.
 *                Combines book search (Phase 4) and member
 *                search (Phase 5) into one screen.
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>

#include "search.h"
#include "menu.h"
#include "validation.h"

using namespace std;

// ==========================================================
//  Helpers
// ==========================================================
static string toLower(string s)
{
    for (size_t i = 0; i < s.length(); i++)
        s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

static bool containsIgnoreCase(const string& text, const string& pattern)
{
    return toLower(text).find(toLower(pattern)) != string::npos;
}

static void printBookHeader()
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

static void printMemberHeader()
{
    cout << "\n"
         << left
         << setw(9)  << "ID"
         << setw(20) << "Name"
         << setw(15) << "Phone"
         << setw(24) << "Email"
         << setw(10) << "Type"
         << "Limit" << "\n";
    cout << string(95, '-') << "\n";
}

static void printMemberRow(const Member* m)
{
    cout << left
         << setw(9)  << m->getMemberId()
         << setw(20) << m->getName()
         << setw(15) << m->getPhone()
         << setw(24) << m->getEmail()
         << setw(10) << m->getTypeName()
         << m->getBorrowLimit() << "\n";
}


// ==========================================================
//  MODULE ENTRY POINT - Search menu loop
// ==========================================================
void runSearchModule(const vector<Book>&    books,
                     const vector<Member*>& members)
{
    int choice;
    do
    {
        showSearchMenu();
        choice = readInt("Enter your choice (0-5): ", 0, 5);
        if (choice == 0)
        {
            cout << "\nReturning to Main Menu...\n";
            break;
        }

        bool found = false;
        string keyword;

        // ---------- Book searches ----------
        if (choice >= 1 && choice <= 3)
        {
            if (books.empty())
            {
                cout << "\nNo books to search.\n";
                pauseScreen();
                continue;
            }

            if (choice == 1)      keyword = readField("Enter Title keyword : ");
            else if (choice == 2) keyword = readField("Enter Author keyword: ");
            else                  keyword = readField("Enter Book ID        : ");

            printBookHeader();
            for (size_t i = 0; i < books.size(); i++)
            {
                bool match = false;
                if (choice == 1)
                    match = containsIgnoreCase(books[i].getTitle(), keyword);
                else if (choice == 2)
                    match = containsIgnoreCase(books[i].getAuthor(), keyword);
                else
                    match = (books[i].getBookId() == keyword); // exact ID

                if (match)
                {
                    printBookRow(books[i]);
                    found = true;
                }
            }
            if (found) cout << string(92, '-') << "\n";
        }
        // ---------- Member searches ----------
        else if (choice == 4 || choice == 5)
        {
            if (members.empty())
            {
                cout << "\nNo members to search.\n";
                pauseScreen();
                continue;
            }

            if (choice == 4) keyword = readField("Enter Name keyword: ");
            else             keyword = readField("Enter Member ID   : ");

            printMemberHeader();
            for (size_t i = 0; i < members.size(); i++)
            {
                bool match = false;
                if (choice == 4)
                    match = containsIgnoreCase(members[i]->getName(), keyword);
                else
                    match = (members[i]->getMemberId() == keyword);

                if (match)
                {
                    printMemberRow(members[i]);
                    found = true;
                }
            }
            if (found) cout << string(95, '-') << "\n";
        }

        if (!found)
            cout << ">> No result found for '" << keyword << "'.\n";

        pauseScreen();
    } while (choice != 0);
}
