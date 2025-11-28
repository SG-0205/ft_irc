#include "ServerResponses.hpp"

const char serverReply::ARG_PLACEHOLDER = '\x1A';

serverReply::serverReply(void)
    : _code(_R_C_SIZE), _rpl_code_string(), _format_string(), _fillable(false) {
}

serverReply::serverReply(const ReplyCodes &code,
                         const std::string &code_nb_string,
                         const std::string &format_string, const bool &fillable)
    : _code(code), _rpl_code_string(code_nb_string),
      _format_string(format_string), _fillable(fillable) {}

serverReply::serverReply(const serverReply &other)
    : _code(other._code), _rpl_code_string(other._rpl_code_string),
      _format_string(other._format_string), _fillable(other._fillable) {}

serverReply &serverReply::operator=(const serverReply &other) {
  if (this == &other)
    return (*this);

  _code = other._code;
  _rpl_code_string = other._rpl_code_string;
  _format_string = other._format_string;
  _fillable = other._fillable;

  return (*this);
}

const ReplyCodes &serverReply::GetCodeEnum(void) const { return (_code); }

const std::string &serverReply::GetCodeString(void) const {
  return (_rpl_code_string);
}

const std::string &serverReply::GetRawFormatString(void) const {
  return (_format_string);
}

std::string
serverReply::makeServerReply(const std::vector<std::string> &reply_params) {
  if (!_fillable)
    return (_format_string);
}
