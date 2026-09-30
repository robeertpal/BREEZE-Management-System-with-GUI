// credentials.cpp
#include "credentials.hpp"
#include <iostream>

bool Credentials::isValidPassword(const std::string& password_) const {
    return password_.size() >= 6;
}

Credentials::Credentials(const std::string& username_,
                         const std::string& password_,
                         const std::string& /*role*/)
    : username(username_), password(password_)
{
    if (!isValidPassword(password_)) {
        throw WeakPassword();
    }
}

const std::string& Credentials::getUsername() const {
    return username;
}

void Credentials::changeUsername(const std::string& newUsername) {
    username = newUsername;
}

void Credentials::changePassword(const std::string& newPassword) {
    if (!isValidPassword(newPassword)) {
        throw WeakPassword();
    }
    password = newPassword;
}

bool Credentials::authenticate(const std::string& inputPassword) const {
    return password == inputPassword;
}

void Credentials::show() const {
    std::cout << "Username: " << username << '\n';
}
