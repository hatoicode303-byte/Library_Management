/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 3: Project Skeleton
 * ============================================================
 *  Module      : menu.h
 *  Description : Declaration of all menu / screen display
 *                functions.
 *  Compiler    : Dev-C++ (MinGW g++)
 * ============================================================
 */

#ifndef MENU_H
#define MENU_H

void showBanner();        // Welcome screen shown once at start
void showMainMenu();      // Top-level menu
void showBookMenu();      // Book Management sub-menu (Phase 4)
void showMemberMenu();    // Member Management sub-menu (Phase 5)
void showReportsMenu();   // Reports sub-menu (Phase 8)
void showSearchMenu();    // Search sub-menu (Phase 8)
void pauseScreen();       // "Press Enter to continue..."

#endif // MENU_H
