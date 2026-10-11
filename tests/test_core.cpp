#include <iostream>
#include <cassert>
#include <vector>
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

static bool approxEqual(double a, double b)
{
    return std::abs(a - b) < 0.001;
}

void testOOPPolymorphism()
{
    cout << "\n[TEST 1] Verifying OOP Polymorphism & Inheritance..." << endl;
    vector<Transaction*> list;
    list.push_back(new Income(1, 15000.0, "2026-10-10", "Salary"));
    list.push_back(new Expense(2, 500.0, "2026-10-10", "Food"));
    list.push_back(new Expense(3, 1000.0, "2026-10-11", "Travel"));

    assert(list[0]->getType() == "Income");
    assert(list[0]->getCategoryOrSource() == "Salary");
    assert(approxEqual(list[0]->getAmount(), 15000.0));
    assert(list[0]->getId() == 1);

    assert(list[1]->getType() == "Expense");
    assert(list[1]->getCategoryOrSource() == "Food");
    assert(approxEqual(list[1]->getAmount(), 500.0));

    assert(list[2]->getType() == "Expense");
    assert(list[2]->getCategoryOrSource() == "Travel");
    assert(approxEqual(list[2]->getAmount(), 1000.0));

    // Polymorphic display test
    for (auto* t : list)
    {
        t->display();
        delete t;
    }
    cout << "-> TEST 1 PASSED: Polymorphism and virtual methods work correctly." << endl;
}

void testSaltedPasswordHashing()
{
    cout << "\n[TEST 2] Verifying Salted Password Hashing & Authentication..." << endl;
    // Clean test data directory
    try {
        fs::remove("data/users.txt");
    } catch (...) {}

    Authentication auth;
    // Attempt with short password
    assert(!auth.registerUser("testuser", "short"));
    assert(auth.getLastError() == "Password must contain at least 8 characters!");

    // Valid registration
    assert(auth.registerUser("alice", "Password123"));
    // Duplicate registration should fail
    assert(!auth.registerUser("alice", "AnotherPassword123"));
    assert(auth.getLastError() == "Username already exists!");

    // Salt check: register another user with identical plain password
    assert(auth.registerUser("bob", "Password123"));

    // Verify both users were saved to file
    vector<User> loadedUsers;
    FileManager::loadUsers(loadedUsers);
    assert(loadedUsers.size() >= 2);

    string aliceHash = "", bobHash = "", aliceSalt = "", bobSalt = "";
    for (const auto& u : loadedUsers)
    {
        if (u.getUsername() == "alice") { aliceHash = u.getPasswordHash(); aliceSalt = u.getSalt(); }
        if (u.getUsername() == "bob") { bobHash = u.getPasswordHash(); bobSalt = u.getSalt(); }
    }
    assert(!aliceSalt.empty() && !bobSalt.empty());
    assert(aliceSalt != bobSalt); // Different salts
    assert(aliceHash != bobHash); // Different hashes despite identical plain password!
    assert(aliceHash != "Password123"); // Not plain text!

    // Login tests
    assert(auth.loginUser("alice", "Password123"));
    assert(auth.isLoggedIn());
    assert(auth.getCurrentUser().getUsername() == "alice");

    // Login with wrong password
    Authentication auth2;
    assert(!auth2.loginUser("alice", "WrongPassword999"));
    assert(!auth2.isLoggedIn());

    auth.logoutUser();
    assert(!auth.isLoggedIn());

    cout << "-> TEST 2 PASSED: Salted SHA-256 hashing and authentication secure." << endl;
}

void testFinancialCalculationsAndReports()
{
    cout << "\n[TEST 3] Verifying Financial Calculations & Reports..." << endl;
    vector<Transaction*> list;
    list.push_back(new Income(1, 15000.0, "2026-10-01", "Salary"));
    list.push_back(new Income(2, 5000.0, "2026-10-05", "Freelancing"));
    list.push_back(new Expense(3, 500.0, "2026-10-08", "Food"));
    list.push_back(new Expense(4, 1000.0, "2026-10-10", "Travel"));
    list.push_back(new Expense(5, 1500.0, "2026-10-12", "Food"));

    double inc = Reports::calculateTotalIncome(list);
    double exp = Reports::calculateTotalExpense(list);
    double bal = Reports::calculateBalance(list);

    assert(approxEqual(inc, 20000.0));
    assert(approxEqual(exp, 3000.0));
    assert(approxEqual(bal, 17000.0));

    auto catExpenses = Reports::getCategoryExpenses(list);
    assert(catExpenses.size() == 2);
    // Food = 500 + 1500 = 2000.0
    assert(catExpenses[0].first == "Food" && approxEqual(catExpenses[0].second, 2000.0));
    // Travel = 1000.0
    assert(catExpenses[1].first == "Travel" && approxEqual(catExpenses[1].second, 1000.0));

    auto monthly = Reports::getMonthlySummaries(list);
    assert(monthly.size() == 1);
    assert(monthly[0].monthYear == "2026-10");
    assert(approxEqual(monthly[0].totalIncome, 20000.0));
    assert(approxEqual(monthly[0].totalExpense, 3000.0));
    assert(approxEqual(monthly[0].netSavings, 17000.0));

    for (auto* t : list) delete t;
    cout << "-> TEST 3 PASSED: Financial calculations and summaries accurate." << endl;
}

