#include "PRNG.h"
#include <cassert>

byte PRNG::getByte() {
    /*************************************************************************
     * Aufgabe 6d.
     *************************************************************************/

    byte ret = 0;

    for (int i = 0; i < 8; i++) {
        ret = ret << 1;
        ret += getBit();
    }

    return ret;
}

void PRNG::getBlock(byte *buffer, unsigned int n) {
    /*************************************************************************
     * Aufgabe 6e.
     *************************************************************************/

    for(int i = 0; i < n; i++) {
        buffer[i] = getByte();
    }
}

Integer PRNG::getInteger(unsigned int size) {
    /*************************************************************************
     * Aufgabe 6g.
     *************************************************************************/

    byte arr[size];
    getBlock(arr, size);

    return Integer(arr, size);
}

Integer PRNG::getInteger(const Integer &m) {
    /*************************************************************************
     * Aufgabe 6i.
     *************************************************************************/

    // Der Parameter m muss laut Aufgabenstellung eine ganze Zahl > 0 sein.
    assert(m > 0);

    Integer ret;

    do {
        ret = getInteger(m.ByteCount());
    } while (ret >= m);

    return ret;
}
