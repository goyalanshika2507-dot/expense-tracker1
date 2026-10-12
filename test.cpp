#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <filesystem>

#include "Transaction.h"
#include "Income.h"
#include "Expense.h"
#include "ExpenseTracker.h"
#include "Budget.h"
#include "Reports.h"

namespace fs = std::filesystem;
using namespace std;

bool approxEqual(double a, double b)
{
    return std::fabs(a - b) < 0.001;
}

void testTransactionPolymorphism()
{
    cout << "[TEST 1] Testing Transaction Polymorphism..." << endl;
    vector<Transaction*> list;
    list.push_back(new Income(101, 15000, "10-10-2026", "Salary"));
    list.push_back(new Expense(102, 500, "10-10-2026", "Food"));

    assert(list[0]->getType() == "Income");
    assert(list[0]->getCategoryOrSource() == "Salary");
    assert(approxEqual(list[0]->getAmount(), 15000));

    assert(list[1]->getType() == "Expense");
    assert(list[1]->getCategoryOrSource() == "Food");
    assert(approxEqual(list[1]->getAmount(), 500));

    for (auto* t : list) delete t;
    cout << "-> TEST 1 PASSED: Transaction polymorphism verified." << endl;
}

void testExpenseTrackerOperations()
{
    cout << "\n[TEST 2] Testing ExpenseTracker Operations & Persistence..." << endl;
    try {
        fs::remove("test_tx.txt");
    } catch (...) {}

    ExpenseTracker tracker;
    tracker.clearData();

    assert(tracker.addIncome(25000.0, "2026-10-01", "Salary"));
    assert(tracker.addIncome(5000.0, "2026-10-05", "Freelancing"));
    assert(tracker.addExpense(1200.0, "2026-10-06", "Food"));
    assert(tracker.addExpense(3000.0, "2026-10-08", "Travel"));

    assert(approxEqual(tracker.getTotalIncome(), 30000.0));
    assert(approxEqual(tracker.getTotalExpense(), 4200.0));
    assert(approxEqual(tracker.getBalance(), 25800.0));

    // Save to test file
    tracker.saveTransactions("test_tx.txt");

    // Load in fresh tracker
    ExpenseTracker tracker2;
    tracker2.clearData();
    tracker2.loadTransactions("test_tx.txt");

    assert(approxEqual(tracker2.getTotalIncome(), 30000.0));
    assert(approxEqual(tracker2.getTotalExpense(), 4200.0));
    assert(approxEqual(tracker2.getBalance(), 25800.0));

    // Test Edit
    int firstId = tracker2.getTransactions()[0]->getId();
    assert(tracker2.editTransaction(firstId, 28000.0, "2026-10-01", "Salary Revised"));
    assert(approxEqual(tracker2.getTotalIncome(), 33000.0));

    // Test Delete
    int foodId = tracker2.getTransactions()[2]->getId();
    assert(tracker2.deleteTransaction(foodId));
    assert(approxEqual(tracker2.getTotalExpense(), 3000.0));

    try {
        fs::remove("test_tx.txt");
    } catch (...) {}

    cout << "-> TEST 2 PASSED: ExpenseTracker operations and persistence verified." << endl;
}

void testBudgetAndReports()
{
    cout << "\n[TEST 3] Testing Budget & Reports Module..." << endl;
    vector<Transaction*> list;
    list.push_back(new Income(1, 20000.0, "2026-10-01", "Salary"));
    list.push_back(new Expense(2, 2500.0, "2026-10-02", "Food"));
    list.push_back(new Expense(3, 1000.0, "2026-10-03", "Travel"));
    list.push_back(new Expense(4, 800.0, "2026-10-04", "Food"));

    // Budget checks
    Budget bFood("Food", 3000.0);
    assert(approxEqual(bFood.getSpent(list), 3300.0));
    assert(bFood.isExceeded(list) == true);
    assert(approxEqual(bFood.getRemaining(list), -300.0));

    Budget bTravel("Travel", 2000.0);
    assert(approxEqual(bTravel.getSpent(list), 1000.0));
    assert(bTravel.isExceeded(list) == false);
    assert(approxEqual(bTravel.getRemaining(list), 1000.0));

    // Reports checks
    double inc = Reports::calculateTotalIncome(list);
    double exp = Reports::calculateTotalExpense(list);
    double bal = Reports::calculateBalance(list);
    assert(approxEqual(inc, 20000.0));
    assert(approxEqual(exp, 4300.0));
    assert(approxEqual(bal, 15700.0));

    auto catExpenses = Reports::getCategoryExpenses(list);
    assert(catExpenses.size() == 2);
    assert(catExpenses[0].first == "Food" && approxEqual(catExpenses[0].second, 3300.0));
    assert(catExpenses[1].first == "Travel" && approxEqual(catExpenses[1].second, 1000.0));

    for (auto* t : list) delete t;
    cout << "-> TEST 3 PASSED: Budget and Reports accurate." << endl;
}

int main()
{
    cout << "===============================================" << endl;
    cout << "  RUNNING EXPENSE TRACKING MODULE TEST SUITE   " << endl;
    cout << "===============================================" << endl;

    testTransactionPolymorphism();
    testExpenseTrackerOperations();
    testBudgetAndReports();

    cout << "\n===============================================" << endl;
    cout << "  ALL EXPENSE TRACKING TESTS PASSED (100%)!    " << endl;
    cout << "===============================================" << endl;

    return 0;
}