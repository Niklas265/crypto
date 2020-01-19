/*
 * RabinDecryptor.cpp
 */

#include <vector>
#include "RabinDecryptor.h"
#include "PublicKeyAlgorithmBox.h"

RabinDecryptor::RabinDecryptor(const Integer& p, const Integer& q,
		const Integer& padding) {
}

// #compute()
bool RabinDecryptor::compute(const Integer& y, vector<Integer>& xv) {
  return false;
}

bool RabinDecryptor::compute(const Integer& y, Integer& x) {
  return false;
}

// #compute2()
bool RabinDecryptor::compute2(const Integer& y, Integer& x) {
  return false;
}

RabinDecryptor::~RabinDecryptor() {
}

