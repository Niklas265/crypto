/*
 * RabinEncryptor.cpp
 */

#include "RabinEncryptor.h"

RabinEncryptor::RabinEncryptor(const Integer& n, const Integer& padding) {
    this->n = n;
    this->padding = padding;
}

RabinEncryptor::~RabinEncryptor() {
}

// #compute()
bool RabinEncryptor::compute(const Integer& x, Integer& y) {
    if (x >= n) return false;
    y = a_exp_b_mod_c(x, 2, n);
    return true;
}

// #compute2()
bool RabinEncryptor::compute2(const Integer& x, Integer& y) {
    if (x >= n) return false;
    y = a_exp_b_mod_c(x, 2, n);
    return true;
}
