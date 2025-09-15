#include "messageContainer.hpp"
#include "commandToken.hpp"
#include <vector>

messageContainer::messageContainer(void)
    : _is_command(false), _message(""), _command() {}

messageContainer::messageContainer(
    const std::string &message, const std::vector<commandToken> &command_tokens)
    : _is_command(command_tokens.empty() ? false : true), _message(message),
      _command(command_tokens) {}

messageContainer::messageContainer(const messageContainer &other)
    : _is_command(other._is_command), _message(other._message),
      _command(other._command) {}

messageContainer &messageContainer::operator=(const messageContainer &other) {
  if (this == &other)
    return (*this);

  this->_is_command = other._is_command;
  this->_message = other._message;
  this->_command = other._command;

  return (*this);
}

messageContainer::~messageContainer(void) {}

const bool &messageContainer::IsACommand(void) const { return (_is_command); }

const std::string &messageContainer::GetMessage(void) const {
  return (_message);
}

const std::vector<commandToken> &
messageContainer::GetCommandTokens(void) const {
  return (_command);
}
