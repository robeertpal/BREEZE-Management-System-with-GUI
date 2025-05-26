#include "phone.hpp"
#include "validators.hpp"
#include <utility>

PhoneNumber::PhoneNumber(const std::string& number_) {
    validatePhoneNumber(number_);
    number = number_;
}

PhoneNumber::PhoneNumber(const PhoneNumber& other) = default;

PhoneNumber& PhoneNumber::operator=(PhoneNumber other) {
    std::swap(number, other.number);
    return *this;
}

PhoneNumber::~PhoneNumber() = default;

const std::string& PhoneNumber::getNumber() const {
    return number;
}

PhoneNumber* PhoneNumber::clone() const {
    return new PhoneNumber(*this);
}
