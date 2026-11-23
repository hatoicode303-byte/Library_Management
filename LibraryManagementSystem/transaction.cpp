/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 6: Issue & Return
 * ============================================================
 *  Module      : transaction.cpp
 *  Description : Implementation of:
 *                  - Date helpers (today, addDays, between)
 *                  - Transaction class
 *                  - issueBook()  : full validation chain
 *                  - returnBook() : updates copies + record
 *
 *  Issue validation chain (in order):
 *    1. Book must exist
 *    2. Book must have copies available
 *    3. Member must exist
 *    4. Member must not already hold this book
 *    5. Member must be below their borrow limit
 *       (limit comes from the POLYMORPHIC getBorrowLimit())
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <ctime>      // time, localtime -> today's date
#include <sstream>    // ostringstream -> format dates
#include <iomanip>    // setw, setfill -> table layout
#include <cstdio>     // sscanf -> parseDate()

#include "transaction.h"
#include "library.h"  // LOAN_PERIOD_DAYS
#include "menu.h"     // pauseScreen()
#include "validation.h"

using namespace std;

// ==========================================================
//  Date - implementation
// ==========================================================
Date::Date()              { day = 1; month = 1; year = 2000; }
Date::Date(int d, int m, int y) { day = d; month = m; year = y; }

// ==========================================================
//  DATE CONVERSION (the trickiest code in the project)
//
//  Idea: convert any date <-> a simple day count, so that
//  adding days or comparing dates becomes plain arithmetic.
//  Example:  23-11-2026  <->  20757 (some day number)
//            + 14 days   ->  20771  ->  07-12-2026
//
//  The formulas also handle leap years and month lengths
//  automatically (this is the standard civil-calendar
//  algorithm), so no if-rules for Feb/leap years are needed.
// ==========================================================

// Date  ->  day number
static long daysFromCivil(int y, int m, int d)
{
    y -= m <= 2;                                 // Jan/Feb belong to prev year
    const long era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = (unsigned)(y - era * 400);   // year of era (0..399)
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
                                                  // day of year (0..365)
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
                                                  // days in era + day of year
    return era * 146097 + (long)doe - 719468;    // full day count since 0000-03-01
}

// Day number  ->  date (reverse of the above)
static void civilFromDays(long z, int& y, int& m, int& d)
{
    z += 719468;                                 // shift epoch
    const long era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = (unsigned)(z - era * 146097);   // day of era
    const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
                                                  // year of era
    y = (int)yoe + (int)era * 400;
    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
                                                  // day of year
    const unsigned mp = (5 * doy + 2) / 153;      // month (0..11)
    d = (int)(doy - (153 * mp + 2) / 5 + 1);      // day of month
    m = (int)(mp + (mp < 10 ? 3 : -9));           // 0..11 -> 3..12, 1..2
    y += (m <= 2);                                // Jan/Feb -> next year
}

Date getToday()
{
    time_t t = time(0);
    struct tm* now = localtime(&t);
    return Date(now->tm_mday,
                now->tm_mon + 1,
                now->tm_year + 1900);
}

Date addDays(const Date& d, int n)
{
    long z = daysFromCivil(d.year, d.month, d.day) + n;
    int y, m, day;
    civilFromDays(z, y, m, day);
    return Date(day, m, y);
}

int daysBetween(const Date& from, const Date& to)
{
    return (int)(daysFromCivil(to.year, to.month, to.day)
               - daysFromCivil(from.year, from.month, from.day));
}

string formatDate(const Date& d)
{
    ostringstream out;
    out << setw(2) << setfill('0') << d.day   << "-"
        << setw(2) << setfill('0') << d.month << "-"
        << d.year;
    return out.str();
}

Date parseDate(const string& s)
{
    int d = 0, m = 0, y = 0;
    // Expected format "dd-mm-yyyy". Any other input gives
    // the default date instead of crashing.
    if (sscanf(s.c_str(), "%d-%d-%d", &d, &m, &y) == 3
        && d >= 1 && d <= 31 && m >= 1 && m <= 12 && y > 0)
        return Date(d, m, y);
    return Date();
}


// ==========================================================
//  Transaction - implementation
// ==========================================================
Transaction::Transaction()
{
    bookId = "";
    memberId = "";
    status = "Issued";
    fine = 0.0;
}

