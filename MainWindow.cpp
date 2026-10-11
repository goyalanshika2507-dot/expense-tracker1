#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QDialog>
#include <QDialogButtonBox>
#include <QTextCharFormat>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isRegisterMode(false)
{
    setWindowTitle("Personal Expense Tracker — C++ OOP PBL");
    resize(1020, 720);
    setMinimumSize(880, 600);

    setupGlobalStyle();

    rootStack = new QStackedWidget(this);
    setCentralWidget(rootStack);

    setupAuthPage();
    setupMainAppPage();

    rootStack->addWidget(authPage);    // Index 0
    rootStack->addWidget(mainAppPage); // Index 1
    rootStack->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupGlobalStyle()
{
    setStyleSheet(R"(
        QMainWindow {
            background-color: #f1f5f9;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        }
        QWidget {
            color: #1e293b;
            font-size: 13px;
        }
        QGroupBox {
            font-weight: bold;
            border: 1px solid #cbd5e1;
            border-radius: 8px;
            margin-top: 12px;
            padding-top: 14px;
            background-color: #ffffff;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 14px;
            padding: 0 6px;
            color: #334155;
        }
        QLineEdit, QDoubleSpinBox, QDateEdit, QComboBox {
            background-color: #ffffff;
            border: 1px solid #cbd5e1;
            border-radius: 6px;
            padding: 7px 10px;
            color: #0f172a;
        }
        QLineEdit:focus, QDoubleSpinBox:focus, QDateEdit:focus, QComboBox:focus {
            border: 1px solid #2563eb;
            background-color: #f8fafc;
        }
        QPushButton {
            background-color: #2563eb;
            color: #ffffff;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #1d4ed8;
        }
        QPushButton:pressed {
            background-color: #1e40af;
        }
        QTabWidget::pane {
            border: 1px solid #e2e8f0;
            background-color: #ffffff;
            border-radius: 8px;
        }
        QTabBar::tab {
            background-color: #e2e8f0;
            color: #475569;
            padding: 10px 20px;
            margin-right: 4px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            font-weight: 600;
        }
        QTabBar::tab:selected {
            background-color: #ffffff;
            color: #2563eb;
            border-bottom: 2px solid #2563eb;
        }
        QTableWidget {
            background-color: #ffffff;
            alternate-background-color: #f8fafc;
            border: 1px solid #e2e8f0;
            border-radius: 6px;
            gridline-color: #f1f5f9;
        }
        QHeaderView::section {
            background-color: #f1f5f9;
            color: #475569;
            padding: 8px;
            font-weight: bold;
            border: none;
            border-bottom: 1px solid #cbd5e1;
        }

        /* ── Calendar Popup – clean minimal card ────────────────────────── */
        QCalendarWidget {
            background-color: #ffffff;
            border: 1px solid #e2e8f0;
            border-radius: 10px;
            font-size: 12px;
            min-width: 230px;
            max-width: 260px;
        }

        /* Navigation bar (month + year header) */
        QCalendarWidget QWidget#qt_calendar_navigationbar {
            background-color: #ffffff;
            border-bottom: 1px solid #f1f5f9;
            padding: 6px 4px 4px 4px;
        }

        /* Month/year button in the navigation bar */
        QCalendarWidget QToolButton {
            background-color: transparent;
            color: #1e293b;
            font-size: 13px;
            font-weight: 700;
            border: none;
            border-radius: 4px;
            padding: 3px 6px;
        }
        QCalendarWidget QToolButton:hover {
            background-color: #f1f5f9;
            color: #2563eb;
        }

        /* Prev / Next arrow buttons */
        QCalendarWidget QToolButton#qt_calendar_prevmonth {
            qproperty-icon: none;
            qproperty-text: "↑";
            font-size: 14px;
            font-weight: 600;
            color: #64748b;
            padding: 2px 6px;
        }
        QCalendarWidget QToolButton#qt_calendar_nextmonth {
            qproperty-icon: none;
            qproperty-text: "↓";
            font-size: 14px;
            font-weight: 600;
            color: #64748b;
            padding: 2px 6px;
        }
        QCalendarWidget QToolButton#qt_calendar_prevmonth:hover,
        QCalendarWidget QToolButton#qt_calendar_nextmonth:hover {
            background-color: #f1f5f9;
            color: #2563eb;
        }

        /* Month/Year spinbox in calendar */
        QCalendarWidget QSpinBox {
            background-color: transparent;
            border: none;
            color: #1e293b;
            font-weight: 700;
            font-size: 13px;
            selection-background-color: #dbeafe;
            selection-color: #1e293b;
        }
        QCalendarWidget QSpinBox::up-button,
        QCalendarWidget QSpinBox::down-button {
            width: 0px;
            height: 0px;
        }

        /* Day-of-week header row */
        QCalendarWidget QAbstractItemView {
            background-color: #ffffff;
            alternate-background-color: #ffffff;
            selection-background-color: #2563eb;
            selection-color: #ffffff;
            outline: none;
            border: none;
            gridline-color: transparent;
            font-size: 12px;
        }

        /* Week number column (hidden by default) */
        QCalendarWidget QAbstractItemView:enabled {
            color: #374151;
        }
        QCalendarWidget QAbstractItemView:disabled {
            color: #cbd5e1;
        }

        /* Today's date */
        QCalendarWidget QAbstractItemView:enabled[today="true"] {
            color: #2563eb;
            font-weight: 700;
        }

        /* Clear / Today footer links */
        QCalendarWidget QWidget#qt_calendar_calendarview {
            background-color: #ffffff;
        }
    )");
}

