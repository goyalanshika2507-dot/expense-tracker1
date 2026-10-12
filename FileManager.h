#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "user.h"
#include "Transaction.h"
#include "Budget.h"

using namespace std;

class FileManager
{
private:
    static string getDataDirectory();
    static string sanitizeFilename(const string& name);
    static void ensureDirectoryExists(const string& path);

public:
    static bool saveUsers(const vector<User>& users);
    static bool loadUsers(vector<User>& users);

    static bool saveTransactions(const string& username, const vector<Transaction*>& transactions);
    static bool loadTransactions(const string& username, vector<Transaction*>& transactions, int& maxId);

    static bool saveBudgets(const string& username, const vector<Budget>& budgets);
    static bool loadBudgets(const string& username, vector<Budget>& budgets);
};

#endif