Transaction::Transaction(const std::string& bId,
                         const std::string& mId,
                         const Date& issue,
                         const Date& due)
{
    bookId   = bId;
    memberId = mId;
    issueDate = issue;
    dueDate   = due;
    status    = "Issued";
    fine      = 0.0;
}

string Transaction::getBookId()     const { return bookId;     }
string Transaction::getMemberId()   const { return memberId;   }
Date   Transaction::getIssueDate()  const { return issueDate;  }
Date   Transaction::getDueDate()    const { return dueDate;    }
Date   Transaction::getReturnDate() const { return returnDate; }
string Transaction::getStatus()     const { return status;     }
double Transaction::getFine()       const { return fine;       }

bool Transaction::isActive() const { return status == "Issued"; }

void Transaction::setReturned(const Date& on)
{
    returnDate = on;
    status = "Returned";
}

void Transaction::setFine(double amount) { fine = amount; }


// ==========================================================
//  Internal helpers
// ==========================================================
static int findBookIndex(const vector<Book>& books, const string& id)
{
    for (size_t i = 0; i < books.size(); i++)
        if (books[i].getBookId() == id)
            return (int)i;
    return -1;
}

static int findMemberIndex(const vector<Member*>& members, const string& id)
{
    for (size_t i = 0; i < members.size(); i++)
        if (members[i]->getMemberId() == id)
            return (int)i;
    return -1;
}

// Finds the ACTIVE transaction for this book + member
static int findActiveIndex(const vector<Transaction*>& transactions,
                           const string& bookId,
                           const string& memberId)
{
    for (size_t i = 0; i < transactions.size(); i++)
    {
        if (transactions[i]->isActive()
            && transactions[i]->getBookId() == bookId
            && transactions[i]->getMemberId() == memberId)
            return (int)i;
    }
    return -1;
}

// How many books does this member currently hold?
static int activeCountFor(const vector<Transaction*>& transactions,
                          const string& memberId)
{
    int count = 0;
    for (size_t i = 0; i < transactions.size(); i++)
        if (transactions[i]->isActive()
            && transactions[i]->getMemberId() == memberId)
            count++;
    return count;
}


// ==========================================================
//  ISSUE BOOK
// ==========================================================
void issueBook(vector<Book>&          books,
               vector<Member*>&       members,
               vector<Transaction*>&  transactions)
{
    cout << "\n================ ISSUE BOOK ==================\n";

    // ----- Step 1: book must exist -----
    string bookId = readField("Enter Book ID   : ");
    int bIdx = findBookIndex(books, bookId);
    if (bIdx == -1)
    {
        cout << ">> ERROR: No book found with ID '" << bookId << "'.\n";
        pauseScreen();
        return;
    }

    // ----- Step 2: copies must be available -----
    if (books[bIdx].getCopies() <= 0)
    {
        cout << ">> ERROR: '" << books[bIdx].getTitle()
             << "' has no copies available.\n";
        pauseScreen();
        return;
    }

    // ----- Step 3: member must exist -----
    string memberId = readField("Enter Member ID : ");
    int mIdx = findMemberIndex(members, memberId);
    if (mIdx == -1)
    {
        cout << ">> ERROR: No member found with ID '" << memberId << "'.\n";
        pauseScreen();
        return;
    }

    // ----- Step 4: member must not already hold this book -----
    if (findActiveIndex(transactions, bookId, memberId) != -1)
    {
        cout << ">> ERROR: This member already holds this book.\n";
        pauseScreen();
        return;
    }

    // ----- Step 5: borrow limit (POLYMORPHISM in action) -----
    int held    = activeCountFor(transactions, memberId);
    int limit   = members[mIdx]->getBorrowLimit();   // virtual call!
    if (held >= limit)
    {
        cout << ">> ERROR: Borrow limit reached. "
             << members[mIdx]->getName() << " (" 
             << members[mIdx]->getTypeName()
             << ") already holds " << held
             << " of " << limit << " allowed books.\n";
        pauseScreen();
        return;
    }

    // ----- All checks passed: create the transaction -----
    Date issue = getToday();
    Date due   = addDays(issue, LOAN_PERIOD_DAYS);

    transactions.push_back(
        new Transaction(bookId, memberId, issue, due));

    books[bIdx].setCopies(books[bIdx].getCopies() - 1);

    // ----- Issue receipt -----
    cout << "\n------------ ISSUE RECEIPT ------------------\n";
    cout << "  Book     : " << books[bIdx].getBookId()
         << " - " << books[bIdx].getTitle() << "\n";
    cout << "  Member   : " << members[mIdx]->getMemberId()
         << " - " << members[mIdx]->getName()
         << " (" << members[mIdx]->getTypeName() << ")\n";
    cout << "  Issued on: " << formatDate(issue) << "\n";
    cout << "  Due date : " << formatDate(due)
         << "  (" << LOAN_PERIOD_DAYS << " days)\n";
    cout << "  Copies left: " << books[bIdx].getCopies() << "\n";
    cout << "---------------------------------------------\n";
    cout << ">> Book issued successfully!\n";
    pauseScreen();
}


