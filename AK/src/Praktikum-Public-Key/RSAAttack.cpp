/*
 * RSAAttack.cpp
 */

#include <vector>
#include "PublicKeyAlgorithmBox.h"
#include "RSAAttack.h"

using namespace std;

RSAAttack::RSAAttack() {
}

RSAAttack::~RSAAttack() {
}

// #factorizeN()
bool RSAAttack::factorizeN(const Integer& n, const Integer& phi_n, Integer& p,
		Integer& q) const {
  return false;
}

// #wienerAttack()
bool RSAAttack::wienerAttack(const Integer& n, const Integer& e, Integer& p,
		Integer& q) const {
  return false;
}

// #halfAttack()
Integer RSAAttack::halfAttack(const Integer& n, const Integer& e, const Integer& y,
		RSAOracle& rsa_oracle) const {
  return Integer("1");
}
