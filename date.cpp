// date.cpp
#include "date.hpp"
#include "validators.hpp"
#include <iostream>

Date::Date(int day_, int month_, int year_) {
    validateDate(day_, month_, year_);
    day = day_;
    month = month_;
    year = year_;
}

int Date::getDay() const {
    return day;
}

int Date::getMonth() const {
    return month;
}

int Date::getYear() const {
    return year;
}

void Date::show() const {
    std::cout << (day < 10 ? "0" : "") << day << '/'
              << (month < 10 ? "0" : "") << month << '/'
              << year << '\n';
}

std::string Date::toString() const {
    std::string s;
    if (day < 10) s += "0";
    s += std::to_string(day) + "/";
    if (month < 10) s += "0";
    s += std::to_string(month) + "/";
    s += std::to_string(year);
    return s;
}
