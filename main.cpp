#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Personal Expense Tracker");
    app.setOrganizationName("OOP PBL Team");

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}