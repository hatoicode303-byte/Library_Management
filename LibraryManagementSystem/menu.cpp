/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 3: Project Skeleton
 * ============================================================
 *  Module      : menu.cpp
 *  Description : Implementation of all menu display functions.
 *  Compiler    : Dev-C++ (MinGW g++)
 * ============================================================
 */

#include <iostream>
#include <string>

#include "menu.h"

using namespace std;

void showBanner()
{
    cout << "\n";
    cout << "###########################################################\n";
    cout << "#                                                         #\n";
    cout << "#      L I B R A R Y   M A N A G E M E N T   S Y S T E M  #\n";
    cout << "#                                                         #\n";
    cout << "#        A C++ Console Application - Project Demo        #\n";
    cout << "#                                                         #\n";
    cout << "###########################################################\n";
    cout << "\n";
}

void showMainMenu()
{
    cout << "\n============================================================\n";
    cout << "                      MAIN MENU\n";
    cout << "============================================================\n";
    cout << "   1. Book Management\n";
    cout << "   2. Member Management\n";
    cout << "   3. Issue Book\n";
    cout << "   4. Return Book\n";
    cout << "   5. Reports\n";
    cout << "   6. Search\n";
    cout << "   0. Exit\n";
    cout << "============================================================\n";
}

void showBookMenu()
{
    cout << "\n------------------------------------------------------------\n";
    cout << "                 BOOK MANAGEMENT\n";
    cout << "------------------------------------------------------------\n";
    cout << "   1. Add Book\n";
    cout << "   2. Display All Books\n";
    cout << "   3. Search Book\n";
    cout << "   4. Update Book\n";
    cout << "   5. Delete Book\n";
    cout << "   0. Back to Main Menu\n";
    cout << "------------------------------------------------------------\n";
}

void showMemberMenu()
{
    cout << "\n------------------------------------------------------------\n";
    cout << "                MEMBER MANAGEMENT\n";
    cout << "------------------------------------------------------------\n";
    cout << "   1. Register Member\n";
    cout << "   2. Display All Members\n";
    cout << "   3. Search Member\n";
    cout << "   4. Update Member\n";
    cout << "   5. Delete Member\n";
    cout << "   0. Back to Main Menu\n";
    cout << "------------------------------------------------------------\n";
}

void showReportsMenu()
{
    cout << "\n------------------------------------------------------------\n";
    cout << "                       REPORTS\n";
    cout << "------------------------------------------------------------\n";
    cout << "   1. Total Books Summary\n";
    cout << "   2. Issued Books List\n";
    cout << "   3. Overdue Books List\n";
    cout << "   4. Member-wise History\n";
    cout << "   0. Back to Main Menu\n";
    cout << "------------------------------------------------------------\n";
}

void showSearchMenu()
{
    cout << "\n------------------------------------------------------------\n";
    cout << "                        SEARCH\n";
    cout << "------------------------------------------------------------\n";
    cout << "   1. Search Book by Title\n";
    cout << "   2. Search Book by Author\n";
    cout << "   3. Search Book by ID\n";
    cout << "   4. Search Member by Name\n";
    cout << "   5. Search Member by ID\n";
    cout << "   0. Back to Main Menu\n";
    cout << "------------------------------------------------------------\n";
}

void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    string dummy;
    getline(cin, dummy);
}
