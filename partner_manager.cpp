// partner_manager.cpp
#include "partner_manager.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <limits>

#include <nlohmann/json.hpp>

std::vector<Partner> loadPartners(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open " + filename + " for reading");
    }
    nlohmann::json j;
    file >> j;

    std::vector<Partner> partners;
    for (const auto& item : j) {
        int id        = item.value("partnerID", 0);
        std::string name    = item.value("partnerName", "");
        std::string type    = item.value("partnerType", "");
        float balance = item.value("partnerBalance", 0.0f);
        partners.emplace_back(id, name, type, balance);
    }
    return partners;
}

void savePartners(const std::vector<Partner>& partners,
                  const std::string& filename) {
    nlohmann::json j = nlohmann::json::array();
    for (const auto& p : partners) {
        j.push_back({
            {"partnerID", p.getID()},
            {"partnerName", p.getName()},
            {"partnerType", p.getType()},
            {"partnerBalance", p.getBalance()}
        });
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open " + filename + " for writing");
    }
    file << j.dump(4);
}

std::vector<PartnerCredential> loadPartnerCredentials(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open " + filename + " for reading");
    }
    nlohmann::json j;
    file >> j;

    std::vector<PartnerCredential> creds;
    for (const auto& item : j) {
        PartnerCredential pc;
        pc.partnerID = item.value("partnerID", 0);
        pc.staffID   = item.value("staffID", 0);
        pc.username  = item.value("username", "");
        pc.password  = item.value("password", "");
        creds.push_back(pc);
    }
    return creds;
}

void savePartnerCredentials(const std::vector<PartnerCredential>& creds,
                            const std::string& filename) {
    nlohmann::json j = nlohmann::json::array();
    for (const auto& pc : creds) {
        j.push_back({
            {"partnerID", pc.partnerID},
            {"staffID", pc.staffID},
            {"username", pc.username},
            {"password", pc.password}
        });
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open " + filename + " for writing");
    }
    file << j.dump(4);
}

std::shared_ptr<Partner> loginPartner(const std::vector<Partner>& partners) {
    auto creds = loadPartnerCredentials();
    std::string user, pass;
    std::cout << "Username: "; std::getline(std::cin, user);
    std::cout << "Password: "; std::getline(std::cin, pass);

    for (const auto& c : creds) {
        if (c.username == user && c.password == pass) {
            for (const auto& p : partners) {
                if (p.getID() == c.partnerID) {
                    std::cout << "Login successful for partner: " << p.getName() << "\n";
                    return std::make_shared<Partner>(p);
                }
            }
        }
    }
    std::cout << "Error: invalid credentials.\n";
    return nullptr;
}

void payPartner(std::shared_ptr<Partner> logged,
                std::vector<Partner>& partners) {
    if (!logged) {
        throw std::runtime_error("No partner logged in.");
    }
    double amt;
    std::cout << "Enter payment amount: ";
    std::cin >> amt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (amt <= 0) {
        throw std::invalid_argument("Amount must be positive");
    }
    int id = logged->getID();
    for (auto& p : partners) {
        if (p.getID() == id) {
            p.receivePayment(static_cast<float>(amt));
            logged->receivePayment(static_cast<float>(amt));
            savePartners(partners);
            std::cout << "Payment of " << amt << " recorded.\n";
            return;
        }
    }
    throw std::runtime_error("Logged partner not found.");
}
