/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 8: Global Search
 * ============================================================
 *  Module      : search.h
 *  Description : Global search menu:
 *                  1. Search Book by Title
 *                  2. Search Book by Author
 *                  3. Search Book by ID
 *                  4. Search Member by Name
 *                  5. Search Member by ID
 *
 *  All text searches are CASE-INSENSITIVE, so "cpp" finds
 *  "C++ Programming".
 *  Compiler : Dev-C++ (MinGW g++)
 * ============================================================
 */

#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <vector>

#include "book.h"
#include "member.h"

// Module entry point (called from main menu option 6)
void runSearchModule(const std::vector<Book>&    books,
                     const std::vector<Member*>& members);

#endif // SEARCH_H
