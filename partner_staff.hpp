#pragma once

#include <string>
#include <memory>

#include "member.hpp"
#include "credentials.hpp"

class PartnerStaff {
private:
    static int nextID;
    int ID;
    Member member;
    Credentials credentials;

public:
    PartnerStaff(const Member& member_, const Credentials& credentials_);
    int getPartnerStaffID() const;
    const Member& getMember() const;
    const Credentials& getCredentials() const;
    void show() const;
    void permissions() const;
};
