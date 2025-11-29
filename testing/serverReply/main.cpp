#include "ServerResponses.hpp"
#include <iomanip>
#include <iostream>

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

std::vector<std::string> vec(const char *s1 = "", const char *s2 = "",
                             const char *s3 = "", const char *s4 = "") {
  std::vector<std::string> v;
  if (std::string(s1).size())
    v.push_back(s1);
  if (std::string(s2).size())
    v.push_back(s2);
  if (std::string(s3).size())
    v.push_back(s3);
  if (std::string(s4).size())
    v.push_back(s4);
  return v;
}

void printReply(
    const std::string &title, const serverReply &tmpl,
    const std::vector<std::string> &params = std::vector<std::string>()) {
  std::string result = tmpl.makeServerReply(params);

  std::cout << CYAN << std::setw(30) << std::left << title << RESET << " → ";

  if (result.empty() && params.empty() == false &&
      tmpl.GetRawFormatString().find(serverReply::ARG_PLACEHOLDER) !=
          std::string::npos) {
    std::cout << RED << "ERROR (empty result)" << RESET;
  } else if (result.empty() &&
             tmpl.GetRawFormatString().find(serverReply::ARG_PLACEHOLDER) ==
                 std::string::npos) {
    std::cout << YELLOW << "OK (no params needed)" << RESET;
  } else {
    std::cout << GREEN << ":ft_irc.42 " << std::setw(3) << tmpl.GetCodeString()
              << " " << result << RESET;
  }
  std::cout << std::endl;
}

int main() {
  std::cout << YELLOW << "\n=== TEST SYSTEME DE REPONSES FT_IRC (C++98) ===\n"
            << RESET << std::endl;

  std::string nick = "toto";
  std::string userhost = nick + "!~toto@127.0.0.1";
  std::string channel = "#42paris";
  std::string topic = "Bienvenue les mouettes !";

  // ===================================================================
  // Messages de connexion
  // ===================================================================
  printReply("RPL_WELCOME", serverReply::GetReplyTemplate(RPL_WELCOME),
             vec(userhost.c_str()));
  printReply("RPL_YOURHOST", serverReply::GetReplyTemplate(RPL_YOURHOST),
             vec("ft_irc.42.fr", "ft_irc-1.0"));
  printReply("RPL_CREATED", serverReply::GetReplyTemplate(RPL_CREATED),
             vec("29/11/2025"));
  printReply("RPL_MYINFO", serverReply::GetReplyTemplate(RPL_MYINFO),
             vec("ft_irc.42.fr", "1.0", "iosw", "bklove"));
  printReply("RPL_ISUPPORT", serverReply::GetReplyTemplate(RPL_ISUPPORT),
             vec("CHANTYPES=# PREFIX=(ov)@+ CHANMODES=b,k,l,imnpst "
                 "MAXLIST=b:50 :are supported"));

  std::cout << std::endl;

  // ===================================================================
  // Messages utilisateur
  // ===================================================================
  printReply("RPL_UNAWAY", serverReply::GetReplyTemplate(RPL_UNAWAY));
  printReply("RPL_NOWAWAY", serverReply::GetReplyTemplate(RPL_NOWAWAY));
  printReply("RPL_TOPIC", serverReply::GetReplyTemplate(RPL_TOPIC),
             vec(channel.c_str(), topic.c_str()));
  printReply("RPL_NOTOPIC", serverReply::GetReplyTemplate(RPL_NOTOPIC),
             vec(channel.c_str()));
  printReply("RPL_NAMEREPLY", serverReply::GetReplyTemplate(RPL_NAMEREPLY),
             vec(channel.c_str(), "@toto +alice bob @dupont"));
  printReply("RPL_ENDOFNAMES", serverReply::GetReplyTemplate(RPL_ENDOFNAMES),
             vec(channel.c_str()));

  std::cout << std::endl;

  // ===================================================================
  // Erreurs classiques
  // ===================================================================
  printReply("ERR_NOSUCHNICK", serverReply::GetReplyTemplate(ERR_NOSUCHNICK),
             vec("fantome"));
  printReply("ERR_NOSUCHCHANNEL",
             serverReply::GetReplyTemplate(ERR_NOSUCHCHANNEL), vec("#ghost"));
  printReply("ERR_NEEDMOREPARAMS",
             serverReply::GetReplyTemplate(ERR_NEEDMOREPARAMS), vec("JOIN"));
  printReply("ERR_ALREADYREGISTERED",
             serverReply::GetReplyTemplate(ERR_ALREADYREGISTERED));
  printReply("ERR_PASSWDMISMATCH",
             serverReply::GetReplyTemplate(ERR_PASSWDMISMATCH));
  printReply("ERR_NOTONCHANNEL",
             serverReply::GetReplyTemplate(ERR_NOTONCHANNEL), vec("#secret"));
  printReply("ERR_CHANNELISFULL",
             serverReply::GetReplyTemplate(ERR_CHANNELISFULL), vec("#full"));

  // ===================================================================
  // Tests de robustesse
  // ===================================================================
  std::cout << YELLOW << "\n=== TESTS DE ROBUSTESSE ===\n" << RESET;

  std::cout << "Trop peu de params pour RPL_WELCOME → ";
  std::string r1 = serverReply::GetReplyTemplate(RPL_WELCOME)
                       .makeServerReply(std::vector<std::string>());
  std::cout << (r1.empty() ? GREEN "OK (retourne vide)" : RED "KO") << RESET
            << std::endl;

  std::cout << "Trop de params pour RPL_UNAWAY → ";
  std::vector<std::string> extra;
  extra.push_back("toto");
  std::string r2 =
      serverReply::GetReplyTemplate(RPL_UNAWAY).makeServerReply(extra);
  std::string expected =
      serverReply::GetReplyTemplate(RPL_UNAWAY).GetRawFormatString();
  std::cout << (r2 == expected ? GREEN "OK (ignore les extras)" : RED "KO")
            << RESET << std::endl;

  std::cout
      << YELLOW
      << "\nSi tout est vert → ton système de replies est PARFAIT pour 42\n"
      << RESET;

  return 0;
}