// ── Calendar Popup Helper ────────────────────────────────────────────────────
// Configures a QCalendarWidget to match the clean minimal card style
// (reference: Mannat Events booking page design)
void MainWindow::applyCalendarStyle(QCalendarWidget *cal)
{
    if (!cal) return;

    // Use short 2-letter weekday names: Su Mo Tu We Th Fr Sa
    cal->setHorizontalHeaderFormat(QCalendarWidget::ShortDayNames);
    // Hide week number column on the left
    cal->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    // Show navigation bar with month/year
    cal->setNavigationBarVisible(true);
    // Show "Today" and "Clear" buttons in the navigation bar
    cal->setGridVisible(false);

    // Style the day cells: today highlighted in blue, weekends in natural color
    QTextCharFormat todayFmt;
    todayFmt.setBackground(QColor("#2563eb"));
    todayFmt.setForeground(QColor("#ffffff"));
    todayFmt.setFontWeight(QFont::Bold);
    cal->setDateTextFormat(QDate::currentDate(), todayFmt);

    // Style weekend days — subtle red, matching the reference
    QTextCharFormat wkndFmt;
    wkndFmt.setForeground(QColor("#ef4444"));
    cal->setWeekdayTextFormat(Qt::Saturday, wkndFmt);
    cal->setWeekdayTextFormat(Qt::Sunday, wkndFmt);

    // Weekdays — clean dark slate
    QTextCharFormat wkdayFmt;
    wkdayFmt.setForeground(QColor("#374151"));
    for (int d = Qt::Monday; d <= Qt::Friday; d++)
        cal->setWeekdayTextFormat(static_cast<Qt::DayOfWeek>(d), wkdayFmt);
}

void MainWindow::setupAuthPage()
{
    authPage = new QWidget(this);
    QVBoxLayout *outerLayout = new QVBoxLayout(authPage);
    outerLayout->setAlignment(Qt::AlignCenter);

    // Card frame
    QGroupBox *authCard = new QGroupBox(authPage);
    authCard->setFixedWidth(380);
    authCard->setStyleSheet(R"(
        QGroupBox {
            background-color: #ffffff;
            border: 1px solid #e2e8f0;
            border-radius: 12px;
            padding: 24px;
        }
    )");

    QVBoxLayout *cardLayout = new QVBoxLayout(authCard);
    cardLayout->setSpacing(14);

    QLabel *appTitle = new QLabel("Personal Expense Tracker", authCard);
    appTitle->setAlignment(Qt::AlignCenter);
    appTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: #1e293b;");
    cardLayout->addWidget(appTitle);

    QLabel *appSubtitle = new QLabel("OOP PBL Desktop Application", authCard);
    appSubtitle->setAlignment(Qt::AlignCenter);
    appSubtitle->setStyleSheet("font-size: 12px; color: #64748b; margin-bottom: 8px;");
    cardLayout->addWidget(appSubtitle);

    QLabel *userLabel = new QLabel("Username", authCard);
    userLabel->setStyleSheet("font-weight: 600;");
    cardLayout->addWidget(userLabel);

    authUsernameInput = new QLineEdit(authCard);
    authUsernameInput->setObjectName("authUsernameInput");
    authUsernameInput->setPlaceholderText("Enter username");
    cardLayout->addWidget(authUsernameInput);

    QLabel *passLabel = new QLabel("Password (min 8 chars)", authCard);
    passLabel->setStyleSheet("font-weight: 600;");
    cardLayout->addWidget(passLabel);

    authPasswordInput = new QLineEdit(authCard);
    authPasswordInput->setObjectName("authPasswordInput");
    authPasswordInput->setEchoMode(QLineEdit::Password);
    authPasswordInput->setPlaceholderText("Enter password");
    cardLayout->addWidget(authPasswordInput);

    authConfirmLabel = new QLabel("Confirm Password", authCard);
    authConfirmLabel->setStyleSheet("font-weight: 600;");
    authConfirmLabel->setVisible(false);
    cardLayout->addWidget(authConfirmLabel);

    authConfirmPasswordInput = new QLineEdit(authCard);
    authConfirmPasswordInput->setObjectName("authConfirmPasswordInput");
    authConfirmPasswordInput->setEchoMode(QLineEdit::Password);
    authConfirmPasswordInput->setPlaceholderText("Re-enter password");
    authConfirmPasswordInput->setVisible(false);
    cardLayout->addWidget(authConfirmPasswordInput);

    authStatusLabel = new QLabel("", authCard);
    authStatusLabel->setAlignment(Qt::AlignCenter);
    authStatusLabel->setStyleSheet("color: #ef4444; font-size: 12px; font-weight: 600;");
    cardLayout->addWidget(authStatusLabel);

    authSubmitBtn = new QPushButton("Login", authCard);
    authSubmitBtn->setFixedHeight(38);
    cardLayout->addWidget(authSubmitBtn);

    authSwitchBtn = new QPushButton("Don't have an account? Register", authCard);
    authSwitchBtn->setStyleSheet("background: transparent; color: #2563eb; font-weight: normal; text-decoration: underline;");
    authSwitchBtn->setCursor(Qt::PointingHandCursor);
    cardLayout->addWidget(authSwitchBtn);

    connect(authSubmitBtn, &QPushButton::clicked, this, [this]() {
        if (isRegisterMode) handleRegister();
        else handleLogin();
    });
    connect(authSwitchBtn, &QPushButton::clicked, this, &MainWindow::toggleAuthMode);

    outerLayout->addWidget(authCard);
}

void MainWindow::toggleAuthMode()
{
    isRegisterMode = !isRegisterMode;
    authStatusLabel->setText("");
    if (isRegisterMode)
    {
        authConfirmLabel->setVisible(true);
        authConfirmPasswordInput->setVisible(true);
        authSubmitBtn->setText("Create Account");
        authSwitchBtn->setText("Already have an account? Login");
    }
    else
    {
        authConfirmLabel->setVisible(false);
        authConfirmPasswordInput->setVisible(false);
        authSubmitBtn->setText("Login");
        authSwitchBtn->setText("Don't have an account? Register");
    }
}

void MainWindow::handleLogin()
{
    QString u = authUsernameInput->text().trimmed();
    QString p = authPasswordInput->text();

    if (u.isEmpty() || p.isEmpty())
    {
        authStatusLabel->setStyleSheet("color: #ef4444; font-weight: 600;");
        authStatusLabel->setText("Please enter both username and password.");
        return;
    }

    if (auth.loginUser(u.toStdString(), p.toStdString()))
    {
        tracker.setCurrentUser(u.toStdString());
        userWelcomeLabel->setText("Logged in: <b>" + u + "</b>");
        authPasswordInput->clear();
        authConfirmPasswordInput->clear();
        authStatusLabel->setText("");
        rootStack->setCurrentIndex(1);
        refreshAllViews();
    }
    else
    {
        authStatusLabel->setStyleSheet("color: #ef4444; font-weight: 600;");
        authStatusLabel->setText(QString::fromStdString(auth.getLastError()));
    }
}

