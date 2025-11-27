#include "Server.hpp"
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"
#include "../utils/colors.h"
#include "../utils/utils.hpp"
#include <asm-generic/socket.h>
#include <bits/getopt_core.h>
#include <cctype>
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <limits.h>
#include <netinet/in.h>
#include <ostream>
#include <signal.h>
#include <sstream>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

const std::string Server::SERVER_PASSWORD = "TEST";

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

void Server::_infoMessage(const std::string &msg) {
  std::cout << GREEN ITALIC << msg << RESET << '\n';
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
  Channel *channel = _channels[normalizeString(channel_name)];
  if (!channel)
    _warningMessage(channel_name + ": no such channel.");

  return (channel);
}

void Server::sendToChannel(const std::string &channel_name,
                           const std::string &message, Client *exclude) {
  // On assume que le message respecte deja la RFC

  Channel *channel = _fetchChannelByName(channel_name);
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

void Server::removeChannel(const std::string &channel_name) {
  Channel *channel = _fetchChannelByName(channel_name);
  if (!channel || !channel->getRegisteredClients().empty())
    return;

  delete channel;
  _channels.erase(channel_name);
  std::cout << "Deleted channel " << channel_name << std::endl;
}

static bool validateChannelName(const std::string &channel_name) {
  // cas "" / "#" / "#oooo...[50]"
  if (channel_name.empty() || channel_name.length() > 50 ||
      channel_name.length() == 1)
    return (false);
  // cas 'channelName'
  if (channel_name[0] != '#')
    return (false);
  // mauvaise separation des args
  if (channel_name.find(',') != std::string::npos)
    return (false);

  // caracteres non imprimables (i = 1 pour skip le prefixe)
  static const std::string extra_chars = "-_";
  for (int i = 1; i < channel_name.length(); i++)
    if (!std::isalnum(channel_name[i]) &&
        extra_chars.find(channel_name[i]) == std::string::npos)
      return (false);

  return (true);
}

Channel *Server::createChannel(const std::string &channel_name) {
  // Renvoyer NULL en cas d'erreur de format;
  if (!validateChannelName(channel_name))
    return (NULL);

  // Gestion du join sur un channel existant;
  Channel *fetch_attempt = _fetchChannelByName(channel_name);
  if (fetch_attempt)
    return (fetch_attempt);

  // Pas de normalize pour l'objet;
  Channel *new_channel = new Channel(channel_name);
  _channels.insert({normalizeString(channel_name), new_channel});

  return (new_channel);
}

Client *Server::findClientByNick(const std::string &nick) {
  for (std::map<int, Client *>::const_iterator client_it = _clients.begin();
       client_it != _clients.end(); client_it++)
    if (normalizeString(client_it->second->GetNickName()) ==
        normalizeString(nick))
      return (client_it->second);

  return (NULL);
}

void Server::_handleNewConnection(void) {
  sockaddr_in client_address;
  socklen_t ca_size = sizeof(client_address);
  int new_fd = -1;

  // nouvel fd & verif de l'operation
  if ((new_fd = accept(_server_fd, (sockaddr *)(&client_address), &ca_size)) <
      0) {
    _warningMessage("Error during accept(): " + std::string(strerror(errno)));

    if (errno != EAGAIN && errno != EWOULDBLOCK)
      return;
  };

  // Passe en mode non bloquant
  if (fcntl(new_fd, F_SETFL, O_NONBLOCK) < 0) {
    _warningMessage("Couldn't setup new client fd.");
    close(new_fd);
    return;
  }

  if (_clients.size() < 128) {
    Client *new_client = new Client(new_fd, *this);
    if (!new_client) {
      _warningMessage("Couldn't create new client object. (" +
                      std::string(strerror(errno)) + ")");
      close(new_fd);
      return;
    }

    _clients.insert({new_fd, new_client});
    _fds.push_back(_newPollFd(new_fd, POLLIN | POLLOUT, 0));
    std::stringstream buf;
    buf << GREEN ITALIC << "New client with fd " << new_fd << '\n';
    _infoMessage(buf.str());

  } else {
    // TODO : NOTIFIER CLIENT;
    close(new_fd);
  }
}
