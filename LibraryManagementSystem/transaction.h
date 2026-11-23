/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 6: Issue & Return
 * ============================================================
 *  Module      : transaction.h
 *  Description : Date helper struct + Transaction class +
 *                the two circulation operations.
 *
 *  OOP concepts demonstrated HERE:
 *    - Class & Object     : class Transaction
 *    - Encapsulation      : private attributes + methods
 *    - Composition        : Transaction USES a Date struct
 *    - Reuse of Phase 5   : borrow limit via polymorphism
 *
 *  Business rules:
 *    - Due date = issue date + LOAN_PERIOD_DAYS (14)
 *    - Copies decrement on issue, increment on return
 *    - A member cannot exceed their borrow limit
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>

#include "book.h"
#include "member.h"

// ----------------------------------------------------------
//  Simple date structure (day / month / year)
// ----------------------------------------------------------
struct Date
{
    int day;
    int month;
    int year;

    Date();
    Date(int day, int month, int year);
};

// ----- Date helper functions -----
Date        getToday();                          // system date
Date        addDays(const Date& d, int n);       // d + n days
int         daysBetween(const Date& from,
                        const Date& to);         // to - from
std::string formatDate(const Date& d);           // "dd-mm-yyyy"
Date        parseDate(const std::string& s);     // "dd-mm-yyyy" -> Date
                                                        // (invalid -> default)

// ----------------------------------------------------------
//  Transaction (an issue / return record)
// ----------------------------------------------------------
class Transaction
{
private:
    std::string bookId;
    std::string memberId;
    Date        issueDate;
    Date        dueDate;
    Date        returnDate;
    std::string status;      // "Issued" or "Returned"
    double      fine;        // calculated in Phase 7

public:
    Transaction();
    Transaction(const std::string& bookId,
                const std::string& memberId,
                const Date& issueDate,
                const Date& dueDate);

    // ----- Getters -----
    std::string getBookId()     const;
    std::string getMemberId()   const;
    Date        getIssueDate()  const;
    Date        getDueDate()    const;
    Date        getReturnDate() const;
    std::string getStatus()     const;
    double      getFine()       const;

    // ----- Behaviour -----
    bool isActive() const;                 // status == "Issued"
    void setReturned(const Date& on);      // mark as returned
    void setFine(double amount);
};

// ----------------------------------------------------------
//  Operations (called directly from the main menu)
// ----------------------------------------------------------
void issueBook(std::vector<Book>&          books,
               std::vector<Member*>&       members,
               std::vector<Transaction*>&  transactions);

void returnBook(std::vector<Book>&          books,
                std::vector<Member*>&       members,
                std::vector<Transaction*>&  transactions);

#endif // TRANSACTION_H