void MainWindow::handleRegister()
{
    QString u = authUsernameInput->text().trimmed();
    QString p = authPasswordInput->text();
    QString cp = authConfirmPasswordInput->text();

    if (u.isEmpty() || p.isEmpty())
    {
        authStatusLabel->setStyleSheet("color: #ef4444; font-weight: 600;");
        authStatusLabel->setText("Username and password cannot be empty.");
        return;
    }

    if (p.length() < 8)
    {
        authStatusLabel->setStyleSheet("color: #ef4444; font-weight: 600;");
        authStatusLabel->setText("Password must contain at least 8 characters.");
        return;
    }

    if (p != cp)
    {
        authStatusLabel->setStyleSheet("color: #ef4444; font-weight: 600;");
        authStatusLabel->setText("Passwords do not match.");
        return;
    }

    if (auth.registerUser(u.toStdString(), p.toStdString()))
    {
        authStatusLabel->setStyleSheet("color: #10b981; font-weight: 600;");
        authStatusLabel->setText("Registration successful! You may now login.");
        toggleAuthMode();
    }
    else
    {
        authStatusLabel->setStyleSheet("color: #ef4444; font-weight: 600;");
        authStatusLabel->setText(QString::fromStdString(auth.getLastError()));
    }
}

void MainWindow::handleLogout()
{
    auth.logoutUser();
    tracker.clearData();
    authPasswordInput->clear();
    authConfirmPasswordInput->clear();
    authStatusLabel->setText("");
    rootStack->setCurrentIndex(0);
}

void MainWindow::setupMainAppPage()
{
    mainAppPage = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainAppPage);
    mainLayout->setContentsMargins(16, 12, 16, 16);
    mainLayout->setSpacing(12);

    // Top Header Bar
    QWidget *headerBar = new QWidget(mainAppPage);
    headerBar->setStyleSheet("background-color: #ffffff; border-radius: 8px; border: 1px solid #e2e8f0;");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerBar);
    headerLayout->setContentsMargins(16, 8, 16, 8);

    QLabel *brandLabel = new QLabel("💳 Expense Tracker", headerBar);
    brandLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1e293b;");
    headerLayout->addWidget(brandLabel);

    headerLayout->addStretch();

    userWelcomeLabel = new QLabel("Logged in: user", headerBar);
    userWelcomeLabel->setStyleSheet("color: #475569; font-size: 13px;");
    headerLayout->addWidget(userWelcomeLabel);

    logoutBtn = new QPushButton("Logout", headerBar);
    logoutBtn->setStyleSheet("background-color: #ef4444; color: white; border-radius: 6px; padding: 6px 14px;");
    headerLayout->addWidget(logoutBtn);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::handleLogout);

    mainLayout->addWidget(headerBar);

    // Navigation Tabs
    appTabs = new QTabWidget(mainAppPage);
    appTabs->addTab(createDashboardTab(), "📊 Dashboard");
    appTabs->addTab(createIncomeTab(), "💰 Add Income");
    appTabs->addTab(createExpenseTab(), "💸 Add Expense");
    appTabs->addTab(createHistoryTab(), "📜 History");
    appTabs->addTab(createBudgetTab(), "🎯 Budget");
    appTabs->addTab(createReportsTab(), "📈 Reports");

    connect(appTabs, &QTabWidget::currentChanged, this, [this](int) {
        refreshAllViews();
    });

    mainLayout->addWidget(appTabs);
}

QWidget* MainWindow::createDashboardTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(16);

    // KPI Summary Cards
    QHBoxLayout *kpiLayout = new QHBoxLayout();
    kpiLayout->setSpacing(16);

    auto makeCard = [](const QString &title, QLabel *&valLabel, const QString &bgColor, const QString &fgColor) -> QWidget* {
        QWidget *card = new QWidget();
        card->setStyleSheet(QString("background-color: %1; border-radius: 10px; border: 1px solid #e2e8f0;").arg(bgColor));
        QVBoxLayout *cLayout = new QVBoxLayout(card);
        cLayout->setContentsMargins(16, 14, 16, 14);

        QLabel *tLabel = new QLabel(title, card);
        tLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #64748b; text-transform: uppercase;");
        cLayout->addWidget(tLabel);

        valLabel = new QLabel("Rs. 0.00", card);
        valLabel->setStyleSheet(QString("font-size: 22px; font-weight: bold; color: %1;").arg(fgColor));
        cLayout->addWidget(valLabel);

        return card;
    };

    kpiLayout->addWidget(makeCard("Total Income", dashIncomeLabel, "#ecfdf5", "#059669"));
    dashIncomeLabel->setObjectName("dashIncomeLabel");
    kpiLayout->addWidget(makeCard("Total Expenses", dashExpenseLabel, "#fef2f2", "#dc2626"));
    dashExpenseLabel->setObjectName("dashExpenseLabel");
    kpiLayout->addWidget(makeCard("Current Balance", dashBalanceLabel, "#eff6ff", "#2563eb"));
    dashBalanceLabel->setObjectName("dashBalanceLabel");
    layout->addLayout(kpiLayout);

    // Budget Alert Banner
    dashBudgetAlertBanner = new QLabel("", tab);
    dashBudgetAlertBanner->setObjectName("dashBudgetAlertBanner");
    dashBudgetAlertBanner->setStyleSheet("background-color: #fef3c7; color: #b45309; padding: 10px; border-radius: 6px; font-weight: bold; border: 1px solid #fde68a;");
    dashBudgetAlertBanner->setVisible(false);
    layout->addWidget(dashBudgetAlertBanner);

    // Recent Transactions Section
    QGroupBox *recentBox = new QGroupBox("Recent Transactions", tab);
    QVBoxLayout *recentLayout = new QVBoxLayout(recentBox);

    dashRecentTable = new QTableWidget(recentBox);
    dashRecentTable->setObjectName("dashRecentTable");
    dashRecentTable->setColumnCount(5);
    dashRecentTable->setHorizontalHeaderLabels({"ID", "Type", "Amount", "Source / Category", "Date"});
    dashRecentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    dashRecentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    dashRecentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    recentLayout->addWidget(dashRecentTable);

    layout->addWidget(recentBox);
    return tab;
}

