#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>
#include <limits>

#include "partner_manager.hpp"
#include "credentials_manager.hpp"
#include "guest.hpp"
#include "guest_operations.hpp"

std::vector<Partner> loadPartnersFromFile();
void savePartnersToFile(const std::vector<Partner>& partners);
std::shared_ptr<Partner> loginPartnerStaff(const std::vector<Partner>& partners);
void registerPaymentToPartner(const std::shared_ptr<Partner>& loggedPartner,
                              std::vector<Partner>& partners,
                              std::vector<std::shared_ptr<Guest>>& guests);
void showPartnerBalance(const std::shared_ptr<Partner>& loggedPartner);
void createPartnerWorker(const std::shared_ptr<Partner>& loggedPartner);
void createPartner(std::vector<Partner>& partners);
void changeWorkerUsername(const std::shared_ptr<Partner>& loggedPartner, int staffID);
void changeWorkerPassword(const std::shared_ptr<Partner>& loggedPartner, int staffID);
void changePasswordPartner(const std::shared_ptr<Partner>& loggedPartner);
void changeUsernamePartner(const std::shared_ptr<Partner>& loggedPartner);
