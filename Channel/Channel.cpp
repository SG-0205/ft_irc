#include "Channel.hpp"
#include <cctype>

const int Channel::DEF_USER_LIMIT = -1;

Channel::Channel(const std::string &channel_name)
    : _name(channel_name), _topic(""), _members(), _operators(), _invited(),
      _key(""), _user_limit(DEF_USER_LIMIT), _invite_only(false),
      _topic_restricted(false) {}

Channel::~Channel(void) {
  _members.clear();
  _operators.clear();
  _invited.clear();
}
