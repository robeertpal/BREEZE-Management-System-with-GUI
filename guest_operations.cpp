// guest_operations.cpp
#include "guest_operations.hpp"

#include <iostream>
#include <limits>
#include <string>

#include "guest_manager.hpp"
#include "generator.hpp"
#include "validators.hpp"
#include "exceptions.hpp"
#include "member.hpp"
#include "ticket.hpp"
#include "email.hpp"
#include "phone.hpp"
#include "date.hpp"
#include "guest.hpp"
#include "ticket_factory.hpp"


void showAllGuests(const std::vector<std::shared_ptr<Guest>>& guests) {
    for (const auto& guest : guests) {
        guest->show();
        std::cout << "--------\n";
    }
}

void addGuest(std::vector<std::shared_ptr<Guest>>& guests) {
    std::string firstName, lastName, emailStr, phoneStr, nationality;
    int day, month, year;
    std::string ticketTypeCode;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "=== Adaugare participant nou ===\n";
    std::cout << "Prenume: ";
    std::getline(std::cin, firstName);
    std::cout << "Nume: ";
    std::getline(std::cin, lastName);

    do {
        std::cout << "Email: ";
        std::getline(std::cin, emailStr);
        try {
            validateEmail(emailStr);
            break;
        } catch (const InvalidEmail& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        }
    } while (true);

    do {
        std::cout << "Telefon: ";
        std::getline(std::cin, phoneStr);
        try {
            validatePhoneNumber(phoneStr);
            break;
        } catch (const InvalidPhoneNumber& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        }
    } while (true);

    do {
        std::cout << "Data nasterii (zi luna an): ";
        std::cin >> day >> month >> year;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        try {
            validateDate(day, month, year);
            validateAdult(day, month, year);
            break;
        } catch (const InvalidDate& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        } catch (const AgeRestriction& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        }
    } while (true);

    std::cout << "Nationalitate: ";
    std::getline(std::cin, nationality);

    do {
        std::cout << "Tip bilet (GA / GV / MG): ";
        std::getline(std::cin, ticketTypeCode);
        if (ticketTypeCode == "GA" || ticketTypeCode == "GV" || ticketTypeCode == "MG") {
            break;
        } else {
            std::cerr << "Eroare: Tip de bilet invalid! Incearca din nou.\n";
        }
    } while (true);

    std::string ticketCode = generateTicketCode(
        firstName, lastName,
        day, month, year,
        ticketTypeCode
    );
    std::cout << "Cod bilet generat: " << ticketCode << "\n";

    try {
        Member m(
            firstName, lastName,
            Email(emailStr),
            PhoneNumber(phoneStr),
            Date(day, month, year),
            nationality
        );

        auto ticket = TicketFactory::createTicket(ticketTypeCode, ticketCode);


        auto guest = std::make_shared<Guest>(m, ticket);
        guests.push_back(guest);
        saveGuestsToFile(guests);

        std::cout << "Participant adaugat si salvat cu succes!\n";
    } catch (const std::exception& e) {
        std::cerr << "Eroare la adaugare: " << e.what() << '\n';
    }
}

void checkInGuest(std::vector<std::shared_ptr<Guest>>& guests) {
    std::string code;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, code);

    bool found = false;
    for (auto& guest : guests) {
        if (guest->getTicket()->getTicketCode() == code) {
            try {
                guest->checkIn();
                saveGuestsToFile(guests);
                std::cout << "Check-in realizat si salvat cu succes!\n";
            } catch (const std::exception& e) {
                std::cerr << "Eroare la check-in: " << e.what() << '\n';
            }
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Participantul cu acest cod nu a fost gasit!\n";
    }
}

void showGuestTicketInfo(const std::vector<std::shared_ptr<Guest>>& guests) {
    std::string ticketCode;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, ticketCode);

    bool found = false;
    for (const auto& guest : guests) {
        if (guest->getTicket()->getTicketCode() == ticketCode) {
            guest->show();
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Biletul nu a fost gasit!\n";
    }
}

