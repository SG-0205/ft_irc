#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>

enum ReplyCodes {
  RPL_WELCOME,
  RPL_YOURHOST,
  RPL_CREATED,
  RPL_MYINFO,
  RPL_ISUPPORT,
  RPL_UMODEIS,
  RPL_LUSERCLIENT,
  RPL_LUSEROP,
  RPL_LUSERUNKNOWN,
  RPL_LUSERCHANNELS,
  RPL_LUSERME,
  RPL_AWAY,
  RPL_UNAWAY,
  RPL_NOWAWAY,
  RPL_WHOISUSER,
  RPL_WHOISSERVER,
  RPL_WHOISOPERATOR,
  RPL_WHOISIDLE,
  RPL_ENDOFWHOIS,
  RPL_WHOISCHANNELS,
  RPL_LIST,
  RPL_LISTEND,
  RPL_CHANNELMODEIS,
  RPL_CREATIONTIME,
  RPL_NOTOPIC,
  RPL_TOPIC,
  RPL_INVITING,
  RPL_INVITELIST,
  RPL_ENDOFINVITELIST,
  RPL_EXCEPTLIST,
  RPL_ENDOFEXCEPTLIST,
  RPL_VERSION,
  RPL_NAMEREPLY,
  RPL_ENDOFNAMES,
  RPL_WHOISMODES,
  ERR_NOSUCHNICK,
  ERR_NOSUCHSERVER,
  ERR_NOSUCHCHANNEL,
  ERR_CANNOTSENDTOCHAN,
  ERR_TOOMANYCHANNELS,
  ERR_WASNOSUCHNICK,
  ERR_TOOMANYTARGETS,
  ERR_NOORIGIN,
  ERR_NORECIPIENT,
  ERR_NOTEXTTOSEND,
  ERR_UNKNOWNCOMMAND,
  ERR_NOMOTD,
  ERR_NONICKNAMEGIVEN,
  ERR_ERRONEUSNICKNAME,
  ERR_NICKNAMEINUSER,
  ERR_NICKCOLLISION,
  ERR_USERNOTINCHANNEL,
  ERR_NOTONCHANNEL,
  ERR_USERONCHANNEL,
  ERR_NOTREGISTERED,
  ERR_NEEDMOREPARAMS,
  ERR_ALREADYREGISTERED,
  ERR_PASSWDMISMATCH,
  ERR_YOUREBANNEDCREEP,
  ERR_CHANNELISFULL,
  ERR_UNKNOWNMODE,
  ERR_INVITEONLYCHAN,
  ERR_BANNEDFROMCHAN,
  ERR_BADCHANNELKEY,
  ERR_BANLISTFULL,
  ERR_NOPRIVILEGES,
  ERR_CHANOPRIVSNEEDED,
  ERR_CANTKILLSERVER,
  ERR_NOOPERHOST,
  ERR_UMODEUNKNOWNFLAG,
  ERR_USERDONTMATCH,
  _R_C_SIZE
};

struct serverReply {
private:
  static const char ARG_PLACEHOLDER;
  static const std::map<int, serverReply> _templates;
  ReplyCodes _code;
  std::string _rpl_code_string;
  std::string _format_string;
  bool _fillable;
  size_t _max_params;

public:
  serverReply(void);
  serverReply(const ReplyCodes &code, const std::string &code_nb_string,
              const std::string &format_string, const bool &fillable);
  serverReply(const serverReply &other);
  serverReply &operator=(const serverReply &other);
  ~serverReply(void);

  const ReplyCodes &GetCodeEnum(void) const;
  const std::string &GetCodeString(void) const;
  const std::string &GetRawFormatString(void) const;
  const size_t &GetMaxParamsCount(void) const;
  std::string
  makeServerReply(const std::vector<std::string> &reply_params) const;

  static const serverReply &GetReplyTemplate(const ReplyCodes &code);

  void print(void) const;
};
