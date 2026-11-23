/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 3: Project Skeleton
 * ============================================================
 *  Module      : main.cpp
 *  Description : Entry point of the program. Shows the main
 *                menu loop and routes the user to each module.
 *
 *  Phases completed here : Phase 3 (Skeleton)
 *  Next phases plug in    : Phase 4  -> Book Management
 *                           Phase 5  -> Member Management
 *                           Phase 6  -> Issue / Return
 *                           Phase 7  -> Fine calculation
 *                           Phase 8  -> Reports / Search
 *                           Phase 9  -> File storage
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>

#include "library.h"     // Project constants (files, rules)
#include "menu.h"        // Menu display functions
#include "validation.h"  // Safe input functions
#include "book.h"        // Book class + Book Module (Phase 4)
#include "member.h"      // Member hierarchy + Module (Phase 5)
#include "transaction.h" // Date + Transaction + Issue/Return (Phase 6)
#include "reports.h"     // Reports module (Phase 8)
#include "search.h"      // Global search module (Phase 8)
#include "storage.h"     // File storage / persistence (Phase 9)

using namespace std;

// ----------------------------------------------------------
// Shows a message for features not yet implemented.
// Each message names the phase that will implement it.
// ----------------------------------------------------------
void showPlaceholder(const string& feature, int phase)
{
    cout << "\n>> [" << feature << "] will be implemented in Phase "
         << phase << ".\n";
    pauseScreen();
}

// ----------------------------------------------------------
// MAIN - program entry point
// ----------------------------------------------------------
int main()
{
    showBanner();

    // ---------- In-memory records (saved to file in Phase 9) ----------
    vector<Book>          books;
    vector<Member*>       members;   // pointers -> polymorphism works
    vector<Transaction*>  transactions;

    // ---------- Load existing data from files (Phase 9) ----------
    loadAllData(books, members, transactions);
    cout << "  Loaded: " << books.size() << " book(s), "
         << members.size() << " member(s), "
         << transactions.size() << " transaction(s) from disk.\n";

    int choice;
    do
    {
        showMainMenu();
        choice = readInt("Enter your choice (0-6): ", 0, 6);

        switch (choice)
        {
            case 1:
                runBookModule(books);   // Phase 4 - fully implemented
                break;
            case 2:
                runMemberModule(members);  // Phase 5 - fully implemented
                break;
            case 3:
                issueBook(books, members, transactions);   // Phase 6
                break;
            case 4:
                returnBook(books, members, transactions);  // Phase 6
                break;
            case 5:
                runReportsModule(books, members, transactions); // Phase 8
                break;
            case 6:
                runSearchModule(books, members);               // Phase 8
                break;
            case 0:
                cout << "\n============================================================\n";
                cout << "  Saving data ...        (File Storage - Phase 9)\n";
                saveAllData(books, members, transactions);
                backupBooks(books);   // FR-12: safety copy
                cout << "  Backup created: " << FILE_BOOKS_BACKUP << "\n";
                cout << "  Thank you for using the Library Management System!\n";
                cout << "  Goodbye!\n";
                cout << "============================================================\n";
                break;
            default:
                // Unreachable - readInt already validates the range
                break;
        }

        // ---------- Persist after EVERY action (Phase 9) ----------
        if (choice != 0)
            saveAllData(books, members, transactions);
    } while (choice != 0);

    // ---------- Clean up dynamically created records ----------
    for (size_t i = 0; i < members.size(); i++)
        delete members[i];
    for (size_t i = 0; i < transactions.size(); i++)
        delete transactions[i];

    return 0;
}
