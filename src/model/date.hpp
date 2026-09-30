// date.hpp
#pragma once

#include <string>

class Date {
private:
    int day;
    int month;
    int year;

public:
    Date(int day_, int month_, int year_);
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    void show() const;
    std::string toString() const;
};
