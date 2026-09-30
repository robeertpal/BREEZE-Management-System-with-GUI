#include "staff.hpp"
#include <iostream>

int Staff::nextID = 0;

Staff::Staff(const Member& member_, const Credentials& credentials_)
    : ID(nextID++), member(member_), credentials(credentials_)
{}

Staff::~Staff() = default;

int Staff::getStaffID() const {
    return ID;
}

const Member& Staff::getMember() const {
    return member;
}

const Credentials& Staff::getCredentials() const {
    return credentials;
}

void Staff::setStaffID(int id_) {
    ID = id_;
}

// --- TicketAgent ---

TicketAgent::TicketAgent(const Member& member_, const Credentials& credentials_)
    : Staff(member_, credentials_)
{}

void TicketAgent::show() const {
    std::cout << "Ticket Agent ID: " << ID << '\n';
    member.show();
    credentials.show();
}

void TicketAgent::permissions() const {
    std::cout << "Permisiune: Check-in-ul și verificarea participanților.\n";
}

// --- SecurityAgent ---

SecurityAgent::SecurityAgent(const Member& member_, const Credentials& credentials_)
    : Staff(member_, credentials_)
{}

void SecurityAgent::show() const {
    std::cout << "Security Agent ID: " << ID << '\n';
    member.show();
    credentials.show();
}

void SecurityAgent::permissions() const {
    std::cout << "Permisiune: Inspecția participanților, verificarea brățărilor și restricționarea accesului în caz de nereguli.\n";
}

TopUpAgent::TopUpAgent(const Member& member_, const Credentials& credentials_)
    : Staff(member_, credentials_)
{}

void TopUpAgent::show() const {
    std::cout << "Top Up Agent ID: " << ID << '\n';
    member.show();
    credentials.show();
}

void TopUpAgent::permissions() const {
    std::cout << "Permisiune: Încărcarea brățărilor participanților.\n";
}
