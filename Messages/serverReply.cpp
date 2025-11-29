#include "ServerResponses.hpp"
#include <cstddef>

const char serverReply::ARG_PLACEHOLDER = '\x1A';

static const std::map<int, serverReply> _templates = {
    {RPL_WELCOME, serverReply(RPL_WELCOME, "001",
                              ":Welcome to the Internet Relay Network \x1A",
                              true)}, // \x1A = <nick>!<user>@<host>
    {RPL_YOURHOST, serverReply(RPL_YOURHOST, "002",
                               ":Your host is \x1A, running version \x1A",
                               true)}, // servername, version
    {RPL_CREATED, serverReply(RPL_CREATED, "003",
                              ":This server was created \x1A", true)}, // date
    {RPL_MYINFO, serverReply(RPL_MYINFO, "004", "\x1A \x1A \x1A \x1A",
                             true)}, // servername version available-user-modes
                                     // available-channel-modes
    {RPL_ISUPPORT,
     serverReply(RPL_ISUPPORT, "005", "\x1A :are supported by this server",
                 true)}, // tokens séparés par espace + le texte final
    {RPL_UMODEIS, serverReply(RPL_UMODEIS, "221", "\x1A", true)}, // user modes
    {RPL_LUSERCLIENT,
     serverReply(RPL_LUSERCLIENT, "251",
                 ":There are \x1A users and \x1A invisible on \x1A servers",
                 true)}, // users, invis, servers
    {RPL_LUSEROP, serverReply(RPL_LUSEROP, "252", "\x1A :operator(s) online",
                              true)}, // nb opers
    {RPL_LUSERUNKNOWN, serverReply(RPL_LUSERUNKNOWN, "253",
                                   "\x1A :unknown connection(s)", true)}, // nb
    {RPL_LUSERCHANNELS,
     serverReply(RPL_LUSERCHANNELS, "254", "\x1A :channels formed",
                 true)}, // nb channels
    {RPL_LUSERME,
     serverReply(RPL_LUSERME, "255", ":I have \x1A clients and \x1A servers",
                 true)}, // clients, servers
    {RPL_AWAY,
     serverReply(RPL_AWAY, "301", "\x1A :\x1A", true)}, // nick, away message
    {RPL_UNAWAY, serverReply(RPL_UNAWAY, "305",
                             ":You are no longer marked as being away", false)},
    {RPL_NOWAWAY, serverReply(RPL_NOWAWAY, "306",
                              ":You have been marked as being away", false)},
    {RPL_WHOISUSER, serverReply(RPL_WHOISUSER, "311", "\x1A \x1A \x1A * :\x1A",
                                true)}, // nick user host *, realname
    {RPL_WHOISSERVER, serverReply(RPL_WHOISSERVER, "312", "\x1A \x1A :\x1A",
                                  true)}, // nick server, server info
    {RPL_WHOISOPERATOR, serverReply(RPL_WHOISOPERATOR, "313",
                                    "\x1A :is an IRC operator", true)}, // nick
    {RPL_WHOISIDLE, serverReply(RPL_WHOISIDLE, "317",
                                "\x1A \x1A \x1A :seconds idle, signon time",
                                true)}, // nick idle signon
    {RPL_ENDOFWHOIS, serverReply(RPL_ENDOFWHOIS, "318",
                                 "\x1A :End of WHOIS list", true)}, // nick
    {RPL_WHOISCHANNELS, serverReply(RPL_WHOISCHANNELS, "319", "\x1A :\x1A",
                                    true)}, // nick, liste canaux avec préfixes
    {RPL_LIST, serverReply(RPL_LIST, "322", "\x1A \x1A :\x1A",
                           true)}, // channel, nb visible, topic
    {RPL_LISTEND, serverReply(RPL_LISTEND, "323", ":End of LIST", false)},
    {RPL_CHANNELMODEIS, serverReply(RPL_CHANNELMODEIS, "324", "\x1A \x1A",
                                    true)}, // channel, modes + params
    {RPL_CREATIONTIME, serverReply(RPL_CREATIONTIME, "329", "\x1A \x1A",
                                   true)}, // channel, timestamp
    {RPL_NOTOPIC,
     serverReply(RPL_NOTOPIC, "331", "\x1A :No topic is set", true)}, // channel
    {RPL_TOPIC,
     serverReply(RPL_TOPIC, "332", "\x1A :\x1A", true)}, // channel, topic
    {RPL_INVITING,
     serverReply(RPL_INVITING, "341", "\x1A \x1A", true)}, // nick, channel
    {RPL_INVITELIST, serverReply(RPL_INVITELIST, "346", "\x1A \x1A",
                                 true)}, // channel, invitemask
    {RPL_ENDOFINVITELIST,
     serverReply(RPL_ENDOFINVITELIST, "347", "\x1A :End of invite list",
                 true)}, // channel
    {RPL_EXCEPTLIST, serverReply(RPL_EXCEPTLIST, "348", "\x1A \x1A",
                                 true)}, // channel, exceptionmask
    {RPL_ENDOFEXCEPTLIST,
     serverReply(RPL_ENDOFEXCEPTLIST, "349", "\x1A :End of exception list",
                 true)}, // channel
    {RPL_VERSION, serverReply(RPL_VERSION, "351", "\x1A.\x1A \x1A :\x1A",
                              true)}, // version.debug server, comments
    {RPL_NAMEREPLY, serverReply(RPL_NAMEREPLY, "353", "= \x1A :\x1A",
                                true)}, // channel, liste nicks avec préfixes
    {RPL_ENDOFNAMES, serverReply(RPL_ENDOFNAMES, "366",
                                 "\x1A :End of NAMES list", true)}, // channel
    {RPL_WHOISMODES,
     serverReply(RPL_WHOISMODES, "379", "\x1A :is using modes \x1A",
                 true)}, // nick, modes

    // === ERRORS ===
    {ERR_NOSUCHNICK,
     serverReply(ERR_NOSUCHNICK, "401", "\x1A :No such nick/channel", true)},
    {ERR_NOSUCHSERVER,
     serverReply(ERR_NOSUCHSERVER, "402", "\x1A :No such server", true)},
    {ERR_NOSUCHCHANNEL,
     serverReply(ERR_NOSUCHCHANNEL, "403", "\x1A :No such channel", true)},
    {ERR_CANNOTSENDTOCHAN, serverReply(ERR_CANNOTSENDTOCHAN, "404",
                                       "\x1A :Cannot send to channel", true)},
    {ERR_TOOMANYCHANNELS,
     serverReply(ERR_TOOMANYCHANNELS, "405",
                 "\x1A :You have joined too many channels", true)},
    {ERR_WASNOSUCHNICK, serverReply(ERR_WASNOSUCHNICK, "406",
                                    "\x1A :There was no such nickname", true)},
    {ERR_TOOMANYTARGETS,
     serverReply(ERR_TOOMANYTARGETS, "407", "\x1A :Too many targets", true)},
    {ERR_NOORIGIN,
     serverReply(ERR_NOORIGIN, "409", ":No origin specified", false)},
    {ERR_NORECIPIENT,
     serverReply(ERR_NORECIPIENT, "411", ":No recipient given (\x1A)",
                 true)}, // command
    {ERR_NOTEXTTOSEND,
     serverReply(ERR_NOTEXTTOSEND, "412", ":No text to send", false)},
    {ERR_UNKNOWNCOMMAND,
     serverReply(ERR_UNKNOWNCOMMAND, "421", "\x1A :Unknown command", true)},
    {ERR_NOMOTD,
     serverReply(ERR_NOMOTD, "422", ":MOTD File is missing", false)},
    {ERR_NONICKNAMEGIVEN,
     serverReply(ERR_NONICKNAMEGIVEN, "431", ":No nickname given", false)},
    {ERR_ERRONEUSNICKNAME, serverReply(ERR_ERRONEUSNICKNAME, "432",
                                       "\x1A :Erroneous nickname", true)},
    {ERR_NICKNAMEINUSER,
     serverReply(ERR_NICKNAMEINUSER, "432", "\x1A :Nickname is already in use",
                 true)}, // certains serveurs utilisent 433 ici aussi
    {ERR_NICKCOLLISION, serverReply(ERR_NICKCOLLISION, "436",
                                    "\x1A :Nickname collision KILL", true)},
    {ERR_USERNOTINCHANNEL, serverReply(ERR_USERNOTINCHANNEL, "441",
                                       "\x1A \x1A :They aren't on that channel",
                                       true)}, // nick channel
    {ERR_NOTONCHANNEL, serverReply(ERR_NOTONCHANNEL, "442",
                                   "\x1A :You're not on that channel", true)},
    {ERR_USERONCHANNEL,
     serverReply(ERR_USERONCHANNEL, "443", "\x1A \x1A :is already on channel",
                 true)}, // nick channel
    {ERR_NOTREGISTERED,
     serverReply(ERR_NOTREGISTERED, "451", ":You have not registered", false)},
    {ERR_NEEDMOREPARAMS, serverReply(ERR_NEEDMOREPARAMS, "461",
                                     "\x1A :Not enough parameters", true)},
    {ERR_ALREADYREGISTERED, serverReply(ERR_ALREADYREGISTERED, "462",
                                        ":You may not reregister", false)},
    {ERR_PASSWDMISMATCH,
     serverReply(ERR_PASSWDMISMATCH, "464", ":Password incorrect", false)},
    {ERR_YOUREBANNEDCREEP,
     serverReply(ERR_YOUREBANNEDCREEP, "465",
                 ":You are banned from this server", false)},
    {ERR_CHANNELISFULL, serverReply(ERR_CHANNELISFULL, "471",
                                    "\x1A :Cannot join channel (+l)", true)},
    {ERR_UNKNOWNMODE, serverReply(ERR_UNKNOWNMODE, "472",
                                  "\x1A :is unknown mode char to me", true)},
    {ERR_INVITEONLYCHAN, serverReply(ERR_INVITEONLYCHAN, "473",
                                     "\x1A :Cannot join channel (+i)", true)},
    {ERR_BANNEDFROMCHAN, serverReply(ERR_BANNEDFROMCHAN, "474",
                                     "\x1A :Cannot join channel (+b)", true)},
    {ERR_BADCHANNELKEY, serverReply(ERR_BADCHANNELKEY, "475",
                                    "\x1A :Cannot join channel (+k)", true)},
    {ERR_BANLISTFULL,
     serverReply(ERR_BANLISTFULL, "478", "\x1A \x1A :Channel ban list is full",
                 true)}, // channel mask
    {ERR_NOPRIVILEGES,
     serverReply(ERR_NOPRIVILEGES, "481",
                 ":Permission Denied - You're not an IRC operator", false)},
    {ERR_CHANOPRIVSNEEDED,
     serverReply(ERR_CHANOPRIVSNEEDED, "482",
                 "\x1A :You're not channel operator", true)},
    {ERR_CANTKILLSERVER, serverReply(ERR_CANTKILLSERVER, "483",
                                     ":You can't kill a server!", false)},
    {ERR_NOOPERHOST,
     serverReply(ERR_NOOPERHOST, "491", ":No O-lines for your host", false)},
    {ERR_UMODEUNKNOWNFLAG,
     serverReply(ERR_UMODEUNKNOWNFLAG, "501", ":Unknown MODE flag", false)},
    {ERR_USERDONTMATCH,
     serverReply(ERR_USERDONTMATCH, "502",
                 ":Cannot change mode for other users", false)},
};

