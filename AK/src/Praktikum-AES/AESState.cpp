#include <string>
#include "AESState.h"

#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

AESState::AESState(const AESMath& aesmath, bool debug_mode)
    : debug_mode(debug_mode), math((const AESMath *) &aesmath)
{
    for (size_t i=0; i<16; i++) {
        state[i] = 0;
    }
}

void AESState::addKey(const word* key) {
    /*
     * Aufgabe 19
     */
}

string AESState::format() const {
    ostringstream os;

    for (size_t i=0; i<16; i++) {
        os << setw(2) << setfill('0') << hex << (unsigned)state[i] << " ";
    }
    return os.str();
}


void AESState::get(byte* out) {
    /*
     * Aufgabe 14b
     */
    assert(out!=NULL);
    memcpy(out, state, 16);
}

byte AESState::getCell(size_t row, size_t col) {
    return state[row + 4*col];
}

void AESState::setCell(size_t row, size_t col, byte b) {
    state[row + 4*col] = b;
}


void AESState::subBytes() {
    /*
     * Aufgabe 17a
     */
}


void AESState::invSubBytes() {
    /*
     * Aufgabe 17b
     */
}

void AESState::set(const byte* in) {
    /*
     * Aufgabe 14a
     */
}


void AESState::mixColumns() {
    /*
     * Aufgabe 18a
     */
}

void AESState::invMixColumns() {
    /*
     * Aufgabe 18b
     */
}

void AESState::shiftRow(size_t row, size_t shift) {
    /*
     * Aufgabe 15
     */
}


void AESState::shiftRows() {
    /*
     * Aufgabe 16a
     */
}


void AESState::invShiftRows() {
    /*
     * Aufgabe 16b
     */
}