void modifyGuestFirstName(std::vector<std::shared_ptr<Guest>>& guests) {
    std::string ticketCode;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, ticketCode);

    for (auto& guest : guests) {
        if (guest->getTicket()->getTicketCode() == ticketCode) {
            std::string newFirstName;
            std::cout << "Prenume nou: ";
            std::getline(std::cin, newFirstName);
            const_cast<Member&>(guest->getMember()).setFirstName(newFirstName);

            // Regeneram codul
            const auto& member = guest->getMember();
            std::string ticketTypeCode = guest->getTicket()->getTicketCode().substr(7, 2);
            std::string newTicketCode = generateTicketCode(
                member.getFirstName(),
                member.getLastName(),
                member.getBirthDate().getDay(),
                member.getBirthDate().getMonth(),
                member.getBirthDate().getYear(),
                ticketTypeCode
            );
            const_cast<std::shared_ptr<Ticket>&>(guest->getTicket())->setTicketCode(newTicketCode);

            saveGuestsToFile(guests);
            std::cout << "Prenumele a fost modificat si codul biletului a fost regenerat!\n";
            return;
        }
    }
    std::cout << "Biletul nu a fost gasit!\n";
}

void modifyGuestLastName(std::vector<std::shared_ptr<Guest>>& guests) {
    std::string ticketCode;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, ticketCode);

    for (auto& guest : guests) {
        if (guest->getTicket()->getTicketCode() == ticketCode) {
            std::string newLastName;
            std::cout << "Nume nou: ";
            std::getline(std::cin, newLastName);
            const_cast<Member&>(guest->getMember()).setLastName(newLastName);

            // Regeneram codul
            const auto& member = guest->getMember();
            std::string ticketTypeCode = guest->getTicket()->getTicketCode().substr(7, 2);
            std::string newTicketCode = generateTicketCode(
                member.getFirstName(),
                member.getLastName(),
                member.getBirthDate().getDay(),
                member.getBirthDate().getMonth(),
                member.getBirthDate().getYear(),
                ticketTypeCode
            );
            const_cast<std::shared_ptr<Ticket>&>(guest->getTicket())->setTicketCode(newTicketCode);

            saveGuestsToFile(guests);
            std::cout << "Numele a fost modificat si codul biletului a fost regenerat!\n";
            return;
        }
    }
    std::cout << "Biletul nu a fost gasit!\n";
}

void modifyGuestBirthDate(std::vector<std::shared_ptr<Guest>>& guests) {
    std::string ticketCode;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, ticketCode);

    for (auto& guest : guests) {
        if (guest->getTicket()->getTicketCode() == ticketCode) {
            int newDay, newMonth, newYear;
            do {
                std::cout << "Data nasterii noua (zi luna an): ";
                std::cin >> newDay >> newMonth >> newYear;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                try {
                    validateDate(newDay, newMonth, newYear);
                    validateAdult(newDay, newMonth, newYear);
                    break;
                } catch (const InvalidDate& e) {
                    std::cerr << "Eroare: " << e.what() << "\n";
                } catch (const AgeRestriction& e) {
                    std::cerr << "Eroare: " << e.what() << "\n";
                }
            } while (true);

            const_cast<Member&>(guest->getMember()).setBirthDate(Date(newDay, newMonth, newYear));

            const auto& member = guest->getMember();
            std::string ticketTypeCode = guest->getTicket()->getTicketCode().substr(7, 2);
            std::string newTicketCode = generateTicketCode(
                member.getFirstName(),
                member.getLastName(),
                member.getBirthDate().getDay(),
                member.getBirthDate().getMonth(),
                member.getBirthDate().getYear(),
                ticketTypeCode
            );
            const_cast<std::shared_ptr<Ticket>&>(guest->getTicket())->setTicketCode(newTicketCode);

            saveGuestsToFile(guests);
            std::cout << "Data nasterii a fost modificata si codul biletului a fost regenerat!\n";
            return;
        }
    }
    std::cout << "Biletul nu a fost gasit!\n";
}

void modifyGuestEmail(std::vector<std::shared_ptr<Guest>>& guests) {
    std::string ticketCode;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, ticketCode);

    for (auto& guest : guests) {
        if (guest->getTicket()->getTicketCode() == ticketCode) {
            std::string newEmail;
            do {
                std::cout << "Email nou: ";
                std::getline(std::cin, newEmail);
                try {
                    validateEmail(newEmail);
                    break;
                } catch (const InvalidEmail& e) {
                    std::cerr << "Eroare: " << e.what() << "\n";
                }
            } while (true);

            const_cast<Member&>(guest->getMember()).setEmail(Email(newEmail));
            saveGuestsToFile(guests);
            std::cout << "Email-ul a fost modificat cu succes!\n";
            return;
        }
    }
    std::cout << "Biletul nu a fost gasit!\n";
}

