//
// Created by daniel on 6/9/15.
//

#include "PrintOperator.hpp"

template<typename Type>
void PrintOperator<Type>::open() {

}

template<typename Type>
std::string PrintOperator<Type>::toString(Type val) {
  return std::to_string(val);
}

template<typename Type>
bool PrintOperator<Type>::next() {

};

template<typename Type>
void PrintOperator<Type>::close() {

};

template<typename Type>
std::vector<Register < Type> *>

PrintOperator<Type>::getOutput() {

};
