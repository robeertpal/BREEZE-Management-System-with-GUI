#pragma once

#include <string>
#include "exceptions.hpp"
#include "credentials_manager.hpp"

void validateAdult(int year, int month, int day);
void validateBraceletCode(const std::string& code);
void validateEmail(const std::string& email);
void validatePhoneNumber(const std::string& number);
void validateDate(int day, int month, int year);
void validateUsername(const std::string& username_);
