// guest_operations.hpp
#pragma once

#include <vector>
#include <memory>
#include <string>

#include "guest.hpp"
#include "guest_manager.hpp"
#include "generator.hpp"
#include "validators.hpp"
#include "exceptions.hpp"

void showAllGuests(const std::vector<std::shared_ptr<Guest>>& guests);
void addGuest(std::vector<std::shared_ptr<Guest>>& guests);
void checkInGuest(std::vector<std::shared_ptr<Guest>>& guests);
void showGuestTicketInfo(const std::vector<std::shared_ptr<Guest>>& guests);
void showGuestInfo(const std::vector<std::shared_ptr<Guest>>& guests);
void restrictAccess(std::vector<std::shared_ptr<Guest>>& guests);
void showBraceletBalance(const std::vector<std::shared_ptr<Guest>>& guests);
void modifyGuestFirstName(std::vector<std::shared_ptr<Guest>>& guests);
void modifyGuestLastName(std::vector<std::shared_ptr<Guest>>& guests);
void modifyGuestBirthDate(std::vector<std::shared_ptr<Guest>>& guests);
void modifyGuestEmail(std::vector<std::shared_ptr<Guest>>& guests);
void modifyGuestPhone(std::vector<std::shared_ptr<Guest>>& guests);
void modifyGuestNationality(std::vector<std::shared_ptr<Guest>>& guests);
void addBraceletBalance(std::vector<std::shared_ptr<Guest>>& guests);
void payWithBracelet(std::vector<std::shared_ptr<Guest>>& guests);
void removeGuestByTicketCode(std::vector<std::shared_ptr<Guest>>& guests, const std::string& ticketCode);

