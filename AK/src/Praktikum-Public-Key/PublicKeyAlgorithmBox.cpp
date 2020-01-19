#include <iostream>
#include "nbtheory.h"
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace CryptoPP;
using namespace std;

// #EEA()
bool PublicKeyAlgorithmBox::EEA(const Integer& a, const Integer& b,
		Integer& d, Integer& x, Integer& y) {



    return false;
} // EEA()

// #modularExponentation()
Integer PublicKeyAlgorithmBox::modularExponentation(const Integer& a,
		const Integer& b, const Integer& n) {
  return Integer("1");
} // modularExponentation()

// #multInverse()
bool PublicKeyAlgorithmBox::multInverse(const Integer& a, const Integer& n,
		Integer& a_inv) {
  return false;
} // multInverse()

// #witness()
bool PublicKeyAlgorithmBox::witness(const Integer& a, const Integer& n) {
  return false;
} // witness()

// randomInteger()
Integer PublicKeyAlgorithmBox::randomInteger(const Integer& n) {
  return Integer("1");
}

// #millerRabinTest()
bool PublicKeyAlgorithmBox::millerRabinTest(Integer& n, unsigned int s) {
	return true;
} // millerRabinTest()

// #randomPrime()
unsigned int PublicKeyAlgorithmBox::randomPrime(Integer &p,
		unsigned int bitlen, unsigned int s) {
	return 0;
} // randomPrime()

// #randomPrime()
unsigned int PublicKeyAlgorithmBox::randomRabinPrime(Integer &p,
		unsigned int bitlen, unsigned int s) {
  return 0;
} // randomRabinPrime()

// #modPrimeSqrt()
bool PublicKeyAlgorithmBox::modPrimeSqrt(const Integer& y, const Integer& p,
		vector<Integer>& v) {
  return false;
}

Integer PublicKeyAlgorithmBox::euklid(const Integer& a, const Integer& b,
		vector<Integer>& q) {
  return Integer("1");
}

unsigned int PublicKeyAlgorithmBox::computeConvergents(const Integer& a,
		const Integer& b, vector<Integer>& c, vector<Integer>& d) {
  return 1;
}

// #sqrt()
bool PublicKeyAlgorithmBox::sqrt(const Integer& x, Integer& s) const {
  return false;
}

void PublicKeyAlgorithmBox::generateRSAParams(Integer& p, Integer& q,
		Integer& e, Integer& d, unsigned int bitlen, unsigned int s) {
}

