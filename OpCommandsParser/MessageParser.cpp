#include "OpCommandsParser.hpp"
#include "messageContainer.hpp"
#include <stdexcept>

messageContainer MessageParser::readMessage(const std::string &message) {
  if (message.empty())
    throw std::runtime_error("MessageParser::readMessage: empty message.");

  // Check first letter of message;
  // parse with full tokenizer or register raw data.
}
