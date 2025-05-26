#ifndef GUEST_HPP
#define GUEST_HPP

#include "member.hpp"
#include "ticket.hpp"
#include <memory>
#include <iostream>

class Guest {
public:
    Guest(const Member& member_, std::shared_ptr<Ticket> ticket_);
    ~Guest();

    void show() const;
    const Member& getMember() const;
    const std::shared_ptr<Ticket>& getTicket() const;
    void checkIn();
    bool isCheckedIn() const;

private:
    Member member;
    std::shared_ptr<Ticket> ticket;
    bool checkedIn;
};

// 🔽 AICI — declarația operatorului
std::ostream& operator<<(std::ostream& os, const Guest& guest);

#endif // GUEST_HPP