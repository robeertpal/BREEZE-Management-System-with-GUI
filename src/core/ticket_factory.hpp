#pragma once
#include <memory>
#include <string>
#include "ticket.hpp"
#include "fee_strategy.hpp"

class TicketFactory {
public:
    static std::shared_ptr<Ticket> createTicket(const std::string& ticketType, const std::string& ticketCode) {
        static StandardFee<float> standardFee;
        static MGFee<float> mgFee;

        std::shared_ptr<Ticket> ticket;

        if (ticketType == "GA") {
            ticket = std::make_shared<GeneralAccess>(ticketCode);
            ticket->getBracelet().setFeeStrategy(&standardFee);
        } else if (ticketType == "GV") {
            ticket = std::make_shared<GeneralVIP>(ticketCode);
            ticket->getBracelet().setFeeStrategy(&standardFee);
        } else if (ticketType == "MG") {
            ticket = std::make_shared<VIPMeetAndGreet>(ticketCode);
            ticket->getBracelet().setFeeStrategy(&mgFee);
        } else {
            throw std::runtime_error("Tip de bilet necunoscut: " + ticketType);
        }
        return ticket;
    }
};
