/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 8: Reports
 * ============================================================
 *  Module      : reports.h
 *  Description : The four management reports:
 *                  1. Total Books Summary
 *                  2. Issued Books List
 *                  3. Overdue Books List  (with live fine)
 *                  4. Member-wise History
 *
 *  All reports are computed LIVE from the in-memory records
 *  each time - no stored counters, so totals are always
 *  correct even after add/update/delete.
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#ifndef REPORTS_H
#define REPORTS_H

#include <string>
#include <vector>

#include "book.h"
#include "member.h"
#include "transaction.h"

// Module entry point (called from main menu option 5)
void runReportsModule(const std::vector<Book>&         books,
                      const std::vector<Member*>&      members,
                      const std::vector<Transaction*>& transactions);

#endif // REPORTS_H
