#pragma once

#include <string>
#include "bracelet.hpp"
#include "validators.hpp"

class Ticket {
protected:
    std::string ticketCode;
    Bracelet<float> bracelet;

public:
    Ticket(const std::string& ticketCode_);
    virtual ~Ticket();

    void setTicketCode(const std::string& newTicketCode);

    const std::string& getTicketCode() const;

    virtual void showDetails() const = 0;

    Bracelet<float>& getBracelet();
    const Bracelet<float>& getBracelet() const;
};

class GeneralAccess : public Ticket {
public:
    GeneralAccess(const std::string& ticketCode_);
    void showDetails() const override;
};

class GeneralVIP : public Ticket {
public:
    GeneralVIP(const std::string& ticketCode_);
    void showDetails() const override;
};

class VIPMeetAndGreet : public Ticket {
public:
    VIPMeetAndGreet(const std::string& ticketCode_);
    void showDetails() const override;
};
