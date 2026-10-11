#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QTabWidget>
#include <QProgressBar>
#include <QCalendarWidget>

#include "authentication.h"
#include "ExpenseTracker.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Authentication slots
    void handleLogin();
    void handleRegister();
    void handleLogout();
    void toggleAuthMode();

    // Transaction slots
    void handleAddIncome();
    void handleAddExpense();
    void handleEditTransaction();
    void handleDeleteTransaction();
    void handleFilterTransactions();

    // Budget slots
    void handleSaveBudget();

    // Navigation and refresh
    void refreshAllViews();

private:
    // Core Domain Models
    Authentication auth;
    ExpenseTracker tracker;

    // Root UI
    QStackedWidget *rootStack;
    QWidget *authPage;
    QWidget *mainAppPage;

    // Auth UI elements
    QLineEdit *authUsernameInput;
    QLineEdit *authPasswordInput;
    QLineEdit *authConfirmPasswordInput;
    QLabel *authConfirmLabel;
    QLabel *authStatusLabel;
    QPushButton *authSubmitBtn;
    QPushButton *authSwitchBtn;
    bool isRegisterMode;

    // Header UI elements
    QLabel *userWelcomeLabel;
    QPushButton *logoutBtn;

    // Main App Navigation
    QTabWidget *appTabs;

    // 1. Dashboard Tab UI
    QLabel *dashIncomeLabel;
    QLabel *dashExpenseLabel;
    QLabel *dashBalanceLabel;
    QLabel *dashBudgetAlertBanner;
    QTableWidget *dashRecentTable;

    // 2. Income Tab UI
    QDoubleSpinBox *incomeAmountSpin;
    QDateEdit *incomeDateEdit;
    QComboBox *incomeSourceCombo;
    QTableWidget *incomeTable;
    QLabel *incomeTotalLabel;
    QLabel *incomeStatusLabel;

    // 3. Expense Tab UI
    QDoubleSpinBox *expenseAmountSpin;
    QDateEdit *expenseDateEdit;
    QComboBox *expenseCategoryCombo;
    QTableWidget *expenseTable;
    QLabel *expenseTotalLabel;
    QLabel *expenseStatusLabel;

    // 4. History Tab UI
    QLineEdit *historySearchInput;
    QComboBox *historyTypeFilter;
    QTableWidget *historyTable;
    QPushButton *editTransactionBtn;
    QPushButton *deleteTransactionBtn;
    QLabel *historyStatusLabel;

    // 5. Budget Tab UI
    QComboBox *budgetCategoryCombo;
    QDoubleSpinBox *budgetLimitSpin;
    QTableWidget *budgetTable;
    QLabel *budgetAlertLabel;
    QLabel *budgetStatusLabel;

    // 6. Reports Tab UI
    QLabel *repTotalIncomeLabel;
    QLabel *repTotalExpenseLabel;
    QLabel *repNetSavingsLabel;
    QTableWidget *repCategoryTable;
    QTableWidget *repMonthlyTable;

    // UI Builders
    void setupGlobalStyle();
    void setupAuthPage();
    void setupMainAppPage();
    QWidget* createDashboardTab();
    QWidget* createIncomeTab();
    QWidget* createExpenseTab();
    QWidget* createHistoryTab();
    QWidget* createBudgetTab();
    QWidget* createReportsTab();

    // UI Refreshers
    void updateDashboard();
    void updateIncomeTab();
    void updateExpenseTab();
    void updateHistoryTab();
    void updateBudgetTab();
    void updateReportsTab();

    // Calendar Popup Helper
    void applyCalendarStyle(QCalendarWidget *cal);
};

#endif // MAINWINDOW_H
