/*
 * MillerRabinTest.cpp
 */
#include <iostream>
#include <cassert>
#include "MillerRabinTest.h"

using namespace std;

Integer MillerRabinTest::boundedExponentation(const Integer& b, const Integer& e, const Integer& bound) const {
    Integer d("1");

    for (int i = e.BitCount()-1; i >= 0; i--) {
        d = (d * d);
        if (d > bound) return bound + 1;
        if (e.GetBit(i) == 1) {
            d = (d * b);
        }
        if (d > bound) return bound + 1;
    }

    return d;
}

bool MillerRabinTest::isPerfectPower(const Integer& n, Integer& b, Integer& e) const {
    bool found = false;
    Integer curE = 2;
    Integer curB;
    while (curE <= n.BitCount() + 1 && !found) {
        curB = searchBase(n, curE);
        if (curB > 0) {
            found = true;
            break;
        } else {
            curE += 1;
        }
    }
    if (found) {
        b = curB;
        e = curE;
        return true;
    }
    return false;
}

Integer MillerRabinTest::modularExponentation
(
 const Integer& a,
 const Integer& b,
 const Integer& n
) const
{
    Integer d("1");

    for (int i = b.BitCount()-1; i >= 0; i--) {
        d = (d * d) % n;
        if (b.GetBit(i) == 1) {
            d = (d * a) % n;
        }
    }

    return d;
}

bool MillerRabinTest::witness(const Integer& a, const Integer& n) const {
    Integer nm = n-1;
    Integer r;
    Integer u;
    // 2^r * u

    for (int i = 0; i < nm.BitCount(); i++) {
        if (!nm.GetBit(i)) continue;
        Integer ipow2 = Integer::Power2(i);

        Integer rest = nm / ipow2;
        if (nm % ipow2 == 0 && rest % 2 == 1) {
            r = i;
            u = rest;
            break;
        }
    }

    Integer d = modularExponentation(a, u, n);

    //cout << "r " << r << "u " << u << endl;

    for (int i = 0; i < r; i++) {
        Integer x = d;
        d = (d * d) % n;
        if (d == 1 && x != 1 && x != n-1) {
            return true;
        }
    }

    return d != 1;
}


bool MillerRabinTest::isPrime(PRNG* rng, const Integer& n, unsigned int s) const {
    if (n % 2 == 0) {
        return false;
    }

    for (int i = 0; i < s; i++) {
        Integer a = rng->getInteger(n-1) + 1;
        
        Integer left = a;
        Integer right = n;
        Integer tmp;
        do {
            tmp = left % right;
            left = right;
            right = tmp;
        } while (right != 0);

        if (left > 1) {
            return false;
        }
        if (witness(a, n)) {
            return false;
        }
    }
    return true;
}


Integer MillerRabinTest::searchBase(const Integer& n, const Integer& e) const {
    Integer left = 2;
    Integer right = n;
    Integer x;
    Integer b;

    do {
        b = left + (right-left) / 2;
        x = boundedExponentation(b, e, n);
        if (x > n) {
            right = b;
        } else {
            left = b;
        }
    } while (x != n && (right-left) / 2 > 0);

    if (x == n) {
        return b;
    }
    return 0;
}
