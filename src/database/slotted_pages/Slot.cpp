//
// Created by Martin on 18.05.2015.
//

#include "Slot.hpp"
#include <cstring> // memcpy

bool Slot::isTid() {
	return T == 0x11111111b;
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
	T = 0;
	S = 0;
}

void Slot::markAsFree() {
	O = 0;
	L = 0;
}

bool Slot::isFree() {
	return O == 0 && L == 0;
}

bool Slot::isEmptyData() {
	return O > 0 && L == 0;
}
