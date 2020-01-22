/*
 * RSAOracle.cpp
 */

#include <cassert>
#include <iostream>
#include "RSAOracle.h"
#include "PublicKeyAlgorithmBox.h"

using namespace std;

RSAOracle::RSAOracle(Integer& p, Integer& q, Integer& d) {
    n = p * q;
    this->d = d;
}

RSAOracle::~RSAOracle() {
}

// #half()
bool RSAOracle::half(const Integer& y) const {

    PublicKeyAlgorithmBox pb;

    Integer x = pb.modularExponentation(y, d, n);

    return x > n / 2;
}

// #parity()
bool RSAOracle::parity(const Integer& y) const {
  return false;
}
