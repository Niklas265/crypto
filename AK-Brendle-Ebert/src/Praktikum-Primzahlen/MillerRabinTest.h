/*
 * MillerRabinTest.h
 */

#ifndef MILLERRABINTEST_H_
#define MILLERRABINTEST_H_

#include "integer.h"
#include "PRNG.h"

using namespace CryptoPP;

class MillerRabinTest {
public:

	Integer boundedExponentation(const Integer& b, const Integer& e,
			const Integer& bound) const;

	Integer modularExponentation(const Integer& a, const Integer& b,
			const Integer& n) const;

	bool witness(const Integer& a, const Integer& n) const;

	bool witness2(const Integer& a, const Integer& n) const;

	bool isPerfectPower(const Integer& n, Integer& b, Integer& e) const;

	Integer searchBase(const Integer& n, const Integer& e) const;

	bool extendedEuklid(const Integer& a, const Integer& b, Integer& d,
			Integer& x, Integer& y) const;

	bool isPrime(PRNG* rng, const Integer& n, unsigned int s) const;

};

#endif /* MILLERRABINTEST_H_ */
