/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 3: Project Skeleton
 * ============================================================
 *  Module      : validation.h
 *  Description : Input validation helpers. These functions are
 *                used by EVERY module so the program never
 *                crashes on wrong input.
 *  Compiler    : Dev-C++ (MinGW g++)
 * ============================================================
 */

#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>

// Reads an integer within [min, max].
// If the user types text or a number out of range, asks again.
int readInt(const std::string& prompt, int min, int max);

// Reads a decimal number (price, fine...) within [min, max].
// Rejects text input and out-of-range values.
double readDouble(const std::string& prompt, double min, double max);

// Reads a non-empty line of text (spaces allowed inside).
// Rejects blank / whitespace-only input and asks again.
std::string readLine(const std::string& prompt);

// Like readLine, but also rejects the '|' character, which is
// reserved as the field separator when saving to file (Phase 9).
std::string readField(const std::string& prompt);

// Asks a yes/no question. Returns true for y/Y, false for n/N.
bool confirm(const std::string& prompt);

#endif // VALIDATION_H
