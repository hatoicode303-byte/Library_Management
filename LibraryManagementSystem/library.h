/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 3: Project Skeleton
 * ============================================================
 *  Module      : library.h
 *  Description : Project-wide constants and file names.
 *                This header is the single place where all
 *                global settings are defined.
 *  Compiler    : Dev-C++ (MinGW g++)
 * ============================================================
 */

#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>

// ---------- Business Rules ----------
const int    LOAN_PERIOD_DAYS = 14;   // Days a book can be borrowed
const double FINE_PER_DAY     = 5.0;  // Fine (Rs.) per late day

// ---------- Data Files (the "database") ----------
// These files are created automatically by the program.
const std::string FILE_BOOKS         = "books.dat";
const std::string FILE_MEMBERS       = "members.dat";
const std::string FILE_TRANSACTIONS  = "transactions.dat";
const std::string FILE_BOOKS_BACKUP  = "books_backup.dat";

#endif // LIBRARY_H
