# Personal Expense Tracker (C++ OOP PBL)

A desktop Personal Expense Tracker built in C++ using Object-Oriented Programming (OOP) principles and the Qt 6 framework.

---

## 1. Project Overview & Team Roles

This application was developed as an Object-Oriented Programming Problem-Based Learning (PBL) college project:

- **Member 1 (Anshika): Authentication Module**
  - User registration, login, logout, session tracking (`user.h`/`user.cpp`, `authentication.h`/`authentication.cpp`).
  - Cryptographic salted SHA-256 password hashing.
- **Member 2 (Agrim): Transaction Module**
  - OOP inheritance hierarchy: Base class `Transaction` with specialized derived classes `Income` and `Expense` (`Transaction.h`/`Transaction.cpp`, `Income.h`/`Income.cpp`, `Expense.h`/`Expense.cpp`).
  - Runtime polymorphism via virtual functions (`display()`, `getType()`, `getCategoryOrSource()`).
- **Member 3 (Garima): Budget & Reports Module**
  - Category and overall budget limit tracking and overspending alerts (`Budget.h`/`Budget.cpp`).
  - Financial calculations, category breakdowns, and monthly summaries (`Reports.h`/`Reports.cpp`).
- **Member 4 (Amruta): File Handling & Integration**
  - Multi-user persistent storage (`FileManager.h`/`FileManager.cpp`).
  - Isolation of user transactions (`data/transactions_<username>.txt`) and budgets (`data/budgets_<username>.txt`).
  - Application coordination (`ExpenseTracker.h`/`ExpenseTracker.cpp`, `MainWindow.h`/`MainWindow.cpp`).

---

## 2. Key Features

1. **Secure Authentication:**
   - Registration with password validation ($\ge 8$ characters).
   - Salted SHA-256 password hashing per user (no plain text passwords).
2. **Interactive Dashboard:**
   - Real-time KPI summaries: Total Income, Total Expenses, Current Balance.
   - Dynamic over-budget alerts.
   - Recent transactions table.
3. **Income Management:**
   - Add income with amount, date, and source (Salary, Freelancing, Pocket Money, Investment, Business, etc.).
   - History table of recorded incomes.
4. **Expense Management:**
   - Add expenses with amount, date, and category (Food, Travel, Shopping, Education, Bills, Healthcare, Entertainment, etc.).
   - Instant budget checking and category warning indicators.
5. **Transaction History:**
   - Search by keyword or ID, filter by type (Income/Expense/All).
   - Edit and delete transaction records with persistent updates.
6. **Budget Management:**
   - Set monthly spending limits per category.
   - Visual progress indicators and warnings when spending exceeds limits.
7. **Reports & Summaries:**
   - Category-wise expense distribution and percentages.
   - Monthly summary table (Income, Expenses, Net Savings).
8. **Multi-User Isolation & Persistence:**
   - Data persists across application restarts.
   - Every user's records remain private and isolated from others.

---

## 3. How to Build and Run

### Prerequisites
- Qt 6 (Qt 6.11.2 or compatible, with MinGW 64-bit)
- CMake 3.16+
- Ninja build tool (or MinGW32-make)

### Build Command (Windows PowerShell)

```powershell
# Set Qt and MinGW in PATH
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.11.2\mingw_64\bin;C:\Qt\Tools\Ninja;C:\Qt\Tools\CMake_64\bin;" + $env:PATH

# Configure project
cmake -B build -G "Ninja" -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" -DCMAKE_PREFIX_PATH="C:/Qt/6.11.2/mingw_64" -DCMAKE_MAKE_PROGRAM="C:/Qt/Tools/Ninja/ninja.exe"

# Build full application and tests
cmake --build build --target ExpenseTracker test_core test_gui
```

### Running the Application

```powershell
# Run the Desktop GUI Application
.\build\ExpenseTracker.exe

# Run the Automated Core Unit Tests
.\build\test_core.exe

# Run the Automated Qt GUI Tests
.\build\test_gui.exe
```
