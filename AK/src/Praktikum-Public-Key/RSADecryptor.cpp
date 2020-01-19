/*
 * RSADecryptor.cpp
 */

#include "PublicKeyAlgorithmBox.h"
#include "RSADecryptor.h"

#include <iostream>

using namespace std;

RSADecryptor::RSADecryptor(const Integer& p, const Integer& q,
		const Integer& d) {
}

RSADecryptor::~RSADecryptor() {
}

// #compute()
bool RSADecryptor::compute(const Integer& y, Integer& x) const {
  return false;
}

// #crt()
bool RSADecryptor::crt(const Integer& y, Integer& x) const {
  return false;
}

// #garner()
bool RSADecryptor::garner(const Integer& y, Integer& x) const {
  return false;
}
