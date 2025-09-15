
#pragma once

#include <stdlib.h>
#include <string>

enum tokenType {
  KICK,
  INVITE,
  TOPIC,
  MODE,
  MODE_ID,
  STRING,
  CHAR,
  NUMBER,
  EOS,
  INVALID,
  _T_T_SIZE
};

struct commandToken {
private:
  tokenType _type;
  int64_t _num_data;
  std::string _text_data;

public:
  commandToken(void);
  commandToken(const tokenType &token_type);
  commandToken(const commandToken &other);
  commandToken &operator=(const commandToken &other);
  ~commandToken(void);

  void setNumericalData(const int64_t &number);
  void setTextData(const std::string &text);

  bool matchTokenType(const tokenType &type) const;
  bool hasNumeralData(void) const;
  bool hasTextData(void) const;
  bool isACommandIdentifier(void) const;
  bool isAModeIdentifier(void) const;

  const tokenType &GetTokenType(void) const;
  const int64_t &GetNumericalData(void) const;
  const std::string &GetTextData(void) const;
};
