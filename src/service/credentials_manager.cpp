// credentials_manager.cpp
#include "credentials_manager.hpp"

#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

std::vector<Credential> loadCredentials(const std::string& filename) {
    std::vector<Credential> credentialsList;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Nu s-a putut deschide fisierul credentials!");
    }

    nlohmann::json j;
    file >> j;

    for (const auto& item : j) {
        credentialsList.push_back({
            item["id"].get<int>(),
            item["username"].get<std::string>(),
            item["password"].get<std::string>()
        });
    }
    return credentialsList;
}

void saveCredentials(const std::vector<Credential>& credentialsList,
                     const std::string& filename) {
    nlohmann::json jArray = nlohmann::json::array();
    for (const auto& cred : credentialsList) {
        jArray.push_back({
            {"id", cred.id},
            {"username", cred.username},
            {"password", cred.password}
        });
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Nu s-a putut deschide fisierul credentials pentru scriere!");
    }
    file << jArray.dump(4);
}
