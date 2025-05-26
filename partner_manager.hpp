#pragma once

#include <vector>
#include <string>
#include <memory>

#include "partner.hpp"

struct PartnerCredential {
    int partnerID;
    int staffID;
    std::string username;
    std::string password;
};
std::vector<Partner> loadPartners(const std::string& filename = "partners.json");
void savePartners(const std::vector<Partner>& partners,
                  const std::string& filename = "partners.json");
std::vector<PartnerCredential> loadPartnerCredentials(
    const std::string& filename = "partners_credentials.json");
void savePartnerCredentials(const std::vector<PartnerCredential>& creds,
                            const std::string& filename = "partners_credentials.json");
std::shared_ptr<Partner> loginPartner(const std::vector<Partner>& partners);
void payPartner(std::shared_ptr<Partner> logged,
                std::vector<Partner>& partners);
