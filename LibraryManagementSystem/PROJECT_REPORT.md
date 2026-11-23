# 📚 LIBRARY MANAGEMENT SYSTEM
## Complete Project Report — Everything About This Project

**Language:** C++ (Pure, no external tools)
**Type:** Console Application with File-Based Storage
**Date:** 23 November 2026

---

# TABLE OF CONTENTS

1. [What This Project Is](#1-what-this-project-is)
2. [What Problem It Solves](#2-what-problem-it-solves)
3. [Objectives](#3-objectives)
4. [Scope (Included / Excluded)](#4-scope)
5. [System Architecture — How It Works](#5-system-architecture)
6. [The 6 Modules — Detailed](#6-the-6-modules)
7. [File Structure — Every File Explained](#7-file-structure)
8. [The "Database" — File Storage Design](#8-the-database)
9. [OOP Concepts — Where Each One Lives](#9-oop-concepts)
10. [Key Algorithms & Logic](#10-key-algorithms--logic)
11. [Development Phases — What Was Done When](#11-development-phases)
12. [Testing — 28 Test Cases](#12-testing)
13. [How to Build & Run (Dev-C++)](#13-how-to-build--run)
14. [How to Demo It (5 Minutes)](#14-how-to-demo-it)
15. [Viva Q&A](#15-viva-qa)
16. [Future Enhancements](#16-future-enhancements)
17. [Conclusion](#17-conclusion)

---

# 1. WHAT THIS PROJECT IS

A **Library Management System** is a computerized application that performs all
routine library operations through a menu-driven interface. This version is
written **entirely in C++** and stores its data in plain text files using C++
file handling — **no MySQL, no SQLite, no external tool of any kind**.

**In one sentence:**
> It replaces the librarian's paper register with a program that manages books,
> members, issuing/returning, fines, and reports — built with OOP in C++.

**At a glance:**

| Item | Value |
|------|-------|
| Language | C++11 (standard library only) |
| Interface | Console (menu-driven, two-level menus) |
| Storage | 3 data files + 1 backup (`.dat`) |
| Source files | 9 `.cpp` + 9 `.h` = 18 files |
| Lines of code | ~2,450 |
| Warnings | 0 (compiled with `-Wall -Wextra`) |
| External libraries | **None** |
| Test cases | 28 / 28 passing |

---

# 2. WHAT PROBLEM IT SOLVES

| # | Manual Library Problem | How This System Solves It |
|---|------------------------|---------------------------|
| 1 | Searching a paper register is slow | Instant search by ID, title, author, name (case-insensitive) |
| 2 | No record of who borrowed what | Every issue creates a transaction with dates |
| 3 | Manual fine calculation causes disputes | Auto fine = days late × Rs.5/day |
| 4 | Duplicate book/member entries | Duplicate-ID check on every insertion |
| 5 | Records get lost or damaged | Auto-save to file + backup copy on exit |
| 6 | No statistics (totals, overdue lists) | 4 live-computed reports |
| 7 | Books issued beyond allowed limit | Borrow-limit check (Student 3 / Teacher 5) |

---

# 3. OBJECTIVES

1. Apply **Object-Oriented Programming** concepts in a practical application
2. Implement **file handling** for persistence without any external tool
3. Automate book inventory, member management, and circulation
4. Provide **automatic due-date tracking** and fine calculation
5. Produce **searchable records** and useful management reports
6. Build a **modular, well-documented, thoroughly tested** program

---

# 4. SCOPE

## ✅ Included
- Book CRUD + search · Member CRUD + search
- Issue/return with a 5-step validation chain
- Due dates (issue + 14 days) and fine calculation (Rs.5/day)
- 4 reports: Summary, Issued, Overdue (with live fine), Member history
- Global case-insensitive search
- File persistence: load at start, save after **every** action, backup at exit
- Input validation — never crashes on wrong input
- Corrupt data lines are skipped safely

## ❌ Excluded (kept simple by design)
- GUI · Networking/multi-user · Barcode/QR · Email/SMS alerts
- External databases · Web/mobile access

---

# 5. SYSTEM ARCHITECTURE — HOW IT WORKS

## 5.1 Program Flow
```
        ┌──────────────────────┐
        │   PROGRAM START      │
        │  loadAllData()       │ ← reads books.dat, members.dat,
        └──────────┬───────────┘    transactions.dat (if they exist)
                   ▼
        ┌──────────────────────┐
        │     MAIN MENU        │◄──────────────┐
        │  1 Books  2 Members  │               │
        │  3 Issue  4 Return   │   loops until │
        │  5 Reports 6 Search  │   user picks 0│
        │  0 Exit              │               │
        └──────────┬───────────┘               │
                   ▼                           │
        ┌──────────────────────┐               │
        │  Perform operation   │───────────────┘
        │  (with validation)   │
        └──────────┬───────────┘
                   ▼
        ┌──────────────────────┐
        │  saveAllData()       │ ← after EVERY action
        └──────────────────────┘
                   │ (user picks 0)
                   ▼
        ┌──────────────────────┐
        │  save + backupBooks()│ → books_backup.dat
        │  cleanup (delete)    │ → no memory leaks
        │  PROGRAM END         │
        └──────────────────────┘
```

## 5.2 The Four Data Files
```
books.dat          One line per book
members.dat        One line per member
transactions.dat   One line per issue/return
books_backup.dat   Safety copy, written on exit
```

## 5.3 Classes (Design)
| Class | Purpose | Key Attributes |
|-------|---------|----------------|
| `Book` | A book in the library | ID, Title, Author, Category, Price, Copies |
| `Member` (abstract) | Base for all members | ID, Name, Phone, Email |
| `StudentMember` | Student member | borrow limit = **3** |
| `TeacherMember` | Teacher member | borrow limit = **5** |
| `Transaction` | An issue/return record | BookID, MemberID, Issue/Due/Return dates, Status, Fine |
| `Date` | Day/Month/Year struct | used by transactions |

---

# 6. THE 6 MODULES — DETAILED

## Module 1 — Book Management (`book.h/.cpp`)
| Operation | Behavior |
|-----------|----------|
| Add | Rejects duplicate Book ID; validates price (0–1,000,000) and copies (1–10,000) |
| Display | Formatted table (ID, Title, Author, Category, Price, Copies) + total |
| Search | By exact ID, or by title/author **case-insensitively** |
| Update | Pick a field (title/author/category/price/copies); shows old + new record |
| Delete | Shows the record, asks **y/n confirmation** before removing |

## Module 2 — Member Management (`member.h/.cpp`)
- Same 5 operations as books, plus **member type**: Student (limit 3) or Teacher (limit 5)
- Changing a member's type **replaces the object** with the other derived class (same ID/details)
- Deleting frees the dynamic memory (`delete` before `erase`)

## Module 3 — Issue Book (`transaction.cpp`)
**The 5-step validation chain — every step must pass:**
```
Step 1: Book exists?           → else "No book found"
Step 2: Copies available?      → else "no copies available"
Step 3: Member exists?         → else "No member found"
Step 4: Already holding it?    → else "already holds this book"
Step 5: Under borrow limit?    → else "holds 3 of 3 allowed books"
─────────────────────────────────────────────────────────────
ALL PASS → create Transaction (issue=today, due=today+14)
         → decrement copies → print ISSUE RECEIPT
```

## Module 4 — Return Book (`transaction.cpp`)
1. Find the **active** transaction (book ID + member ID) — else error
2. Show details → ask **y/n confirmation**
3. Mark returned (return date = today), **increment copies**
4. Calculate fine if late → print **FINE SLIP**

## Module 5 — Search & Reports (`search.cpp`, `reports.cpp`)
**Global Search (5 ways):** book by title / author / ID · member by name / ID

**4 Reports (all computed LIVE from records — never stale):**
| Report | Shows |
|--------|-------|
| Library Summary | Titles, copies available/issued, members by type, total book value |
| Issued Books | Every active issue with title, member name, dates |
| Overdue Books | Only past-due issues with **days late + running fine** |
| Member History | One member's full record: dates, status, fine |

## Module 6 — File Storage (`storage.h/.cpp`)
- **Load** at startup (missing file = empty library, no crash)
- **Save** after every menu action (a crash loses nothing)
- **Backup** `books_backup.dat` on exit
- **Corrupt lines** (wrong field count) are skipped, never crash

---

# 7. FILE STRUCTURE — EVERY FILE EXPLAINED

```
LibraryManagementSystem/
│
├── main.cpp           Entry point · main menu loop · load/save wiring · cleanup
├── menu.h / menu.cpp  All screens: banner, main menu, 4 sub-menus, pauseScreen()
├── validation.h/.cpp  Crash-proof input: readInt, readDouble, readLine,
│                      readField (rejects '|'), confirm (y/n) + EOF safety
├── library.h          Constants: LOAN_PERIOD_DAYS=14, FINE_PER_DAY=5.0,
│                      the 4 file names
│
├── book.h / book.cpp      Module 1: Book class + CRUD
├── member.h / member.cpp  Module 2: Member hierarchy + CRUD
├── transaction.h/.cpp     Modules 3-4: Date math, Transaction, issue, return, fine
├── reports.h / reports.cpp Module 5a: the 4 reports
├── search.h / search.cpp   Module 5b: global search
├── storage.h / storage.cpp  Module 6: load / save / backup
│
├── README.md              Features + build instructions
├── TEST_REPORT.md         28 test cases + requirement coverage
├── DEMO_AND_VIVA.md       Demo script + viva Q&A
├── PROJECT_REPORT.md      This document
│
└── (auto-generated) books.dat, members.dat,
    transactions.dat, books_backup.dat
```

---

# 8. THE "DATABASE" — FILE STORAGE DESIGN

**Why files?** The project rule is *C++ only, no external tool*. C++'s
`<fstream>` provides persistent storage — the same concept as a database:

| Real Database | This Project |
|---------------|--------------|
| Database server | Local disk files |
| Table | One file |
| Row | One line |
| Column | Value between `|` separators |
| SQL query | C++ read → compare → display |
| Survives restart | ✅ Yes |

**Record formats (one record per line):**
```
books.dat
B101|Object Oriented Programming|Robert Lafore|Programming|450.00|2

members.dat
M001|Ali Khan|0300-1234567|ali@mail.com|Student

transactions.dat
B101|M001|23-11-2026|07-12-2026|-|Issued|0.00
                      ▲ return date is "-" while still issued
```

**Why `|` as separator?** Titles contain spaces ("Data Structures"), so
space-splitting would break. Input already **rejects `|`** (`readField`), so
data can never break the format.

**Lifecycle:**
```
Start  → loadAllData()        files → memory
Action → saveAllData()        memory → files (after EVERY action)
Exit   → saveAllData() + backupBooks()
```

---

# 9. OOP CONCEPTS — WHERE EACH ONE LIVES

| Concept | Where in This Project | Code Evidence |
|---------|----------------------|---------------|
| **Class & Object** | `Book`, `Member`, `Transaction`, `Date` | `books.push_back(Book(...))` |
| **Encapsulation** | Private attributes + getters/setters | `book.cpp`: `setTitle()`, `getPrice()` |
| **Abstraction** | `Member` is an abstract class (pure virtual) | `virtual int getBorrowLimit() const = 0;` |
| **Inheritance** | `StudentMember`/`TeacherMember` ← `Member` | `: Member(id, n, p, e)` |
| **Polymorphism** | One vector, different behavior per type | `members[i]->getBorrowLimit()` → 3 or 5 |
| **Virtual destructor** | Safe deletion via base pointer | `virtual ~Member();` |
| **Dynamic memory** | `vector<Member*>`, `vector<Transaction*>` | `new` / `delete` paired |
| **Data structures** | `vector` for all collections | `vector<Book> books;` |
| **File handling** | `<fstream>` load/save/backup | `storage.cpp` |
| **Functions/modularity** | One module = one `.h`/`.cpp` pair | 9 modules |
| **Date arithmetic** | Custom calendar algorithm | `daysFromCivil` / `civilFromDays` |

**The #1 viva answer:**
> *"A `vector<Member*>` holds both Student and Teacher objects. When the
> program calls `getBorrowLimit()`, the correct version runs automatically —
> 3 for students, 5 for teachers. That's runtime polymorphism."*

---

# 10. KEY ALGORITHMS & LOGIC

### 10.1 Issue validation — 5 sequential checks
Shown in Module 3 above; each check returns early with a clear error message.

### 10.2 Date arithmetic (no library used)
```
Date → day number (daysFromCivil)
day number + 14 → new day number → Date (civilFromDays)
Difference between two dates = subtract day numbers
```
This handles **leap years and 30/31-day months automatically** — no manual
Feb/leap rules needed. Used for: due date, days late, fine.

### 10.3 Fine calculation
```
lateDays = daysBetween(dueDate, returnDate)      // if positive
fine     = (lateDays > 0) ? lateDays × 5.0 : 0.0
```
- Return **on/before** due date → Rs.0
- Return **1 day late** → Rs.5.00
- Fine is **stored** on the transaction and shown in the Overdue report

### 10.4 Borrow-limit check
```
held  = count of ACTIVE transactions for this member
limit = member->getBorrowLimit()          // polymorphic: 3 or 5
allow issue only if held < limit
```

### 10.5 Case-insensitive search
```
convert both haystack and needle to lowercase, then find()
"object" finds "Object Oriented Programming"
```

### 10.6 Crash-proof input
```
readInt:   cin fails on text → clear error state → discard line → ask again
           out of range      → ask again
           EOF (Ctrl+Z)      → exit cleanly (no infinite loop)
readField: empty input rejected, '|' rejected
confirm:   only y/Y/n/N accepted
```

---

# 11. DEVELOPMENT PHASES — WHAT WAS DONE WHEN

| Phase | Name | What Was Built | Status |
|-------|------|----------------|--------|
| 0 | Problem Definition | Problem statement, constraints (C++ only) | ✅ |
| 1 | Requirements | FR-1…FR-12 + non-functional requirements | ✅ |
| 2 | System Design | Class design, menu structure, file design | ✅ |
| 3 | Skeleton | Menu loop, navigation, input validation, EOF fix | ✅ |
| 4 | Book Module | Book class + Add/Display/Search/Update/Delete | ✅ |
| 5 | Member Module | Member hierarchy (inheritance/polymorphism) + CRUD | ✅ |
| 6 | Issue & Return | Date math, Transaction, 5-step validation chain | ✅ |
| 7 | Fine Calculation | Fine slip, late/on-time/early paths | ✅ |
| 8 | Search & Reports | 4 reports + global case-insensitive search | ✅ |
| 9 | File Storage | Load/save/backup, corrupt-line safety, factory reload | ✅ |
| 10 | Testing | 28 test cases, FR coverage, defect log | ✅ |
| 11 | Documentation | Proposal, README, test report, demo/viva guide, this file | ✅ |

**Defect found & fixed during development:**
- **D-1:** Input validation looped forever at end-of-input (EOF/Ctrl+Z) —
  fixed by adding EOF detection to all 4 input functions.

---

# 12. TESTING — 28 TEST CASES

Full details in `TEST_REPORT.md`. Summary:

| Area | Cases | Result |
|------|-------|--------|
| Startup / first run | T-1 | ✅ |
| Book CRUD + duplicate + search | T-2…T-5 | ✅ |
| Issue 5-step validation chain | T-6…T-10 | ✅ |
| Return + fine (on-time, late, cancel, not-issued) | T-11…T-14 | ✅ |
| Member CRUD + duplicate | T-15…T-17 | ✅ |
| Input validation (text, range, empty) | T-18…T-20 | ✅ |
| Persistence + corrupt files | T-21…T-22 | ✅ |
| Reports | T-23 | ✅ |
| Full end-to-end workflow | T-24 | ✅ |
| Backup | T-25 | ✅ |
| Search (found + not found) | T-26…T-27 | ✅ |
| Memory / clean exit | T-28 | ✅ |

**Score: 28/28 PASS · 12/12 requirements covered · 0 defects outstanding**

---

# 13. HOW TO BUILD & RUN (Dev-C++)

### Option A — Dev-C++ IDE
1. **File → New → Project → Console Application → C++**
2. Delete the template `main.cpp` it creates
3. **File → Add Existing Files** → add all 9 `.cpp` files
4. Press **F11** (Compile & Run)

### Option B — Command line
```bash
g++ -std=c++11 -Wall -Wextra -o LibraryManagementSystem.exe ^
    main.cpp menu.cpp validation.cpp book.cpp member.cpp ^
    transaction.cpp reports.cpp search.cpp storage.cpp

LibraryManagementSystem.exe
```

**First run:** starts empty (no files needed). All `.dat` files are created
automatically as you use the program.

---

# 14. HOW TO DEMO IT (5 MINUTES)

Full script in `DEMO_AND_VIVA.md`. Quick version:

| Min | Do | Say |
|-----|-----|-----|
| 1 | Add book B101, then try adding it again | *"Duplicate IDs are rejected"* |
| 2 | Register a Student and a Teacher, display | *"Limits 3 vs 5 come from polymorphism"* |
| 3 | Issue: try B999 (no book), M999 (no member), then real issue | *"5-step validation chain"* |
| 4 | Return on time → show reports | *"Fine is automatic, reports are live"* |
| 5 | Exit, show `.dat` files, **run again** | *"Data survived the restart — no database used"* |

---

# 15. VIVA Q&A

**Q: Why files instead of MySQL?**
A: Project scope was C++ only. `<fstream>` gives persistent structured storage —
tables→files, rows→lines, columns→`|` fields — with zero external tools.

**Q: Give a real example of polymorphism.**
A: `vector<Member*>` holds `StudentMember` and `TeacherMember`. Calling
`getBorrowLimit()` runs the right version automatically (3 or 5).

**Q: Why a virtual destructor?**
A: Objects are deleted through `Member*`. Virtual ensures the derived
destructor also runs — avoids undefined behavior.

**Q: How is the fine calculated?**
A: `daysBetween(dueDate, returnDate) × Rs.5` — only when positive (late).

**Q: What if a data file is corrupted?**
A: Lines with wrong field count are skipped; valid records still load; no crash.
(Tested in T-22.)

**Q: Why `vector` not arrays?**
A: Records are added/deleted at runtime; vectors resize and support `erase()`.

**Q: How do you prevent memory leaks?**
A: Every `new` has a `delete` — in delete operations and in the cleanup loop
at exit (all `Member*` and `Transaction*` freed).

**Q: Why `|` as a separator?**
A: Titles contain spaces. Input rejects `|`, so the format can never break.

---

# 16. FUTURE ENHANCEMENTS

- Admin login (username/password)
- Graphical User Interface (GUI)
- Bar-code based issuing
- SMS/email due-date reminders
- Integration with a real database engine (if allowed later)
- Multiple librarian accounts with roles

---

# 17. CONCLUSION

The Library Management System successfully replaces manual library operations
with a single, well-structured C++ application. It covers **all major syllabus
topics** — Object-Oriented Programming (classes, encapsulation, inheritance,
polymorphism, abstraction), data structures (`vector`), and file handling —
while solving a **real practical problem**.

The project was built in **11 phases**, from problem definition to
documentation, with testing baked in at every step. The final product:

- ✅ 18 source files, ~2,450 lines, **0 compiler warnings**
- ✅ 6 fully working modules with proper validation
- ✅ **28/28 test cases passing**, all 12 requirements covered
- ✅ Persistent storage with auto-save and backup — **no external tools**
- ✅ Complete documentation: proposal, README, test report, demo/viva guide

**Everything needed for submission and presentation is in this folder.**

---

*End of Project Report*
