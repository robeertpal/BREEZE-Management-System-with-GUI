// exceptions.hpp
#pragma once

#include <exception>
#include <string>

class Exception : public std::exception {
protected:
    std::string message;

public:
    explicit Exception(const std::string& message_);
    const char* what() const noexcept override;
};

class InvalidEmail : public Exception {
public:
    explicit InvalidEmail(const std::string& message_ = "Adresă de email invalidă!");
};
class InvalidPhoneNumber : public Exception {
public:
    explicit InvalidPhoneNumber(const std::string& message_ = "Număr de telefon invalid!");
};
class InvalidDate : public Exception {
public:
    explicit InvalidDate(const std::string& message_ = "Dată invalidă!");
};
class AgeRestriction : public Exception {
public:
    explicit AgeRestriction(const std::string& message_ = "Candidatul trebuie să aibă 18 ani împliniți!");
};
class WeakPassword : public Exception {
public:
    explicit WeakPassword(const std::string& message_ = "Parola trebuie să aibă cel puțin 6 caractere!");
};
class InsufficientFunds : public Exception {
public:
    explicit InsufficientFunds(const std::string& message_ = "Fonduri insuficiente pentru această tranzacție!");
};
class InvalidTicket : public Exception {
public:
    explicit InvalidTicket(const std::string& message_ = "Cod de ticket invalid!");
};
class UsernameAlreadyExists : public Exception {
public:
    explicit UsernameAlreadyExists(const std::string& message_ = "Username-ul este deja folosit!");
};
