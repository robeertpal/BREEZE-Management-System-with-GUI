// credentials_operations.cpp
#include "credentials_operations.hpp"

#include <iostream>
#include <limits>

#include "credentials_manager.hpp"
#include "staff_manager.hpp"
#include "staff_operations.hpp"  // toLower()

void changeUsername(std::shared_ptr<Staff>& loggedStaff,
                    std::vector<std::shared_ptr<Staff>>& staffList) 
{
    std::string newUsername;
    std::cout << "Introdu noul username: ";
    std::getline(std::cin, newUsername);
    newUsername = toLower(newUsername);

    try {
        validateUsername(newUsername);
    } catch (const Exception& e) {
        std::cerr << "Eroare: " << e.what() << "\n";
        return;
    }

    auto credentialsList = loadCredentials();

    const_cast<Credentials&>(loggedStaff->getCredentials())
        .changeUsername(newUsername);

    for (auto& cred : credentialsList) {
        if (cred.id == loggedStaff->getStaffID()) {
            cred.username = newUsername;
            break;
        }
    }

    saveCredentials(credentialsList);
    saveStaffToFile(staffList);

    std::cout << "Username schimbat cu succes!\n";
}

void changeUsernameForStaff(std::vector<std::shared_ptr<Staff>>& staffList) {
    std::string currentUsername;
    std::cout << "Introdu username-ul actual al agentului caruia vrei sa-i schimbi username-ul: ";
    std::getline(std::cin, currentUsername);
    currentUsername = toLower(currentUsername);

    for (auto& staff : staffList) {
        // comparăm în lowercase
        std::string stored = toLower(staff->getCredentials().getUsername());
        std::cout << "Stored: " << stored << ", Current: " << currentUsername << "\n";
        if (stored == currentUsername) {
            std::string newUsername;
            while (true) {
                std::cout << "Introdu noul username: ";
                std::getline(std::cin, newUsername);
                newUsername = toLower(newUsername);
                try {
                    validateUsername(newUsername);
                    break;
                } catch (const Exception& e) {
                    std::cerr << "Eroare: " << e.what() << "\n";
                }
            }
            // actualizează în obiect și în fișier
            auto& creds = const_cast<Credentials&>(staff->getCredentials());
            creds.changeUsername(newUsername);

            auto credentialsList = loadCredentials();
            for (auto& cred : credentialsList) {
                if (cred.id == staff->getStaffID()) {
                    cred.username = newUsername;
                    break;
                }
            }

            saveCredentials(credentialsList);
            saveStaffToFile(staffList);

            std::cout << "Username-ul a fost modificat cu succes!\n";
            return;
        }
    }

    std::cout << "Agentul cu acest username nu a fost gasit!\n";
}

void changePassword(std::shared_ptr<Staff>& loggedStaff,
                    std::vector<std::shared_ptr<Staff>>& staffList) 
{
    std::string oldPassword, newPassword;
    std::cout << "Introdu parola curenta: ";
    std::getline(std::cin, oldPassword);

    auto credentialsList = loadCredentials();
    bool found = false;

    for (auto& cred : credentialsList) {
        if (cred.id == loggedStaff->getStaffID()) {
            if (cred.password != oldPassword) {
                std::cout << "Parola curenta este gresita!\n";
                return;
            }
            found = true;
            while (true) {
                std::cout << "Introdu noua parola (minim 6 caractere): ";
                std::getline(std::cin, newPassword);
                try {
                    if (newPassword.size() < 6) throw WeakPassword();
                    break;
                } catch (const WeakPassword& e) {
                    std::cerr << "Eroare: " << e.what() << "\n";
                }
            }
            cred.password = newPassword;
            break;
        }
    }

    if (found) {
        saveCredentials(credentialsList);
        saveStaffToFile(staffList);
        std::cout << "Parola a fost schimbata cu succes!\n";
    } else {
        std::cout << "Nu s-a gasit credentialul asociat contului!\n";
    }
}

void changePasswordForStaff(std::vector<std::shared_ptr<Staff>>& staffList) 
{
    std::string username;
    std::cout << "Introdu username-ul agentului caruia vrei sa-i schimbi parola: ";
    std::getline(std::cin, username);

    auto credentialsList = loadCredentials();
    bool found = false;

    for (auto& staff : staffList) {
        if (staff->getCredentials().getUsername() == username) {
            found = true;
            std::string newPassword;
            while (true) {
                std::cout << "Introdu noua parola (minim 6 caractere): ";
                std::getline(std::cin, newPassword);
                try {
                    if (newPassword.size() < 6) throw WeakPassword();
                    break;
                } catch (const WeakPassword& e) {
                    std::cerr << "Eroare: " << e.what() << "\n";
                }
            }
            for (auto& cred : credentialsList) {
                if (cred.id == staff->getStaffID()) {
                    cred.password = newPassword;
                    break;
                }
            }
            saveCredentials(credentialsList);
            saveStaffToFile(staffList);
            std::cout << "Parola agentului a fost schimbata cu succes!\n";
            return;
        }
    }

    if (!found) {
        std::cout << "Agentul cu acest username nu a fost gasit!\n";
    }
}
