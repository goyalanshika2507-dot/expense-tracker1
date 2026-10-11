#include <iostream>
#include <cassert>
#include <cmath>
#include <filesystem>
#include "user.h"
#include "authentication.h"
#include "Transaction.h"
#include "Income.h"
#include "Expense.h"
#include "Budget.h"
#include "Reports.h"
#include "FileManager.h"
#include "ExpenseTracker.h"

using namespace std;
namespace fs = std::filesystem;

static bool approx(double a, double b)
{
    return std::abs(a - b) < 0.001;
}

int main()
{
    cout << "=====================================================" << endl;
    cout << "  EXPENSE TRACKER - 9 END-TO-END WORKFLOW VERIFICATION" << endl;
    cout << "=====================================================" << endl;

    string u1 = "eval_user_one";
    string u2 = "eval_user_two";
    string pass1 = "SuperSecure2026!";
    string pass2 = "AnotherPass2026!";

    // Clean previous test runs for clean verification
    try {
        fs::remove("data/transactions_" + u1 + ".txt");
        fs::remove("data/budgets_" + u1 + ".txt");
        fs::remove("data/transactions_" + u2 + ".txt");
        fs::remove("data/budgets_" + u2 + ".txt");
    } catch (...) {}

    // ----------------------------------------------------
    // STEP 1: Register new user and log in
    // ----------------------------------------------------
    cout << "\n[STEP 1] Register new user and log in..." << endl;
    Authentication auth;
    bool regOk = auth.registerUser(u1, pass1);
    if (!regOk && auth.getLastError() != "Username already exists!")
    {
        cerr << "Registration failed: " << auth.getLastError() << endl;
        return 1;
    }
    bool loginOk = auth.loginUser(u1, pass1);
    assert(loginOk);
    assert(auth.isLoggedIn());
    assert(auth.getCurrentUser().getUsername() == u1);
    cout << "-> STEP 1 SUCCESS: Registered and authenticated user '" << u1 << "'." << endl;

    // ----------------------------------------------------
    // STEP 2: Add income and expenses
    // ----------------------------------------------------
    cout << "\n[STEP 2] Adding income (15000) and expenses (500 Food, 1000 Travel)..." << endl;
    ExpenseTracker tracker(u1);
    assert(tracker.addIncome(15000.0, "2026-10-10", "Salary"));
    assert(tracker.addExpense(500.0, "2026-10-10", "Food"));
    assert(tracker.addExpense(1000.0, "2026-10-11", "Travel"));
    assert(tracker.getTransactions().size() == 3);
    cout << "-> STEP 2 SUCCESS: 1 Income and 2 Expense records added." << endl;

    // ----------------------------------------------------
    // STEP 3: Verify dashboard totals and balance
    // ----------------------------------------------------
    cout << "\n[STEP 3] Verifying dashboard totals and balance..." << endl;
    double inc = tracker.getTotalIncome();
    double exp = tracker.getTotalExpense();
    double bal = tracker.getBalance();
    cout << "   Total Income   : Rs. " << inc << " (Expected: 15000.00)" << endl;
    cout << "   Total Expenses : Rs. " << exp << " (Expected: 1500.00)" << endl;
    cout << "   Net Balance    : Rs. " << bal << " (Expected: 13500.00)" << endl;
    assert(approx(inc, 15000.0));
    assert(approx(exp, 1500.0));
    assert(approx(bal, 13500.0));
    cout << "-> STEP 3 SUCCESS: Dashboard totals and balance match calculations perfectly." << endl;

    // ----------------------------------------------------
    // STEP 4: Edit and delete transactions
    // ----------------------------------------------------
    cout << "\n[STEP 4] Editing and deleting transactions..." << endl;
    int foodId = tracker.getTransactions()[1]->getId();
    int travelId = tracker.getTransactions()[2]->getId();

    // Edit Food from 500 to 700
    assert(tracker.editTransaction(foodId, 700.0, "2026-10-10", "Food & Groceries"));
    assert(approx(tracker.findTransactionById(foodId)->getAmount(), 700.0));
    assert(tracker.findTransactionById(foodId)->getCategoryOrSource() == "Food & Groceries");

    // Delete Travel (1000)
    assert(tracker.deleteTransaction(travelId));
    assert(tracker.getTransactions().size() == 2);
    assert(tracker.findTransactionById(travelId) == nullptr);

    // Verify recalculation
    cout << "   Post-edit Total Expenses : Rs. " << tracker.getTotalExpense() << " (Expected: 700.00)" << endl;
    cout << "   Post-edit Net Balance    : Rs. " << tracker.getBalance() << " (Expected: 14300.00)" << endl;
    assert(approx(tracker.getTotalExpense(), 700.0));
    assert(approx(tracker.getBalance(), 14300.0));
    cout << "-> STEP 4 SUCCESS: Edit and delete operations updated records and dashboard values." << endl;

    // ----------------------------------------------------
    // STEP 5: Set a budget and trigger an overspending alert
    // ----------------------------------------------------
    cout << "\n[STEP 5] Setting budget (Food = 600.00) and checking overspending..." << endl;
    tracker.setBudget("Food & Groceries", 600.0);
    const Budget* b = tracker.getBudgetForCategory("Food & Groceries");
    assert(b != nullptr);
    double spentOnFood = b->getSpent(tracker.getTransactions());
    double limitOnFood = b->getLimitAmount();
    double remaining = b->getRemaining(tracker.getTransactions());
    bool exceeded = b->isExceeded(tracker.getTransactions());

    cout << "   Budget Limit : Rs. " << limitOnFood << endl;
    cout << "   Amount Spent : Rs. " << spentOnFood << endl;
    cout << "   Remaining    : Rs. " << remaining << endl;
    cout << "   Overspent?   : " << (exceeded ? "YES (ALERT TRIGGERED)" : "NO") << endl;
    assert(approx(spentOnFood, 700.0));
    assert(approx(limitOnFood, 600.0));
    assert(approx(remaining, -100.0));
    assert(exceeded == true);
    cout << "-> STEP 5 SUCCESS: Budget limit tracked and overspending alert triggered." << endl;

    // ----------------------------------------------------
    // STEP 6: Verify reports
    // ----------------------------------------------------
    cout << "\n[STEP 6] Verifying financial reports and breakdowns..." << endl;
    auto catBreakdown = Reports::getCategoryExpenses(tracker.getTransactions());
    assert(catBreakdown.size() == 1);
    assert(catBreakdown[0].first == "Food & Groceries");
    assert(approx(catBreakdown[0].second, 700.0));

    auto monthlyBreakdown = Reports::getMonthlySummaries(tracker.getTransactions());
    assert(monthlyBreakdown.size() == 1);
    assert(monthlyBreakdown[0].monthYear == "2026-10");
    assert(approx(monthlyBreakdown[0].totalIncome, 15000.0));
    assert(approx(monthlyBreakdown[0].totalExpense, 700.0));
    assert(approx(monthlyBreakdown[0].netSavings, 14300.0));
    cout << "-> STEP 6 SUCCESS: Category-wise and monthly reports match stored transactions." << endl;

    // ----------------------------------------------------
    // STEP 7: Log out and log in again
    // ----------------------------------------------------
    cout << "\n[STEP 7] Logging out and logging back in..." << endl;
    auth.logoutUser();
    assert(!auth.isLoggedIn());
    assert(auth.loginUser(u1, pass1));
    assert(auth.isLoggedIn());
    cout << "-> STEP 7 SUCCESS: User logged out and authenticated back in." << endl;

    // ----------------------------------------------------
    // STEP 8: Close and restart application, verify data persistence
    // ----------------------------------------------------
    cout << "\n[STEP 8] Simulating application restart and verifying persistent storage..." << endl;
    // Instantiate brand new tracker simulating cold launch
    ExpenseTracker restartTracker(u1);
    assert(restartTracker.getTransactions().size() == 2);
    assert(approx(restartTracker.getTotalIncome(), 15000.0));
    assert(approx(restartTracker.getTotalExpense(), 700.0));
    assert(approx(restartTracker.getBalance(), 14300.0));
    assert(restartTracker.getBudgets().size() == 1);
    assert(restartTracker.getBudgets()[0].isExceeded(restartTracker.getTransactions()) == true);
    cout << "-> STEP 8 SUCCESS: All incomes, expenses, budgets, and alert states persisted." << endl;

    // ----------------------------------------------------
    // STEP 9: Register second user and verify data isolation
    // ----------------------------------------------------
    cout << "\n[STEP 9] Registering second user and verifying strict multi-user isolation..." << endl;
    Authentication auth2;
    auth2.registerUser(u2, pass2);
    assert(auth2.loginUser(u2, pass2));

    ExpenseTracker user2Tracker(u2);
    cout << "   User 2 Total Income   : Rs. " << user2Tracker.getTotalIncome() << endl;
    cout << "   User 2 Total Expenses : Rs. " << user2Tracker.getTotalExpense() << endl;
    cout << "   User 2 Net Balance    : Rs. " << user2Tracker.getBalance() << endl;
    cout << "   User 2 Transactions   : " << user2Tracker.getTransactions().size() << endl;
    cout << "   User 2 Budgets        : " << user2Tracker.getBudgets().size() << endl;

    // Crucial isolation check: User 2 must see 0 transactions and 0 budgets
    assert(user2Tracker.getTransactions().empty());
    assert(user2Tracker.getBudgets().empty());
    assert(approx(user2Tracker.getTotalIncome(), 0.0));
    assert(approx(user2Tracker.getTotalExpense(), 0.0));
    assert(approx(user2Tracker.getBalance(), 0.0));
    cout << "-> STEP 9 SUCCESS: User 2 cannot access any of User 1's records. Strict data isolation verified!" << endl;

    cout << "\n=====================================================" << endl;
    cout << "  ALL 9 WORKFLOWS VERIFIED AND PASSED 100%!" << endl;
    cout << "=====================================================" << endl;
    return 0;
}