void testBudgetManagement()
{
    cout << "\n[TEST 4] Verifying Budget Management..." << endl;
    vector<Transaction*> list;
    list.push_back(new Expense(1, 600.0, "2026-10-01", "Food"));
    list.push_back(new Expense(2, 450.0, "2026-10-02", "Travel"));

    Budget bFood("Food", 500.0);
    Budget bTravel("Travel", 1000.0);

    assert(approxEqual(bFood.getSpent(list), 600.0));
    assert(approxEqual(bFood.getRemaining(list), -100.0));
    assert(bFood.isExceeded(list) == true);
    assert(approxEqual(bFood.getPercentageUsed(list), 120.0));

    assert(approxEqual(bTravel.getSpent(list), 450.0));
    assert(approxEqual(bTravel.getRemaining(list), 550.0));
    assert(bTravel.isExceeded(list) == false);
    assert(approxEqual(bTravel.getPercentageUsed(list), 45.0));

    for (auto* t : list) delete t;
    cout << "-> TEST 4 PASSED: Budget limits and overspending checks work as expected." << endl;
}

void testPersistenceAndUserIsolation()
{
    cout << "\n[TEST 5] Verifying Persistence & Multi-User Data Isolation..." << endl;
    string user1 = "test_user_alpha";
    string user2 = "test_user_beta";

    try {
        fs::remove("data/transactions_" + user1 + ".txt");
        fs::remove("data/transactions_" + user2 + ".txt");
        fs::remove("data/budgets_" + user1 + ".txt");
        fs::remove("data/budgets_" + user2 + ".txt");
    } catch (...) {}

    // Session 1: User 1 adds income and expense
    {
        ExpenseTracker tracker1(user1);
        tracker1.addIncome(15000.0, "2026-10-01", "Salary");
        tracker1.addExpense(500.0, "2026-10-02", "Food");
        tracker1.setBudget("Food", 1000.0);

        assert(approxEqual(tracker1.getTotalIncome(), 15000.0));
        assert(approxEqual(tracker1.getTotalExpense(), 500.0));
        assert(approxEqual(tracker1.getBalance(), 14500.0));
    }

    // Session 2: User 2 adds their own different transactions
    {
        ExpenseTracker tracker2(user2);
        tracker2.addIncome(4000.0, "2026-10-05", "Pocket Money");
        tracker2.addExpense(1200.0, "2026-10-06", "Books");

        assert(approxEqual(tracker2.getTotalIncome(), 4000.0));
        assert(approxEqual(tracker2.getTotalExpense(), 1200.0));
        assert(approxEqual(tracker2.getBalance(), 2800.0));
    }

    // Session 3: Reload User 1 and verify data didn't leak from User 2
    {
        ExpenseTracker tracker1(user1);
        assert(approxEqual(tracker1.getTotalIncome(), 15000.0));
        assert(approxEqual(tracker1.getTotalExpense(), 500.0));
        assert(approxEqual(tracker1.getBalance(), 14500.0));
        assert(tracker1.getTransactions().size() == 2);
        assert(tracker1.getBudgets().size() == 1);
        assert(tracker1.getBudgets()[0].getCategory() == "Food");

        // Test editing transaction
        int idToEdit = tracker1.getTransactions()[0]->getId();
        bool edited = tracker1.editTransaction(idToEdit, 18000.0, "2026-10-01", "Salary Revised");
        assert(edited);
        assert(approxEqual(tracker1.getTotalIncome(), 18000.0));

        // Test deleting transaction
        int idToDelete = tracker1.getTransactions()[1]->getId();
        bool deleted = tracker1.deleteTransaction(idToDelete);
        assert(deleted);
        assert(approxEqual(tracker1.getTotalExpense(), 0.0));
        assert(approxEqual(tracker1.getBalance(), 18000.0));
    }

    // Session 4: Re-open and verify edits/deletions persisted
    {
        ExpenseTracker tracker1(user1);
        assert(tracker1.getTransactions().size() == 1);
        assert(approxEqual(tracker1.getTotalIncome(), 18000.0));
        assert(approxEqual(tracker1.getBalance(), 18000.0));
    }

    cout << "-> TEST 5 PASSED: Multi-user isolation and persistence verified." << endl;
}

int main(int argc, char* argv[])
{
    cout << "========================================" << endl;
    cout << "  RUNNING EXPENSE TRACKER CORE TESTS" << endl;
    cout << "========================================" << endl;

    testOOPPolymorphism();
    testSaltedPasswordHashing();
    testFinancialCalculationsAndReports();
    testBudgetManagement();
    testPersistenceAndUserIsolation();

    cout << "\n========================================" << endl;
    cout << "  ALL 5 TEST PHASES PASSED SUCCESSFULLY!" << endl;
    cout << "========================================" << endl;
    return 0;
}
