#include "staff_operations.hpp"

#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>

std::string toLower(const std::string& str) {
    std::string lowerStr;
    lowerStr.reserve(str.size());
    for (unsigned char ch : str) {
        lowerStr += static_cast<char>(std::tolower(ch));
    }
    return lowerStr;
}

std::shared_ptr<Staff> loginStaff(const std::vector<std::shared_ptr<Staff>>& staffList) {
    std::string username, password;
    std::cout << "Username: ";
    std::getline(std::cin, username);
    username = toLower(username);

    std::cout << "Parola: ";
    std::getline(std::cin, password);

    auto credentialsList = loadCredentials();
    for (const auto& cred : credentialsList) {
        if (cred.username == username && cred.password == password) {
            for (const auto& staff : staffList) {
                if (staff->getStaffID() == cred.id) {
                    std::cout << "Autentificare reusită! ID: " << staff->getStaffID() << "\n";
                    return staff;
                }
            }
        }
    }

    std::cout << "Username sau parola invalidă!\n";
    return nullptr;
}

void addStaffMember(std::vector<std::shared_ptr<Staff>>& staffList) {
    std::string firstName, lastName, emailStr, phoneStr, nationality;
    int day, month, year;
    std::string username, password, role;

    std::cout << "=== Adaugare nou membru staff ===\n";
    std::cout << "Prenume: "; std::getline(std::cin, firstName);
    std::cout << "Nume: ";   std::getline(std::cin, lastName);

    do {
        std::cout << "Email: "; std::getline(std::cin, emailStr);
        try {
            validateEmail(emailStr);
            break;
        } catch (const InvalidEmail& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        }
    } while (true);

    do {
        std::cout << "Telefon: "; std::getline(std::cin, phoneStr);
        try {
            validatePhoneNumber(phoneStr);
            break;
        } catch (const InvalidPhoneNumber& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        }
    } while (true);

    do {
        std::cout << "Data nașterii (zi lună an): ";
        std::cin >> day >> month >> year;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        try {
            validateDate(day, month, year);
            validateAdult(year, month, day);
            break;
        } catch (const InvalidDate& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        } catch (const AgeRestriction& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        }
    } while (true);

    std::cout << "Naționalitate: "; std::getline(std::cin, nationality);

    do {
        std::cout << "Username: "; std::getline(std::cin, username);
        username = toLower(username);
        try {
            validateUsername(username);
            break;
        } catch (const UsernameAlreadyExists& e) {
            std::cerr << "Eroare: " << e.what() << "\nIntrodu un alt username!\n";
        }
    } while (true);

    do {
        std::cout << "Parola trebuie să aibă minim 6 caractere: "; std::getline(std::cin, password);
        try {
            if (password.size() < 6) throw WeakPassword();
            break;
        } catch (const WeakPassword& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        }
    } while (true);

    do {
        std::cout << "Rol (TicketAgent / SecurityAgent / TopUpAgent): "; std::getline(std::cin, role);
        if (role == "TicketAgent" || role == "SecurityAgent" || role == "TopUpAgent") break;
        std::cerr << "Eroare: Rol invalid!\n";
    } while (true);

    Member member(
        firstName, lastName,
        Email(emailStr), PhoneNumber(phoneStr),
        Date(day, month, year),
        nationality
    );
    Credentials credentials(username, password);

    std::shared_ptr<Staff> newStaff;
    if (role == "TicketAgent") {
        newStaff = std::make_shared<TicketAgent>(member, credentials);
    } else if (role == "SecurityAgent") {
        newStaff = std::make_shared<SecurityAgent>(member, credentials);
    } else {
        newStaff = std::make_shared<TopUpAgent>(member, credentials);
    }

    newStaff->setStaffID(static_cast<int>(staffList.size()));
    staffList.push_back(newStaff);
    saveStaffToFile(staffList);

    auto creds = loadCredentials();
    creds.push_back({newStaff->getStaffID(), username, password});
    saveCredentials(creds);

    std::cout << "Membru staff adăugat și salvat cu succes!\n";
}

