
#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
enum tokenType { COMMAND, MODE, USERNAME, NUMBER, _T_T_SIZE };

enum modeTokenType {
  SWITCH_INVITE,
  SWITCH_TOPIC_RESTRICTIONS,
  SWITCH_CANAL_KEY,
  SWITCH_PRIVILEGES,
  SWITCH_USER_LIMIT,
  _M_T_T_SIZE
};

// Refaire sans distinction mode / type
struct commandToken {
private:
  tokenType _type;
  modeTokenType _mode_type;
  int64_t _num_data;
  std::string _text_data;

public:
  commandToken(void);
  // Bad
  commandToken(const tokenType &token_type,
               const modeTokenType &mode_type = _M_T_T_SIZE,
               const int64_t &num_data = 0, const std::string &str_data = "");
  commandToken(const commandToken &other);
  commandToken &operator=(const commandToken &other);

  bool matchTokenType(const tokenType &type) const;
  bool matchModeTokenType(const modeTokenType &type) const;
  bool hasNumeralData(void) const;
  bool hasTextData(void) const;

  const tokenType &GetTokenType
};
