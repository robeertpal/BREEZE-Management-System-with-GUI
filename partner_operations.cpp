#include "partner_operations.hpp"
#include "partner_manager.hpp"
#include "credentials_manager.hpp"
#include <iostream>
#include <algorithm>
#include <limits>
#include "guest_manager.hpp"
#include "guest.hpp"

std::vector<Partner> loadPartnersFromFile() {
    return loadPartners();
}

void savePartnersToFile(const std::vector<Partner>& partners) {
    savePartners(partners);
}

std::shared_ptr<Partner> loginPartnerStaff(const std::vector<Partner>& partners) {
    return loginPartner(partners);
}

void showPartnerBalance(const std::shared_ptr<Partner>& loggedPartner) {
    if (!loggedPartner) {
        std::cout << "Error: no partner is logged in." << std::endl;
        return;
    }
    std::cout << "Current balance for " << loggedPartner->getName()
              << ": " << loggedPartner->getBalance() << " lei" << std::endl;
}

void createPartnerWorker(const std::shared_ptr<Partner>& loggedPartner) {
    if (!loggedPartner) {
        std::cout << "Error: no partner is logged in." << std::endl;
        return;
    }
    auto creds = loadPartnerCredentials();
    int maxID = 0;
    for (const auto& c : creds) {
        maxID = std::max(maxID, c.staffID);
    }
    int newID = maxID + 1;

    std::string username;
    std::string password;
    std::cout << "Enter new worker username: "; std::getline(std::cin, username);
    std::cout << "Enter new worker password: "; std::getline(std::cin, password);

    creds.push_back({ loggedPartner->getID(), newID, username, password });
    savePartnerCredentials(creds);

    std::cout << "Worker created with staffID " << newID
              << " for partner " << loggedPartner->getName() << std::endl;
}

void createPartner(std::vector<Partner>& partners) {
    int maxID = 0;
    for (const auto& p : partners) {
        maxID = std::max(maxID, p.getID());
    }
    int newID = maxID + 1;

    std::string name;
    std::string type;
    float balance;
    std::cout << "Enter new partner name: "; std::getline(std::cin, name);
    std::cout << "Enter new partner type: "; std::getline(std::cin, type);
    std::cout << "Enter initial balance: "; std::cin >> balance;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    partners.emplace_back(newID, name, type, balance);
    savePartners(partners);
    std::cout << "Partner created with ID " << newID << ": " << name << std::endl;
}

void changeWorkerUsername(const std::shared_ptr<Partner>& loggedPartner,
                          int staffID) {
    if (!loggedPartner) {
        std::cout << "Error: no partner is logged in." << std::endl;
        return;
    }
    auto creds = loadPartnerCredentials();
    bool found = false;
    for (auto& c : creds) {
        if (c.partnerID == loggedPartner->getID() && c.staffID == staffID) {
            std::string newUsername;
            std::cout << "Enter new username: "; std::getline(std::cin, newUsername);
            c.username = newUsername;
            savePartnerCredentials(creds);
            std::cout << "Username changed successfully for staffID " << staffID << std::endl;
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Error: worker with staffID " << staffID << " not found for this partner." << std::endl;
    }
}

void changeWorkerPassword(const std::shared_ptr<Partner>& loggedPartner,
                          int staffID) {
    if (!loggedPartner) {
        std::cout << "Error: no partner is logged in." << std::endl;
        return;
    }
    auto creds = loadPartnerCredentials();
    bool found = false;
    for (auto& c : creds) {
        if (c.partnerID == loggedPartner->getID() && c.staffID == staffID) {
            std::string newPassword;
            std::cout << "Enter new password: "; std::getline(std::cin, newPassword);
            c.password = newPassword;
            savePartnerCredentials(creds);
            std::cout << "Password changed successfully for staffID " << staffID << std::endl;
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Error: worker with staffID " << staffID << " not found for this partner." << std::endl;
    }
}

void changePasswordPartner(const std::shared_ptr<Partner>& loggedPartner){
    if (!loggedPartner) {
        std::cout << "Error: no partner is logged in." << std::endl;
        return;
    }
    auto creds = loadPartnerCredentials();
    bool found = false;
    for (auto& c : creds) {
        if (c.partnerID == loggedPartner->getID()) {
            std::string newPassword;
            std::cout << "Enter new password: "; std::getline(std::cin, newPassword);
            c.password = newPassword;
            savePartnerCredentials(creds);
            std::cout << "Password changed successfully for partner " << loggedPartner->getName() << std::endl;
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Error: partner not found." << std::endl;
    }
}

void changeUsernamePartner(const std::shared_ptr<Partner>& loggedPartner){
    if (!loggedPartner) {
        std::cout << "Error: no partner is logged in." << std::endl;
        return;
    }
    auto creds = loadPartnerCredentials();
    bool found = false;
    for (auto& c : creds) {
        if (c.partnerID == loggedPartner->getID()) {
            std::string newUsername;
            std::cout << "Enter new username: "; std::getline(std::cin, newUsername);
            c.username = newUsername;
            savePartnerCredentials(creds);
            std::cout << "Username changed successfully for partner " << loggedPartner->getName() << std::endl;
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Error: partner not found." << std::endl;
    }
}

void registerPaymentToPartner(const std::shared_ptr<Partner>& loggedPartner,
    std::vector<Partner>& partners,
    std::vector<std::shared_ptr<Guest>>& guests) {
    if (!loggedPartner) {
        std::cout << "Error: nu e niciun partener autentificat." << std::endl;
        return;
    }

    std::string ticketCode;
    std::cout << "Introdu codul biletului clientului: ";
    std::getline(std::cin, ticketCode);

    // Căutăm guest-ul cu codul respectiv
    auto itGuest = std::find_if(guests.begin(), guests.end(),
        [&](const std::shared_ptr<Guest>& g) {
            return g->getTicket()->getTicketCode() == ticketCode;
        });
    if (itGuest == guests.end()) {
        std::cout << "Participant cu codul " << ticketCode << " nu a fost găsit!" << std::endl;
        return;
    }

    float amount;
    do {
        std::cout << "Introdu suma de încasat (lei): ";
        std::cin >> amount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (amount <= 0);

    try {
        (*itGuest)->getTicket()->getBracelet().pay(amount);

        int pid = loggedPartner->getID();
        for (auto& p : partners) {
            if (p.getID() == pid) {
                p.receivePayment(amount);
                break;
            }
        }
        
        saveGuestsToFile(guests);
        savePartnersToFile(partners);

        auto itP = std::find_if(partners.begin(), partners.end(),
            [&](const Partner& p){ return p.getID() == pid; });
        if (itP != partners.end()) {
            std::cout << "Încasare de " << amount << " lei realizată cu succes. "
                      << "Sold partener: " << itP->getBalance() << " lei." << std::endl;
        }
    } catch (const InsufficientFunds& e) {
        std::cerr << "Eroare: " << e.what() << std::endl;
    }
}
