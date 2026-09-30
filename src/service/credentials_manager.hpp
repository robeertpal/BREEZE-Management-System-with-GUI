// credentials_manager.hpp
#pragma once

#include <string>
#include <vector>

struct Credential {
    int id;
    std::string username;
    std::string password;
};

std::vector<Credential> loadCredentials(const std::string& filename = "credentials.json");
void saveCredentials(const std::vector<Credential>& credentialsList,
                     const std::string& filename = "credentials.json");
