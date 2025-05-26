// member.cpp
#include "member.hpp"
#include <iostream>

Member::Member(const std::string& firstName_,
               const std::string& lastName_,
               const Email& email_,
               const PhoneNumber& phone_,
               const Date& birthDate_,
               const std::string& nationality_)
    : firstName(firstName_)
    , lastName(lastName_)
    , email(email_)
    , phone(phone_)
    , birthDate(birthDate_)
    , nationality(nationality_)
{
    validateAdult(birthDate_.getYear(),
                  birthDate_.getMonth(),
                  birthDate_.getDay());
}

Date Member::getBirthDate() const {
    return birthDate;
}

const std::string& Member::getFirstName() const {
    return firstName;
}

const std::string& Member::getLastName() const {
    return lastName;
}

Email Member::getEmail() const {
    return email;
}

PhoneNumber Member::getPhone() const {
    return phone;
}

const std::string& Member::getNationality() const {
    return nationality;
}

void Member::setEmail(const Email& newEmail) {
    email = newEmail;
}

void Member::setPhone(const PhoneNumber& newPhone) {
    phone = newPhone;
}

void Member::setBirthDate(const Date& newBirthDate) {
    birthDate = newBirthDate;
}

void Member::setNationality(const std::string& newNationality) {
    nationality = newNationality;
}

void Member::setFirstName(const std::string& newFirstName) {
    firstName = newFirstName;
}

void Member::setLastName(const std::string& newLastName) {
    lastName = newLastName;
}

bool Member::operator==(const Member& other) const {
    return email.getEmail() == other.email.getEmail()
        && phone.getNumber() == other.phone.getNumber();
}

void Member::show() const {
    std::cout << "Name: " << firstName << " " << lastName << "\n"
              << "Email: " << email.getEmail() << "\n"
              << "Phone: " << phone.getNumber() << "\n"
              << "Birth date: ";
    birthDate.show();
    std::cout << "Nationality: " << nationality << "\n";
}
