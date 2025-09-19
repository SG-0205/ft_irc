#pragma once

#include <set>
#include <string>

#include "../Client/Client.hpp"

class Channel {
private:
  static const int DEF_USER_LIMIT;

  std::string _name;
  std::string _topic;
  std::set<Client *> _members;
  std::set<Client *> _operators;
  std::set<Client *> _invited;
  std::string _key;
  int _user_limit;
  bool _invite_only;
  bool _topic_restricted;

  bool _hasKey(void) const;

public:
  Channel(std::string &channel_name);
  virtual ~Channel(void);

  // tente d'ajouter un membre au channel
  bool addMember(Client *client, const std::string key = "");

  // enleve un membre du channel (penser a enlever des operateurs)
  void removeMember(Client *client);

  // Envoie un message a tous les membres sauf celui exclu (sender)
  void broadcast(const std::string &message, Client *exclude = NULL);

  // Gestion KICK si le sender est un operateur
  void tryKick(Client *sender, Client *target, const std::string &reason);

  // Gestion INVITE
  void tryInvite(Client *sender, Client *target);

  // Gestion TOPIC
  void tryTopic(Client *sender, const std::string &new_topic);

  // Gestion MODE
  void tryMode(Client *sender, const std::string &mode_message);

  bool isOperator(Client *client);
};
