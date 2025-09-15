#pragma once

#include "commandToken.hpp"
#include "messageContainer.hpp"

class MessageParser {
private:
  MessageParser(void);
  ~MessageParser(void);

public:
  static messageContainer readMessage(const std::string &message);
};
