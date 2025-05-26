// guest_manager.hpp
#pragma once

#include <vector>
#include <memory>

#include "guest.hpp"

std::vector<std::shared_ptr<Guest>> loadGuestsFromFile();
void saveGuestsToFile(const std::vector<std::shared_ptr<Guest>>& guests);
