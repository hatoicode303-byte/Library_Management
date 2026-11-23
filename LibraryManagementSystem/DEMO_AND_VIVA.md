# 🎤 Demo Script & Viva Preparation — Library Management System

Phase 11 deliverable: exactly what to do and say in front of your incharge/examiner.

---

## Part 1 — 5-Minute Live Demo Script

### Setup (before they arrive)
```bash
g++ -std=c++11 -Wall -Wextra -o LibraryManagementSystem.exe main.cpp menu.cpp validation.cpp book.cpp member.cpp transaction.cpp reports.cpp search.cpp storage.cpp
```
Delete any `.dat` files so you start clean (demonstrates first-run handling).

---

### Minute 1 — Introduction + first run
**Say:** *"This is my Library Management System in C++. It uses OOP and file
handling as the database — no external tool. Let me start with a fresh run."*

| Press | Say |
|-------|-----|
| Run exe | *"First run: no files exist, it starts empty without crashing."* |
| `1` → `1` | Add a book: `B101`, `Object Oriented Programming`, `Robert Lafore`, `Programming`, `450`, `2` |
| `1` → `1` again, same ID `B101` | *"Duplicate IDs are rejected — data integrity."* |
| `0` | Back to main menu |

### Minute 2 — Members + inheritance
| Press | Say |
|-------|-----|
| `2` → `1` | Register: `M001`, `Ali Khan`, phone, email, type `1` (Student) |
| `2` → `1` | Register: `M002`, `Sara Ahmed`, ..., type `2` (Teacher) |
| `2` → `2` | *"Student limit 3, Teacher limit 5 — this comes from inheritance and polymorphism: both are stored as Member objects, but getBorrowLimit() returns different values."* |
| `0` | Back |

### Minute 3 — Issue (the 5-check chain)
| Press | Say |
|-------|-----|
| `3` → `B999` | *"Book must exist — rejected."* |
| `3` → `B101` + `M999` | *"Member must exist — rejected."* |
| `3` → `B101` + `M001` | *"Issued. Note the receipt: today + 14 days = due date, auto-calculated."* |
| `3` → `B101` + `M001` again | *"Can't hold the same book twice."* |

### Minute 4 — Return + fine + reports
| Press | Say |
|-------|-----|
| `4` → `B101` + `M001` → `y` | *"On-time return, no fine, copy restored."* |
| `5` → `1` | *"Summary is computed live from records — always accurate."* |
| `5` → `2` | Issued list |
| `5` → `3` | *"Overdue list with running fine — currently none."* |
| `5` → `4` → `M001` | Member history with status and fine |

### Minute 5 — Persistence (the finisher)
| Press | Say |
|-------|-----|
| `6` → `1` → `database` | Case-insensitive global search |
| `0` | *"Now I exit — data is saved automatically."* |
| Point at files | *"These .dat files are my database. Backup is also created."* |
| **Run exe again** | *"Loaded: 2 book(s), 2 member(s)... Everything survived the restart."* |
| `0` | *"Thank you — that's my project."* |

**Backup proof (if asked):** open `books.dat` and `books_backup.dat` in Notepad — identical.

---

## Part 2 — Likely Viva Questions & Answers

### Conceptual
**Q: Why file handling instead of MySQL?**
A: The project scope was C++ only. File handling with `<fstream>` provides
persistent structured storage — the same concept as a database (tables →
files, rows → lines, columns → `|`-separated fields) — with zero external tools.

**Q: What is polymorphism in your project? Give a real example.**
A: A `vector<Member*>` holds `StudentMember` and `TeacherMember` objects.
When `getBorrowLimit()` is called, the correct version runs automatically —
3 for students, 5 for teachers. The display table prints both with one loop.

**Q: Why a virtual destructor?**
A: Members are deleted through base-class `Member*`. A virtual destructor
ensures the derived object's destructor also runs — prevents undefined
behavior and memory leaks.

**Q: What is abstraction vs encapsulation here?**
A: Encapsulation = private data with public getters/setters (`Book`).
Abstraction = `Member` is abstract (pure virtual functions) — it defines *what*
a member must do, not *how*.

**Q: How do you prevent memory leaks?**
A: Every `new` has a matching `delete`: erased objects are deleted in
`deleteBook/deleteMember`, and all `Member*`/`Transaction*` are freed at exit.

### Design
**Q: What happens if two users enter the same Book ID?**
A: Rejected — `findIndexById()` checks before insertion (T-3 test).

**Q: How is the fine calculated?**
A: `daysBetween(dueDate, returnDate) × FINE_PER_DAY` — only if late; the
value is stored on the transaction and shown in the overdue report.

**Q: What if the data file is corrupted?**
A: Lines with the wrong field count are skipped (T-22 tested with garbage
lines) — the program loads valid records and never crashes.

**Q: Where is the validation done for issue?**
A: Five sequential checks: book exists → copies > 0 → member exists →
not already holding → under borrow limit (T-6 to T-10).

### Code specifics
**Q: Why `vector` and not arrays?**
A: Records are added/deleted at runtime — vectors resize automatically and
support `erase()` for deletions.

**Q: How does date arithmetic handle leap years?**
A: `daysFromCivil`/`civilFromDays` (civil calendar algorithm) converts dates
to day counts — leap years and month lengths handled mathematically.

**Q: Why is `|` used as a separator?**
A: Titles/names contain spaces, so space-delimited parsing would break.
`readField()` rejects `|` in user input, guaranteeing safe round-trip.

---

## Part 3 — One-Minute Elevator Pitch

> *"I built a Library Management System entirely in C++ using object-oriented
> programming. It manages books and members, issues and returns with a
> five-step validation chain, calculates fines for late returns, and generates
> four management reports. Data persists across runs using C++ file handling
> as the database — no external tools. The code demonstrates inheritance,
> polymorphism, abstraction, and encapsulation across 18 files and about 2,400
> lines, tested with 28 test cases, all passing with zero compiler warnings."*

---

## Part 4 — Submission Checklist

- [x] Source code (9 `.cpp` + 9 `.h`)
- [x] Project Proposal (phases, requirements, design)
- [x] Test report (28 cases, FR coverage, defect log)
- [x] README (features, build instructions, OOP mapping)
- [x] Demo script + viva notes (this file)
- [ ] Final report (combine proposal + screenshots + test report)
- [ ] Screenshot the running program for the report
- [ ] Demo rehearsal once, alone, timing yourself at 5 minutes
