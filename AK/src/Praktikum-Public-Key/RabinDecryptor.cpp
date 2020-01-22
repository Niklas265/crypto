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
    offset = 1;
    while (offset <= padding) {
        offset *= 10;
    }
}

// #compute()
bool RabinDecryptor::compute(const Integer& y, vector<Integer>& xv) {

    // cant check anymore because y is padded
    //if (y >= p || y >= q) return false;

    Integer n = p * q;
    vector<Integer> vp, vq;
    PublicKeyAlgorithmBox pb;
    pb.modPrimeSqrt(y, p, vp);
    pb.modPrimeSqrt(y, q, vq);

    Integer d, mq, mp;
    pb.EEA(p, q, d, mq, mp);
    if (d != 1) return false;
    if((mp * q) % p != 1) return false;
    if((mq * p) % q != 1) return false;

    xv.push_back((vp[0] * q * mp + vq[0] * p * mq) % n);
    xv.push_back((vp[0] * q * mp + vq[1] * p * mq) % n);
    xv.push_back((vp[1] * q * mp + vq[0] * p * mq) % n);
    xv.push_back((vp[1] * q * mp + vq[1] * p * mq) % n);

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
    vector<Integer> xv;
    compute(y, xv);

    for (auto xPadded : xv) {
        if (xPadded % offset == padding) {
            x = xPadded / offset;
            return true;
        }
    }

    return false;
}

RabinDecryptor::~RabinDecryptor() {
}

