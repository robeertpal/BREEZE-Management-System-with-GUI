// generator.cpp
#include "generator.hpp"

int sumAscii(const std::string& s) {
    int total = 0;
    for (unsigned char ch : s) {
        total += static_cast<int>(ch);
    }
    return total;
}

std::string generateTicketCode(const std::string& firstName,
                               const std::string& lastName,
                               int day,
                               int month,
                               int year,
                               const std::string& ticketTypeCode) {
    int sumFirst = sumAscii(firstName);
    int sumLast  = sumAscii(lastName);

    long long computedNumber = 1LL * sumFirst * day * month * year + sumLast;

    return "BREEZE-" + ticketTypeCode + "-" + std::to_string(computedNumber);
}
