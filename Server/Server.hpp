#pragma once

#include <map>
#include <poll.h>
#include <string>
#include <vector>

class Client;
class Channel;

class Server {
private:
  static const std::string SERVER_PASSWORD;

  int _port;
  std::string _password;
  int _server_fd;
  std::vector<pollfd> _fds;
  std::map<int, Client * > _clients;
  std::map<std::string, Channel *> _channels;
  std::string _hostname;

  Server(const Server &other);
  Server &operator=(const Server &other);

public:
  Server(void);
  ~Server(void);

  void init(void);
  void run(void);
  Client *findClientByNick(const std::string &nick);
  void sendMessageToChannel(const std::string &channel, const std::string &msg);
  void cleanup(void);
};
