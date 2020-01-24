/*
 * RSAAttack.cpp
 */

#include <vector>
#include "PublicKeyAlgorithmBox.h"
#include "RSAAttack.h"

#include <iostream>
using namespace std;

RSAAttack::RSAAttack() {
}

RSAAttack::~RSAAttack() {
}

// #factorizeN()
bool RSAAttack::factorizeN(const Integer& n, const Integer& phi_n, Integer& p,
		Integer& q) const {
    // Für die Attacke ist n und ϕ(n) von n bekannt ist.
    // (1) n = p * q und (2) ϕ(n) = (p-1) * (q-1).
    // Mit p und q kann RSA gebrochen werden, also es können verschlüsselte
    // Nachrichten entschlüsselt werden.
    // Indem man Gleichung (1) in Gleichung (2) mit p = n / q einsetzt:
    // ϕ(n) = (p-1) * (n/q - 1)
    // Diese Gleichung kann in eine Form gebracht werden, um p und q mit der
    // Mitternachtsformel zu berechnen:
    // 0 = p² + (ϕ(n) − n − 1)p + n
    // Bei der Mitternachtsformel ist a = 1, b = ϕ(n) - n - 1 und c = n

    PublicKeyAlgorithmBox pb;

    // Berechnen von b für die Mitternachtsformel
    Integer b = phi_n - n - 1;

    // Berechnen der Wurzel der Mitternachtsformel
    Integer s;
    pb.sqrt(b * b - 4 * n, s);

    // Berechnen der Mitternachtsformel für p und q
    Integer pTemp = (-b + s) / 2;
    Integer qTemp = (-b - s) / 2;

    // Überprüfen, ob die Faktorisierung Erfolgreich war.
    // Das ist zum Beispiel nicht der Fall, falls das ϕ(n) falsch ist.
    if (pTemp * qTemp == n) {
        // Bei Erfolg werden die beiden Faktoren von n in p und q gespeichert.
        p = pTemp;
        q = qTemp;
        return true;
    }

    return false;
}

// #wienerAttack()
bool RSAAttack::wienerAttack(const Integer& n, const Integer& e, Integer& p,
		Integer& q) const {

    PublicKeyAlgorithmBox pb;

    vector<Integer> c, d;
    pb.computeConvergents(e, n, c, d);

    for (int i = 2; i < c.size(); i++) {
        if ((e * d[i] - 1) % c[i] != 0) continue;
        Integer phiN = (e * d[i] - 1) / c[i];
        Integer pTemp, qTemp;
        if (factorizeN(n, phiN, pTemp, qTemp)) {
            p = pTemp;
            q = qTemp;
            return true;
        }
    }

    return false;
}

// #halfAttack()
Integer RSAAttack::halfAttack(const Integer& n, const Integer& e, const Integer& y,
		RSAOracle& rsa_oracle) const {

    PublicKeyAlgorithmBox pb;
    vector<bool> h;

    int l = n.BitCount();
    Integer y2 = y;
    for (int i = 0; i < l; i++) {
        h.push_back(rsa_oracle.half(y2));
        y2 = (y2 * pb.modularExponentation(2,e,n)) % n;
    }

    Integer lo = 0;
    Integer hi = n;
    for (int i = 0; i < l; i++) hi *= 10;

    for (int i = 0; i < l; i++) {
        Integer mid = (hi + lo) / 2;
        if (h[i]) {
            lo = mid;
        } else {
            hi = mid;
        }
    }

    for (int i = 0; i < l; i++) hi /= 10;

    return hi;
}
