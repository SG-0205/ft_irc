
#include "commandToken.hpp"
#include <cstdint>
#include <stdlib.h>

commandToken::commandToken(void)
    : _type(_T_T_SIZE), _num_data(0), _text_data("") {}

commandToken::commandToken(const tokenType &token_type)
    : _type(token_type), _num_data(0), _text_data("") {}

commandToken::commandToken(const commandToken &other)
    : _type(other._type), _num_data(other._num_data),
      _text_data(other._text_data) {}

commandToken &commandToken::operator=(const commandToken &other) {
  if (this == &other)
    return (*this);

  this->_type = other._type;
  this->_num_data = other._num_data;
  this->_text_data = other._text_data;

  return (*this);
}

commandToken::~commandToken(void) {}

void commandToken::setNumericalData(const int64_t &number) {
  _num_data = number;
}

void commandToken::setTextData(const std::string &text) { _text_data = text; }

bool commandToken::matchTokenType(const tokenType &type) const {
  return (type == _type);
}

bool commandToken::hasNumeralData(void) const { return (_type == NUMBER); }

bool commandToken::hasTextData(void) const {
  return (_type == CHAR || _type == STRING || isACommandIdentifier() ||
          isAModeIdentifier());
}

bool commandToken::isACommandIdentifier(void) const {
  return (_type == KICK || _type == INVITE || _type == TOPIC || _type == MODE);
}

bool commandToken::isAModeIdentifier(void) const { return (_type == MODE_ID); }

const tokenType &commandToken::GetTokenType(void) const { return (_type); }

const int64_t &commandToken::GetNumericalData(void) const {
  return (_num_data);
}

const std::string &commandToken::GetTextData(void) const {
  return (_text_data);
}