QWidget* MainWindow::createIncomeTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(14);

    QGroupBox *formBox = new QGroupBox("Add New Income Record", tab);
    QGridLayout *formGrid = new QGridLayout(formBox);
    formGrid->setSpacing(12);

    formGrid->addWidget(new QLabel("Amount (Rs.):"), 0, 0);
    incomeAmountSpin = new QDoubleSpinBox(formBox);
    incomeAmountSpin->setObjectName("incomeAmountSpin");
    incomeAmountSpin->setRange(1.0, 100000000.0);
    incomeAmountSpin->setValue(1000.0);
    formGrid->addWidget(incomeAmountSpin, 0, 1);

    formGrid->addWidget(new QLabel("Date:"), 0, 2);
    incomeDateEdit = new QDateEdit(QDate::currentDate(), formBox);
    incomeDateEdit->setObjectName("incomeDateEdit");
    incomeDateEdit->setCalendarPopup(true);
    incomeDateEdit->setDisplayFormat("yyyy-MM-dd");
    applyCalendarStyle(incomeDateEdit->calendarWidget());
    formGrid->addWidget(incomeDateEdit, 0, 3);

    formGrid->addWidget(new QLabel("Income Source:"), 1, 0);
    incomeSourceCombo = new QComboBox(formBox);
    incomeSourceCombo->setObjectName("incomeSourceCombo");
    incomeSourceCombo->addItems({"Salary", "Pocket Money", "Freelancing", "Investment", "Business", "Gift", "Other"});
    incomeSourceCombo->setEditable(true);
    formGrid->addWidget(incomeSourceCombo, 1, 1);

    QPushButton *addIncBtn = new QPushButton("➕ Add Income", formBox);
    addIncBtn->setStyleSheet("background-color: #059669; color: white; padding: 8px 16px;");
    formGrid->addWidget(addIncBtn, 1, 3);
    connect(addIncBtn, &QPushButton::clicked, this, &MainWindow::handleAddIncome);

    incomeStatusLabel = new QLabel("", formBox);
    incomeStatusLabel->setStyleSheet("font-size: 13px; padding-top: 4px;");
    formGrid->addWidget(incomeStatusLabel, 2, 0, 1, 4);

    layout->addWidget(formBox);

    // Income records table
    QGroupBox *tableBox = new QGroupBox("Recorded Incomes", tab);
    QVBoxLayout *tLayout = new QVBoxLayout(tableBox);

    incomeTable = new QTableWidget(tableBox);
    incomeTable->setColumnCount(4);
    incomeTable->setHorizontalHeaderLabels({"ID", "Amount (Rs.)", "Source", "Date"});
    incomeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    incomeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tLayout->addWidget(incomeTable);

    incomeTotalLabel = new QLabel("Total Income: Rs. 0.00", tableBox);
    incomeTotalLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #059669;");
    tLayout->addWidget(incomeTotalLabel);

    layout->addWidget(tableBox);
    return tab;
}

QWidget* MainWindow::createExpenseTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(14);

    QGroupBox *formBox = new QGroupBox("Add New Expense Record", tab);
    QGridLayout *formGrid = new QGridLayout(formBox);
    formGrid->setSpacing(12);

    formGrid->addWidget(new QLabel("Amount (Rs.):"), 0, 0);
    expenseAmountSpin = new QDoubleSpinBox(formBox);
    expenseAmountSpin->setObjectName("expenseAmountSpin");
    expenseAmountSpin->setRange(1.0, 100000000.0);
    expenseAmountSpin->setValue(200.0);
    formGrid->addWidget(expenseAmountSpin, 0, 1);

    formGrid->addWidget(new QLabel("Date:"), 0, 2);
    expenseDateEdit = new QDateEdit(QDate::currentDate(), formBox);
    expenseDateEdit->setObjectName("expenseDateEdit");
    expenseDateEdit->setCalendarPopup(true);
    expenseDateEdit->setDisplayFormat("yyyy-MM-dd");
    applyCalendarStyle(expenseDateEdit->calendarWidget());
    formGrid->addWidget(expenseDateEdit, 0, 3);

    formGrid->addWidget(new QLabel("Expense Category:"), 1, 0);
    expenseCategoryCombo = new QComboBox(formBox);
    expenseCategoryCombo->setObjectName("expenseCategoryCombo");
    expenseCategoryCombo->addItems({"Food", "Travel", "Shopping", "Education", "Bills", "Healthcare", "Entertainment", "Other"});
    expenseCategoryCombo->setEditable(true);
    formGrid->addWidget(expenseCategoryCombo, 1, 1);

    QPushButton *addExpBtn = new QPushButton("➕ Add Expense", formBox);
    addExpBtn->setStyleSheet("background-color: #dc2626; color: white; padding: 8px 16px;");
    formGrid->addWidget(addExpBtn, 1, 3);
    connect(addExpBtn, &QPushButton::clicked, this, &MainWindow::handleAddExpense);

    expenseStatusLabel = new QLabel("", formBox);
    expenseStatusLabel->setStyleSheet("font-size: 13px; padding-top: 4px;");
    formGrid->addWidget(expenseStatusLabel, 2, 0, 1, 4);

    layout->addWidget(formBox);

    // Expense records table
    QGroupBox *tableBox = new QGroupBox("Recorded Expenses", tab);
    QVBoxLayout *tLayout = new QVBoxLayout(tableBox);

    expenseTable = new QTableWidget(tableBox);
    expenseTable->setColumnCount(4);
    expenseTable->setHorizontalHeaderLabels({"ID", "Amount (Rs.)", "Category", "Date"});
    expenseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    expenseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tLayout->addWidget(expenseTable);

    expenseTotalLabel = new QLabel("Total Expenses: Rs. 0.00", tableBox);
    expenseTotalLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #dc2626;");
    tLayout->addWidget(expenseTotalLabel);

    layout->addWidget(tableBox);
    return tab;
}

