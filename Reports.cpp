#include "Reports.h"
#include <map>
#include <algorithm>

static string extractMonthYear(const string& date)
{
    // Handles YYYY-MM-DD, DD/MM/YYYY, DD-MM-YYYY
    if (date.length() >= 7)
    {
        if (date[4] == '-' || date[4] == '/')
        {
            // YYYY-MM-...
            return date.substr(0, 7);
        }
        else if (date.length() >= 10 && (date[2] == '-' || date[2] == '/'))
        {
            // DD-MM-YYYY or DD/MM/YYYY
            string year = date.substr(6, 4);
            string month = date.substr(3, 2);
            return year + "-" + month;
        }
    }
    return date.empty() ? "Unknown" : date;
}

double Reports::calculateTotalIncome(const vector<Transaction*>& transactions)
{
    double total = 0.0;
    for (const auto* t : transactions)
    {
        if (t && t->getType() == "Income")
        {
            total += t->getAmount();
        }
    }
    return total;
}

double Reports::calculateTotalExpense(const vector<Transaction*>& transactions)
{
    double total = 0.0;
    for (const auto* t : transactions)
    {
        if (t && t->getType() == "Expense")
        {
            total += t->getAmount();
        }
    }
    return total;
}

double Reports::calculateBalance(const vector<Transaction*>& transactions)
{
    return calculateTotalIncome(transactions) - calculateTotalExpense(transactions);
}

vector<pair<string, double>> Reports::getCategoryExpenses(const vector<Transaction*>& transactions)
{
    map<string, double> catMap;
    for (const auto* t : transactions)
    {
        if (t && t->getType() == "Expense")
        {
            string cat = t->getCategoryOrSource();
            if (cat.empty()) cat = "Other";
            catMap[cat] += t->getAmount();
        }
    }

    vector<pair<string, double>> result(catMap.begin(), catMap.end());
    // Sort descending by amount
    sort(result.begin(), result.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        return a.second > b.second;
    });
    return result;
}

vector<pair<string, double>> Reports::getSourceIncomes(const vector<Transaction*>& transactions)
{
    map<string, double> srcMap;
    for (const auto* t : transactions)
    {
        if (t && t->getType() == "Income")
        {
            string src = t->getCategoryOrSource();
            if (src.empty()) src = "Other";
            srcMap[src] += t->getAmount();
        }
    }

    vector<pair<string, double>> result(srcMap.begin(), srcMap.end());
    sort(result.begin(), result.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        return a.second > b.second;
    });
    return result;
}

vector<MonthlySummary> Reports::getMonthlySummaries(const vector<Transaction*>& transactions)
{
    struct TempMonth {
        double inc = 0.0;
        double exp = 0.0;
    };
    map<string, TempMonth> monthMap;

    for (const auto* t : transactions)
    {
        if (!t) continue;
        string m = extractMonthYear(t->getDate());
        if (t->getType() == "Income")
        {
            monthMap[m].inc += t->getAmount();
        }
        else
        {
            monthMap[m].exp += t->getAmount();
        }
    }

    vector<MonthlySummary> summaries;
    for (const auto& item : monthMap)
    {
        MonthlySummary s;
        s.monthYear = item.first;
        s.totalIncome = item.second.inc;
        s.totalExpense = item.second.exp;
        s.netSavings = item.second.inc - item.second.exp;
        summaries.push_back(s);
    }
    return summaries;
}
