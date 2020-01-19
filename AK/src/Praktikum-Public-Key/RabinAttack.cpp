/*
 * RabinAttack.cpp
 */

#include "PublicKeyAlgorithmBox.h"
#include "RabinDecryptor.h"
#include "RabinAttack.h"
#include <iostream>

using namespace std;

RabinAttack::RabinAttack() {
}

RabinAttack::~RabinAttack() {
}

int RabinAttack::factorize(const Integer& n, Integer& f, int max_tries,
		RabinDecryptor& rabin_dec) {
  return 0;
}
