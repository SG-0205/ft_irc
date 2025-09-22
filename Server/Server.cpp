#include "Server.hpp"
#include "../rpl_codes.h"
#include <asm-generic/socket.h>
#include <bits/getopt_core.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <limits.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/socket.h>
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

void Server::_signalHandler(int signal_nb) {
  if (signal_nb == SIGINT || signal_nb == SIGKILL || signal_nb == SIGTERM) {
    getInstance()->_stop_flag = true;
  }
}

Server::Server(const int &port, const std::string &password)
    : _clients(), _channels(), _stop_flag(false) {
  if (port < 1024)
    throw std::invalid_argument("Server::Server: port is not usable.");
  else if (password.empty() || password != SERVER_PASSWORD)
    throw std::invalid_argument("Server::Server: invalid password.");

  SERVER_INSTANCE = this;
  _port = port;
  _password = password;
  _server_fd = -1;

  signal(SIGINT, _signalHandler);
  signal(SIGKILL, _signalHandler);
  signal(SIGTERM, _signalHandler);
}

Server::~Server(void) {
  cleanup();

  while (_fds.size() > 0)
    _fds.erase(_fds.begin());

  while (_clients.size() > 0)
    _clients.erase(_clients.begin());

  while (_channels.size() > 0)
    _channels.erase(_channels.begin());

  close(_server_fd);
}

static sockaddr_in &initSocketAddrIn(sockaddr_in &socket_address,
                                     const int &port) {
  memset(&socket_address, 0, sizeof(socket_address));

  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = INADDR_ANY;
  socket_address.sin_port = htons(port);

  return (socket_address);
}

void Server::init(void) {
  _server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_server_fd < 0)
    throw std::runtime_error("Server::init: couldn't create socket.");

  int fd_flags = fcntl(_server_fd, F_GETFL, 0);
  if (fd_flags < 0)
    throw std::runtime_error("Server::init: couldn't get socket fd flags.");
  else if (fcntl(_server_fd, F_SETFL, fd_flags | O_NONBLOCK) < 0)
    throw std::runtime_error(
        "Server::init: couldn't make the socket fd non blocking.");

  int opt_on = 1;
  if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_on,
                 sizeof(opt_on)) < 1 ||
      setsockopt(_server_fd, SOL_SOCKET, SO_REUSEPORT, &opt_on,
                 sizeof(optopt)) < 1)
    throw std::runtime_error("Server::init: couldn't setup socket options.");

  sockaddr_in socket_address = initSocketAddrIn(socket_address, _port);
  if (bind(_server_fd, (sockaddr *)&socket_address, sizeof(socket_address)) <
      1) {
    throw std::runtime_error(
        std::string("Server::init: couldn't perform binding: ") +
        strerror(errno));
    close(_server_fd);
  }

  // CONTINUE HERE
}
