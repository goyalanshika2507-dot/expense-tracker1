#ifndef REPORTS_H
#define REPORTS_H

#include <string>
#include <vector>
#include <utility>
#include "Transaction.h"

using namespace std;

struct MonthlySummary
{
    string monthYear; // e.g. "2026-10" or "10/2026"
    double totalIncome;
    double totalExpense;
    double netSavings;
};

class Reports
{
public:
    static double calculateTotalIncome(const vector<Transaction*>& transactions);
    static double calculateTotalExpense(const vector<Transaction*>& transactions);
    static double calculateBalance(const vector<Transaction*>& transactions);

    static vector<pair<string, double>> getCategoryExpenses(const vector<Transaction*>& transactions);
    static vector<pair<string, double>> getSourceIncomes(const vector<Transaction*>& transactions);
    static vector<MonthlySummary> getMonthlySummaries(const vector<Transaction*>& transactions);
};

#endif
