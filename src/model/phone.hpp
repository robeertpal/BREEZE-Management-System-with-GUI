#pragma once

#include <iostream>
#include <string>
#include "exceptions.hpp"

// PhoneNumber e clonabilă polimorf
class PhoneNumber {
private:
    std::string number;

public:
    // Destructor virtual pentru polimorfism
    virtual ~PhoneNumber();

    // Constructor principal cu validare
    explicit PhoneNumber(const std::string& number_);

    // Constructor de copiere
    PhoneNumber(const PhoneNumber& other);

    // Operator de atribuire (copy-and-swap)
    PhoneNumber& operator=(PhoneNumber other);

    // Acces la număr
    const std::string& getNumber() const;

    // Constructor virtual (clone)
    virtual PhoneNumber* clone() const;
};