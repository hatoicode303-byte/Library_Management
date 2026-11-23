/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 9: File Storage
 * ============================================================
 *  Module      : storage.h
 *  Description : The "database layer" of the system, built
 *                with pure C++ file handling (no external
 *                tool). Three data files + one backup:
 *
 *                  books.dat         - one line per book
 *                  members.dat       - one line per member
 *                  transactions.dat  - one line per issue
 *                  books_backup.dat  - safety copy (FR-12)
 *
 *                Record format: fields separated by '|'
 *                  B101|Title|Author|Category|550.00|2
 *                  M001|Name|Phone|Email|Student
 *                  B101|M001|23-11-2026|07-12-2026|-|Issued|0.00
 *
 *  Lifecycle:
 *    Program start -> loadAllData()  (files -> memory)
 *    After action  -> saveAllData()  (memory -> files)
 *    Program exit  -> saveAllData() + backupBooks()
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <vector>

#include "book.h"
#include "member.h"
#include "transaction.h"

// Reads all three data files into memory.
// If a file does not exist (first run), that record set
// simply starts empty - no crash.
void loadAllData(std::vector<Book>&         books,
                 std::vector<Member*>&      members,
                 std::vector<Transaction*>& transactions);

// Writes all three data files from memory (overwrites them).
void saveAllData(const std::vector<Book>&         books,
                 const std::vector<Member*>&      members,
                 const std::vector<Transaction*>& transactions);

// Creates books_backup.dat from the current book records (FR-12).
void backupBooks(const std::vector<Book>& books);

#endif // STORAGE_H