QWidget* MainWindow::createHistoryTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(12);

    // Filters and Actions Bar
    QHBoxLayout *filterBar = new QHBoxLayout();

    QLabel *searchLbl = new QLabel("Search:", tab);
    filterBar->addWidget(searchLbl);

    historySearchInput = new QLineEdit(tab);
    historySearchInput->setPlaceholderText("Filter by ID, category, or date...");
    filterBar->addWidget(historySearchInput);
    connect(historySearchInput, &QLineEdit::textChanged, this, &MainWindow::handleFilterTransactions);

    QLabel *typeLbl = new QLabel("Type:", tab);
    filterBar->addWidget(typeLbl);

    historyTypeFilter = new QComboBox(tab);
    historyTypeFilter->addItems({"All Transactions", "Income Only", "Expense Only"});
    filterBar->addWidget(historyTypeFilter);
    connect(historyTypeFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::handleFilterTransactions);

    filterBar->addStretch();

    editTransactionBtn = new QPushButton("✏️ Edit Selected", tab);
    editTransactionBtn->setStyleSheet("background-color: #f59e0b; color: white;");
    filterBar->addWidget(editTransactionBtn);
    connect(editTransactionBtn, &QPushButton::clicked, this, &MainWindow::handleEditTransaction);

    deleteTransactionBtn = new QPushButton("🗑️ Delete Selected", tab);
    deleteTransactionBtn->setStyleSheet("background-color: #ef4444; color: white;");
    filterBar->addWidget(deleteTransactionBtn);
    connect(deleteTransactionBtn, &QPushButton::clicked, this, &MainWindow::handleDeleteTransaction);

    layout->addLayout(filterBar);

    historyStatusLabel = new QLabel("", tab);
    historyStatusLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    layout->addWidget(historyStatusLabel);

    historyTable = new QTableWidget(tab);
    historyTable->setColumnCount(5);
    historyTable->setHorizontalHeaderLabels({"ID", "Type", "Amount (Rs.)", "Source / Category", "Date"});
    historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    historyTable->setSelectionMode(QAbstractItemView::SingleSelection);
    layout->addWidget(historyTable);

    return tab;
}

QWidget* MainWindow::createBudgetTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(14);

    QGroupBox *budgetFormBox = new QGroupBox("Set Category Budget Limit", tab);
    QHBoxLayout *bForm = new QHBoxLayout(budgetFormBox);

    bForm->addWidget(new QLabel("Category:"));
    budgetCategoryCombo = new QComboBox(budgetFormBox);
    budgetCategoryCombo->setObjectName("budgetCategoryCombo");
    budgetCategoryCombo->addItems({"Overall", "Food", "Travel", "Shopping", "Education", "Bills", "Healthcare", "Entertainment", "Other"});
    budgetCategoryCombo->setEditable(true);
    bForm->addWidget(budgetCategoryCombo);

    bForm->addWidget(new QLabel("Limit Amount (Rs.):"));
    budgetLimitSpin = new QDoubleSpinBox(budgetFormBox);
    budgetLimitSpin->setObjectName("budgetLimitSpin");
    budgetLimitSpin->setRange(10.0, 100000000.0);
    budgetLimitSpin->setValue(2000.0);
    bForm->addWidget(budgetLimitSpin);

    QPushButton *saveBudgetBtn = new QPushButton("💾 Save Budget", budgetFormBox);
    saveBudgetBtn->setStyleSheet("background-color: #2563eb; color: white;");
    bForm->addWidget(saveBudgetBtn);
    connect(saveBudgetBtn, &QPushButton::clicked, this, &MainWindow::handleSaveBudget);

    layout->addWidget(budgetFormBox);

    budgetStatusLabel = new QLabel("", tab);
    budgetStatusLabel->setStyleSheet("font-size: 13px; font-weight: bold; padding: 2px 6px;");
    layout->addWidget(budgetStatusLabel);

    budgetAlertLabel = new QLabel("", tab);
    budgetAlertLabel->setStyleSheet("color: #b45309; font-weight: bold; padding: 6px;");
    layout->addWidget(budgetAlertLabel);

    QGroupBox *tableBox = new QGroupBox("Active Budgets & Spending Tracking", tab);
    QVBoxLayout *tLayout = new QVBoxLayout(tableBox);

    budgetTable = new QTableWidget(tableBox);
    budgetTable->setColumnCount(6);
    budgetTable->setHorizontalHeaderLabels({"Category", "Limit (Rs.)", "Spent (Rs.)", "Remaining (Rs.)", "Usage %", "Status"});
    budgetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    budgetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    budgetTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    tLayout->addWidget(budgetTable);

    layout->addWidget(tableBox);
    return tab;
}