void removeStaffMember(std::vector<std::shared_ptr<Staff>>& staffList) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string username;
    std::cout << "Introduceți username-ul agentului de șters: ";
    std::getline(std::cin, username);

    auto credentialsList = loadCredentials();
    auto credIt = std::find_if(
        credentialsList.begin(), credentialsList.end(),
        [&](const Credential& cred){ return cred.username == username; }
    );

    if (credIt != credentialsList.end()) {
        int staffIdToRemove = credIt->id;
        credentialsList.erase(credIt);
        saveCredentials(credentialsList);

        auto staffIt = std::remove_if(
            staffList.begin(), staffList.end(),
            [&](const std::shared_ptr<Staff>& staff){
                return staff->getStaffID() == staffIdToRemove;
            }
        );

        if (staffIt != staffList.end()) {
            staffList.erase(staffIt, staffList.end());
            saveStaffToFile(staffList);
            std::cout << "Membru staff șters cu succes!\n";
        } else {
            std::cout << "Eroare: Membru staff nu a fost găsit după ID-ul din credențiale!\n";
        }
    } else {
        std::cout << "Nu s-a găsit niciun agent cu username-ul introdus.\n";
    }
}

void showStaffByDepartment(const std::vector<std::shared_ptr<Staff>>& staffList) {
    int option;
    do {
        std::cout << "\n=== Vizualizare Staff ===\n"
                  << "1. Ticket Agents\n"
                  << "2. Security Agents\n"
                  << "3. Top Up Agents\n"
                  << "4. Toți agenții\n"
                  << "0. Înapoi\n"
                  << "Optiunea ta: ";
        std::cin >> option;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (option) {
            case 1:
                std::cout << "\n--- Ticket Agents ---\n";
                for (const auto& s : staffList)
                    if (dynamic_cast<TicketAgent*>(s.get())) {
                        s->show(); std::cout << "--------\n";
                    }
                break;
            case 2:
                std::cout << "\n--- Security Agents ---\n";
                for (const auto& s : staffList)
                    if (dynamic_cast<SecurityAgent*>(s.get())) {
                        s->show(); std::cout << "--------\n";
                    }
                break;
            case 3:
                std::cout << "\n--- Top Up Agents ---\n";
                for (const auto& s : staffList)
                    if (dynamic_cast<TopUpAgent*>(s.get())) {
                        s->show(); std::cout << "--------\n";
                    }
                break;
            case 4:
                std::cout << "\n--- Toți Agenții ---\n";
                for (const auto& s : staffList) {
                    s->show(); std::cout << "--------\n";
                }
                break;
            case 0: return;
            default:
                std::cout << "Opțiune invalidă! Încearcă din nou.\n";
        }
    } while (true);
}

void changeFirstNameForStaff(std::vector<std::shared_ptr<Staff>>& staffList) {
    std::string username;
    std::cout << "Introdu username-ul agentului căruia vrei să-i schimbi prenumele: ";
    std::getline(std::cin, username);

    for (auto& s : staffList) {
        if (s->getCredentials().getUsername() == username) {
            std::string newFirstName;
            std::cout << "Introdu noul prenume: ";
            std::getline(std::cin, newFirstName);
            const_cast<Member&>(s->getMember()).setFirstName(newFirstName);
            saveStaffToFile(staffList);
            std::cout << "Prenumele a fost modificat cu succes!\n";
            return;
        }
    }
    std::cout << "Nu s-a găsit niciun agent cu username-ul introdus!\n";
}

void changeLastNameForStaff(std::vector<std::shared_ptr<Staff>>& staffList) {
    std::string username;
    std::cout << "Introdu username-ul agentului căruia vrei să-i schimbi numele: ";
    std::getline(std::cin, username);

    for (auto& s : staffList) {
        if (s->getCredentials().getUsername() == username) {
            std::string newLastName;
            std::cout << "Introdu noul nume: ";
            std::getline(std::cin, newLastName);
            const_cast<Member&>(s->getMember()).setLastName(newLastName);
            saveStaffToFile(staffList);
            std::cout << "Numele a fost modificat cu succes!\n";
            return;
        }
    }
    std::cout << "Nu s-a găsit niciun agent cu username-ul introdus!\n";
}

void changeBirthDateForStaff(std::vector<std::shared_ptr<Staff>>& staffList) {
    std::string username;
    std::cout << "Introdu username-ul agentului căruia vrei să-i schimbi data nașterii: ";
    std::getline(std::cin, username);

    for (auto& s : staffList) {
        if (s->getCredentials().getUsername() == username) {
            int newDay, newMonth, newYear;
            do {
                std::cout << "Introdu noua dată a nașterii (zi lună an): ";
                std::cin >> newDay >> newMonth >> newYear;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                try {
                    validateDate(newDay, newMonth, newYear);
                    validateAdult(newDay, newMonth, newYear);
                    break;
                } catch (const InvalidDate& e) {
                    std::cerr << "Eroare: " << e.what() << "\n";
                } catch (const AgeRestriction& e) {
                    std::cerr << "Eroare: " << e.what() << "\n";
                }
            } while (true);

            const_cast<Member&>(s->getMember()).setBirthDate(Date(newDay, newMonth, newYear));
            saveStaffToFile(staffList);
            std::cout << "Data nașterii a fost modificată cu succes!\n";
            return;
        }
    }
    std::cout << "Nu s-a găsit niciun agent cu username-ul introdus!\n";
}

