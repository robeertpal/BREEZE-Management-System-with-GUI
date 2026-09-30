// credentials.hpp
#pragma once

#include <string>
#include "exceptions.hpp"

class Credentials {
private:
    std::string username;
    std::string password;

    bool isValidPassword(const std::string& password_) const;

public:
    Credentials(const std::string& username_,
                const std::string& password_,
                const std::string& role_ = "agent");
    const std::string& getUsername() const;
    void changeUsername(const std::string& newUsername);
    void changePassword(const std::string& newPassword);
    bool authenticate(const std::string& inputPassword) const;
    void show() const;
};