QWidget* MainWindow::createReportsTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(14);

    // KPI Summary for Reports
    QHBoxLayout *kpiLayout = new QHBoxLayout();
    auto makeCard = [](const QString &title, QLabel *&valLabel, const QString &bgColor, const QString &fgColor) -> QWidget* {
        QWidget *card = new QWidget();
        card->setStyleSheet(QString("background-color: %1; border-radius: 8px; border: 1px solid #e2e8f0;").arg(bgColor));
        QVBoxLayout *cLayout = new QVBoxLayout(card);
        cLayout->setContentsMargins(14, 10, 14, 10);
        QLabel *tLabel = new QLabel(title, card);
        tLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #64748b; text-transform: uppercase;");
        cLayout->addWidget(tLabel);
        valLabel = new QLabel("Rs. 0.00", card);
        valLabel->setStyleSheet(QString("font-size: 18px; font-weight: bold; color: %1;").arg(fgColor));
        cLayout->addWidget(valLabel);
        return card;
    };

    kpiLayout->addWidget(makeCard("Lifetime Income", repTotalIncomeLabel, "#ecfdf5", "#059669"));
    kpiLayout->addWidget(makeCard("Lifetime Expenses", repTotalExpenseLabel, "#fef2f2", "#dc2626"));
    kpiLayout->addWidget(makeCard("Net Savings", repNetSavingsLabel, "#eff6ff", "#2563eb"));
    layout->addLayout(kpiLayout);

    QHBoxLayout *tablesLayout = new QHBoxLayout();
    tablesLayout->setSpacing(14);

    // Category Expense Breakdown
    QGroupBox *catBox = new QGroupBox("Expense Breakdown by Category", tab);
    QVBoxLayout *catLayout = new QVBoxLayout(catBox);
    repCategoryTable = new QTableWidget(catBox);
    repCategoryTable->setColumnCount(3);
    repCategoryTable->setHorizontalHeaderLabels({"Category", "Total Spent (Rs.)", "% of Expenses"});
    repCategoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    repCategoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    catLayout->addWidget(repCategoryTable);
    tablesLayout->addWidget(catBox);

    // Monthly Financial Breakdown
    QGroupBox *monthBox = new QGroupBox("Monthly Income & Expense Summary", tab);
    QVBoxLayout *monthLayout = new QVBoxLayout(monthBox);
    repMonthlyTable = new QTableWidget(monthBox);
    repMonthlyTable->setColumnCount(4);
    repMonthlyTable->setHorizontalHeaderLabels({"Month", "Income (Rs.)", "Expenses (Rs.)", "Net (Rs.)"});
    repMonthlyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    repMonthlyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    monthLayout->addWidget(repMonthlyTable);
    tablesLayout->addWidget(monthBox);

    layout->addLayout(tablesLayout);
    return tab;
}

void MainWindow::handleAddIncome()
{
    double amount = incomeAmountSpin->value();
    QString date = incomeDateEdit->date().toString("yyyy-MM-dd");
    QString source = incomeSourceCombo->currentText().trimmed();

    if (source.isEmpty())
    {
        incomeStatusLabel->setStyleSheet("color: #dc2626; font-weight: bold;");
        incomeStatusLabel->setText("❌ Income source cannot be empty.");
        return;
    }

    if (tracker.addIncome(amount, date.toStdString(), source.toStdString()))
    {
        refreshAllViews();
        incomeStatusLabel->setStyleSheet("color: #059669; font-weight: bold;");
        incomeStatusLabel->setText("✅ Income of Rs. " + QString::number(amount, 'f', 2) + " recorded successfully!");
    }
    else
    {
        incomeStatusLabel->setStyleSheet("color: #dc2626; font-weight: bold;");
        incomeStatusLabel->setText("❌ Failed to record income record.");
    }
}

void MainWindow::handleAddExpense()
{
    double amount = expenseAmountSpin->value();
    QString date = expenseDateEdit->date().toString("yyyy-MM-dd");
    QString category = expenseCategoryCombo->currentText().trimmed();

    if (category.isEmpty())
    {
        expenseStatusLabel->setStyleSheet("color: #dc2626; font-weight: bold;");
        expenseStatusLabel->setText("❌ Expense category cannot be empty.");
        return;
    }

    if (tracker.addExpense(amount, date.toStdString(), category.toStdString()))
    {
        refreshAllViews();

        // Check if adding this expense triggered an over-budget condition
        const Budget* b = tracker.getBudgetForCategory(category.toStdString());
        if (b && b->isExceeded(tracker.getTransactions()))
        {
            expenseStatusLabel->setStyleSheet("color: #dc2626; font-weight: bold;");
            expenseStatusLabel->setText(QString("⚠️ Expense recorded. Alert: Budget exceeded for '%1' (Limit: Rs. %2, Spent: Rs. %3)!")
                .arg(category)
                .arg(b->getLimitAmount(), 0, 'f', 2)
                .arg(b->getSpent(tracker.getTransactions()), 0, 'f', 2));
        }
        else
        {
            expenseStatusLabel->setStyleSheet("color: #059669; font-weight: bold;");
            expenseStatusLabel->setText("✅ Expense of Rs. " + QString::number(amount, 'f', 2) + " recorded successfully!");
        }
    }
    else
    {
        expenseStatusLabel->setStyleSheet("color: #dc2626; font-weight: bold;");
        expenseStatusLabel->setText("❌ Failed to record expense record.");
    }
}

void MainWindow::handleEditTransaction()
{
    int row = historyTable->currentRow();
    if (row < 0)
    {
        QMessageBox::information(this, "Selection Required", "Please select a transaction to edit.");
        return;
    }

    int id = historyTable->item(row, 0)->text().toInt();
    Transaction* t = tracker.findTransactionById(id);
    if (!t) return;

    QDialog editDlg(this);
    editDlg.setWindowTitle("Edit Transaction #" + QString::number(id));
    editDlg.setFixedSize(340, 240);

    QFormLayout form(&editDlg);
    QLabel typeLbl(QString::fromStdString(t->getType()), &editDlg);
    typeLbl.setStyleSheet("font-weight: bold;");
    form.addRow("Type:", &typeLbl);

    QDoubleSpinBox amountSpin(&editDlg);
    amountSpin.setRange(0.01, 100000000.0);
    amountSpin.setValue(t->getAmount());
    form.addRow("Amount (Rs.):", &amountSpin);

    QLineEdit catEdit(&editDlg);
    catEdit.setText(QString::fromStdString(t->getCategoryOrSource()));
    form.addRow(t->getType() == "Income" ? "Source:" : "Category:", &catEdit);

    QDateEdit dateEdit(&editDlg);
    dateEdit.setCalendarPopup(true);
    dateEdit.setDisplayFormat("yyyy-MM-dd");
    applyCalendarStyle(dateEdit.calendarWidget());
    QDate d = QDate::fromString(QString::fromStdString(t->getDate()), "yyyy-MM-dd");
    if (!d.isValid()) d = QDate::currentDate();
    dateEdit.setDate(d);
    form.addRow("Date:", &dateEdit);

    QDialogButtonBox btnBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &editDlg);
    form.addRow(&btnBox);
    connect(&btnBox, &QDialogButtonBox::accepted, &editDlg, &QDialog::accept);
    connect(&btnBox, &QDialogButtonBox::rejected, &editDlg, &QDialog::reject);

    if (editDlg.exec() == QDialog::Accepted)
    {
        double newAmt = amountSpin.value();
        string newCat = catEdit.text().trimmed().toStdString();
        string newDate = dateEdit.date().toString("yyyy-MM-dd").toStdString();

        if (newCat.empty())
        {
            QMessageBox::warning(this, "Validation Error", "Category/Source cannot be empty.");
            return;
        }

        if (tracker.editTransaction(id, newAmt, newDate, newCat))
        {
            refreshAllViews();
            QMessageBox::information(this, "Updated", "Transaction #" + QString::number(id) + " updated successfully.");
        }
    }
}

