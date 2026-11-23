/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 5: Member Module
 * ============================================================
 *  Module      : member.h
 *  Description : Member class hierarchy.
 *
 *  OOP concepts demonstrated HERE:
 *    - Abstraction        : Member is an ABSTRACT base class
 *                           (pure virtual functions)
 *    - Inheritance        : StudentMember / TeacherMember
 *                           inherit from Member
 *    - Polymorphism       : getBorrowLimit() behaves
 *                           differently for each member type
 *    - Encapsulation      : private/protected attributes with
 *                           getters / setters
 *
 *  Borrowing rules:
 *    - StudentMember : may borrow up to 3 books
 *    - TeacherMember : may borrow up to 5 books
 *    (used by the Issue module in Phase 6)
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <vector>

// ----------------------------------------------------------
//  Abstract base class
// ----------------------------------------------------------
class Member
{
protected:
    std::string memberId;   // Unique ID, e.g. "M001"
    std::string name;
    std::string phone;
    std::string email;

public:
    Member();
    Member(const std::string& id,
           const std::string& name,
           const std::string& phone,
           const std::string& email);

    virtual ~Member();      // Virtual destructor (required
                            // when deleting via base pointer)

    // ----- Pure virtual functions (make Member abstract) -----
    virtual int         getBorrowLimit() const = 0;
    virtual std::string getTypeName()    const = 0;

    // ----- Getters -----
    std::string getMemberId() const;
    std::string getName()     const;
    std::string getPhone()    const;
    std::string getEmail()    const;

    // ----- Setters -----
    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void setEmail(const std::string& email);
};

// ----------------------------------------------------------
//  Derived class: Student (borrow limit = 3)
// ----------------------------------------------------------
class StudentMember : public Member
{
public:
    StudentMember(const std::string& id,
                  const std::string& name,
                  const std::string& phone,
                  const std::string& email);

    int         getBorrowLimit() const;   // returns 3
    std::string getTypeName()    const;   // returns "Student"
};

// ----------------------------------------------------------
//  Derived class: Teacher (borrow limit = 5)
// ----------------------------------------------------------
class TeacherMember : public Member
{
public:
    TeacherMember(const std::string& id,
                  const std::string& name,
                  const std::string& phone,
                  const std::string& email);

    int         getBorrowLimit() const;   // returns 5
    std::string getTypeName()    const;   // returns "Teacher"
};

// ----------------------------------------------------------
//  Factory: creates the right object from a type name
//           ("Student" or "Teacher"). Used by registration
//           AND by file loading in Phase 9.
// ----------------------------------------------------------
Member* createMember(const std::string& type,
                     const std::string& id,
                     const std::string& name,
                     const std::string& phone,
                     const std::string& email);

// ---------- Module entry point (called from main menu) ----------
void runMemberModule(std::vector<Member*>& members);

#endif // MEMBER_H