// ==========================================================
//  RETURN BOOK
// ==========================================================
void returnBook(vector<Book>&          books,
                vector<Member*>&       members,
                vector<Transaction*>&  transactions)
{
    cout << "\n=============== RETURN BOOK ==================\n";

    string bookId = readField("Enter Book ID   : ");
    string memberId = readField("Enter Member ID : ");

    int tIdx = findActiveIndex(transactions, bookId, memberId);
    if (tIdx == -1)
    {
        cout << ">> ERROR: No active issue found for this book "
             << "and member combination.\n";
        pauseScreen();
        return;
    }

    // Show what is being returned
    int bIdx = findBookIndex(books, bookId);
    int mIdx = findMemberIndex(members, memberId);

    cout << "\n------------ RETURN DETAILS -----------------\n";
    if (bIdx != -1)
        cout << "  Book     : " << books[bIdx].getBookId()
             << " - " << books[bIdx].getTitle() << "\n";
    if (mIdx != -1)
        cout << "  Member   : " << members[mIdx]->getMemberId()
             << " - " << members[mIdx]->getName() << "\n";
    cout << "  Issued on: " << formatDate(transactions[tIdx]->getIssueDate()) << "\n";
    cout << "  Due date : " << formatDate(transactions[tIdx]->getDueDate()) << "\n";
    cout << "---------------------------------------------\n";

    if (!confirm("Confirm return of this book?"))
    {
        cout << ">> Return cancelled.\n";
        pauseScreen();
        return;
    }

    // ----- Update the transaction -----
    Date today = getToday();
    transactions[tIdx]->setReturned(today);

    // ----- Give the copy back to the library -----
    if (bIdx != -1)
        books[bIdx].setCopies(books[bIdx].getCopies() + 1);

    // ==========================================================
    //  FINE CALCULATION (Phase 7)
    //    - Due date = issue date + LOAN_PERIOD_DAYS
    //    - Fine     = late days x FINE_PER_DAY
    //    - Returning ON or BEFORE the due date = no fine
    // ==========================================================
    int lateDays = daysBetween(transactions[tIdx]->getDueDate(), today);
    double fine = 0.0;
    if (lateDays > 0)
    {
        fine = lateDays * FINE_PER_DAY;
        transactions[tIdx]->setFine(fine);   // store on record
    }

    cout << "\n--------------- FINE SLIP -------------------\n";
    cout << "  Due date    : " << formatDate(transactions[tIdx]->getDueDate()) << "\n";
    cout << "  Returned on : " << formatDate(today) << "\n";

    if (lateDays > 0)
    {
        cout << "  Days late   : " << lateDays << "\n";
        cout << "  Fine rate   : Rs." << fixed << setprecision(2)
             << FINE_PER_DAY << " per day\n";
        cout << "  -------------------------------------\n";
        cout << "  TOTAL FINE  : Rs." << fine << "\n";
    }
    else
    {
        int daysEarly = -lateDays;   // lateDays <= 0 here
        if (lateDays == 0)
            cout << "  Status      : Returned ON the due date\n";
        else
            cout << "  Status      : Returned " << daysEarly
                 << " day(s) early\n";
        cout << "  TOTAL FINE  : Rs.0.00\n";
    }
    cout << "---------------------------------------------\n";

    cout << "\n>> Book returned successfully!\n";
    if (lateDays > 0)
        cout << ">> OVERDUE by " << lateDays
             << " day(s). Fine collected: Rs." << fixed
             << setprecision(2) << fine << "\n";
    else
        cout << ">> Returned on time. No fine.\n";

    cout << "  Copies now available: ";
    if (bIdx != -1) cout << books[bIdx].getCopies();
    cout << "\n";

    pauseScreen();
}
