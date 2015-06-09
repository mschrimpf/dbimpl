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

  inline bool operator<(Register val) {
    switch (currentType) {
      case INTEGER:
        return intValue < val.getIntegerValue();
      case STRING:
        return stringValue < val.getStringValue();
    }
    return false;
  }

  inline bool operator==(Register val) {
    switch (currentType) {
      case INTEGER:
        return intValue == val.getIntegerValue();
      case STRING:
        return stringValue == val.getStringValue();
    }
    return false;
  }

  inline void setStringValue(const char *val) {
    stringValue = val;
  }

  inline void setIntegerValue(uint64_t val) {
    intValue = val;
  }

  inline const char *getStringValue() {
    return stringValue;
  }

  inline uint64_t getIntegerValue() {
    return intValue;
  }

  inline size_t hashValue() const {
    switch (currentType) {
      case INTEGER:
        break;
      case STRING:
        break;
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
