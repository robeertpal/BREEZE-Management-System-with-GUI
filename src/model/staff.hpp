#pragma once

#include <string>
#include "member.hpp"
#include "credentials.hpp"

class Staff {
protected:
    static int nextID;
    int ID;
    Member member;
    Credentials credentials;

public:
    Staff(const Member& member_, const Credentials& credentials_);
    virtual ~Staff();
    int getStaffID() const;
    const Member& getMember() const;
    const Credentials& getCredentials() const;
    void setStaffID(int id_);
    virtual void show() const = 0;
    virtual void permissions() const = 0;
};

class TicketAgent : public Staff {
public:
    TicketAgent(const Member& member_, const Credentials& credentials_);
    void show() const override;
    void permissions() const override;
};

class SecurityAgent : public Staff {
public:
    SecurityAgent(const Member& member_, const Credentials& credentials_);
    void show() const override;
    void permissions() const override;
};

class TopUpAgent : public Staff {
public:
    TopUpAgent(const Member& member_, const Credentials& credentials_);
    void show() const override;
    void permissions() const override;
};
