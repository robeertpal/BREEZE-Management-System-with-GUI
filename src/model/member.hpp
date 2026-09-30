// member.hpp
#pragma once

#include <string>
#include "email.hpp"
#include "phone.hpp"
#include "date.hpp"
#include "validators.hpp"

class Member {
private:
    std::string firstName;
    std::string lastName;
    Email email;
    PhoneNumber phone;
    Date birthDate;
    std::string nationality;

public:
    Member(const std::string& firstName_,
           const std::string& lastName_,
           const Email& email_,
           const PhoneNumber& phone_,
           const Date& birthDate_,
           const std::string& nationality_);
    Date getBirthDate() const;
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    Email getEmail() const;
    PhoneNumber getPhone() const;
    const std::string& getNationality() const;
    void setEmail(const Email& newEmail);
    void setPhone(const PhoneNumber& newPhone);
    void setBirthDate(const Date& newBirthDate);
    void setNationality(const std::string& newNationality);
    void setFirstName(const std::string& newFirstName);
    void setLastName(const std::string& newLastName);
    bool operator==(const Member& other) const;
    void show() const;
};
