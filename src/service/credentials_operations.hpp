// credentials_operations.hpp
#pragma once

#include <memory>
#include <vector>
#include <string>

#include "staff.hpp"
#include "validators.hpp"
#include "exceptions.hpp"
#include "credentials_manager.hpp"
#include "staff_manager.hpp"
#include "staff_operations.hpp"    // for toLower()

void changeUsername(std::shared_ptr<Staff>& loggedStaff,
                    std::vector<std::shared_ptr<Staff>>& staffList);
void changeUsernameForStaff(std::vector<std::shared_ptr<Staff>>& staffList);
void changePassword(std::shared_ptr<Staff>& loggedStaff,
                    std::vector<std::shared_ptr<Staff>>& staffList);
void changePasswordForStaff(std::vector<std::shared_ptr<Staff>>& staffList);