void changePhoneForStaff(std::vector<std::shared_ptr<Staff>>& staffList) {
    std::string username;
    std::cout << "Introdu username-ul agentului căruia vrei să-i schimbi numărul de telefon: ";
    std::getline(std::cin, username);

    for (auto& s : staffList) {
        if (s->getCredentials().getUsername() == username) {
            std::string newPhone;
            do {
                std::cout << "Introdu noul număr de telefon: ";
                std::getline(std::cin, newPhone);
                try {
                    validatePhoneNumber(newPhone);
                    break;
                } catch (const InvalidPhoneNumber& e) {
                    std::cerr << "Eroare: " << e.what() << "\n";
                }
            } while (true);

            const_cast<Member&>(s->getMember()).setPhone(PhoneNumber(newPhone));
            saveStaffToFile(staffList);
            std::cout << "Numărul de telefon a fost modificat cu succes!\n";
            return;
        }
    }
    std::cout << "Nu s-a găsit niciun agent cu username-ul introdus!\n";
}

void changeNationalityForStaff(std::vector<std::shared_ptr<Staff>>& staffList) {
    std::string username;
    std::cout << "Introdu username-ul agentului căruia vrei să-i schimbi naționalitatea: ";
    std::getline(std::cin, username);

    for (auto& s : staffList) {
        if (s->getCredentials().getUsername() == username) {
            std::string newNat;
            std::cout << "Introdu noua naționalitate: ";
            std::getline(std::cin, newNat);
            const_cast<Member&>(s->getMember()).setNationality(newNat);
            saveStaffToFile(staffList);
            std::cout << "Naționalitatea a fost modificată cu succes!\n";
            return;
        }
    }
    std::cout << "Nu s-a găsit niciun agent cu username-ul introdus!\n";
}

void changeRoleForStaff(std::vector<std::shared_ptr<Staff>>& staffList) {
    std::string username;
    std::cout << "Introdu username-ul agentului căruia vrei să-i schimbi rolul: ";
    std::getline(std::cin, username);

    for (auto it = staffList.begin(); it != staffList.end(); ++it) {
        if ((*it)->getCredentials().getUsername() == username) {
            std::string newRole;
            std::cout << "Rol nou (TicketAgent / SecurityAgent / TopUpAgent): ";
            std::getline(std::cin, newRole);

            if (newRole != "TicketAgent" && newRole != "SecurityAgent" && newRole != "TopUpAgent") {
                std::cout << "Rol invalid! Operația a fost anulată.\n";
                return;
            }

            Member member = (*it)->getMember();
            Credentials creds = (*it)->getCredentials();
            staffList.erase(it);

            if (newRole == "TicketAgent") {
                staffList.push_back(std::make_shared<TicketAgent>(member, creds));
            } else if (newRole == "SecurityAgent") {
                staffList.push_back(std::make_shared<SecurityAgent>(member, creds));
            } else {
                staffList.push_back(std::make_shared<TopUpAgent>(member, creds));
            }

            saveStaffToFile(staffList);
            std::cout << "Rolul agentului a fost modificat cu succes!\n";
            return;
        }
    }
    std::cout << "Nu s-a găsit niciun agent cu username-ul introdus!\n";
}

void changeEmailForStaff(std::vector<std::shared_ptr<Staff>>& staffList) {
    std::string username;
    std::cout << "Introdu username-ul agentului căruia vrei să-i schimbi email-ul: ";
    std::getline(std::cin, username);

    for (auto& s : staffList) {
        if (s->getCredentials().getUsername() == username) {
            std::string newEmail;
            do {
                std::cout << "Introdu noul email: ";
                std::getline(std::cin, newEmail);
                try {
                    validateEmail(newEmail);
                    break;
                } catch (const InvalidEmail& e) {
                    std::cerr << "Eroare: " << e.what() << "\n";
                }
            } while (true);

            const_cast<Member&>(s->getMember()).setEmail(Email(newEmail));
            saveStaffToFile(staffList);
            std::cout << "Email-ul a fost modificat cu succes!\n";
            return;
        }
    }
    std::cout << "Nu s-a găsit niciun agent cu username-ul introdus!\n";
}
