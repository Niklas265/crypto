/*
 * RabinDecryptor.cpp
 */

#include <vector>
#include "RabinDecryptor.h"
#include "PublicKeyAlgorithmBox.h"

RabinDecryptor::RabinDecryptor(const Integer& p, const Integer& q,
		const Integer& padding) {
    this->p = p;
    this->q = q;
    this->padding = padding;
}

// #compute()
bool RabinDecryptor::compute(const Integer& y, vector<Integer>& xv) {

    if (y >= p || y >= q) return false;

    vector<Integer> v;
    PublicKeyAlgorithmBox pb;
    pb.modPrimeSqrt(y, p, v);
    xv.push_back(v[0]);
    xv.push_back(v[1]);
    // v cleared by modPrimeSqrt
    pb.modPrimeSqrt(y, q, v);
    xv.push_back(v[0]);
    xv.push_back(v[1]);

    return true;
}

bool RabinDecryptor::compute(const Integer& y, Integer& x) {
    vector<Integer> xv;
    if (compute(y, xv)) {
        x = xv[0];
        return true;
    }
    return false;
}

// #compute2()
bool RabinDecryptor::compute2(const Integer& y, Integer& x) {
  return false;
}

RabinDecryptor::~RabinDecryptor() {
}

