//
// Created by daniel on 6/9/15.
//

#ifndef PROJECT_REGISTER_H
#define PROJECT_REGISTER_H


#include <stdint.h>
#include <functional>
#include <stddef.h>

//The register class can be used to store and retrieve values of any type
class Register {

private:
  const char *stringValue;
  uint64_t intValue;
public:
  enum type {
    UNDEFINED, INTEGER, STRING
  };
  type currentType;

  Register() : currentType(Register::type::UNDEFINED) { };

  Register(uint64_t val) : intValue(val), currentType(Register::type::INTEGER) { };

  Register(const char *val) : stringValue(val), currentType(Register::type::STRING) { };

  inline bool operator<(const Register &val) const {
    switch (currentType) {
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
    switch (currentType) {
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

  inline void setStringValue(const char *val) {
    stringValue = val;
  }

  inline void setIntegerValue(uint64_t val) {
    intValue = val;
  }

  inline const char *getStringValue() const {
    return stringValue;
  }

  inline uint64_t getIntegerValue() const {
    return intValue;
  }

  inline size_t hashValue() const {
    throw;
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