void MainWindow::handleDeleteTransaction()
{
    int row = historyTable->currentRow();
    if (row < 0)
    {
        QMessageBox::information(this, "Selection Required", "Please select a transaction to delete.");
        return;
    }

    int id = historyTable->item(row, 0)->text().toInt();
    auto reply = QMessageBox::question(this, "Confirm Deletion",
        "Are you sure you want to delete Transaction #" + QString::number(id) + "?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (tracker.deleteTransaction(id))
        {
            refreshAllViews();
            QMessageBox::information(this, "Deleted", "Transaction #" + QString::number(id) + " deleted successfully.");
        }
    }
}

void MainWindow::handleFilterTransactions()
{
    updateHistoryTab();
}

void MainWindow::handleSaveBudget()
{
    QString cat = budgetCategoryCombo->currentText().trimmed();
    double limit = budgetLimitSpin->value();

    if (cat.isEmpty())
    {
        budgetStatusLabel->setStyleSheet("color: #dc2626; font-weight: bold;");
        budgetStatusLabel->setText("❌ Category cannot be empty.");
        return;
    }

    tracker.setBudget(cat.toStdString(), limit);
    refreshAllViews();
    budgetStatusLabel->setStyleSheet("color: #059669; font-weight: bold;");
    budgetStatusLabel->setText("✅ Budget of Rs. " + QString::number(limit, 'f', 2) + " set for '" + cat + "'.");
}

void MainWindow::refreshAllViews()
{
    updateDashboard();
    updateIncomeTab();
    updateExpenseTab();
    updateHistoryTab();
    updateBudgetTab();
    updateReportsTab();
}

void MainWindow::updateDashboard()
{
    dashIncomeLabel->setText("Rs. " + QString::number(tracker.getTotalIncome(), 'f', 2));
    dashExpenseLabel->setText("Rs. " + QString::number(tracker.getTotalExpense(), 'f', 2));
    dashBalanceLabel->setText("Rs. " + QString::number(tracker.getBalance(), 'f', 2));

    // Check for budget alerts
    QStringList exceededCategories;
    for (const auto& b : tracker.getBudgets())
    {
        if (b.isExceeded(tracker.getTransactions()))
        {
            exceededCategories << QString::fromStdString(b.getCategory());
        }
    }

    if (!exceededCategories.isEmpty())
    {
        dashBudgetAlertBanner->setText("⚠️ Budget Alert: You have exceeded budget limits for: " + exceededCategories.join(", ") + "!");
        dashBudgetAlertBanner->setVisible(true);
    }
    else
    {
        dashBudgetAlertBanner->setVisible(false);
    }

    // Recent Transactions (up to last 10)
    const auto& list = tracker.getTransactions();
    dashRecentTable->setRowCount(0);
    int startIdx = std::max(0, static_cast<int>(list.size()) - 10);
    int row = 0;
    for (int i = static_cast<int>(list.size()) - 1; i >= startIdx; --i)
    {
        const auto* t = list[i];
        if (!t) continue;
        dashRecentTable->insertRow(row);
        dashRecentTable->setItem(row, 0, new QTableWidgetItem(QString::number(t->getId())));

        QTableWidgetItem *typeItem = new QTableWidgetItem(QString::fromStdString(t->getType()));
        if (t->getType() == "Income") typeItem->setForeground(QColor("#059669"));
        else typeItem->setForeground(QColor("#dc2626"));
        dashRecentTable->setItem(row, 1, typeItem);

        dashRecentTable->setItem(row, 2, new QTableWidgetItem("Rs. " + QString::number(t->getAmount(), 'f', 2)));
        dashRecentTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(t->getCategoryOrSource())));
        dashRecentTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(t->getDate())));
        row++;
    }
}

void MainWindow::updateIncomeTab()
{
    const auto& list = tracker.getTransactions();
    incomeTable->setRowCount(0);
    double total = 0.0;
    int row = 0;

    for (const auto* t : list)
    {
        if (t && t->getType() == "Income")
        {
            incomeTable->insertRow(row);
            incomeTable->setItem(row, 0, new QTableWidgetItem(QString::number(t->getId())));
            incomeTable->setItem(row, 1, new QTableWidgetItem("Rs. " + QString::number(t->getAmount(), 'f', 2)));
            incomeTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(t->getCategoryOrSource())));
            incomeTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(t->getDate())));
            total += t->getAmount();
            row++;
        }
    }
    incomeTotalLabel->setText("Total Income: Rs. " + QString::number(total, 'f', 2));
}

void MainWindow::updateExpenseTab()
{
    const auto& list = tracker.getTransactions();
    expenseTable->setRowCount(0);
    double total = 0.0;
    int row = 0;

    for (const auto* t : list)
    {
        if (t && t->getType() == "Expense")
        {
            expenseTable->insertRow(row);
            expenseTable->setItem(row, 0, new QTableWidgetItem(QString::number(t->getId())));
            expenseTable->setItem(row, 1, new QTableWidgetItem("Rs. " + QString::number(t->getAmount(), 'f', 2)));
            expenseTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(t->getCategoryOrSource())));
            expenseTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(t->getDate())));
            total += t->getAmount();
            row++;
        }
    }
    expenseTotalLabel->setText("Total Expenses: Rs. " + QString::number(total, 'f', 2));
}

