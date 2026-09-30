// guest.cpp
#include "guest.hpp"
#include <iostream>

Guest::Guest(const Member& member_, std::shared_ptr<Ticket> ticket_)
    : member(member_), ticket(std::move(ticket_)), checkedIn(false)
{}

Guest::~Guest() = default;

void Guest::show() const {
    member.show();
    std::cout << "Informatii despre bilet:\n";
    ticket->showDetails();
}

const Member& Guest::getMember() const {
    return member;
}

const std::shared_ptr<Ticket>& Guest::getTicket() const {
    return ticket;
}

void Guest::checkIn() {
    if (checkedIn) {
        std::cout << "Participantul " << member.getFirstName() << " " << member.getLastName()
                  << " are deja check-in-ul realizat cu succes!\n";
        return;
    } else {
        checkedIn = true;
        std::cout << "Check-in realizat cu succes pentru " << member.getFirstName()
                  << " " << member.getLastName()
                  << ", cu biletul " << ticket->getTicketCode() << "!\n";
    }
}

bool Guest::isCheckedIn() const {
    return checkedIn;
}

std::ostream& operator<<(std::ostream& os, const Guest& guest) {
    os << "Nume: " << guest.getMember().getLastName()
       << ",\nPrenume: " << guest.getMember().getFirstName()
       << ",\nCod bilet: " << guest.getTicket()->getTicketCode()
       << ",\nCheck-in: " << (guest.isCheckedIn() ? "Da" : "Nu")
       << "\n";
    return os;
}

