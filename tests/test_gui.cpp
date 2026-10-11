#include <QApplication>
#include <QTest>
#include <cassert>
#include <iostream>
#include <filesystem>

#include "MainWindow.h"

using namespace std;
namespace fs = std::filesystem;

class TestGui : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        cout << "\n========================================" << endl;
        cout << "  RUNNING EXPENSE TRACKER GUI TESTS" << endl;
        cout << "========================================" << endl;
    }

    void testCompleteGuiFlow()
    {
        cout << "\n[GUI TEST 1] Initializing MainWindow and Checking Auth Screen..." << endl;
        MainWindow window;
        window.show();
        QTest::qWait(200);

        // Find child widgets via qobject_cast / findChild
        auto* stack = window.findChild<QStackedWidget*>();
        QVERIFY(stack != nullptr);
        QCOMPARE(stack->currentIndex(), 0); // Auth page

        auto* userIn = window.findChild<QLineEdit*>();
        QVERIFY(userIn != nullptr);

        cout << "-> Auth screen initialized successfully." << endl;

        cout << "\n[GUI TEST 2] Testing User Registration & Login..." << endl;
        // Access tracker and auth directly or simulate UI
        // Let's create an isolated test user
        string testUser = "gui_student_test";
        string testPass = "CollegePass2026";

        // Clean previous test data for this test user to test clean state
        try {
            fs::remove("data/transactions_" + testUser + ".txt");
            fs::remove("data/budgets_" + testUser + ".txt");
        } catch (...) {}

        // Register directly using domain controller if not already registered
        Authentication authTester;
        authTester.registerUser(testUser, testPass);

        // Find username and password inputs in window
        auto lineEdits = window.findChildren<QLineEdit*>();
        QLineEdit *uEdit = nullptr;
        QLineEdit *pEdit = nullptr;
        for (auto* le : lineEdits)
        {
            if (le->placeholderText() == "Enter username") uEdit = le;
            if (le->placeholderText() == "Enter password") pEdit = le;
        }

        QVERIFY(uEdit != nullptr);
        QVERIFY(pEdit != nullptr);

        uEdit->setText(QString::fromStdString(testUser));
        pEdit->setText(QString::fromStdString(testPass));

        // Trigger login
        QMetaObject::invokeMethod(&window, "handleLogin", Qt::DirectConnection);
        QTest::qWait(200);

        QCOMPARE(stack->currentIndex(), 1); // Switched to main application page
        cout << "-> Successfully authenticated and transitioned to Dashboard." << endl;

        cout << "\n[GUI TEST 3] Adding Income and Expenses..." << endl;
        // Find inputs for Income
        auto spinBoxes = window.findChildren<QDoubleSpinBox*>();
        auto combos = window.findChildren<QComboBox*>();

        // Find tab widget
        auto* tabs = window.findChild<QTabWidget*>();
        QVERIFY(tabs != nullptr);

        // 1. Add Income: 15000.00 Salary
        tabs->setCurrentIndex(1); // Income tab
        QTest::qWait(100);

        auto* incSpin = window.findChild<QDoubleSpinBox*>("incomeAmountSpin");
        auto* incSrc = window.findChild<QComboBox*>("incomeSourceCombo");
        QVERIFY(incSpin != nullptr);
        QVERIFY(incSrc != nullptr);
        incSpin->setValue(15000.0);
        incSrc->setCurrentText("Salary");

        QMetaObject::invokeMethod(&window, "handleAddIncome", Qt::DirectConnection);
        QTest::qWait(100);

        // 2. Add Expense 1: 500.00 Food
        tabs->setCurrentIndex(2); // Expense tab
        QTest::qWait(100);

        auto* expSpin = window.findChild<QDoubleSpinBox*>("expenseAmountSpin");
        auto* expCat = window.findChild<QComboBox*>("expenseCategoryCombo");
        QVERIFY(expSpin != nullptr);
        QVERIFY(expCat != nullptr);
        expSpin->setValue(500.0);
        expCat->setCurrentText("Food");

        QMetaObject::invokeMethod(&window, "handleAddExpense", Qt::DirectConnection);
        QTest::qWait(100);

        // 3. Add Expense 2: 1000.00 Travel
        expSpin->setValue(1000.0);
        expCat->setCurrentText("Travel");
        QMetaObject::invokeMethod(&window, "handleAddExpense", Qt::DirectConnection);
        QTest::qWait(100);

        cout << "\n[GUI TEST 4] Verifying Dashboard Calculation Values..." << endl;
        tabs->setCurrentIndex(0); // Dashboard
        QTest::qWait(100);

        auto* dashInc = window.findChild<QLabel*>("dashIncomeLabel");
        auto* dashExp = window.findChild<QLabel*>("dashExpenseLabel");
        auto* dashBal = window.findChild<QLabel*>("dashBalanceLabel");
        QVERIFY(dashInc != nullptr);
        QVERIFY(dashExp != nullptr);
        cout << "dashInc text: " << dashInc->text().toStdString() << endl;
        cout << "dashExp text: " << dashExp->text().toStdString() << endl;
        cout << "dashBal text: " << dashBal->text().toStdString() << endl;

        QVERIFY(dashInc->text().contains("15000.00"));
        QVERIFY(dashExp->text().contains("1500.00"));
        QVERIFY(dashBal->text().contains("13500.00"));
        cout << "-> Dashboard figures verified exactly: Income=15000.00, Expenses=1500.00, Balance=13500.00." << endl;

        cout << "\n[GUI TEST 5] Testing Budget Alerts..." << endl;
        tabs->setCurrentIndex(4); // Budget tab
        QTest::qWait(100);

        auto* bCat = window.findChild<QComboBox*>("budgetCategoryCombo");
        auto* bLimit = window.findChild<QDoubleSpinBox*>("budgetLimitSpin");
        QVERIFY(bCat != nullptr);
        QVERIFY(bLimit != nullptr);

        bCat->setCurrentText("Food");
        bLimit->setValue(400.0);

        QMetaObject::invokeMethod(&window, "handleSaveBudget", Qt::DirectConnection);
        QTest::qWait(100);

        // Verify that budget tab and dashboard reflect exceeded budget
        tabs->setCurrentIndex(0); // Back to Dashboard
        QTest::qWait(100);

        auto* dashAlert = window.findChild<QLabel*>("dashBudgetAlertBanner");
        QVERIFY(dashAlert != nullptr);
        QVERIFY(dashAlert->isVisible());
        QVERIFY(dashAlert->text().contains("Food"));
        cout << "-> Budget overspending alert triggered and displayed on Dashboard." << endl;

        cout << "\n[GUI TEST 6] Testing Logout..." << endl;
        QMetaObject::invokeMethod(&window, "handleLogout", Qt::DirectConnection);
        QTest::qWait(100);
        QCOMPARE(stack->currentIndex(), 0); // Returned to Auth page
        cout << "-> Logged out successfully." << endl;

        cout << "\n[GUI TEST 7] Testing Persistence on New Session Window..." << endl;
        // Create second window simulating app restart
        MainWindow restartWindow;
        restartWindow.show();
        QTest::qWait(100);

        auto* uEdit2 = restartWindow.findChild<QLineEdit*>("authUsernameInput");
        auto* pEdit2 = restartWindow.findChild<QLineEdit*>("authPasswordInput");
        QVERIFY(uEdit2 != nullptr && pEdit2 != nullptr);
        uEdit2->setText(QString::fromStdString(testUser));
        pEdit2->setText(QString::fromStdString(testPass));

        QMetaObject::invokeMethod(&restartWindow, "handleLogin", Qt::DirectConnection);
        QTest::qWait(200);

        auto* stack2 = restartWindow.findChild<QStackedWidget*>();
        QVERIFY(stack2 != nullptr);
        QCOMPARE(stack2->currentIndex(), 1);

        // Check if totals persisted across application sessions
        auto* dashInc2 = restartWindow.findChild<QLabel*>("dashIncomeLabel");
        auto* dashExp2 = restartWindow.findChild<QLabel*>("dashExpenseLabel");
        auto* dashBal2 = restartWindow.findChild<QLabel*>("dashBalanceLabel");
        QVERIFY(dashInc2 != nullptr);
        QVERIFY(dashExp2 != nullptr);
        QVERIFY(dashBal2 != nullptr);

        QVERIFY(dashInc2->text().contains("15000.00"));
        QVERIFY(dashExp2->text().contains("1500.00"));
        QVERIFY(dashBal2->text().contains("13500.00"));
        cout << "-> Persistence confirmed: All transaction and calculation data retained after restart." << endl;

        cout << "\n========================================" << endl;
        cout << "  ALL GUI TESTS PASSED SUCCESSFULLY!" << endl;
        cout << "========================================" << endl;
    }
};

QTEST_MAIN(TestGui)
#include "test_gui.moc"
