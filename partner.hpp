#pragma once

#include <string>
#include <vector>
#include <memory>

#include "partner_staff.hpp"

class Partner {
private:
    int partnerID;
    std::string partnerName;
    std::string partnerType;
    float partnerBalance;
    std::vector<std::shared_ptr<PartnerStaff>> partnerStaff;

public:
    Partner(int id_, const std::string& name_, const std::string& type_, float balance_ = 0.0f);
    int getID() const noexcept;
    const std::string& getName() const noexcept;
    const std::string& getType() const noexcept;
    float getBalance() const noexcept;
    void setName(const std::string& name_);
    void setType(const std::string& type_);
    void setBalance(float balance_);
    void receivePayment(float amount);
    void addStaff(std::shared_ptr<PartnerStaff> staffMember);
    const std::vector<std::shared_ptr<PartnerStaff>>& getStaffList() const noexcept;
};
