#include "../rpl_codes.h"
#include "Server.hpp"
#include <climits>
#include <stdexcept>
#include <unistd.h>

static const std::string generateHostname(void) {
  char host_buffer[HOST_NAME_MAX];
  if (gethostname(host_buffer, HOST_NAME_MAX) < 0)
    throw std::runtime_error("generateHostname::gethostname failed.");

  return (std::string("ft_irc.") + host_buffer);
}

std::string Server::_hostname = generateHostname();

Server *Server::SERVER_INSTANCE = NULL;

const std::string &Server::getServerHostname(void) {
  if (_hostname.empty())
    throw std::runtime_error("Server::getServerHostname: undefined hostname.");

  return (_hostname);
}

const std::string Server::buildServerReply(const std::string &command,
                                           const std::string &nick,
                                           const std::string &params,
                                           const std::string &trailing) {
  if (command.empty())
    throw std::invalid_argument("Server::buildServerReply: empty command.");
  else if (nick.empty())
    throw std::invalid_argument("Server::buildServerReply: empty nick.");

  std::string prefix = ":" + getServerHostname() + " ";
  std::string message = prefix + command + " " + nick;

  if (!params.empty())
    message += " " + params;
  if (!trailing.empty())
    message += " " + trailing;

  message += MSG_SEPARATOR;

  return (message);
}

const std::string
Server::buildUserReply(const std::string &nick, const std::string &user,
                       const std::string &host, const std::string &command,
                       const std::string &target, const std::string &trailing) {
  if (nick.empty())
    throw std::invalid_argument("Server::buildUserReply: empty nick.");
  else if (user.empty())
    throw std::invalid_argument("Server::buildUserReply: empty user.");
  else if (host.empty())
    throw std::invalid_argument("Server::buildUserReply: empty host.");
  else if (command.empty())
    throw std::invalid_argument("Server::buildUserReply: empty command.");
  else if (target.empty())
    throw std::invalid_argument("Server::buildUserReply: empty target.");

  std::string prefix = ":" + nick + "! " + user + "@" + host + " ";
  std::string message = prefix + command + " " + target;

  if (!trailing.empty())
    message += " :" + trailing;

  message += MSG_SEPARATOR;

  return (message);
}

const std::string Server::buildErrorReply(const std::string &command,
                                          const std::string &nick,
                                          const std::string &target,
                                          const std::string &reason) {
  if (command.empty())
    throw std::invalid_argument("Server::buildErrorReply: empty command.");
  else if (nick.empty())
    throw std::invalid_argument("Server::buildErrorReply: empty nick.");
  else if (target.empty())
    throw std::invalid_argument("Server::buildErrorReply: empty target.");
  else if (reason.empty())
    throw std::invalid_argument("Server::buildErrorReply: empty reason.");

  std::string prefix = ":" + getServerHostname() + " ";
  std::string message =
      prefix + command + " " + nick + " " + target + " :" + reason;
  message += MSG_SEPARATOR;

  return (message);
}
