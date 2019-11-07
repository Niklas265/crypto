#include "BlumBlumShubGenerator.h"
#include <cassert>

BlumBlumShubGenerator::BlumBlumShubGenerator(const Integer &n, const Integer &seed) {
    assert(n != 0 && "Error: Modulo 0");
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

