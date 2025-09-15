#pragma once

#include "commandToken.hpp"
#include <vector>

struct messageContainer {
private:
  bool _is_command;
  std::string _message;
  std::vector<commandToken> _command;

public:
  messageContainer(void);
  messageContainer(const std::string &message,
                   const std::vector<commandToken> &command_tokens =
                       std::vector<commandToken>());
  messageContainer(const messageContainer &other);
  messageContainer &operator=(const messageContainer &other);
  ~messageContainer(void);

  const bool &IsACommand(void) const;
  const std::string &GetMessage(void) const;
  const std::vector<commandToken> &GetCommandTokens(void) const;
};
