cmake_minimum_required(VERSION 3.16)

project(ExpenseTracker VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Test)

set(CORE_SOURCES
    user.h
    user.cpp
    authentication.h
    authentication.cpp
    Transaction.h
    Transaction.cpp
    Income.h
    Income.cpp
    Expense.h
    Expense.cpp
    Budget.h
    Budget.cpp
    Reports.h
    Reports.cpp
    FileManager.h
    FileManager.cpp
    ExpenseTracker.h
    ExpenseTracker.cpp
)

# Core Unit Tests
add_executable(test_core
    tests/test_core.cpp
    ${CORE_SOURCES}
)
target_include_directories(test_core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_core PRIVATE Qt6::Core Qt6::Widgets Qt6::Network)

# End-to-End Workflow Verification Tests
add_executable(test_workflows
    tests/test_workflows.cpp
    ${CORE_SOURCES}
)
target_include_directories(test_workflows PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_workflows PRIVATE Qt6::Core Qt6::Widgets Qt6::Network)

# Full Application
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/MainWindow.h")
    add_executable(ExpenseTracker
        main.cpp
        MainWindow.h
        MainWindow.cpp
        ${CORE_SOURCES}
    )
    target_include_directories(ExpenseTracker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ExpenseTracker PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

    # Automated GUI Integration Tests
    add_executable(test_gui
        tests/test_gui.cpp
        MainWindow.h
        MainWindow.cpp
        ${CORE_SOURCES}
    )
    target_include_directories(test_gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(test_gui PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network Qt6::Test)
endif()
