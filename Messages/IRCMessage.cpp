#include "IRCMessage.hpp"
#include <cstddef>
#include <iostream>
#include <sys/socket.h>

IRCMessage::IRCMessage(void)
    : _raw(), _prefix(), _command(CMD_INVALID), _params() {}

IRCMessage::IRCMessage(const IRCMessage &other)
    : _raw(other._raw), _prefix(other._prefix), _command(other._command),
      _params(other._params) {}

IRCMessage &IRCMessage::operator=(const IRCMessage &other) {
  if (this == &other)
    return (*this);

  _raw = other._raw;
  _prefix = other._prefix;
  _command = other._command;
  _params = other._params;

  return (*this);
}

IRCMessage::IRCMessage(const std::string &raw, const std::string &prefix,
                       const ClientCommands &command,
                       const std::vector<std::string> &params)
    : _raw(raw), _prefix(prefix), _command(command), _params(params) {}

const std::string &IRCMessage::GetRaw(void) const { return (_raw); }

const std::string &IRCMessage::GetPrefix(void) const { return (_prefix); }

const std::vector<std::string> &IRCMessage::GetParams(void) const {
  return (_params);
}

size_t IRCMessage::paramsSize(void) const { return (_params.size()); }

bool IRCMessage::hasPrefix(void) const { return (!_prefix.empty()); }

void IRCMessage::print(void) const {
  std::cout << "RAW: [" << _raw << ']' << '\n'
            << "PREFIX: [" << _prefix << ']' << '\n'
            << "COMMAND: " << CommandToString(_command) << '\n'
            << "PARAMS: " << '\n';

  for (int i = 0; i < _params.size(); i++)
    std::cout << '\t' << '[' << _params[i] << ']' << '\n';

  std::cout << std::endl;
}

static void skipSpaces(const std::string &line, size_t &pos) {
  while (pos < line.size() && line[pos] == ' ')
    pos++;
}

static ClientCommands readCommand(const std::string &command_string) {
  if (command_string.empty())
    return (CMD_EMPTY);

  if (command_string == "PASS")
    return (CMD_PASS);
  else if (command_string == "NICK")
    return (CMD_NICK);
  else if (command_string == "USER")
    return (CMD_USER);
  else if (command_string == "JOIN")
    return (CMD_JOIN);
  else if (command_string == "PART")
    return (CMD_PART);
  else if (command_string == "PRIVMSG")
    return (CMD_PRIVMSG);
  else if (command_string == "NOTICE")
    return (CMD_NOTICE);
  else if (command_string == "QUIT")
    return (CMD_QUIT);
  else if (command_string == "PING")
    return (CMD_PING);
  else if (command_string == "PONG")
    return (CMD_PONG);
  else if (command_string == "MODE")
    return (CMD_MODE);
  else if (command_string == "TOPIC")
    return (CMD_TOPIC);
  else if (command_string == "INVITE")
    return (CMD_INVITE);
  else
    return (CMD_INVALID);
}

IRCMessage IRCMessage::newMessage(const std::string &raw) {
  IRCMessage new_message;
  new_message._raw = raw;

  if (raw.empty())
    return (new_message);

  size_t pos = 0;
  std::string buffer = raw;

  if (buffer[pos] == ':') {
    size_t end_prefix_pos = buffer.find(' ', pos);
    if (end_prefix_pos == std::string::npos)
      return (new_message);

    new_message._prefix = buffer.substr(pos + 1, end_prefix_pos - pos - 1);
    pos = end_prefix_pos + 1;
  }

  size_t space_pos = buffer.find(' ', pos);
  if (space_pos == std::string::npos) {
    size_t end_of_message = buffer.find("\r\n");
    if (end_of_message == std::string::npos)
      end_of_message = buffer.find('\n');

    new_message._command =
        readCommand(buffer.substr(pos, end_of_message - pos));
    return (new_message);
  }

  new_message._command = readCommand(buffer.substr(pos, space_pos - pos));

  pos = space_pos + 1;
  while (pos < buffer.size()) {
    skipSpaces(buffer, pos);

    if (pos >= buffer.size())
      break;

    // Parametre avec : == dernier parametre
    if (buffer[pos] == ':') {
      size_t end_of_message = buffer.find("\r\n", pos);
      if (end_of_message == std::string::npos)
        end_of_message = buffer.find('\n', pos);

      new_message._params.push_back(
          buffer.substr(pos + 1, end_of_message - pos - 1));
      break;
    }

    space_pos = buffer.find(' ', pos);
    if (space_pos == std::string::npos) {
      size_t end_of_message = buffer.find("\r\n", pos);
      if (end_of_message == std::string::npos)
        end_of_message = buffer.find('\n', pos);

      new_message._params.push_back(buffer.substr(pos, end_of_message - pos));

      break;

    } else {
      new_message._params.push_back(buffer.substr(pos, space_pos - pos));
      pos = space_pos;
    }
  }

  // EXTERN : verifier que paramsSize < 16
  return (new_message);
}

std::string IRCMessage::CommandToString(const ClientCommands &command) {
  switch (command) {
  case (CMD_PASS):
    return ("PASS");
  case (CMD_NICK):
    return ("NICK");
  case (CMD_USER):
    return ("USER");
  case (CMD_JOIN):
    return ("JOIN");
  case (CMD_PART):
    return ("PART");
  case (CMD_PRIVMSG):
    return ("PRIVMSG");
  case (CMD_NOTICE):
    return ("NOTICE");
  case (CMD_QUIT):
    return ("QUIT");
  case (CMD_PING):
    return ("PING");
  case (CMD_PONG):
    return ("PONG");
  case (CMD_MODE):
    return ("MODE");
  case (CMD_TOPIC):
    return ("TOPIC");
  case (CMD_KICK):
    return ("KICK");
  case (CMD_INVITE):
    return ("INVITE");
  case (CMD_EMPTY):
    return ("EMPTY");
  case (CMD_INVALID):
    return ("!INVALID!");
  default:
    return ("[ENUM_ERROR]");
  };
}
