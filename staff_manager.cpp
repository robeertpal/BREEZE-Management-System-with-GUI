#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <iostream>
#include "nlohmann/json.hpp"
#include "staff.hpp"
#include "member.hpp"
#include "credentials.hpp"
#include "credentials_manager.hpp"
#include "exceptions.hpp"
#include "validators.hpp"
#include "date.hpp"
#include "email.hpp"
#include "phone.hpp"

std::vector<std::shared_ptr<Staff>> loadStaffFromFile() {
    std::vector<std::shared_ptr<Staff>> staffList;

    // Load credentials first
    auto credentialsList = loadCredentials();

    std::ifstream file("staff.json");
    if (!file.is_open()) {
        throw std::runtime_error("Nu s-a putut deschide fisierul de staff!");
    }

    nlohmann::json j;
    file >> j;

    for (const auto& item : j) {
        // parse member fields
        std::string birthStr = item["birthDate"].get<std::string>();
        int day   = std::stoi(birthStr.substr(0, 2));
        int month = std::stoi(birthStr.substr(3, 2));
        int year  = std::stoi(birthStr.substr(6, 4));

        Member member(
            item["firstName"].get<std::string>(),
            item["lastName"].get<std::string>(),
            Email(item["email"].get<std::string>()),
            PhoneNumber(item["phone"].get<std::string>()),
            Date(day, month, year),
            item["nationality"].get<std::string>()
        );

        // find matching credentials
        int id = item["id"].get<int>();
        std::string username = "";
        std::string password = "";
        for (const auto& cred : credentialsList) {
            if (cred.id == id) {
                username = cred.username;
                password = cred.password;
                break;
            }
        }
        if (username.empty()) {
            throw std::runtime_error("Nu s-au găsit credențiale pentru staff ID=" + std::to_string(id));
        }

        // create appropriate Staff subclass with real credentials
        std::shared_ptr<Staff> staff;
        std::string role = item["role"].get<std::string>();
        if (role == "TicketAgent") {
            staff = std::make_shared<TicketAgent>(member, Credentials(username, password));
        } else if (role == "SecurityAgent") {
            staff = std::make_shared<SecurityAgent>(member, Credentials(username, password));
        } else if (role == "TopUpAgent") {
            staff = std::make_shared<TopUpAgent>(member, Credentials(username, password));
        } else {
            throw InvalidTicket("Rol de staff necunoscut la încărcare: " + role);
        }

        // restore stored ID
        staff->setStaffID(id);
        staffList.push_back(staff);
    }

    return staffList;
}

void saveStaffToFile(const std::vector<std::shared_ptr<Staff>>& staffList, const std::string& filename = "staff.json") {
    nlohmann::json jArray = nlohmann::json::array();

    for (const auto& staff : staffList) {
        nlohmann::json jStaff;
        jStaff["id"] = staff->getStaffID();

        const Member& member = staff->getMember();
        jStaff["firstName"] = member.getFirstName();
        jStaff["lastName"]  = member.getLastName();
        jStaff["email"]     = member.getEmail().getEmail();
        jStaff["phone"]     = member.getPhone().getNumber();
        jStaff["birthDate"] = member.getBirthDate().toString();
        jStaff["nationality"] = member.getNationality();

        if (dynamic_cast<TicketAgent*>(staff.get())) {
            jStaff["role"] = "TicketAgent";
        } else if (dynamic_cast<SecurityAgent*>(staff.get())) {
            jStaff["role"] = "SecurityAgent";
        } else if (dynamic_cast<TopUpAgent*>(staff.get())) {
            jStaff["role"] = "TopUpAgent";
        } else {
            throw InvalidTicket("Tip de staff necunoscut la salvare!");
        }

        jArray.push_back(jStaff);
    }

    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw std::runtime_error("Nu s-a putut deschide fisierul pentru scriere: " + filename);
    }
    ofs << jArray.dump(4);
}