void modifyGuestPhone(std::vector<std::shared_ptr<Guest>>& guests) {
    std::string ticketCode;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, ticketCode);

    for (auto& guest : guests) {
        if (guest->getTicket()->getTicketCode() == ticketCode) {
            std::string newPhone;
            do {
                std::cout << "Telefon nou: ";
                std::getline(std::cin, newPhone);
                try {
                    validatePhoneNumber(newPhone);
                    break;
                } catch (const InvalidPhoneNumber& e) {
                    std::cerr << "Eroare: " << e.what() << "\n";
                }
            } while (true);

            const_cast<Member&>(guest->getMember()).setPhone(PhoneNumber(newPhone));
            saveGuestsToFile(guests);
            std::cout << "Telefonul a fost modificat cu succes!\n";
            return;
        }
    }
    std::cout << "Biletul nu a fost gasit!\n";
}

void modifyGuestNationality(std::vector<std::shared_ptr<Guest>>& guests) {
    std::string ticketCode;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, ticketCode);

    for (auto& guest : guests) {
        if (guest->getTicket()->getTicketCode() == ticketCode) {
            std::string newNationality;
            std::cout << "Nationalitate noua: ";
            std::getline(std::cin, newNationality);
            const_cast<Member&>(guest->getMember()).setNationality(newNationality);
            saveGuestsToFile(guests);
            std::cout << "Nationalitatea a fost modificata cu succes!\n";
            return;
        }
    }
    std::cout << "Biletul nu a fost gasit!\n";
}

void addBraceletBalance(std::vector<std::shared_ptr<Guest>>& guests) {
    std::string ticketCode;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, ticketCode);

    for (auto& guest : guests) {
        if (guest->getTicket()->getTicketCode() == ticketCode) {
            float amount;
            do {
                std::cout << "Suma de incarcat (lei): ";
                std::cin >> amount;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (amount <= 0) {
                    std::cout << "Suma trebuie sa fie pozitiva!\n";
                } else {
                    break;
                }
            } while (true);

            const_cast<std::shared_ptr<Ticket>&>(guest->getTicket())->getBracelet().topUp(amount);
            saveGuestsToFile(guests);
            std::cout << "Suma a fost incarcata cu succes pe bratara!\n";
            return;
        }
    }
    std::cout << "Biletul nu a fost gasit!\n";
}

void payWithBracelet(std::vector<std::shared_ptr<Guest>>& guests) {
    std::string ticketCode;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, ticketCode);

    for (auto& guest : guests) {
        if (guest->getTicket()->getTicketCode() == ticketCode) {
            float amount;
            do {
                std::cout << "Suma de platit (lei): ";
                std::cin >> amount;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (amount <= 0) {
                    std::cout << "Suma trebuie sa fie pozitiva!\n";
                } else {
                    break;
                }
            } while (true);

            try {
                const_cast<std::shared_ptr<Ticket>&>(guest->getTicket())->getBracelet().pay(amount);
                saveGuestsToFile(guests);
                std::cout << "Plata a fost efectuata cu succes!\n";
            } catch (const InsufficientFunds& e) {
                std::cerr << "Eroare: " << e.what() << "\n";
            }
            return;
        }
    }
    std::cout << "Biletul nu a fost gasit!\n";
}

void showGuestInfo(const std::vector<std::shared_ptr<Guest>>& guests){
    std::string ticketCode;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, ticketCode);

    for (const auto& guest : guests) {
        if (guest->getTicket()->getTicketCode() == ticketCode) {
            guest->show();
            return;
        }
    }
    std::cout << "Biletul nu a fost gasit!\n";
}

void restrictAccess(std::vector<std::shared_ptr<Guest>>& guests) {
    std::string ticketCode;
    std::cout << "Introdu codul biletului: ";
    std::getline(std::cin, ticketCode);

    auto it = std::remove_if(guests.begin(), guests.end(),
        [&ticketCode](const std::shared_ptr<Guest>& guest) {
            return guest->getTicket()->getTicketCode() == ticketCode;
        });

    if (it != guests.end()) {
        guests.erase(it, guests.end());
        saveGuestsToFile(guests);
        std::cout << "Accesul a fost restrictionat cu succes!\n";
    } else {
        std::cout << "Biletul nu a fost gasit!\n";
    }
}

void removeGuestByTicketCode(std::vector<std::shared_ptr<Guest>>& guests, const std::string& ticketCode) {
    auto it = std::remove_if(guests.begin(), guests.end(),
        [&](const std::shared_ptr<Guest>& guest) {
            return guest->getTicket()->getTicketCode() == ticketCode;
        }
    );
    if (it != guests.end()) {
        guests.erase(it, guests.end());
        std::cout << "Accesul pentru codul " << ticketCode << " a fost restricționat (participantul a fost șters).\n";
    } else {
        std::cout << "Nu s-a găsit participant cu codul " << ticketCode << "!\n";
    }
}