void MainWindow::updateHistoryTab()
{
    const auto& list = tracker.getTransactions();
    historyTable->setRowCount(0);

    QString filterQuery = historySearchInput->text().trimmed().toLower();
    int filterType = historyTypeFilter->currentIndex(); // 0 = All, 1 = Income, 2 = Expense

    int row = 0;
    for (const auto* t : list)
    {
        if (!t) continue;

        if (filterType == 1 && t->getType() != "Income") continue;
        if (filterType == 2 && t->getType() != "Expense") continue;

        QString idStr = QString::number(t->getId());
        QString typeStr = QString::fromStdString(t->getType());
        QString catStr = QString::fromStdString(t->getCategoryOrSource());
        QString dateStr = QString::fromStdString(t->getDate());

        if (!filterQuery.isEmpty())
        {
            if (!idStr.contains(filterQuery) &&
                !catStr.toLower().contains(filterQuery) &&
                !dateStr.contains(filterQuery))
            {
                continue;
            }
        }

        historyTable->insertRow(row);
        historyTable->setItem(row, 0, new QTableWidgetItem(idStr));

        QTableWidgetItem *typeItem = new QTableWidgetItem(typeStr);
        if (t->getType() == "Income") typeItem->setForeground(QColor("#059669"));
        else typeItem->setForeground(QColor("#dc2626"));
        historyTable->setItem(row, 1, typeItem);

        historyTable->setItem(row, 2, new QTableWidgetItem("Rs. " + QString::number(t->getAmount(), 'f', 2)));
        historyTable->setItem(row, 3, new QTableWidgetItem(catStr));
        historyTable->setItem(row, 4, new QTableWidgetItem(dateStr));
        row++;
    }
}

void MainWindow::updateBudgetTab()
{
    const auto& budgets = tracker.getBudgets();
    const auto& trans = tracker.getTransactions();
    budgetTable->setRowCount(0);

    bool anyExceeded = false;
    for (size_t i = 0; i < budgets.size(); ++i)
    {
        const auto& b = budgets[i];
        budgetTable->insertRow(static_cast<int>(i));

        double spent = b.getSpent(trans);
        double rem = b.getRemaining(trans);
        double pct = b.getPercentageUsed(trans);
        bool exceeded = b.isExceeded(trans);
        if (exceeded) anyExceeded = true;

        budgetTable->setItem(static_cast<int>(i), 0, new QTableWidgetItem(QString::fromStdString(b.getCategory())));
        budgetTable->setItem(static_cast<int>(i), 1, new QTableWidgetItem("Rs. " + QString::number(b.getLimitAmount(), 'f', 2)));
        budgetTable->setItem(static_cast<int>(i), 2, new QTableWidgetItem("Rs. " + QString::number(spent, 'f', 2)));
        budgetTable->setItem(static_cast<int>(i), 3, new QTableWidgetItem("Rs. " + QString::number(rem, 'f', 2)));
        budgetTable->setItem(static_cast<int>(i), 4, new QTableWidgetItem(QString::number(pct, 'f', 1) + " %"));

        QTableWidgetItem *statusItem = new QTableWidgetItem(exceeded ? "⚠️ EXCEEDED" : "✅ Normal");
        if (exceeded)
        {
            statusItem->setForeground(QColor("#dc2626"));
            statusItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
        }
        else
        {
            statusItem->setForeground(QColor("#059669"));
        }
        budgetTable->setItem(static_cast<int>(i), 5, statusItem);
    }

    if (anyExceeded)
    {
        budgetAlertLabel->setText("⚠️ Attention: One or more category spending budgets have exceeded their assigned limits.");
    }
    else
    {
        budgetAlertLabel->setText("");
    }
}

void MainWindow::updateReportsTab()
{
    const auto& trans = tracker.getTransactions();
    double inc = Reports::calculateTotalIncome(trans);
    double exp = Reports::calculateTotalExpense(trans);
    double bal = Reports::calculateBalance(trans);

    repTotalIncomeLabel->setText("Rs. " + QString::number(inc, 'f', 2));
    repTotalExpenseLabel->setText("Rs. " + QString::number(exp, 'f', 2));
    repNetSavingsLabel->setText("Rs. " + QString::number(bal, 'f', 2));

    // Category breakdown
    auto catList = Reports::getCategoryExpenses(trans);
    repCategoryTable->setRowCount(0);
    for (size_t i = 0; i < catList.size(); ++i)
    {
        repCategoryTable->insertRow(static_cast<int>(i));
        double pct = (exp > 0) ? (catList[i].second / exp) * 100.0 : 0.0;
        repCategoryTable->setItem(static_cast<int>(i), 0, new QTableWidgetItem(QString::fromStdString(catList[i].first)));
        repCategoryTable->setItem(static_cast<int>(i), 1, new QTableWidgetItem("Rs. " + QString::number(catList[i].second, 'f', 2)));
        repCategoryTable->setItem(static_cast<int>(i), 2, new QTableWidgetItem(QString::number(pct, 'f', 1) + " %"));
    }

    // Monthly breakdown
    auto monthlyList = Reports::getMonthlySummaries(trans);
    repMonthlyTable->setRowCount(0);
    for (size_t i = 0; i < monthlyList.size(); ++i)
    {
        repMonthlyTable->insertRow(static_cast<int>(i));
        repMonthlyTable->setItem(static_cast<int>(i), 0, new QTableWidgetItem(QString::fromStdString(monthlyList[i].monthYear)));
        repMonthlyTable->setItem(static_cast<int>(i), 1, new QTableWidgetItem("Rs. " + QString::number(monthlyList[i].totalIncome, 'f', 2)));
        repMonthlyTable->setItem(static_cast<int>(i), 2, new QTableWidgetItem("Rs. " + QString::number(monthlyList[i].totalExpense, 'f', 2)));
        repMonthlyTable->setItem(static_cast<int>(i), 3, new QTableWidgetItem("Rs. " + QString::number(monthlyList[i].netSavings, 'f', 2)));
    }
}
