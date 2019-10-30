#include "BlumBlumShubGenerator.h"

BlumBlumShubGenerator::BlumBlumShubGenerator(const Integer &n, const Integer &seed) {
    this->n = n;
    this->s = (seed * seed) % n;
}

bool BlumBlumShubGenerator::getBit() {
    /*************************************************************************
     * Aufgabe 6b.
     *************************************************************************/

    s = (s * s) % n;

    return s % 2;
}

