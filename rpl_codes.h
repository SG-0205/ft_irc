#pragma once

// Connection
#define RPL_WELCOME "001"  // Welcome message after successful registration
#define RPL_YOURHOST "002" // Your host is... (server info)
#define RPL_CREATED "003"  // This server was created...
#define RPL_MYINFO "004"   // Server info (name, version, modes)

// Channel
#define RPL_TOPIC "332"      // Current topic of a channel
#define RPL_NAMREPLY "353"   // Names list of channel members
#define RPL_ENDOFNAMES "366" // End of /NAMES list

// Invitation / operators
#define RPL_INVITING "341"      // User is invited to a channel
#define RPL_CHANNELMODEIS "324" // Current modes of a channel

// Errors
#define ERR_NOSUCHNICK "401"        // No such nickname
#define ERR_NOSUCHCHANNEL "403"     // No such channel
#define ERR_CANNOTSENDTOCHAN "404"  // Cannot send to channel
#define ERR_NEEDMOREPARAMS "461"    // Not enough parameters
#define ERR_ALREADYREGISTERED "462" // Already registered
#define ERR_PASSWDMISMATCH "464"    // Password incorrect
#define ERR_CHANOPRIVSNEEDED "482"  // Channel operator privilege needed
#define ERR_NICKNAMEINUSE "433"     // Nickname is already in use #define
#define ERR_NOTONCHANNEL "442"      // You're not on that channel

#define MSG_SEPARATOR "\r\n"
