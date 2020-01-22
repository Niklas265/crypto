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

    for (int i = 1; i <= max_tries; i++) {
        PublicKeyAlgorithmBox pb;
        Integer r = pb.randomInteger(n-2) + 1;
        Integer y = pb.modularExponentation(r, 2, n);
        Integer x;
        rabin_dec.compute(y, x);
        if (x == r || x-n == -r) continue;
        f = Integer::Gcd(x-r, n);
        return i;
    }

    return -1;
}
