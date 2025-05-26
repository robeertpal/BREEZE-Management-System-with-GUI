#include "ticket.hpp"
#include <iostream>

Ticket::Ticket(const std::string& ticketCode_) {
    validateBraceletCode(ticketCode_);
    ticketCode = ticketCode_;
    bracelet = Bracelet<float>(0.0f);
}

Ticket::~Ticket() = default;

void Ticket::setTicketCode(const std::string& newTicketCode) {
    validateBraceletCode(newTicketCode);
    ticketCode = newTicketCode;
}

const std::string& Ticket::getTicketCode() const {
    return ticketCode;
}

Bracelet<float>& Ticket::getBracelet() {
    return bracelet;
}

const Bracelet<float>& Ticket::getBracelet() const {
    return bracelet;
}

GeneralAccess::GeneralAccess(const std::string& ticketCode_)
    : Ticket(ticketCode_)
{}

void GeneralAccess::showDetails() const {
    std::cout << "General Access Ticket (" << ticketCode << ")\n";
}

GeneralVIP::GeneralVIP(const std::string& ticketCode_)
    : Ticket(ticketCode_)
{}

void GeneralVIP::showDetails() const {
    std::cout << "General VIP Ticket (" << ticketCode << ")\n";
}

VIPMeetAndGreet::VIPMeetAndGreet(const std::string& ticketCode_)
    : Ticket(ticketCode_)
{}

void VIPMeetAndGreet::showDetails() const {
    std::cout << "VIP Meet & Greet Ticket (" << ticketCode << ")\n";
}
