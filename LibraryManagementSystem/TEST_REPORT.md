# Library Management System — Test Case Documentation

**Phase 10: Testing & Validation**
**Language:** C++ | **Date:** 23 November 2026

---

## 1. Testing Approach

Three levels of testing were performed:

1. **Unit Testing** — each module tested independently (Phases 4–9).
2. **Integration Testing** — full workflow across modules (T-24).
3. **Error / Edge Testing** — invalid inputs, missing records, corrupt data files.

Environment: Windows, g++ (MinGW) with `-std=c++11 -Wall -Wextra` — compiled
with **zero warnings**.

---

## 2. Test Case Table

| ID | Module | Scenario | Expected Result | Actual Result | Status |
|----|--------|----------|-----------------|---------------|--------|
| T-1 | Startup | First run with no data files | Starts empty, no crash | "Loaded: 0 book(s)..." | ✅ PASS |
| T-2 | Book | Add new book (B101) | Added, appears in display | Book added successfully | ✅ PASS |
| T-3 | Book | Add book with duplicate ID | Rejected with error | "already exists. Duplicate IDs not allowed" | ✅ PASS |
| T-4 | Book | Search title "object" (lowercase) | Case-insensitive match | Found "Object Oriented Programming" | ✅ PASS |
| T-5 | Book | Search non-existent ID B999 | "No book found" | No book found matching 'B999' | ✅ PASS |
| T-6 | Issue | Issue non-existent book B999 | Rejected at check 1 | "No book found with ID 'B999'" | ✅ PASS |
| T-7 | Issue | Issue to non-existent member M999 | Rejected at check 3 | "No member found with ID 'M999'" | ✅ PASS |
| T-8 | Issue | Issue book with 0 copies | Rejected at check 2 | "has no copies available" | ✅ PASS |
| T-9 | Issue | Student issues 4th book (limit 3) | Rejected at check 5 | "holds 3 of 3 allowed books" | ✅ PASS |
| T-10 | Issue | Issue same book+member twice (2 copies) | Rejected at check 4 | "This member already holds this book" | ✅ PASS |
| T-11 | Return | Return before due date | No fine, copies restored | "Returned on time. No fine" | ✅ PASS |
| T-12 | Return | Return 1 day late (test build) | Fine = 1 × Rs.5 = Rs.5.00 | Fine slip shows Rs.5.00 | ✅ PASS |
| T-13 | Return | Return book never issued | Rejected | "No active issue found" | ✅ PASS |
| T-14 | Return | Cancel confirmation (answer n) | Nothing changes | "Return cancelled" | ✅ PASS |
| T-15 | Member | Register duplicate member ID | Rejected | "already exists. Duplicate IDs not allowed" | ✅ PASS |
| T-16 | Member | Update phone number | Old + new record shown | "Member updated successfully" | ✅ PASS |
| T-17 | Member | Delete member, answer n | Cancelled, record kept | "Deletion cancelled" | ✅ PASS |
| T-18 | Input | Text entered in numeric menu | Error + re-prompt, no crash | "Invalid input! Please enter a number" | ✅ PASS |
| T-19 | Input | Number outside allowed range | Error + re-prompt | "Please enter a number between 0 and 6" | ✅ PASS |
| T-20 | Input | Empty field (blank name/title) | Rejected, re-prompted | "Input cannot be empty" | ✅ PASS |
| T-21 | Persistence | Exit, restart program | All records reloaded | "Loaded: 2 book(s), 2 member(s)..." | ✅ PASS |
| T-22 | Persistence | Restart with corrupt file lines | Bad lines skipped, valid loaded | 1 good book loaded, garbage ignored | ✅ PASS |
| T-23 | Reports | Summary / Issued / Overdue / History | Correct live-computed values | Counts matched records | ✅ PASS |
| T-24 | Integration | Full workflow + restart | Everything consistent end-to-end | All values correct after restart | ✅ PASS |
| T-25 | Backup | Exit program | books_backup.dat created (FR-12) | File created, matches books.dat | ✅ PASS |
| T-26 | Search | Member search "ali" (lowercase) | Case-insensitive match | Found "Ali Khan" | ✅ PASS |
| T-27 | Search | Search M999 (non-existent) | "No result found" | No result found for 'M999' | ✅ PASS |
| T-28 | Memory | Exit after adding members/transactions | No memory leaks / crashes | Clean exit every run | ✅ PASS |

---

## 3. Requirement Coverage (FR-1 … FR-12)

| Requirement | Verified by | Status |
|-------------|-------------|--------|
| FR-1 Add book with unique ID | T-2, T-3 | ✅ |
| FR-2 Update / delete books | T-16, T-17 (book variant in Phase 4) | ✅ |
| FR-3 Search books by ID/title/author | T-4, T-5 | ✅ |
| FR-4 Register member with unique ID | T-15 | ✅ |
| FR-5 Update / delete members | T-16, T-17 | ✅ |
| FR-6 Issue with all 5 checks | T-6 … T-10 | ✅ |
| FR-7 Auto issue date + due date (+14 days) | T-11, T-24 | ✅ |
| FR-8 Return updates copies + closes record | T-11, T-14, T-24 | ✅ |
| FR-9 Fine for overdue returns | T-12, overdue report | ✅ |
| FR-10 Reports (4 types) | T-23 | ✅ |
| FR-11 Save on exit / load on startup | T-21, T-24 | ✅ |
| FR-12 Backup before overwriting | T-25 | ✅ |

**Result: 28/28 test cases PASS · 12/12 requirements covered · 0 defects outstanding**

---

## 4. Defects Found & Fixed During Testing

| # | Phase Found | Defect | Fix |
|---|------------|--------|-----|
| D-1 | Phase 4 | Infinite loop when input stream ended (EOF / Ctrl+Z) — program would hang forever | All 4 input functions now detect EOF and exit cleanly |
| D-2 | Phase 10* | *(Design check)* Book/member deletion while records are active is validated by the Issue module's checks; data files tolerate corrupt lines | Corrupt lines skipped on load (verified by T-22) |

*Testing was continuous throughout Phases 4–9 (each phase passed its
acceptance tests before proceeding), not only in Phase 10.*

---

## 5. Test Environment

| Item | Value |
|------|-------|
| OS | Windows |
| Compiler | g++ (MinGW), `-std=c++11 -Wall -Wextra` |
| Warnings | 0 |
| Source files | 9 (.cpp) + 9 (.h) |
| External tools/libraries | None — standard C++ only |
