#include "partner_staff.hpp"
#include <iostream>

int PartnerStaff::nextID = 0;

PartnerStaff::PartnerStaff(const Member& member_, const Credentials& credentials_)
    : ID(nextID++), member(member_), credentials(credentials_)
{}

int PartnerStaff::getPartnerStaffID() const {
    return ID;
}

const Member& PartnerStaff::getMember() const {
    return member;
}

const Credentials& PartnerStaff::getCredentials() const {
    return credentials;
}

void PartnerStaff::show() const {
    std::cout << "Partner Staff ID: " << ID << '\n';
    member.show();
    credentials.show();
}

void PartnerStaff::permissions() const {
    std::cout << "Permisiuni: Inregistrare vanzari si consultare sold partener.\n";
}
