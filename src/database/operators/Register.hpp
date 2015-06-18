//
// Created by daniel on 6/9/15.
//

#ifndef PROJECT_REGISTER_H
#define PROJECT_REGISTER_H


#include <stdint.h>
#include <functional>
#include <stddef.h>
#include <string>
#include "../relations/Types.hpp"

//The register class can be used to store and retrieve values of any type
class Register {

private:
  std::string stringValue;
  Integer intValue;
public:
  enum TYPE {
    UNDEFINED, INTEGER, STRING
  } type;

  Register() : type(Register::TYPE::UNDEFINED) { };

  Register(Integer val) : intValue(val), type(Register::TYPE::INTEGER) { };

  Register(std::string val) : stringValue(val), type(Register::TYPE::STRING) { };

  inline bool operator<(const Register &val) const {
    switch (type) {
      case INTEGER:
        return intValue < val.getIntegerValue();
      case STRING:
        return stringValue < val.getStringValue();
      case UNDEFINED:
        return false;
      default:
        throw std::invalid_argument("invalid state");
    }
  }

  inline bool operator==(const Register &val) const {
    switch (type) {
      case INTEGER:
        return intValue == val.getIntegerValue();
      case STRING:
        return stringValue == val.getStringValue();
      case UNDEFINED:
        return false;
      default:
        throw std::invalid_argument("invalid state");
    }
  }

  inline void setStringValue(std::string val) {
    type = TYPE::STRING;
    stringValue = val;
  }

  inline void setIntegerValue(Integer val) {
    type = TYPE::INTEGER;
    intValue = val;
  }

  inline std::string getStringValue() const {
    return stringValue;
  }

  inline Integer getIntegerValue() const {
    return intValue;
  }

  inline size_t hashValue() const {
    switch (type) {
      case INTEGER:
        return std::hash<Integer>()(intValue);
      case STRING:
        return std::hash<std::string>()(stringValue);
      case UNDEFINED:
        return 0;
      default:
        throw std::invalid_argument("invalid state");
    }
  };
};

namespace std {
  template<>
  struct hash<Register> {
    size_t operator()(const Register &reg) const {
      return reg.hashValue();
    }
  };
}

#endif //PROJECT_REGISTER_H
