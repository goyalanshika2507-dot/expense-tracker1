#include "FileManager.h"
#include "Income.h"
#include "Expense.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <cctype>

namespace fs = std::filesystem;

string FileManager::getDataDirectory()
{
    return "data";
}

string FileManager::sanitizeFilename(const string& name)
{
    string safe = "";
    for (char c : name)
    {
        if (isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-')
        {
            safe += c;
        }
        else
        {
            safe += '_';
        }
    }
    return safe.empty() ? "user" : safe;
}

void FileManager::ensureDirectoryExists(const string& path)
{
    try
    {
        if (!fs::exists(path))
        {
            fs::create_directories(path);
        }
    }
    catch (...)
    {
        // Directory creation error fallback
    }
}

bool FileManager::saveUsers(const vector<User>& users)
{
    ensureDirectoryExists(getDataDirectory());
    string filepath = getDataDirectory() + "/users.txt";
    ofstream file(filepath);
    if (!file.is_open())
    {
        cerr << "Error: Could not open " << filepath << " for writing.\n";
        return false;
    }

    for (const auto& u : users)
    {
        file << u.getUserId() << "|"
             << u.getUsername() << "|"
             << u.getPasswordHash() << "|"
             << u.getSalt() << "\n";
    }

    file.close();
    return true;
}

bool FileManager::loadUsers(vector<User>& users)
{
    string filepath = getDataDirectory() + "/users.txt";
    ifstream file(filepath);
    if (!file.is_open())
    {
        return false;
    }

    users.clear();
    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;
        stringstream ss(line);
        string userId, username, passwordHash, salt;

        getline(ss, userId, '|');
        getline(ss, username, '|');
        getline(ss, passwordHash, '|');
        getline(ss, salt);

        if (!username.empty() && !passwordHash.empty())
        {
            users.push_back(User(userId, username, passwordHash, salt));
        }
    }

    file.close();
    return true;
}

bool FileManager::saveTransactions(const string& username, const vector<Transaction*>& transactions)
{
    ensureDirectoryExists(getDataDirectory());
    string filepath = getDataDirectory() + "/transactions_" + sanitizeFilename(username) + ".txt";
    ofstream file(filepath);
    if (!file.is_open())
    {
        cerr << "Error: Could not open " << filepath << " for writing.\n";
        return false;
    }

    for (const auto* t : transactions)
    {
        if (!t) continue;
        file << t->getId() << "|"
             << t->getType() << "|"
             << t->getAmount() << "|"
             << t->getCategoryOrSource() << "|"
             << t->getDate() << "\n";
    }

    file.close();
    return true;
}

bool FileManager::loadTransactions(const string& username, vector<Transaction*>& transactions, int& maxId)
{
    string filepath = getDataDirectory() + "/transactions_" + sanitizeFilename(username) + ".txt";
    ifstream file(filepath);
    if (!file.is_open())
    {
        return false;
    }

    // Clean up any existing pointers before loading
    for (auto* t : transactions)
    {
        delete t;
    }
    transactions.clear();
    maxId = 0;

    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;
        stringstream ss(line);
        string idStr, type, amountStr, catOrSrc, date;

        getline(ss, idStr, '|');
        getline(ss, type, '|');
        getline(ss, amountStr, '|');
        getline(ss, catOrSrc, '|');
        getline(ss, date);

        try
        {
            int id = stoi(idStr);
            double amount = stod(amountStr);

            if (id > maxId)
            {
                maxId = id;
            }

            if (type == "Income")
            {
                transactions.push_back(new Income(id, amount, date, catOrSrc));
            }
            else if (type == "Expense")
            {
                transactions.push_back(new Expense(id, amount, date, catOrSrc));
            }
        }
        catch (...)
        {
            // Skip invalid or corrupt records safely
            continue;
        }
    }

    file.close();
    return true;
}

bool FileManager::saveBudgets(const string& username, const vector<Budget>& budgets)
{
    ensureDirectoryExists(getDataDirectory());
    string filepath = getDataDirectory() + "/budgets_" + sanitizeFilename(username) + ".txt";
    ofstream file(filepath);
    if (!file.is_open())
    {
        cerr << "Error: Could not open " << filepath << " for writing.\n";
        return false;
    }

    for (const auto& b : budgets)
    {
        file << b.getCategory() << "|" << b.getLimitAmount() << "\n";
    }

    file.close();
    return true;
}

bool FileManager::loadBudgets(const string& username, vector<Budget>& budgets)
{
    string filepath = getDataDirectory() + "/budgets_" + sanitizeFilename(username) + ".txt";
    ifstream file(filepath);
    if (!file.is_open())
    {
        return false;
    }

    budgets.clear();
    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;
        stringstream ss(line);
        string category, limitStr;

        getline(ss, category, '|');
        getline(ss, limitStr);

        try
        {
            double limit = stod(limitStr);
            if (!category.empty())
            {
                budgets.push_back(Budget(category, limit));
            }
        }
        catch (...)
        {
            continue;
        }
    }

    file.close();
    return true;
}
