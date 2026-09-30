#include "partner.hpp"
#include <stdexcept>

Partner::Partner(int id_, const std::string& name_, const std::string& type_, float balance_)
    : partnerID(id_), partnerName(name_), partnerType(type_), partnerBalance(balance_)
{}

int Partner::getID() const noexcept {
    return partnerID;
}

const std::string& Partner::getName() const noexcept {
    return partnerName;
}

const std::string& Partner::getType() const noexcept {
    return partnerType;
}

float Partner::getBalance() const noexcept {
    return partnerBalance;
}

void Partner::setName(const std::string& name_) {
    partnerName = name_;
}

void Partner::setType(const std::string& type_) {
    partnerType = type_;
}

void Partner::setBalance(float balance_) {
    partnerBalance = balance_;
}

void Partner::receivePayment(float amount) {
    if (amount <= 0.0f) {
        throw std::invalid_argument("Payment amount must be positive");
    }
    partnerBalance += amount;
}

void Partner::addStaff(std::shared_ptr<PartnerStaff> staffMember) {
    partnerStaff.push_back(std::move(staffMember));
}

const std::vector<std::shared_ptr<PartnerStaff>>& Partner::getStaffList() const noexcept {
    return partnerStaff;
}
