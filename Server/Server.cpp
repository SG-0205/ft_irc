#include "Server.hpp"
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"
#include "../rpl_codes.h"
#include "../utils/colors.h"
#include "../utils/utils.hpp"
#include <asm-generic/socket.h>
#include <bits/getopt_core.h>
#include <cerrno>
#include <cmath>
#include <csignal>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <iostream>
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

pollfd Server::_newPollFd(const int &fd, const int &events,
                          const int &r_events) {
  pollfd new_pollfd;

  new_pollfd.fd = fd;
  new_pollfd.events = events;
  new_pollfd.revents = r_events;

  return (new_pollfd);
}

void Server::init(void) {
  // IPV4 + TCP (Choisi par defaut par le systeme)
  _server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_server_fd < 0)
    throw std::runtime_error("Server::init: couldn't create socket.");

  // Rendre non blocant
  int fd_flags = fcntl(_server_fd, F_GETFL, 0);
  if (fd_flags < 0)
    throw std::runtime_error("Server::init: couldn't get socket fd flags.");
  else if (fcntl(_server_fd, F_SETFL, fd_flags | O_NONBLOCK) < 0)
    throw std::runtime_error(
        "Server::init: couldn't make the socket fd non blocking.");

  // Permet de reutiliser l'adresse
  int opt_on = 1;
  if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_on,
                 sizeof(opt_on)) < 1 ||
      setsockopt(_server_fd, SOL_SOCKET, SO_REUSEPORT, &opt_on,
                 sizeof(optopt)) < 1)
    throw std::runtime_error("Server::init: couldn't setup socket options.");

  // Bind le socket au port de la machine
  sockaddr_in socket_address = initSocketAddrIn(socket_address, _port);
  if (bind(_server_fd, (sockaddr *)&socket_address, sizeof(socket_address)) <
      1) {
    throw std::runtime_error(
        std::string("Server::init: couldn't perform binding: ") +
        strerror(errno));
    close(_server_fd);
  }

  // Ecouter pour un maximum de SOMAXCONN connexion
  if (listen(_server_fd, SOMAXCONN) < 0) {
    throw std::runtime_error(
        std::string("Server::init: couldn't start listening on socket: ") +
        strerror(errno));
    close(_server_fd);
  }

  // Push back le pollfd du server
  _fds.push_back(_newPollFd(_server_fd, POLLIN, 0));
  _stop_flag = false;
}

void Server::_warningMessage(const std::string &msg) {
  std::cerr << YELLOW << msg << RESET << '\n';
}

Client *Server::_fetchClientByFD(const int &fd, const std::string &prefix) {
  const std::string timestamp = getTimeString();
  Client *client = _clients[fd];
  if (!client)
    _warningMessage(timestamp + prefix + ": Client not found.");

  return (client);
}

void Server::run(void) {
  if (_fds.size() < 1)
    throw std::runtime_error("Server::run: tried to run before init.");

  // _stop_flag est mis a jour dans les handlers de signaux.
  while (!_stop_flag) {

    // Evite les problemes de concurrence

    // Ecouter pour evenements
    int events_count = poll(_fds.data(), _fds.size(), -1);
    if (events_count < 0) {
      // Differencier les interuptions par signal;
      if (errno != EINTR)
        _warningMessage(std::string("poll() failure: ") + strerror(errno));
      else
        continue;
    }

    // Gerer les differents evenements d'ecoute
    // Iterer sur une copie pour eviter les modifications concurrentes.
    Client *client = NULL;
    std::vector<pollfd> fds_snapshot = _fds;
    for (int i = 0; i < fds_snapshot.size() && i < events_count; i++) {
      pollfd &has_event = fds_snapshot[i];

      if (has_event.fd == _server_fd) {
        if (has_event.revents & POLLIN)
          _handleNewConnection();
      } else {
        if (has_event.revents & POLLIN) {

          client = _fetchClientByFD(has_event.fd, "POLLIN");
          if (client)
            client->receiveData(this);

        } else if (has_event.revents & POLLOUT) {

          client = _fetchClientByFD(has_event.fd, "POLLOUT");
          if (client)
            client->processMessages();

        } else if (has_event.revents & POLLERR || has_event.revents & POLLHUP) {

          client = _fetchClientByFD(has_event.fd, "POLLERR/POLLHUP");
          if (client)
            removeClient(has_event.fd);

          _warningMessage("Unexpected deconnection (POLLERR/POLLHUP)");
        }
      }
    }
    if (_clients.size() >= SOMAXCONN)
      _warningMessage("Too much connexions.");

    // Enlever les fd obsoletes
    if (_clients_mod_flag) {
      for (std::vector<pollfd>::iterator fd_it = _fds.begin();
           fd_it != _fds.end();) {
        if ((*fd_it).fd == _server_fd) {
          fd_it++;
          continue;
        }

        int vec_fd = (*fd_it).fd;

        if (_clients.find(vec_fd) == _clients.end()) {
          fd_it = _fds.erase(fd_it);
          continue;
        }

        fd_it++;
      }
    }
  }
}

void Server::removeClient(const int &client_fd) {
  if (client_fd < 0 || client_fd == _server_fd) {
    _warningMessage("Tried to remove an invalid client.");
    return;
  }

  Client *client = _fetchClientByFD(client_fd);
  if (client) {

    // Broadcast QUIT sur chaque channel
    std::set<Channel *> &channels = client->getChannels();
    for (std::set<Channel *>::iterator ch_it = channels.begin();
         ch_it != channels.end(); ch_it++)
      (*ch_it)->removeMember(client);
    // removeMember doit broadcast QUIT && supprimer si channel vide;

    if (close(client_fd) < 0)
      _warningMessage(std::string("Close failed: ") + strerror(errno));

    // Supprime l'entree dans la map
    if (!_clients.erase(client_fd))
      _warningMessage("Couldn't remove client from map.");

    // libere l'allocation
    delete client;

    // supprime l'element pollfd du vecteur;
    for (std::vector<pollfd>::iterator fd_it = _fds.begin();
         fd_it != _fds.end(); fd_it++)
      if ((*fd_it).fd == client_fd) {
        _fds.erase(fd_it);
        break;
      }
  }
}

Channel *Server::_fetchChannelByName(const std::string &channel_name) {
  Channel *channel = _channels[channel_name];
  if (!channel)
    _warningMessage(channel_name + ": no such channel.");

  return (channel);
}

void Server::sendToChannel(const std::string &channel_name,
                           const std::string &message, Client *exclude) {
  // On assume que le message respecte deja la RFC

  Channel *channel = _fetchChannelByName(normalizeString(channel_name));
  if (channel) {

    std::set<Client *> &channel_clients = channel->getRegisteredClients();
    for (std::set<Client *>::iterator cli_it = channel_clients.begin();
         cli_it != channel_clients.end(); cli_it++) {
      if ((*cli_it) == exclude)
        continue;

      // On attend que la boucle principale gere l'envoi.
      // queueMessage doit split le message en chunks de 512 octets si besoin
      (*cli_it)->queueMessage(message);
    }
  }
}
