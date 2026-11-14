#pragma once
#include "Venue.hpp"
#include <memory>
#include <string>

void saveVenue(const Venue& venue, const std::string& path);
std::unique_ptr<Venue> loadVenue(const std::string& path);