#include "utils.hpp"
#include <cctype>
#include <sstream>

std::tm getTime(void) {
  std::time_t now = std::time(NULL);
  std::tm *local_time = std::localtime(&now);

  return (*local_time);
}

const std::string getTimeString(void) {
  std::tm time_data = getTime();
  std::stringstream stream;

  stream << '[' << time_data.tm_hour << ':' << time_data.tm_min << "] ";

  return (stream.str());
}

const std::string normalizeString(const std::string &string) {
  std::string normalized = string;

  for (int i = 0; i < normalized.length(); i++)
    normalized[i] = std::tolower(normalized[i]);

  return (normalized)
}
