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

    // Das Rabin-System ist anfällig gegen eine Attacke mit frei
    // wählbarem Geheimtext.

    // Nach max_tries Versuchen wird die Attacke abgebrochen.
    // In jedem Schleifendurchlauf wird einmal Versucht einen Faktor von n zu
    // berechnen.
    for (int i = 1; i <= max_tries; i++) {
        PublicKeyAlgorithmBox pb;
        // Berechne y = r² mod n für ein zufälliges r in {1,2,..., n-1)
        Integer r = pb.randomInteger(n-2) + 1;
        Integer y = pb.modularExponentation(r, 2, n);
        Integer x;
        // y wird entschlüsselt und in x gespeichert.
        // Bei der Entschlüsselung wird x auf eines der 4 Möglichkeiten für
        // √y mod n gesetzt.
        rabin_dec.compute(y, x);
        // Unter Einsatz des Chinesischen Restsatzes kann man 4 Fälle
        // unterscheiden:
        // (1) x ≡ r (mod p) und x ≡ r (mod q)
        // (2) x ≡ −r (mod p) und x ≡ −r (mod q)
        // (3) x ≡ r (mod p) und x ≡ −r (mod q)
        // (4) x ≡ −r (mod p) und x ≡ r (mod q)
        // Die Fälle (1) und (2) sind nicht verwertbar und y eignet sich
        // nicht für die Attacke.
        if (x == r || x-n == -r) continue;
        // Anhand von Fall (3) oder (4) kann man n faktorisieren.
        // TODO: da noch erklären warum das funktioniert
        f = Integer::Gcd(x-r, n);
        // Falls die Attacke erfolgreich war, dann liefert die Methode die
        // Anzahl der Versuche zurück
        return i;
    }

    // War die Attacke nicht erfolgreich, dann ist der Rückgabewert gleich −1.
    return -1;
}
