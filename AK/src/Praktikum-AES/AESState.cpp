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

    // state[1] ist row 1, colum 0

    for (int col = 0; col < 4; col++) {
        byte keyWordAsByte[4];
        for (int j = 0; j < 4; j++) {
            keyWordAsByte[3-j] = (key[col] >> (8 * j));
        }
        for (int row = 0; row < 4; row++) {
            byte newB = getCell(row, col) ^ keyWordAsByte[row];
            setCell(row, col, newB);
        }
    }
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

    for (int i = 0; i < 16; i++) {
        state[i] = math->sBox(state[i]);
    }
}


void AESState::invSubBytes() {
    /*
     * Aufgabe 17b
     */
    for (int i = 0; i < 16; i++) {
        state[i] = math->invSBox(state[i]);
    }
}

void AESState::set(const byte* in) {
    /*
     * Aufgabe 14a
     */

    for (int i = 0; i < 16; i++) {
        state[i] = in[i];
    }
}


void AESState::mixColumns() {
    /*
     * Aufgabe 18a
     */

    byte m[4][4] = {{2,3,1,1},
                    {1,2,3,1},
                    {1,1,2,3},
                    {3,1,1,2}};

    for (int col = 0; col < 4; col++) {
        byte tmp[4] = {0,0,0,0};    // using tmp to not override values
        for (int row = 0; row < 4; row++) {
            byte cur = 0;
            for (int colMult = 0; colMult < 4; colMult++) {
                cur = math->add(cur, math->mul(m[row][colMult], getCell(colMult, col)) );
            }
            tmp[row] = cur;
        }
        for(int i = 0; i < 4; i++){
            setCell(i, col, tmp[i]);
        }
    }
}

void AESState::invMixColumns() {
    /*
     * Aufgabe 18b
     */

    byte m[4][4] = {{14,11,13,9},
                    {9,14,11,13},
                    {13,9,14,11},
                    {11,13,9,14}};

    for (int col = 0; col < 4; col++) {
        byte tmp[4] = {0,0,0,0};    // using tmp to not override values
        for (int row = 0; row < 4; row++) {
            byte cur = 0;
            for (int colMult = 0; colMult < 4; colMult++) {
                cur = math->add(cur, math->mul(m[row][colMult], getCell(colMult, col)) );
            }
            tmp[row] = cur;
        }
        for(int i = 0; i < 4; i++){
            setCell(i, col, tmp[i]);
        }
    }
}

void AESState::shiftRow(size_t row, size_t shift) {
    /*
     * Aufgabe 15
     */

    // expected columns = nb = 4, also auch 4 zeilen

    for (int i = 0; i < shift; i++) {
        byte tmp = getCell(row, 0);
        for (int j = 0; j < 3; j++) {
            setCell(row, j, getCell(row, j+1));
        }
        setCell(row, 3, tmp);
    }
}


void AESState::shiftRows() {
    /*
     * Aufgabe 16a
     */

    shiftRow(1, 1);
    shiftRow(2, 2);
    shiftRow(3, 3);
}


void AESState::invShiftRows() {
    /*
     * Aufgabe 16b
     */
    shiftRow(1, 3);
    shiftRow(2, 2);
    shiftRow(3, 1);
}
