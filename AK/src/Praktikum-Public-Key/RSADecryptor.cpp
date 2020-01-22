/*
 * RSADecryptor.cpp
 */

#include "PublicKeyAlgorithmBox.h"
#include "RSADecryptor.h"

#include <iostream>

using namespace std;

RSADecryptor::RSADecryptor(const Integer& p, const Integer& q,
		const Integer& d) {
    this->p = p;
    this->q = q;
    this->d = d;
    n = p * q;
}

RSADecryptor::~RSADecryptor() {
}

// #compute()
bool RSADecryptor::compute(const Integer& y, Integer& x) const {
    PublicKeyAlgorithmBox pb;

    x = pb.modularExponentation(y, d, n);

    return y < n;
}

// #crt()
bool RSADecryptor::crt(const Integer& y, Integer& x) const {

    //TODO: die fehlt noch. chinesische restsatz irgendwie verwenden

    return y < n;
}

// #garner()
bool RSADecryptor::garner(const Integer& y, Integer& x) const {
    PublicKeyAlgorithmBox pb;

    Integer a = pb.modularExponentation(y, d, p);
    Integer b = pb.modularExponentation(y, d, q);

    Integer qInv;
    pb.multInverse(q, p, qInv);

    // first mod p probably not needed because multInv only returns positives
    x = (((a - b) * (qInv % p)) % p) * q + b;

    return y < n;
}
