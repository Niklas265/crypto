/*
 * RSAOracle.cpp
 */

#include <cassert>
#include <iostream>
#include "RSAOracle.h"
#include "PublicKeyAlgorithmBox.h"

using namespace std;

RSAOracle::RSAOracle(Integer& p, Integer& q, Integer& d) {
}

RSAOracle::~RSAOracle() {
}

// #half()
bool RSAOracle::half(const Integer& y) const {
  return false;
}

// #parity()
bool RSAOracle::parity(const Integer& y) const {
  return false;
}
