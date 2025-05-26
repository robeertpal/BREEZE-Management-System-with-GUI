#pragma once

#include <vector>
#include <memory>
#include <string>

#include "staff.hpp"
#include "member.hpp"
#include "validators.hpp"
#include "exceptions.hpp"

std::vector<std::shared_ptr<Staff>> loadStaffFromFile();
void saveStaffToFile(const std::vector<std::shared_ptr<Staff>>& staffList,
                     const std::string& filename = "staff.json");
