#pragma once

#include <string>
#include <vector>
#include <memory>

#include "staff.hpp"
#include "credentials_manager.hpp"
#include "staff_manager.hpp"
#include "validators.hpp"
#include "exceptions.hpp"
#include "member.hpp"
#include "credentials.hpp"

std::string toLower(const std::string& str);

std::shared_ptr<Staff> loginStaff(const std::vector<std::shared_ptr<Staff>>& staffList);
void addStaffMember(std::vector<std::shared_ptr<Staff>>& staffList);
void removeStaffMember(std::vector<std::shared_ptr<Staff>>& staffList);
void showStaffByDepartment(const std::vector<std::shared_ptr<Staff>>& staffList);
void changeFirstNameForStaff(std::vector<std::shared_ptr<Staff>>& staffList);
void changeLastNameForStaff(std::vector<std::shared_ptr<Staff>>& staffList);
void changeBirthDateForStaff(std::vector<std::shared_ptr<Staff>>& staffList);
void changePhoneForStaff(std::vector<std::shared_ptr<Staff>>& staffList);
void changeNationalityForStaff(std::vector<std::shared_ptr<Staff>>& staffList);
void changeRoleForStaff(std::vector<std::shared_ptr<Staff>>& staffList);
void changeEmailForStaff(std::vector<std::shared_ptr<Staff>>& staffList);
