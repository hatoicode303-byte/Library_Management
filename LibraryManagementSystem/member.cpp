/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 5: Member Module
 * ============================================================
 *  Module      : member.cpp
 *  Description : Implementation of all Member Management
 *                operations:
 *                  1. Register Member  (duplicate-ID check)
 *                  2. Display All Members (formatted table)
 *                  3. Search Member    (ID / name)
 *                  4. Update Member    (name/phone/email/type)
 *                  5. Delete Member    (with confirmation)
 *
 *  NOTE: Members are stored as vector<Member*> (base-class
 *        pointers) so that polymorphism works: the actual
 *        objects are StudentMember or TeacherMember.
 *        Saving to file comes in Phase 9.
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>   // setw, left  -> table formatting
#include <cctype>    // tolower    -> case-insensitive search

#include "member.h"
#include "menu.h"
#include "validation.h"

using namespace std;

// ==========================================================
//  Member - base class implementation
// ==========================================================
Member::Member()
{
    memberId = "";
    name = "";
    phone = "";
    email = "";
}

Member::Member(const std::string& id,
               const std::string& n,
               const std::string& p,
               const std::string& e)
{
    memberId = id;
    name     = n;
    phone    = p;
    email    = e;
}

// Virtual destructor: required so derived objects are
// destroyed correctly when deleted through a Member*.
Member::~Member()
{
    // nothing extra to clean up, but it MUST be virtual
}

string Member::getMemberId() const { return memberId; }
string Member::getName()     const { return name;     }
string Member::getPhone()    const { return phone;    }
string Member::getEmail()    const { return email;    }

void Member::setName(const std::string& n)  { name  = n; }
void Member::setPhone(const std::string& p) { phone = p; }
void Member::setEmail(const std::string& e) { email = e; }


// ==========================================================
//  StudentMember - borrow limit 3
// ==========================================================
StudentMember::StudentMember(const std::string& id,
                             const std::string& n,
                             const std::string& p,
                             const std::string& e)
    : Member(id, n, p, e)   // calls base class constructor
{
}

int         StudentMember::getBorrowLimit() const { return 3; }
std::string StudentMember::getTypeName()    const { return "Student"; }


// ==========================================================
//  TeacherMember - borrow limit 5
// ==========================================================
TeacherMember::TeacherMember(const std::string& id,
                             const std::string& n,
                             const std::string& p,
                             const std::string& e)
    : Member(id, n, p, e)   // calls base class constructor
{
}

int         TeacherMember::getBorrowLimit() const { return 5; }
std::string TeacherMember::getTypeName()    const { return "Teacher"; }


// ==========================================================
//  Factory function
// ==========================================================
Member* createMember(const std::string& type,
                     const std::string& id,
                     const std::string& name,
                     const std::string& phone,
                     const std::string& email)
{
    if (type == "Teacher")
        return new TeacherMember(id, name, phone, email);
    // default: Student
    return new StudentMember(id, name, phone, email);
}


// ==========================================================
//  Internal helpers (private to this module)
// ==========================================================

// Finds a member by exact ID. Returns index or -1.
static int findIndexById(const vector<Member*>& members, const string& id)
{
    for (size_t i = 0; i < members.size(); i++)
        if (members[i]->getMemberId() == id)
            return (int)i;
    return -1;
}

// Converts a string to lower case (case-insensitive search)
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

// Table header for members
static void printMemberTableHeader()
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

// One row of the members table.
// Note: getBorrowLimit() / getTypeName() are VIRTUAL - the
// correct version runs for Student or Teacher automatically.
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
//  1. REGISTER MEMBER
// ==========================================================
static void registerMember(vector<Member*>& members)
{
    cout << "\n-------------- REGISTER MEMBER ---------------\n";

    string id = readField("Enter Member ID : ");

    if (findIndexById(members, id) != -1)
    {
        cout << "\n>> ERROR: A member with ID '" << id
             << "' already exists. Duplicate IDs are not allowed!\n";
        pauseScreen();
        return;
    }

    string name  = readField("Enter Name      : ");
    string phone = readField("Enter Phone     : ");
    string email = readField("Enter Email     : ");

    cout << "   Member Type:\n";
    cout << "   1. Student  (borrow up to 3 books)\n";
    cout << "   2. Teacher  (borrow up to 5 books)\n";

    int typeChoice = readInt("Enter your choice (1-2): ", 1, 2);
    string type = (typeChoice == 2) ? "Teacher" : "Student";

    // Factory creates the CORRECT derived object
    members.push_back(createMember(type, id, name, phone, email));

    cout << "\n>> Member registered successfully! ("
         << type << ", limit "
         << members.back()->getBorrowLimit() << " books)\n";
    pauseScreen();
}


// ==========================================================
//  2. DISPLAY ALL MEMBERS
// ==========================================================
static void displayMembers(const vector<Member*>& members)
{
    cout << "\n------------ ALL MEMBERS --------------------\n";

    if (members.empty())
    {
        cout << "No members registered yet.\n";
        pauseScreen();
        return;
    }

    printMemberTableHeader();
    for (size_t i = 0; i < members.size(); i++)
        printMemberRow(members[i]);

    cout << string(95, '-') << "\n";
    cout << "Total members: " << members.size() << "\n";
    pauseScreen();
}


