# PROJECT PROPOSAL

## Library Management System

---

| | |
|---|---|
| **Project Title** | Library Management System |
| **Programming Language** | C++ (Console Application) |
| **Storage** | C++ File Handling (No external database or tool) |
| **Project Type** | Semester Project — Object Oriented Programming |
| **Date** | 23 November 2026 |

---

## 1. Introduction

A Library Management System is a computerized application that automates the
routine operations of a library. In most libraries, records of books, members,
and borrowing transactions are maintained manually in registers or simple
spreadsheets. This approach is slow, error-prone, and difficult to search or
summarize.

This project replaces the manual process with a menu-driven C++ console
application. The system manages the complete lifecycle of a library: books are
added and maintained, members are registered, books are issued and returned,
fines are calculated for late returns, and reports are generated — all within a
single program.

Since the project is restricted to C++ only, persistence of data is achieved
through **C++ file handling**, which acts as the database layer of the system.
No external database, library, or software tool is used at any stage.

---

## 2. Problem Statement

Manual library management faces the following problems:

1. **Slow search** — Finding a book or member in a paper register takes
   significant time.
2. **No tracking of issued books** — It is difficult to know who currently
   holds which book and when it is due.
3. **Manual fine calculation** — Calculating overdue fines by hand leads to
   arithmetic errors and disputes.
4. **Duplicate and lost records** — Manual entry allows duplicate book or
   member entries, and records can be misplaced or damaged.
5. **No statistical summaries** — Generating totals (books issued, overdue
   items, member counts) requires manual counting.

---

## 3. Proposed Solution

A console-based Library Management System written entirely in C++ that
provides:

- A structured **menu interface** for all library operations
- **Instant search** of books and members by ID, title, author, or name
- **Automatic issue/return tracking** with pre-condition checks
- **Automatic due date and fine calculation** for late returns
- **Reports** for books, members, issued items, and overdue items
- **Permanent storage** using data files, so records survive program restarts
- **Input validation** so the program does not crash on incorrect entries

---

## 4. Objectives

1. To apply Object-Oriented Programming concepts (classes, encapsulation,
   inheritance, polymorphism) in a practical application.
2. To implement file handling for data persistence without any external tool.
3. To automate book inventory, member management, and circulation operations.
4. To provide automatic due date tracking and fine calculation.
5. To produce searchable records and useful management reports.
6. To build a modular, well-documented, and easily testable program.

---

## 5. Scope of the Project

### 5.1 In Scope (Included)

- Book management (add, update, delete, display, search)
- Member management (register, update, delete, display, search)
- Issue and return of books with availability and limit checks
- Due date assignment and fine calculation on late return
- Search and report generation
- File-based data storage with load on start / save on exit
- Automatic backup of data files on exit (data safety)
- Input validation and error handling

### 5.2 Out of Scope (Excluded)

- Graphical User Interface (GUI)
- Networking or multi-user access
- Barcode / QR code scanning
- Email or SMS notifications
- External databases (MySQL, SQLite, etc.)
- Web or mobile access

---

## 6. Functional Requirements

| No. | Requirement |
|----|-------------|
| FR-1 | The system shall allow adding a new book with a unique Book ID |
| FR-2 | The system shall allow updating and deleting book records |
| FR-3 | The system shall allow searching books by ID, title, or author |
| FR-4 | The system shall allow registering a new member with a unique Member ID |
| FR-5 | The system shall allow updating and deleting member records |
| FR-6 | The system shall issue a book only if the book is available, the member exists, and the borrowing limit is not exceeded |
| FR-7 | The system shall record the issue date and set a due date automatically |
| FR-8 | The system shall process returns, update available copies, and close the transaction |
| FR-9 | The system shall calculate a fine for overdue returns based on days late |
| FR-10 | The system shall generate reports: total books, available vs. issued, member history, and overdue list |
| FR-11 | The system shall save all data to files on exit and load them on startup |
| FR-12 | The system shall back up data files before overwriting them |

### Non-Functional Requirements

- The program shall not crash on invalid input (text instead of numbers, empty fields).
- All operations shall complete instantly for the expected data size.
- Data shall remain intact between program runs.
- The code shall be modular: each feature kept in a separate section/file.

---

## 7. System Design Overview

### 7.1 Main Entities (Classes)

| Class | Purpose | Key Attributes |
|-------|---------|----------------|
| **Book** | Represents a book in the library | BookID, Title, Author, Category, Price, CopiesAvailable |
| **Member** | Represents a registered member | MemberID, Name, Phone, Email, MemberType, BooksIssued |
| **Transaction** | Represents an issue/return record | BookID, MemberID, IssueDate, DueDate, ReturnDate, Status, Fine |
| **Library** | Manages all records and operations | Collections of books, members, transactions; all operations |

### 7.2 Menu Structure

```
=====================================
     LIBRARY MANAGEMENT SYSTEM
=====================================
 1. Book Management
      1.1 Add Book
      1.2 Display All Books
      1.3 Search Book
      1.4 Update Book
      1.5 Delete Book
 2. Member Management
      2.1 Register Member
      2.2 Display All Members
      2.3 Search Member
      2.4 Update Member
      2.5 Delete Member
 3. Issue Book
 4. Return Book (with fine)
 5. Reports
      5.1 Total Books Summary
      5.2 Issued Books List
      5.3 Overdue Books List
      5.4 Member-wise History
 6. Search Menu (Books / Members)
 0. Exit (saves data + creates backup)
=====================================
```

### 7.3 Data Storage Design (The "Database")

