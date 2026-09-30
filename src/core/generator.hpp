// generator.hpp
#pragma once

#include <string>

/// Generates a unique ticket code in the format:
/// BREEZE-<ticketTypeCode>-<computedNumber>
/// where computedNumber = (sum of ASCII of firstName) * day * month * year + (sum of ASCII of lastName)
std::string generateTicketCode(const std::string& firstName,
                               const std::string& lastName,
                               int day,
                               int month,
                               int year,
                               const std::string& ticketTypeCode);
