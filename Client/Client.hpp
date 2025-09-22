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

  void addOperatorPerms(const std::string &channel_name);
  void removeOperatorPerms(const std::string &channel_name);
  bool isOperatorForThisChannel(const std::string &channel_name);

  // Recupere les nouvelles donnees depuis server&
  void receiveData(Server &server);

  // Gere toutes les commandes (enum de type de commande?)
  void handleCommand(Server &server, const std::string &raw_cmd);

  // Enregistre un message dans la queue pour un futur envoi
  void queueMessage(const std::string &message);

  // Envoie les messages en attente par le _fd
  void processMessages(void);

  // Gestion PASS
  void tryAuthenticate(Server &server,
                       const std::vector<std::string> &pass_params);
  // Gestion NICK
  void trySetNickname(Server &server,
                      const std::vector<std::string> &nick_params);
  // Gestion USER
  void trySetUsername(Server &server,
                      const std::vector<std::string> &user_params);
  // Gestion JOIN
  void tryJoinChannel(Server &server,
                      const std::vector<std::string> &join_params);

  // Gestion PRIVMSG
  void tryPrivateMessage(Server &server,
                         const std::vector<std::string> &privmsg_params);

  // Gestion QUIT
  void tryQuit(Server &server, const std::vector<std::string> &quit_params);
};