No external tool is used. Data is stored in three plain text files created and
managed by the program itself using `<fstream>`:

| File | Acts As | Example Record (fields separated by `|`) |
|------|---------|------------------------------------------|
| `books.dat` | Books table | `B101\|Object Oriented Programming\|R. Lafore\|Programming\|3` |
| `members.dat` | Members table | `M001\|Ali Khan\|0300-1234567\|ali@mail.com\|Student` |
| `transactions.dat` | Issue/Return table | `B101\|M001\|12-05-2026\|26-05-2026\|Issued` |
| `books_backup.dat` | Backup of books (safety) | Same format as `books.dat` |

**Data flow:**

```
Program Start  ->  Read files  ->  Load records into memory
User Operation ->  Update memory  ->  Write back to files
Program Exit   ->  Save all files  ->  Create backup copy
Next Run       ->  Load files again  ->  Data preserved
```

### 7.4 OOP Concepts Mapping

| Concept | Where Used |
|---------|-----------|
| Classes & Objects | `Book`, `Member`, `Transaction`, `Library` |
| Encapsulation | Private attributes with getter/setter methods |
| Inheritance | `Member` types: Student / Teacher (different borrowing limits) |
| Polymorphism | Virtual functions for `display()` and `calculateFine()` |
| Data structures | `vector` to store records in memory |
| File handling | `<fstream>` for load, save, and backup |
| Modularity | Separate functions/files per module |

---

## 8. Modules

| No. | Module | Description |
|-----|--------|-------------|
| M1 | Book Management | Add, update, delete, display, search books with duplicate-ID check |
| M2 | Member Management | Register, update, delete, display, search members |
| M3 | Issue & Return | Circulation with availability, existence, and limit checks |
| M4 | Due Date & Fine | Auto due date (issue + 14 days) and fine per day overdue |
| M5 | Search & Reports | Fast search plus summary and overdue reports |
| M6 | File Storage | Load on start, save on exit, backup for data safety |

---

## 9. Development Phases

| Phase | Name | Activities | Deliverable |
|-------|------|-----------|-------------|
| 0 | Problem Definition | Define problem, scope, constraints (C++ only) | Problem statement |
| 1 | Requirements | Functional & non-functional requirements | Requirements list |
| 2 | System Design | Class design, menu flowchart, file design | Diagrams |
| 3 | Project Skeleton | Main menu, navigation, input validation | Running menu |
| 4 | Book Module | Book CRUD + search | Working book module |
| 5 | Member Module | Member CRUD + search | Working member module |
| 6 | Issue/Return Module | Circulation logic with all checks | Working issue/return |
| 7 | Due Date & Fine | Date logic and fine calculation | Fine on return |
| 8 | Search & Reports | Reports and fast search | Reports screen |
| 9 | File Storage | Load, save, backup of all records | Persistent data |
| 10 | Testing | Test cases: normal + error + full workflow | Test case table |
| 11 | Documentation | Report, diagrams, demo preparation | Final submission |

---

## 10. Project Schedule

| Week | Phases | Milestone |
|------|--------|-----------|
| 1 | Phase 0 – 2 | Problem statement, requirements, design diagrams |
| 2 | Phase 3 – 4 | Menu running + Book module complete |
| 3 | Phase 5 – 6 | Member module + Issue/Return complete |
| 4 | Phase 7 – 8 | Fine calculation + Reports complete |
| 5 | Phase 9 – 10 | File storage + Testing complete |
| 6 | Phase 11 | Report writing, demo preparation, submission |

---

## 11. Testing Plan

Testing will be performed in three levels:

1. **Unit Testing** — Each module tested separately (e.g., add book, search
   book, register member).
2. **Integration Testing** — Full workflow: add book → register member →
   issue → return with fine → verify report.
3. **Error/Edge Testing** — Invalid inputs: non-numeric menu choice, duplicate
   IDs, issuing a non-existent book, returning a book never issued, empty
   files on first run.

A test case table (Test ID | Scenario | Expected | Actual | Pass/Fail) will be
maintained and included in the final report.

**Key test scenarios:**

| No. | Scenario | Expected Result |
|-----|----------|-----------------|
| T-1 | Add book with new ID | Book saved and displayed |
| T-2 | Add book with existing ID | Rejected with duplicate message |
| T-3 | Issue a book with 0 copies | Rejected — not available |
| T-4 | Issue to unknown member | Rejected — member not found |
| T-5 | Return after due date | Fine calculated and displayed |
| T-6 | Restart program | All previous records still present |
| T-7 | Enter text in numeric menu | Graceful message, no crash |

---

## 12. Deliverables

1. C++ source code (modular — separate files per module)
2. Data files (`.dat`) created automatically by the program
3. Project report with diagrams (class diagram, flowchart)
4. Test case documentation
5. Live demonstration (5-minute script):
   Add book → Register member → Issue → Late return with fine →
   Report → Exit → Restart to show saved data

---

## 13. Future Enhancements

- Graphical User Interface (using a C++ GUI framework)
- Admin login with username and password
- Bar-code based issuing
- SMS/email due-date reminders
- Integration with a real database engine

---

## 14. Conclusion

The Library Management System addresses the real problems of manual library
operation through a single, well-structured C++ application. It covers all
major areas of the syllabus — Object-Oriented Programming, data structures,
and file handling — while solving a practical problem. The phased development
approach ensures steady progress, thorough testing, and a complete,
documented final product, all achieved using only C++ with no external tools.

---

*End of Proposal*
