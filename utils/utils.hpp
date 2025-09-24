#pragma once

#include <ctime>
#include <string>

// Get time struct.
std::tm getTime(void);

// Formatted timestamp
const std::string getTimeString(void);

// normalize inputs
const std::string normalizeString(const std::string &string);
