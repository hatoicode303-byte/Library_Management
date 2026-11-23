# 📚 Library Management System

A complete library management application built in **pure C++** (console,
menu-driven) using Object-Oriented Programming and file handling — **no
external database or tool**.

---

## ✨ Features

| Module | What it does |
|--------|--------------|
| **Book Management** | Add, display, search (ID/title/author), update, delete — with duplicate-ID protection |
| **Member Management** | Register, display, search, update, delete — Students (limit 3) & Teachers (limit 5) |
| **Issue Book** | 5-step validation: book exists → copies available → member exists → not already held → under borrow limit |
| **Return Book** | Confirmation, copy restoration, automatic due-date tracking |
| **Fine Calculation** | Auto fine for late returns: days late × Rs.5/day (loan period: 14 days) |
| **Reports** | Library summary · Issued books · Overdue books (with live fine) · Member history |
| **Global Search** | Case-insensitive search for books and members |
| **File Storage** | Auto-load at start, auto-save after every action, backup on exit |
| **Input Validation** | Never crashes on wrong input; handles end-of-input safely |

---

## 🖥️ How to Build & Run

### Dev-C++ (recommended for this project)
1. **File → New → Project → Console Application → C++**
2. Add existing files: `main.cpp`, `menu.cpp`, `validation.cpp`, `book.cpp`,
   `member.cpp`, `transaction.cpp`, `reports.cpp`, `search.cpp`, `storage.cpp`
   (plus all `.h` files)
3. Press **F11** (Compile & Run)

### Command line (MinGW g++)
```bash
g++ -std=c++11 -Wall -Wextra -o LibraryManagementSystem.exe ^
    main.cpp menu.cpp validation.cpp book.cpp member.cpp ^
    transaction.cpp reports.cpp search.cpp storage.cpp

LibraryManagementSystem.exe
```

---

## 📁 Project Structure

```
LibraryManagementSystem/
│
├── main.cpp           Entry point · main menu · load/save wiring
├── menu.h/.cpp        All screen/menu display functions
├── validation.h/.cpp  Crash-proof input: readInt, readDouble,
│                      readLine, readField, confirm (+ EOF safety)
├── library.h          Constants: loan period (14), fine (Rs.5),
│                      file names
│
├── book.h/.cpp        Phase 4 — Book class + CRUD module
├── member.h/.cpp      Phase 5 — Member hierarchy (Student/Teacher)
├── transaction.h/.cpp Phase 6-7 — Date math, Transaction class,
│                      issue/return, fine calculation
├── reports.h/.cpp     Phase 8 — 4 management reports
├── search.h/.cpp      Phase 8 — global case-insensitive search
├── storage.h/.cpp     Phase 9 — file load/save/backup ("database")
│
├── books.dat          auto-generated — book records
├── members.dat        auto-generated — member records
├── transactions.dat   auto-generated — issue/return records
└── books_backup.dat   auto-generated — safety copy on exit
```

**Total:** 9 `.cpp` + 9 `.h` files · ~2,400 lines · 0 warnings · 0 external libraries

---

## 🗄️ About the "Database"

No external tool is used. Data lives in three plain-text files managed with
C++ `<fstream>`:

```
books.dat         →  B101|Object Oriented Programming|R. Lafore|Programming|450.00|2
members.dat       →  M001|Ali Khan|0300-1234567|ali@mail.com|Student
transactions.dat  →  B101|M001|23-11-2026|07-12-2026|-|Issued|0.00
```

- One record per line, `|` separates fields
- Loaded at startup, saved after **every** action, backed up at exit
- Corrupt/unknown lines are skipped safely (never crash)

---

## 🎓 OOP Concepts Used (exam points!)

| Concept | Where |
|---------|-------|
| Class & Object | `Book`, `Member`, `Transaction` |
| Encapsulation | Private attributes + getters/setters |
| **Abstraction** | `Member` abstract class (pure virtual functions) |
| **Inheritance** | `StudentMember` / `TeacherMember` ← `Member` |
| **Polymorphism** | `getBorrowLimit()` → 3 for students, 5 for teachers; one table prints both |
| Virtual destructor | `virtual ~Member()` for safe deletion via base pointer |
| Dynamic memory | `vector<Member*>`, `vector<Transaction*>` with proper cleanup |
| Data structures | `vector` for all record collections |
| File handling | `<fstream>` load/save/backup |
| Functions & modularity | One module = one file pair |

---

## 🧪 Testing

- **28 test cases**, all passing — see `TEST_REPORT.md`
- Covers: normal flows, every error path, input validation, persistence,
  corrupt files, full end-to-end workflow
- Compiled with `-Wall -Wextra` → **0 warnings**

---

## 📖 Development Phases (as planned)

| Phase | Name | Status |
|-------|------|--------|
| 0–2 | Problem definition · Requirements · Design | ✅ (Project Proposal) |
| 3 | Skeleton + menu + validation | ✅ |
| 4 | Book module | ✅ |
| 5 | Member module (inheritance) | ✅ |
| 6 | Issue & return | ✅ |
| 7 | Fine calculation | ✅ |
| 8 | Search & reports | ✅ |
| 9 | File storage | ✅ |
| 10 | Testing (28 cases) | ✅ |
| 11 | Documentation & demo | ✅ |

---

## 🚀 Future Enhancements

- Admin login (username/password)
- GUI version
- Bar-code based issuing
- SMS/email due-date reminders
