//
// Created by Martin on 18.05.2015.
//

#include "Slot.hpp"
#include <cstring> // memcpy

bool Slot::isTid() const {
  return T != Slot::T_NO_TID_VALUE;
}

TID Slot::getTid() {
  TID tid(0, 0);
  std::memcpy(&tid, this, sizeof(Slot));
  return tid;
}

bool Slot::wasRedirect() {
  return S != 0;
}

void Slot::nullTS() {
  T = Slot::T_NO_TID_VALUE;
  S = 0;
}

void Slot::markAsFree() {
  O = 0;
  L = 0;
}

bool Slot::isFree() const {
  return O == 0 && L == 0;
}

bool Slot::isEmptyData() const {
  return O > 0 && L == 0;
}

void Slot::redirectTo(TID redirectTid) {
  std::memcpy(this, &redirectTid, sizeof(Slot));
}
