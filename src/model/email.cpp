#include "email.hpp"
#include <utility>
#include "validators.hpp"

Email::Email(const std::string& address_) {
    validateEmail(address_);
    address = address_;
}

Email::Email(const Email& other) = default;  // delegare către copy-ctor implicit

Email& Email::operator=(Email other) {        // copy-and-swap
    std::swap(address, other.address);
    return *this;
}

const std::string& Email::getEmail() const {
    return address;
}
