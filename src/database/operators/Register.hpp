//
// Created by daniel on 6/9/15.
//

#ifndef PROJECT_REGISTER_H
#define PROJECT_REGISTER_H


#include <stdint.h>
#include <functional>
#include <stddef.h>

//The register class can be used to store and retrieve values of any type
template<typename Value>
class Register {

private:
  Value value;
public:

  Register(Value val) : value(val) { };

  bool operator<(Register<Value> val);

  bool operator==(Register<Value> val);

  void setValue(Value val);

  Value getValue();

  size_t hashValue() const;

};

namespace std {
    template<typename Value>
    struct hash<Register<Value>> {
      size_t operator()(const Register<Value> &reg) const {
        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:

        return reg.hashValue();
      }
    };

}

#include "ImplRegister.cpp"
#include "IntRegister.cpp"

#endif //PROJECT_REGISTER_H
