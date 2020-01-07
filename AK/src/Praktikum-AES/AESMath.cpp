#include <sstream>
#include <iostream>
#include <iomanip>
#include "AESMath.h"

using namespace std;

AESMath::AESMath() : exp_table(), log_table(256, 0), sbox(), inv_sbox(256, 0) {

    /*
     * Aufgabe 5a
     */

    /*
     * Aufgabe 5b
     */

    /*
     * Aufgabe 9a
     */

    /*
     * Aufgabe 9b
     */

}

byte AESMath::add(byte a, byte b) {
    /*
     * Aufgabe 2
     */
    return a ^ b;
}

byte AESMath::atrans(byte x) {
    /*
     * Aufgabe 8
     */
    return 0;
}

byte AESMath::exp(byte i) const {
    /*
     * Aufgabe 5c
     */
    return 0;
}

byte AESMath::inv(byte b) const {
    /*
     * Aufgabe 6
     */
  return 0;
}

byte AESMath::log(byte b) const {
    /*
     * Aufgabe 5d
     */
  return 0; 
}

byte AESMath::rpmul(byte a, byte b) {
    /*
     * Aufgabe 4
     */
    byte p = 0;
    while (a > 0) {
        if (a % 2 == 1) {
            p = p ^ b;
        }
        b = xtime(b);
        a = a >> 1;
    }
    return p;
}

byte AESMath::mul(byte a, byte b) const {
    /*
     * Aufgabe 5e
     */
    return 0;
}

byte AESMath::parity(byte b) {
    /*
     * Aufgabe 7
     */
  return 0;
  }

void AESMath::printTable(const vector<byte> &table) {
    cout << "         +----------------------------------------";
    cout << "---------------------------------------+" << endl;
    cout << "         |                                       y";
    cout << "                                       |" << endl;
    cout << "         +----+----+----+----+----+----+----+----+";
    cout << "----+----+----+----+----+----+----+----+" << endl;
    cout << "        ";
    for (size_t i = 0; i < 16; i++) {
        cout << " | " << setw(2) << setfill(' ') << hex << i;
    }
    cout << " |" << endl;
    cout << "+---+----+----+----+----+----+----+----+----+----+";
    cout << "----+----+----+----+----+----+----+----+" << endl;
    for (size_t i = 0; i < 16; i++) {
        cout << "|   | " << setw(2) << setfill(' ') << hex << i;
        for (size_t j = 0; j < 16; j++) {
            cout << " | " << setw(2) << setfill('0') << (unsigned) table[(i << 4) + j];
        }
        cout << " |" << endl;
        if (i < 15) {
            if (i == 7) {
                cout << "| x ";
                cout << "+----+----+----+----+----+----+----+----+----+";
                cout << "----+----+----+----+----+----+----+----+" << endl;

            } else {
                cout << "|   ";
                cout << "+----+----+----+----+----+----+----+----+----+";
                cout << "----+----+----+----+----+----+----+----+" << endl;
            }
        } else {
            cout << "|---";
            cout << "+----+----+----+----+----+----+----+----+----+";
            cout << "----+----+----+----+----+----+----+----+" << endl;
        }
    }
}

byte AESMath::sBox(byte b) const {
    /*
     * Aufgabe 9c
     */
    return 0;
}

byte AESMath::invSBox(byte b) const {
    /*
     * Aufgabe 9d
     */
    return 0;
}

byte AESMath::xtime(byte b) {
    /*
     * Aufgabe 3
     */
    byte r = b << 1;
    if ((b & 0x80) == 0x80) {
        r ^= 0x1b;
    }
    return r;
}

string AESMath::format(byte b) {
    ostringstream os;
    os << "{" << hex << setfill('0') << setw(2) << (unsigned)b <<"}";
    return os.str();
}
