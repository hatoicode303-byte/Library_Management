/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 8: Reports
 * ============================================================
 *  Module      : reports.cpp
 *  Description : Implementation of the four reports.
 *
 *  Report logic:
 *    1. Summary  - counts computed live from all vectors
 *    2. Issued   - transactions with status "Issued"
 *    3. Overdue  - active AND today > due date
 *                   fine shown = days late x FINE_PER_DAY
 *    4. History  - every transaction of one member
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "reports.h"
#include "library.h"   // LOAN_PERIOD_DAYS, FINE_PER_DAY
#include "menu.h"      // pauseScreen()
#include "validation.h"

using namespace std;

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
//  REPORT 1 - Total Books / Library Summary
// ==========================================================
static void reportSummary(const vector<Book>&         books,
                          const vector<Member*>&      members,
                          const vector<Transaction*>& transactions)
{
    cout << "\n=========== LIBRARY SUMMARY ==================\n";

    int totalTitles = (int)books.size();
    int totalCopies = 0;
    int issuedCopies = 0;
    for (size_t i = 0; i < books.size(); i++)
    {
        totalCopies  += books[i].getCopies();
        // copies field = currently AVAILABLE copies
    }
    for (size_t i = 0; i < transactions.size(); i++)
        if (transactions[i]->isActive())
            issuedCopies++;

    int totalMembers = (int)members.size();
    int students = 0, teachers = 0;
    for (size_t i = 0; i < members.size(); i++)
    {
        if (members[i]->getTypeName() == "Student") students++;
        else teachers++;
    }

    double totalValue = 0.0;
    for (size_t i = 0; i < books.size(); i++)
        totalValue += books[i].getPrice() * books[i].getCopies();

    cout << "  Book titles in library : " << totalTitles << "\n";
    cout << "  Copies available       : " << totalCopies << "\n";
    cout << "  Copies currently issued: " << issuedCopies << "\n";
    cout << "  ------------------------------------\n";
    cout << "  Total members          : " << totalMembers
         << "  (Students: " << students
         << ", Teachers: " << teachers << ")\n";
    cout << "  ------------------------------------\n";
    cout << "  Library book value     : Rs." << fixed
         << setprecision(2) << totalValue << "\n";
    cout << "=============================================\n";

    pauseScreen();
}


// ==========================================================
//  REPORT 2 - Issued Books List
// ==========================================================
static void reportIssued(const vector<Book>&         books,
                         const vector<Member*>&      members,
                         const vector<Transaction*>& transactions)
{
    cout << "\n=========== ISSUED BOOKS =====================\n";

    bool any = false;
    cout << left
         << setw(9)  << "Book ID"
         << setw(26) << "Book Title"
         << setw(9)  << "Member"
         << setw(16) << "Member Name"
         << setw(13) << "Issued"
         << "Due" << "\n";
    cout << string(90, '-') << "\n";

    for (size_t i = 0; i < transactions.size(); i++)
    {
        if (!transactions[i]->isActive()) continue;
        any = true;

        int bIdx = findBookIndex(books, transactions[i]->getBookId());
        int mIdx = findMemberIndex(members, transactions[i]->getMemberId());

        cout << left
             << setw(9)  << transactions[i]->getBookId()
             << setw(26) << (bIdx != -1 ? books[bIdx].getTitle() : "?")
             << setw(9)  << transactions[i]->getMemberId()
             << setw(16) << (mIdx != -1 ? members[mIdx]->getName() : "?")
             << setw(13) << formatDate(transactions[i]->getIssueDate())
             << formatDate(transactions[i]->getDueDate()) << "\n";
    }

    if (!any)
        cout << "  No books are currently issued.\n";
    else
        cout << string(90, '-') << "\n";

    pauseScreen();
}