// ==========================================================
//  3. SEARCH MEMBER
// ==========================================================
static void searchMember(const vector<Member*>& members)
{
    cout << "\n-------------- SEARCH MEMBER -----------------\n";

    if (members.empty())
    {
        cout << "No members to search.\n";
        pauseScreen();
        return;
    }

    cout << "   1. Search by Member ID\n";
    cout << "   2. Search by Name\n";
    cout << "   0. Cancel\n";

    int choice = readInt("Enter your choice (0-2): ", 0, 2);
    if (choice == 0) return;

    string keyword;
    bool found = false;

    if (choice == 1)
    {
        keyword = readField("Enter Member ID: ");
        int idx = findIndexById(members, keyword);
        if (idx != -1)
        {
            printMemberTableHeader();
            printMemberRow(members[idx]);
            cout << string(95, '-') << "\n";
            found = true;
        }
    }
    else // search by name
    {
        keyword = readField("Enter Name keyword: ");
        printMemberTableHeader();
        for (size_t i = 0; i < members.size(); i++)
        {
            if (containsIgnoreCase(members[i]->getName(), keyword))
            {
                printMemberRow(members[i]);
                found = true;
            }
        }
        if (found) cout << string(95, '-') << "\n";
    }

    if (!found)
        cout << ">> No member found matching '" << keyword << "'.\n";

    pauseScreen();
}


// ==========================================================
//  4. UPDATE MEMBER
// ==========================================================
static void updateMember(vector<Member*>& members)
{
    cout << "\n-------------- UPDATE MEMBER -----------------\n";

    string id = readField("Enter Member ID of the member to update: ");
    int idx = findIndexById(members, id);

    if (idx == -1)
    {
        cout << ">> ERROR: No member found with ID '" << id << "'.\n";
        pauseScreen();
        return;
    }

    cout << "\nCurrent record:\n";
    printMemberTableHeader();
    printMemberRow(members[idx]);
    cout << string(95, '-') << "\n";

    cout << "   What do you want to update?\n";
    cout << "   1. Name\n";
    cout << "   2. Phone\n";
    cout << "   3. Email\n";
    cout << "   4. Member Type (Student / Teacher)\n";
    cout << "   0. Cancel\n";

    int field = readInt("Enter your choice (0-4): ", 0, 4);

    switch (field)
    {
        case 0:
            cout << "Update cancelled.\n";
            break;
        case 1:
            members[idx]->setName(readField("Enter new Name : "));
            break;
        case 2:
            members[idx]->setPhone(readField("Enter new Phone: "));
            break;
        case 3:
            members[idx]->setEmail(readField("Enter new Email: "));
            break;
        case 4:
        {
            // Changing type = replacing the object with the
            // other derived class (same ID and details).
            cout << "   New Type:\n";
            cout << "   1. Student  (limit 3)\n";
            cout << "   2. Teacher  (limit 5)\n";
            int t = readInt("Enter your choice (1-2): ", 1, 2);
            string newType = (t == 2) ? "Teacher" : "Student";

            Member* updated = createMember(newType,
                                           members[idx]->getMemberId(),
                                           members[idx]->getName(),
                                           members[idx]->getPhone(),
                                           members[idx]->getEmail());
            delete members[idx];          // remove old object
            members[idx] = updated;       // keep same position
            break;
        }
    }

    if (field != 0)
    {
        cout << "\n>> Updated record:\n";
        printMemberTableHeader();
        printMemberRow(members[idx]);
        cout << string(95, '-') << "\n";
        cout << ">> Member updated successfully!\n";
    }

    pauseScreen();
}


// ==========================================================
//  5. DELETE MEMBER
// ==========================================================
static void deleteMember(vector<Member*>& members)
{
    cout << "\n-------------- DELETE MEMBER -----------------\n";

    string id = readField("Enter Member ID of the member to delete: ");
    int idx = findIndexById(members, id);

    if (idx == -1)
    {
        cout << ">> ERROR: No member found with ID '" << id << "'.\n";
        pauseScreen();
        return;
    }

    cout << "\nYou are about to delete:\n";
    printMemberTableHeader();
    printMemberRow(members[idx]);
    cout << string(95, '-') << "\n";

    if (confirm("Are you sure you want to delete this member?"))
    {
        delete members[idx];                       // free memory
        members.erase(members.begin() + idx);
        cout << ">> Member deleted successfully!\n";
    }
    else
    {
        cout << ">> Deletion cancelled.\n";
    }

    pauseScreen();
}


// ==========================================================
//  MODULE ENTRY POINT - Member Management menu loop
// ==========================================================
void runMemberModule(vector<Member*>& members)
{
    int choice;
    do
    {
        showMemberMenu();
        choice = readInt("Enter your choice (0-5): ", 0, 5);

        switch (choice)
        {
            case 1: registerMember(members);  break;
            case 2: displayMembers(members);  break;
            case 3: searchMember(members);    break;
            case 4: updateMember(members);    break;
            case 5: deleteMember(members);    break;
            case 0: cout << "\nReturning to Main Menu...\n"; break;
            default: break; // unreachable - readInt validates
        }
    } while (choice != 0);
}
