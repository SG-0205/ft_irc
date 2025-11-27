#pragma once

#include "../Server/Server.hpp"
#include <queue>
#include <set>
#include <string>
#include <vector>

class Channel;

class Client {
private:
  int _fd;
  std::string _nickname;
  std::string _username;
  std::string _realname;
  std::string _hostname;
  std::string _msg_buffer;
  bool _authenticated;
  std::set<Channel *> _joind_channels;
  std::queue<std::string> _send_queue;

public:
  // stocker fd, hostname = getpeername();
  Client(const int &client_fd, Server &server);

  // Close les fd correspondant au client.
  virtual ~Client(void);

  void addOperatorPerms(Channel *channel);
  void removeOperatorPerms(Channel *channel);
  bool isOperatorForThisChannel(Channel *channel);

  // Recupere les nouvelles donnees depuis server&
  void receiveData(Server &server);

  // Gere toutes les commandes (enum de type de commande?)
  void handleCommand(Server *server, const std::string &raw_cmd);

  // Enregistre un message dans la queue pour un futur envoi
  void queueMessage(const std::string &message);

  // Envoie les messages en attente par le _fd
  bool processMessages(void);

  // Gestion PASS
  bool tryAuthenticate(Server *server,
                       const std::vector<std::string> &pass_params);
  // Gestion NICK
  bool trySetNickname(Server *server,
                      const std::vector<std::string> &nick_params);
  // Gestion USER
  bool trySetUsername(Server *server,
                      const std::vector<std::string> &user_params);
  // Gestion JOIN
  bool tryJoinChannel(Server *server,
                      const std::vector<std::string> &join_params);

  // Gestion PRIVMSG
  void tryPrivateMessage(Server *server,
                         const std::vector<std::string> &privmsg_params);

  // Gestion QUIT
  void tryQuit(Server *server, const std::vector<std::string> &quit_params);

  // Getters pour gestion externe
  std::set<Channel *> &getChannels(void);

  // Getters names;
  const std::string &GetNickName(void) const;
  const std::string &GetUserName(void) const;
  const std::string &GetRealName(void) const;
  const std::string &GetHostname(void) const;
};
