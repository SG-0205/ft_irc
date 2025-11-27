#pragma once

#include <string>
#include <vector>

enum ClientCommands {
  CMD_PASS,
  CMD_NICK,
  CMD_USER,
  CMD_JOIN,
  CMD_PART,
  CMD_PRIVMSG,
  CMD_NOTICE,
  CMD_QUIT,
  CMD_PING,
  CMD_PONG,
  CMD_MODE,
  CMD_TOPIC,
  CMD_KICK,
  CMD_INVITE,
  CMD_EMPTY,
  CMD_INVALID,
  _C_CMD_SIZE
};

struct IRCMessage {
private:
  std::string _raw;
  std::string _prefix;
  ClientCommands _command;
  std::vector<std::string> _params;

  IRCMessage(const std::string &raw, const std::string &prefix,
             const ClientCommands &command,
             const std::vector<std::string> &params);
  IRCMessage(void);

public:
  static IRCMessage newMessage(const std::string &raw);
  IRCMessage(const IRCMessage &other);
  IRCMessage &operator=(const IRCMessage &other);

  const std::string &GetRaw(void) const;
  bool hasPrefix(void) const;
  const std::string &GetPrefix(void) const;
  const ClientCommands &GetCommand(void) const;
  const std::vector<std::string> &GetParams(void) const;
  size_t paramsSize(void) const;

  void print(void) const;
  static std::string CommandToString(const ClientCommands &command);
};
