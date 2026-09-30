#include "validators.hpp"
#include <chrono>
#include <ctime>
#include <cctype>
#include <vector>
#include <algorithm>

void validateAdult(int year, int month, int day) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);

    int currentYear  = now_tm.tm_year + 1900;
    int currentMonth = now_tm.tm_mon  + 1;
    int currentDay   = now_tm.tm_mday;

    int age = currentYear - year;
    if (month > currentMonth ||
        (month == currentMonth && day > currentDay)) {
        --age;
    }

    if (age < 18) {
        throw AgeRestriction("Festivalul este destinat exclusiv persoanelor de peste 18 ani!");
    }
}

void validateBraceletCode(const std::string& code) {
    const std::string prefix = "BREEZE-";
    if (code.substr(0, prefix.size()) != prefix) {
        throw InvalidTicket();
    }

    size_t current = prefix.size();
    if (code.size() <= current + 2) {
        throw InvalidTicket();
    }

    std::string ticketType = code.substr(current, 2);
    if (ticketType != "GA" && ticketType != "GV" && ticketType != "MG") {
        throw InvalidTicket();
    }

    current += 2;
    if (code[current] != '-') {
        throw InvalidTicket();
    }
    ++current;

    std::string rest = code.substr(current);
    if (rest.empty() || rest.size() > 10) {
        throw InvalidTicket();
    }

    for (char ch : rest) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            throw InvalidTicket();
        }
    }
}

void validateEmail(const std::string& email) {
    if (email.empty()) {
        throw InvalidEmail("Nu ați introdus email!");
    }
    size_t atPos = email.find('@');
    if (atPos == std::string::npos ||
        email.find('@', atPos + 1) != std::string::npos) {
        throw InvalidEmail("Lipsește sau este duplicat caracterul '@'!");
    }
    if (atPos == 0 || atPos == email.size() - 1) {
        throw InvalidEmail("'@' plasat incorect!");
    }
    size_t dotPos = email.find('.', atPos);
    if (dotPos == std::string::npos ||
        dotPos == atPos + 1 ||
        dotPos == email.size() - 1) {
        throw InvalidEmail("Domeniu lipsă sau incorect!");
    }
}

void validatePhoneNumber(const std::string& number) {
    if (number.empty()) {
        throw InvalidPhoneNumber("Nu ați introdus niciun număr de telefon!");
    }

    size_t start = 0;
    if (number[0] == '+') {
        if (number.size() == 1) {
            throw InvalidPhoneNumber("'+' fără cifre!");
        }
        start = 1;
    }

    int digitCount    = 0;
    int parenBalance  = 0;
    bool parenAllowed = true;
    bool insideParen  = false;
    int digitsInParen = 0;

    for (size_t i = start; i < number.size(); ++i) {
        char ch = number[i];
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            ++digitCount;
            if (insideParen) ++digitsInParen;
        }
        else if (ch == ' ') {
            continue;
        }
        else if (ch == '(') {
            if (parenBalance >= 1 || !parenAllowed) {
                throw InvalidPhoneNumber("Prea multe '(' sau plasare greșită!");
            }
            ++parenBalance;
            parenAllowed = false;
            insideParen  = true;
            digitsInParen = 0;
        }
        else if (ch == ')') {
            if (parenBalance == 0 || digitsInParen == 0 || digitsInParen > 3) {
                throw InvalidPhoneNumber("Parantezare incorectă!");
            }
            --parenBalance;
            insideParen = false;
        }
        else {
            throw InvalidPhoneNumber("Conține caractere nepermise!");
        }
    }

    if (parenBalance != 0 || digitCount < 10 || digitCount > 15) {
        throw InvalidPhoneNumber("Număr de cifre incorect!");
    }
}

void validateDate(int day, int month, int year) {
    if (year < 1940 || year > 2025) {
        throw InvalidDate("An invalid!");
    }
    if (month < 1 || month > 12) {
        throw InvalidDate("Luna invalidă!");
    }
    if (day < 1) {
        throw InvalidDate("Zi invalidă!");
    }

    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2) {
        bool leap = (year%4==0 && (year%100!=0 || year%400==0));
        if (day > (leap ? 29 : 28)) {
            throw InvalidDate(leap ? "Februarie are 29 zile!" : "Februarie are 28 zile!");
        }
    } else if (day > daysInMonth[month-1]) {
        throw InvalidDate("Zi invalidă!");
    }
}

void validateUsername(const std::string& username_) {
    std::vector<Credential> credentialsList = loadCredentials();
    auto it = std::find_if(
        credentialsList.begin(), credentialsList.end(),
        [&username_](const Credential& cred){ return cred.username == username_; }
    );
    if (it != credentialsList.end()) {
        throw UsernameAlreadyExists("Username-ul există deja! Alege altul.");
    }
}
