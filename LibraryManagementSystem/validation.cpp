/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM - Phase 3: Project Skeleton
 * ============================================================
 *  Module      : validation.cpp
 *  Description : Input validation helpers.
 *                The classic crash in student projects is:
 *                    int x; cin >> x;   // user types "abc"
 *                cin enters a fail state and the loop runs
 *                forever. These functions prevent that once,
 *                for the whole program.
 *  Compiler    : Dev-C++ (MinGW g++)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>   // exit()

#include "validation.h"

using namespace std;

int readInt(const string& prompt, int min, int max)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            // Consumes the leftover newline so later getline() works
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (value >= min && value <= max)
                return value;

            cout << "Please enter a number between "
                 << min << " and " << max << ".\n";
        }
        else if (cin.eof())
        {
            // End of input (e.g. Ctrl+Z on Windows) - stop cleanly
            // instead of looping forever.
            cout << "\n>> End of input detected. Exiting...\n";
            exit(0);
        }
        else
        {
            // User typed text (e.g. "abc") - clear the fail state
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
        }
    }
}

double readDouble(const string& prompt, double min, double max)
{
    double value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (value >= min && value <= max)
                return value;

            cout << "Please enter a value between "
                 << min << " and " << max << ".\n";
        }
        else if (cin.eof())
        {
            // End of input - stop cleanly instead of looping forever.
            cout << "\n>> End of input detected. Exiting...\n";
            exit(0);
        }
        else
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
        }
    }
}

string readLine(const string& prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, value))
        {
            // End of input (Ctrl+Z / piped input exhausted)
            if (cin.eof())
            {
                cout << "\n>> End of input detected. Exiting...\n";
                exit(0);
            }
            cin.clear();
            continue;
        }

        // Trim leading spaces
        size_t start = value.find_first_not_of(" \t");
        if (start == string::npos)
        {
            cout << "Input cannot be empty. Please try again.\n";
            continue;
        }
        // Trim trailing spaces
        size_t end = value.find_last_not_of(" \t");
        value = value.substr(start, end - start + 1);
        return value;
    }
}

string readField(const string& prompt)
{
    while (true)
    {
        string value = readLine(prompt);

        if (value.find('|') != string::npos)
        {
            cout << "The character '|' is not allowed. Please try again.\n";
            continue;
        }
        return value;
    }
}

bool confirm(const string& prompt)
{
    string line;
    while (true)
    {
        cout << prompt << " (y/n): ";
        if (!getline(cin, line))
        {
            if (cin.eof())
            {
                cout << "\n>> End of input detected. Exiting...\n";
                exit(0);
            }
            cin.clear();
            continue;
        }

        if (line.length() >= 1)
        {
            char c = line[0];
            if (c == 'y' || c == 'Y') return true;
            if (c == 'n' || c == 'N') return false;
        }
        cout << "Please enter 'y' or 'n'.\n";
    }
}
