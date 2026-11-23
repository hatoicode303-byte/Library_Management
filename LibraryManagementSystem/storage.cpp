/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 9: File Storage
 * ============================================================
 *  Module      : storage.cpp
 *  Description : Implementation of load / save / backup.
 *
 *  Design decisions:
 *    - One record per line, '|' as field separator
 *      (input already rejects '|' via readField)
 *    - Wrong/corrupt lines are SKIPPED, never crash
 *    - Members are rebuilt with the Phase 5 factory
 *      (createMember) so Student/Teacher polymorphism
 *      survives the round-trip through file
 *    - Dates stored as dd-mm-yyyy, parsed by parseDate()
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>   // atof, atoi

#include "storage.h"
#include "library.h" // FILE_* constants

using namespace std;

// ==========================================================
//  Internal helpers
// ==========================================================

// Splits a line into fields using the '|' separator
static vector<string> split(const string& line, char sep)
{
    vector<string> fields;
    string current = "";
    for (size_t i = 0; i < line.length(); i++)
    {
        if (line[i] == sep)
        {
            fields.push_back(current);
            current = "";
        }
        else
        {
            current += line[i];
        }
    }
    fields.push_back(current);
    return fields;
}

// True if the file exists and can be opened
static bool fileExists(const string& name)
{
    ifstream f(name.c_str());
    return f.good();
}


// ==========================================================
//  LOAD  (files -> memory)
// ==========================================================
void loadAllData(vector<Book>&         books,
                 vector<Member*>&      members,
                 vector<Transaction*>& transactions)
{
    // ---------- books.dat ----------
    if (fileExists(FILE_BOOKS))
    {
        ifstream fin(FILE_BOOKS.c_str());
        string line;
        while (getline(fin, line))
        {
            if (line.empty()) continue;
            vector<string> f = split(line, '|');
            if (f.size() != 6) continue;   // skip corrupt line

            books.push_back(Book(f[0], f[1], f[2], f[3],
                                 atof(f[4].c_str()),
                                 atoi(f[5].c_str())));
        }
        fin.close();
    }

    // ---------- members.dat ----------
    if (fileExists(FILE_MEMBERS))
    {
        ifstream fin(FILE_MEMBERS.c_str());
        string line;
        while (getline(fin, line))
        {
            if (line.empty()) continue;
            vector<string> f = split(line, '|');
            if (f.size() != 5) continue;   // skip corrupt line

            // Factory rebuilds Student / Teacher object (Phase 5)
            members.push_back(createMember(f[4], f[0], f[1], f[2], f[3]));
        }
        fin.close();
    }

    // ---------- transactions.dat ----------
    if (fileExists(FILE_TRANSACTIONS))
    {
        ifstream fin(FILE_TRANSACTIONS.c_str());
        string line;
        while (getline(fin, line))
        {
            if (line.empty()) continue;
            vector<string> f = split(line, '|');
            if (f.size() != 7) continue;   // skip corrupt line

            // f = bookId|memberId|issue|due|return|status|fine
            Transaction* t =
                new Transaction(f[0], f[1],
                                parseDate(f[2]), parseDate(f[3]));
            if (f[5] == "Returned")
                t->setReturned(parseDate(f[4]));
            t->setFine(atof(f[6].c_str()));
            transactions.push_back(t);
        }
        fin.close();
    }
}


// ==========================================================
//  SAVE  (memory -> files)
// ==========================================================
void saveAllData(const vector<Book>&         books,
                 const vector<Member*>&      members,
                 const vector<Transaction*>& transactions)
{
    // ---------- books.dat ----------
    {
        ofstream fout(FILE_BOOKS.c_str());   // truncates old data
        for (size_t i = 0; i < books.size(); i++)
        {
            fout << books[i].getBookId()   << '|'
                 << books[i].getTitle()    << '|'
                 << books[i].getAuthor()   << '|'
                 << books[i].getCategory() << '|'
                 << fixed << setprecision(2) << books[i].getPrice() << '|'
                 << books[i].getCopies()   << '\n';
        }
        fout.close();
    }

    // ---------- members.dat ----------
    {
        ofstream fout(FILE_MEMBERS.c_str());
        for (size_t i = 0; i < members.size(); i++)
        {
            fout << members[i]->getMemberId() << '|'
                 << members[i]->getName()     << '|'
                 << members[i]->getPhone()    << '|'
                 << members[i]->getEmail()    << '|'
                 << members[i]->getTypeName() << '\n';  // virtual!
        }
        fout.close();
    }

    // ---------- transactions.dat ----------
    {
        ofstream fout(FILE_TRANSACTIONS.c_str());
        for (size_t i = 0; i < transactions.size(); i++)
        {
            const Transaction* t = transactions[i];
            fout << t->getBookId()   << '|'
                 << t->getMemberId() << '|'
                 << formatDate(t->getIssueDate()) << '|'
                 << formatDate(t->getDueDate())   << '|'
                 << (t->isActive() ? "-"
                                   : formatDate(t->getReturnDate())) << '|'
                 << t->getStatus() << '|'
                 << fixed << setprecision(2) << t->getFine() << '\n';
        }
        fout.close();
    }
}


// ==========================================================
//  BACKUP (FR-12: safety copy of books before exit)
// ==========================================================
void backupBooks(const vector<Book>& books)
{
    ofstream fout(FILE_BOOKS_BACKUP.c_str());
    for (size_t i = 0; i < books.size(); i++)
    {
        fout << books[i].getBookId()   << '|'
             << books[i].getTitle()    << '|'
             << books[i].getAuthor()   << '|'
             << books[i].getCategory() << '|'
             << fixed << setprecision(2) << books[i].getPrice() << '|'
             << books[i].getCopies()   << '\n';
    }
    fout.close();
}
