#pragma once

#include <map>
#include <poll.h>
#include <string>
#include <sys/poll.h>
#include <vector>

class Client;
class Channel;

class Server {
private:
  static const std::string SERVER_PASSWORD;
  static Server *SERVER_INSTANCE;

  int _port;
  std::string _password;
  int _server_fd;
  std::vector<pollfd> _fds;
  std::map<int, Client * > _clients;
  std::map<std::string, Channel *> _channels;
  volatile bool _stop_flag;
  volatile bool _clients_mod_flag;
  static std::string _hostname;

  Server(const Server &other);
  Server &operator=(const Server &other);

  // Nouvelle connexion
  void _handleNewConnection(void);
  // Logging
  void _warningMessage(const std::string &msg);
  // Fetching du Client*
  Client *_fetchClientByFD(const int &fd, const std::string &prefix = "");
  // Fetching du Channel *
  Channel *_fetchChannelByName(const std::string &channel_name);
  // Creation de pollfd
  static pollfd _newPollFd(const int &fd, const int &events,
                           const int &r_events);

  static void _signalHandler(int signal);

public:
  Server(const int &port, const std::string &password);
  ~Server(void);

  // Init
  void init(void);
  // Launch
  void run(void);
  // Remove client + mets a jour _client_mod_flag;
  void removeClient(const int &client_fd);
  // Channel relay
  void sendToChannel(const std::string &channel_name,
                     const std::string &message, Client *exclude = NULL);
  // Find registered client.
  Client *findClientByNick(const std::string &nick);
  // Create channel
  Channel *createChannel(const std::string &channel_name);
  // Remove channel
  void removeChannel(const std::string &channel_name);
  // Ressource cleaning (attendre que _stop_flag == true)
  void cleanup(void);
  // Password getter
  const std::string &GetPassword(void) const;
  // Instance getter (mainly for signals)
  static Server *getInstance(void);

  static const std::string &getServerHostname(void);
  static const std::string buildServerReply(const std::string &command,
                                            const std::string &nick,
                                            const std::string &params = "",
                                            const std::string &trailing = "");

  static const std::string
  buildUserReply(const std::string &nick, const std::string &user,
                 const std::string &host, const std::string &command,
                 const std::string &target, const std::string &trailing = "");

  static const std::string buildErrorReply(const std::string &command,
                                           const std::string &nick,
                                           const std::string &target,
                                           const std::string &reason);

  // TODO : exception avec errcode pour reply au client.
};