// ==========================================================
//  REPORT 3 - Overdue Books List (with live fine)
// ==========================================================
static void reportOverdue(const vector<Book>&         books,
                          const vector<Member*>&      members,
                          const vector<Transaction*>& transactions)
{
    cout << "\n=========== OVERDUE BOOKS ====================\n";
    cout << "  (today = " << formatDate(getToday())
         << ", fine = Rs." << FINE_PER_DAY << "/day)\n\n";

    bool any = false;
    cout << left
         << setw(9)  << "Book ID"
         << setw(24) << "Book Title"
         << setw(9)  << "Member"
         << setw(15) << "Name"
         << setw(13) << "Due"
         << setw(8)  << "Days"
         << "Fine" << "\n";
    cout << string(86, '-') << "\n";

    Date today = getToday();

    for (size_t i = 0; i < transactions.size(); i++)
    {
        if (!transactions[i]->isActive()) continue;

        int lateDays = daysBetween(transactions[i]->getDueDate(), today);
        if (lateDays <= 0) continue;   // not overdue yet

        any = true;
        int bIdx = findBookIndex(books, transactions[i]->getBookId());
        int mIdx = findMemberIndex(members, transactions[i]->getMemberId());
        double fine = lateDays * FINE_PER_DAY;

        cout << left
             << setw(9)  << transactions[i]->getBookId()
             << setw(24) << (bIdx != -1 ? books[bIdx].getTitle() : "?")
             << setw(9)  << transactions[i]->getMemberId()
             << setw(15) << (mIdx != -1 ? members[mIdx]->getName() : "?")
             << setw(13) << formatDate(transactions[i]->getDueDate())
             << setw(8)  << lateDays
             << "Rs." << fixed << setprecision(2) << fine << "\n";
    }

    if (!any)
        cout << "  Great! No books are overdue.\n";
    else
        cout << string(86, '-') << "\n";

    pauseScreen();
}


// ==========================================================
//  REPORT 4 - Member-wise History
// ==========================================================
static void reportMemberHistory(const vector<Member*>&      members,
                                const vector<Transaction*>& transactions)
{
    cout << "\n======== MEMBER-WISE HISTORY =================\n";

    if (members.empty())
    {
        cout << "  No members registered.\n";
        pauseScreen();
        return;
    }

    string memberId = readField("Enter Member ID : ");
    int mIdx = findMemberIndex(members, memberId);
    if (mIdx == -1)
    {
        cout << ">> ERROR: No member found with ID '"
             << memberId << "'.\n";
        pauseScreen();
        return;
    }

    cout << "\n  Member : " << members[mIdx]->getMemberId()
         << " - " << members[mIdx]->getName()
         << " (" << members[mIdx]->getTypeName()
         << ", limit " << members[mIdx]->getBorrowLimit() << ")\n";
    cout << "  Currently holding : "
         << activeCountFor(transactions, memberId) << " book(s)\n";
    cout << string(70, '-') << "\n";

    cout << left
         << setw(9)  << "Book ID"
         << setw(13) << "Issued"
         << setw(13) << "Due"
         << setw(13) << "Returned"
         << setw(11) << "Status"
         << "Fine" << "\n";
    cout << string(70, '-') << "\n";

    bool any = false;
    for (size_t i = 0; i < transactions.size(); i++)
    {
        if (transactions[i]->getMemberId() != memberId) continue;
        any = true;

        cout << left
             << setw(9)  << transactions[i]->getBookId()
             << setw(13) << formatDate(transactions[i]->getIssueDate())
             << setw(13) << formatDate(transactions[i]->getDueDate())
             << setw(13) << (transactions[i]->isActive()
                             ? "-"
                             : formatDate(transactions[i]->getReturnDate()))
             << setw(11) << transactions[i]->getStatus()
             << "Rs." << fixed << setprecision(2)
             << transactions[i]->getFine() << "\n";
    }

    if (!any)
        cout << "  This member has no borrowing history.\n";
    else
        cout << string(70, '-') << "\n";

    pauseScreen();
}


// ==========================================================
//  MODULE ENTRY POINT - Reports menu loop
// ==========================================================
void runReportsModule(const vector<Book>&         books,
                      const vector<Member*>&      members,
                      const vector<Transaction*>& transactions)
{
    int choice;
    do
    {
        showReportsMenu();
        choice = readInt("Enter your choice (0-4): ", 0, 4);

        switch (choice)
        {
            case 1: reportSummary(books, members, transactions);   break;
            case 2: reportIssued(books, members, transactions);    break;
            case 3: reportOverdue(books, members, transactions);   break;
            case 4: reportMemberHistory(members, transactions);    break;
            case 0: cout << "\nReturning to Main Menu...\n";       break;
            default: break;
        }
    } while (choice != 0);
}
