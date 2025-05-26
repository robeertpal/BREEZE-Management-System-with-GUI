// exceptions.cpp
#include "exceptions.hpp"

Exception::Exception(const std::string& message_)
    : message(message_)
{}

const char* Exception::what() const noexcept {
    return message.c_str();
}

InvalidEmail::InvalidEmail(const std::string& message_)
    : Exception(message_)
{}

InvalidPhoneNumber::InvalidPhoneNumber(const std::string& message_)
    : Exception(message_)
{}

InvalidDate::InvalidDate(const std::string& message_)
    : Exception(message_)
{}

AgeRestriction::AgeRestriction(const std::string& message_)
    : Exception(message_)
{}

WeakPassword::WeakPassword(const std::string& message_)
    : Exception(message_)
{}

InsufficientFunds::InsufficientFunds(const std::string& message_)
    : Exception(message_)
{}

InvalidTicket::InvalidTicket(const std::string& message_)
    : Exception(message_)
{}

UsernameAlreadyExists::UsernameAlreadyExists(const std::string& message_)
    : Exception(message_)
{}
