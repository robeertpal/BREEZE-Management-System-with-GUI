// email.hpp
#pragma once

#include <string>

class Email {
private:
    std::string address;

public:
    explicit Email(const std::string& address_);
    Email(const Email& other);  
    Email& operator=(Email other);   
    ~Email() = default;
    const std::string& getEmail() const;
};