serverReply::serverReply(void)
    : _code(_R_C_SIZE), _rpl_code_string(), _format_string(), _fillable(false),
      _max_params(0) {}

serverReply::serverReply(const ReplyCodes &code,
                         const std::string &code_nb_string,
                         const std::string &format_string, const bool &fillable)
    : _code(code), _rpl_code_string(code_nb_string),
      _format_string(format_string), _fillable(fillable), _max_params(0) {

  for (size_t i = 0; i < _format_string.length(); i++) {
    if (_format_string[i] == ARG_PLACEHOLDER)
      _max_params++;
  }
}

serverReply::serverReply(const serverReply &other)
    : _code(other._code), _rpl_code_string(other._rpl_code_string),
      _format_string(other._format_string), _fillable(other._fillable),
      _max_params(other._max_params) {}

serverReply &serverReply::operator=(const serverReply &other) {
  if (this == &other)
    return (*this);

  _code = other._code;
  _rpl_code_string = other._rpl_code_string;
  _format_string = other._format_string;
  _fillable = other._fillable;
  _max_params = other._max_params;

  return (*this);
}

const ReplyCodes &serverReply::GetCodeEnum(void) const { return (_code); }

const std::string &serverReply::GetCodeString(void) const {
  return (_rpl_code_string);
}

const std::string &serverReply::GetRawFormatString(void) const {
  return (_format_string);
}

const size_t &serverReply::GetMaxParamsCount(void) const {
  return (_max_params);
}

/**
 * @brief Envoie la version remplie du format de reponse;
 *
 * @param const std::vector<std::string>& Parametres a inserer dans la reponse;
 * @return std::string Reponse formattee;
 */
std::string serverReply::makeServerReply(
    const std::vector<std::string> &reply_params) const {
  if (!_fillable)
    return (_format_string);

  std::string reply = _format_string;
  size_t pos = 0;
  size_t param_i = 0;

  while ((pos = reply.find(ARG_PLACEHOLDER, pos)) != std::string::npos) {
    if (param_i >= reply_params.size())
      reply.replace(pos, 1, "");
    else
      reply.replace(pos, 1, reply_params[param_i]);
  }

  return (reply);
}

/**
 * @brief Renvoie le template de reponse en const ref pour copie / build direct
 * de la reponse
 *
 * @param const ReplyCodes& Enum de code de retour
 * @return const serverReply& Template de reponse;
 */
const serverReply &serverReply::GetReplyTemplate(const ReplyCodes &code) {
  return (_templates.at(code));
}
