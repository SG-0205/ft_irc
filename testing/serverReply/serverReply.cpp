#include "ServerResponses.hpp"
#include <cstddef>
#include <iostream>

const char serverReply::ARG_PLACEHOLDER = '\x1A';

static std::map<int, serverReply> _initTemplates(void) {
  std::map<int, serverReply> templates;

  templates.insert(std::make_pair(
      RPL_WELCOME,
      serverReply(RPL_WELCOME, "001",
                  ":Welcome to the Internet Relay Network \x1A", true)));
  templates.insert(std::make_pair(
      RPL_YOURHOST,
      serverReply(RPL_YOURHOST, "002",
                  ":Your host is \x1A, running version \x1A", true)));
  templates.insert(std::make_pair(
      RPL_CREATED,
      serverReply(RPL_CREATED, "003", ":This server was created \x1A", true)));
  templates.insert(std::make_pair(
      RPL_MYINFO, serverReply(RPL_MYINFO, "004", "\x1A \x1A \x1A \x1A", true)));
  templates.insert(std::make_pair(
      RPL_ISUPPORT, serverReply(RPL_ISUPPORT, "005",
                                "\x1A :are supported by this server", true)));
  templates.insert(std::make_pair(
      RPL_UMODEIS, serverReply(RPL_UMODEIS, "221", "\x1A", true)));

  templates.insert(std::make_pair(
      RPL_LUSERCLIENT,
      serverReply(RPL_LUSERCLIENT, "251",
                  ":There are \x1A users and \x1A invisible on \x1A servers",
                  true)));
  templates.insert(std::make_pair(
      RPL_LUSEROP,
      serverReply(RPL_LUSEROP, "252", "\x1A :operator(s) online", true)));
  templates.insert(std::make_pair(
      RPL_LUSERUNKNOWN, serverReply(RPL_LUSERUNKNOWN, "253",
                                    "\x1A :unknown connection(s)", true)));
  templates.insert(std::make_pair(
      RPL_LUSERCHANNELS,
      serverReply(RPL_LUSERCHANNELS, "254", "\x1A :channels formed", true)));
  templates.insert(std::make_pair(
      RPL_LUSERME, serverReply(RPL_LUSERME, "255",
                               ":I have \x1A clients and \x1A servers", true)));

  templates.insert(std::make_pair(
      RPL_AWAY, serverReply(RPL_AWAY, "301", "\x1A :\x1A", true)));
  templates.insert(std::make_pair(
      RPL_UNAWAY,
      serverReply(RPL_UNAWAY, "305", ":You are no longer marked as being away",
                  false)));
  templates.insert(std::make_pair(
      RPL_NOWAWAY, serverReply(RPL_NOWAWAY, "306",
                               ":You have been marked as being away", false)));

  templates.insert(std::make_pair(
      RPL_WHOISUSER,
      serverReply(RPL_WHOISUSER, "311", "\x1A \x1A \x1A * :\x1A", true)));
  templates.insert(
      std::make_pair(RPL_WHOISSERVER, serverReply(RPL_WHOISSERVER, "312",
                                                  "\x1A \x1A :\x1A", true)));
  templates.insert(std::make_pair(
      RPL_WHOISOPERATOR,
      serverReply(RPL_WHOISOPERATOR, "313", "\x1A :is an IRC operator", true)));
  templates.insert(std::make_pair(
      RPL_WHOISIDLE,
      serverReply(RPL_WHOISIDLE, "317",
                  "\x1A \x1A \x1A :seconds idle, signon time", true)));
  templates.insert(std::make_pair(
      RPL_ENDOFWHOIS,
      serverReply(RPL_ENDOFWHOIS, "318", "\x1A :End of WHOIS list", true)));
  templates.insert(
      std::make_pair(RPL_WHOISCHANNELS, serverReply(RPL_WHOISCHANNELS, "319",
                                                    "\x1A :\x1A", true)));

  templates.insert(std::make_pair(
      RPL_LIST, serverReply(RPL_LIST, "322", "\x1A \x1A :\x1A", true)));
  templates.insert(std::make_pair(
      RPL_LISTEND, serverReply(RPL_LISTEND, "323", ":End of LIST", false)));
  templates.insert(
      std::make_pair(RPL_CHANNELMODEIS,
                     serverReply(RPL_CHANNELMODEIS, "324", "\x1A \x1A", true)));
  templates.insert(
      std::make_pair(RPL_CREATIONTIME,
                     serverReply(RPL_CREATIONTIME, "329", "\x1A \x1A", true)));

  templates.insert(
      std::make_pair(RPL_NOTOPIC, serverReply(RPL_NOTOPIC, "331",
                                              "\x1A :No topic is set", true)));
  templates.insert(std::make_pair(
      RPL_TOPIC, serverReply(RPL_TOPIC, "332", "\x1A :\x1A", true)));

  templates.insert(std::make_pair(
      RPL_INVITING, serverReply(RPL_INVITING, "341", "\x1A \x1A", true)));
  templates.insert(std::make_pair(
      RPL_INVITELIST, serverReply(RPL_INVITELIST, "346", "\x1A \x1A", true)));
  templates.insert(std::make_pair(
      RPL_ENDOFINVITELIST, serverReply(RPL_ENDOFINVITELIST, "347",
                                       "\x1A :End of invite list", true)));
  templates.insert(std::make_pair(
      RPL_EXCEPTLIST, serverReply(RPL_EXCEPTLIST, "348", "\x1A \x1A", true)));
  templates.insert(std::make_pair(
      RPL_ENDOFEXCEPTLIST, serverReply(RPL_ENDOFEXCEPTLIST, "349",
                                       "\x1A :End of exception list", true)));

  templates.insert(
      std::make_pair(RPL_VERSION, serverReply(RPL_VERSION, "351",
                                              "\x1A.\x1A \x1A :\x1A", true)));
  templates.insert(std::make_pair(
      RPL_NAMEREPLY, serverReply(RPL_NAMEREPLY, "353", "= \x1A :\x1A", true)));
  templates.insert(std::make_pair(
      RPL_ENDOFNAMES,
      serverReply(RPL_ENDOFNAMES, "366", "\x1A :End of NAMES list", true)));
  templates.insert(std::make_pair(
      RPL_WHOISMODES,
      serverReply(RPL_WHOISMODES, "379", "\x1A :is using modes \x1A", true)));

  // ==================================================================
  // Erreurs (ERR)
  // ==================================================================
  templates.insert(std::make_pair(
      ERR_NOSUCHNICK,
      serverReply(ERR_NOSUCHNICK, "401", "\x1A :No such nick/channel", true)));
  templates.insert(std::make_pair(
      ERR_NOSUCHSERVER,
      serverReply(ERR_NOSUCHSERVER, "402", "\x1A :No such server", true)));
  templates.insert(std::make_pair(
      ERR_NOSUCHCHANNEL,
      serverReply(ERR_NOSUCHCHANNEL, "403", "\x1A :No such channel", true)));
  templates.insert(std::make_pair(
      ERR_CANNOTSENDTOCHAN, serverReply(ERR_CANNOTSENDTOCHAN, "404",
                                        "\x1A :Cannot send to channel", true)));
  templates.insert(std::make_pair(
      ERR_TOOMANYCHANNELS,
      serverReply(ERR_TOOMANYCHANNELS, "405",
                  "\x1A :You have joined too many channels", true)));
  templates.insert(
      std::make_pair(ERR_WASNOSUCHNICK,
                     serverReply(ERR_WASNOSUCHNICK, "406",
                                 "\x1A :There was no such nickname", true)));
  templates.insert(std::make_pair(
      ERR_TOOMANYTARGETS,
      serverReply(ERR_TOOMANYTARGETS, "407", "\x1A :Too many targets", true)));
  templates.insert(
      std::make_pair(ERR_NOORIGIN, serverReply(ERR_NOORIGIN, "409",
                                               ":No origin specified", false)));
  templates.insert(std::make_pair(
      ERR_NORECIPIENT,
      serverReply(ERR_NORECIPIENT, "411", ":No recipient given (\x1A)", true)));
  templates.insert(
      std::make_pair(ERR_NOTEXTTOSEND, serverReply(ERR_NOTEXTTOSEND, "412",
                                                   ":No text to send", false)));
  templates.insert(std::make_pair(
      ERR_UNKNOWNCOMMAND,
      serverReply(ERR_UNKNOWNCOMMAND, "421", "\x1A :Unknown command", true)));
  templates.insert(
      std::make_pair(ERR_NOMOTD, serverReply(ERR_NOMOTD, "422",
                                             ":MOTD File is missing", false)));
  templates.insert(std::make_pair(
      ERR_NONICKNAMEGIVEN,
      serverReply(ERR_NONICKNAMEGIVEN, "431", ":No nickname given", false)));
  templates.insert(std::make_pair(
      ERR_ERRONEUSNICKNAME, serverReply(ERR_ERRONEUSNICKNAME, "432",
                                        "\x1A :Erroneous nickname", true)));
  templates.insert(
      std::make_pair(ERR_NICKNAMEINUSER,
                     serverReply(ERR_NICKNAMEINUSER, "432",
                                 "\x1A :Nickname is already in use", true)));
  templates.insert(std::make_pair(
      ERR_NICKCOLLISION, serverReply(ERR_NICKCOLLISION, "436",
                                     "\x1A :Nickname collision KILL", true)));

  templates.insert(std::make_pair(
      ERR_USERNOTINCHANNEL,
      serverReply(ERR_USERNOTINCHANNEL, "441",
                  "\x1A \x1A :They aren't on that channel", true)));
  templates.insert(std::make_pair(
      ERR_NOTONCHANNEL, serverReply(ERR_NOTONCHANNEL, "442",
                                    "\x1A :You're not on that channel", true)));
  templates.insert(
      std::make_pair(ERR_USERONCHANNEL,
                     serverReply(ERR_USERONCHANNEL, "443",
                                 "\x1A \x1A :is already on channel", true)));
  templates.insert(std::make_pair(
      ERR_NOTREGISTERED, serverReply(ERR_NOTREGISTERED, "451",
                                     ":You have not registered", false)));
  templates.insert(std::make_pair(
      ERR_NEEDMOREPARAMS, serverReply(ERR_NEEDMOREPARAMS, "461",
                                      "\x1A :Not enough parameters", true)));
  templates.insert(std::make_pair(
      ERR_ALREADYREGISTERED, serverReply(ERR_ALREADYREGISTERED, "462",
                                         ":You may not reregister", false)));
  templates.insert(std::make_pair(
      ERR_PASSWDMISMATCH,
      serverReply(ERR_PASSWDMISMATCH, "464", ":Password incorrect", false)));
  templates.insert(
      std::make_pair(ERR_YOUREBANNEDCREEP,
                     serverReply(ERR_YOUREBANNEDCREEP, "465",
                                 ":You are banned from this server", false)));

  templates.insert(std::make_pair(
      ERR_CHANNELISFULL, serverReply(ERR_CHANNELISFULL, "471",
                                     "\x1A :Cannot join channel (+l)", true)));
  templates.insert(std::make_pair(
      ERR_UNKNOWNMODE, serverReply(ERR_UNKNOWNMODE, "472",
                                   "\x1A :is unknown mode char to me", true)));
  templates.insert(std::make_pair(
      ERR_INVITEONLYCHAN, serverReply(ERR_INVITEONLYCHAN, "473",
                                      "\x1A :Cannot join channel (+i)", true)));
  templates.insert(std::make_pair(
      ERR_BANNEDFROMCHAN, serverReply(ERR_BANNEDFROMCHAN, "474",
                                      "\x1A :Cannot join channel (+b)", true)));
  templates.insert(std::make_pair(
      ERR_BADCHANNELKEY, serverReply(ERR_BADCHANNELKEY, "475",
                                     "\x1A :Cannot join channel (+k)", true)));
  templates.insert(
      std::make_pair(ERR_BANLISTFULL,
                     serverReply(ERR_BANLISTFULL, "478",
                                 "\x1A \x1A :Channel ban list is full", true)));

  templates.insert(std::make_pair(
      ERR_NOPRIVILEGES,
      serverReply(ERR_NOPRIVILEGES, "481",
                  ":Permission Denied - You're not an IRC operator", false)));
  templates.insert(
      std::make_pair(ERR_CHANOPRIVSNEEDED,
                     serverReply(ERR_CHANOPRIVSNEEDED, "482",
                                 "\x1A :You're not channel operator", true)));
  templates.insert(std::make_pair(
      ERR_CANTKILLSERVER, serverReply(ERR_CANTKILLSERVER, "483",
                                      ":You can't kill a server!", false)));
  templates.insert(std::make_pair(
      ERR_NOOPERHOST,
      serverReply(ERR_NOOPERHOST, "491", ":No O-lines for your host", false)));
  templates.insert(std::make_pair(
      ERR_UMODEUNKNOWNFLAG,
      serverReply(ERR_UMODEUNKNOWNFLAG, "501", ":Unknown MODE flag", false)));
  templates.insert(std::make_pair(
      ERR_USERDONTMATCH,
      serverReply(ERR_USERDONTMATCH, "502",
                  ":Cannot change mode for other users", false)));

  return (templates);
}

const std::map<int, serverReply> serverReply::_templates = _initTemplates();

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

serverReply::~serverReply(void) {}

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

void serverReply::print(void) const {
  std::cout << "Server Response [" << _rpl_code_string << "]\n"
            << '\t' << "Format: " << _format_string << '\n'
            << '\t' << "Max params: " << _max_params << '\n';
}
