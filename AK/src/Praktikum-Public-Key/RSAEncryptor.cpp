/*
 * RSAEncryptor.cpp
 */

#include "PublicKeyAlgorithmBox.h"
#include "RSAEncryptor.h"

RSAEncryptor::RSAEncryptor(const Integer& n, const Integer& e) {
    this->n = n;
    this->e = e;
}

RSAEncryptor::~RSAEncryptor() {}

bool RSAEncryptor::compute(const Integer& x, Integer& y) const {

    PublicKeyAlgorithmBox pb;
    y = pb.modularExponentation(x, e, n);

    return x < n;
}
