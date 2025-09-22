#include "Client.hpp"
#include "../Channel/Channel.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

Client::Client(const int &client_fd, Server &server)
    : _fd(client_fd), _nickname(""), _username(""), _realname(""),
      _joind_channels(), _send_queue() {
  if (_fd < 0)
    throw std::runtime_error("Client::Client: invalid client fd.");

  sockaddr_in peer_address;
  socklen_t addr_size = sizeof(peer_address);

  if (getpeername(_fd, (sockaddr *)(&peer_address), &addr_size) < 0)
    throw std::runtime_error("Client::Client: getpeername failure.");

  _hostname = std::string(inet_ntoa(peer_address.sin_addr));
}

Client::~Client(void) {
  while (_send_queue.size() != 0)
    _send_queue.pop();

  for (std::set<Channel *>::iterator channel = _joind_channels.begin();
       channel != _joind_channels.end(); channel++)
    (*channel)->removeMember(this);

  _joind_channels.clear();
  close(_fd);
}

// TODO Client::removeJoinedChannel(Channel *)

void Client::receiveData(Server &server) {}
