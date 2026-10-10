#include <iostream>
#include "authentication.h"

using namespace std;

int main()
{
    Authentication auth;

    int choice;

    do
    {
        cout << "\n===== EXPENSE TRACKER =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Logout\n";
        cout << "4. Validate Password\n";
        cout << "5. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                string username, password;

                cout << "Enter username: ";
                cin >> username;

                cout << "Enter password: ";
                cin >> password;

                auth.registerUser(username, password);
                break;
            }

            case 2:
            {
                string username, password;

                cout << "Enter username: ";
                cin >> username;

                cout << "Enter password: ";
                cin >> password;

                auth.loginUser(username, password);
                break;
            }

            case 3:
                auth.logoutUser();
                break;

            case 4:
            {
                string password;

                cout << "Enter password: ";
                cin >> password;

                if (auth.validatePassword(password))
                {
                    cout << "Password length is valid!\n";
                }
                else
                {
                    cout << "Password must contain at least 8 characters!\n";
                }

                break;
            }

            case 5:
                cout << "Exiting Expense Tracker...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 5);

    return 0;
}