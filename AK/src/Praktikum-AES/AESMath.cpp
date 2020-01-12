#include <sstream>
#include <iostream>
#include <iomanip>
#include "AESMath.h"

using namespace std;

AESMath::AESMath() : exp_table(), log_table(256, 0), sbox(), inv_sbox(256, 0) {

    /*
     * Aufgabe 5a
     */

    byte a = 1; // erzeugende element
    byte b;

    exp_table.push_back(a);

    // 254 because 256-2
    for (int i = 1; i < 256; i++) {
        // b = g^i
        b = rpmul(a,3);
        exp_table.push_back(b);
        a = b;
    }

    /*
     * Aufgabe 5b
     */

    // inverse = log
    for (int i = 0; i < 256; i++) {
        log_table[exp_table[i]] = i;
    }


    /*
     * Aufgabe 9a
     */

    for(int i = 0; i < 256; i++) {
        sbox.push_back(atrans(inv(i)));
    }

    /*
     * Aufgabe 9b
     */

    for (int i = 0; i < 256; i++) {
        inv_sbox[sbox[i]] = i;
    }
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

    // 8x8 * 8x1 = 1x8

    // 1 1 1 0 1 0 1 0 = 234
    //x = 234;

    // 1 1 1 1 1 0 0 0 = 4 = 0 + 0 = 0
    // 0 1 1 1 1 1 0 0 = 3 = 1 + 1 = 0
    // 0 0 1 1 1 1 1 0 = 3 = 1 + 1 = 0
    // 0 0 0 1 1 1 1 1 = 2 = 0 + 0 = 0
    // 1 0 0 0 1 1 1 1 = 3 = 1 + 0 = 1
    // 1 1 0 0 0 1 1 1 = 3 = 1 + 0 = 1
    // 1 1 1 0 0 0 1 1 = 4 = 0 + 1 = 1
    // 1 1 1 1 0 0 0 1 = 3 = 1 + 1 = 0

    // i and AND and then parity

    vector<byte> v = { 248, 124, 62, 31, 143, 199, 227, 241 };
    vector<byte> v2 = { 0, 1, 1, 0, 0, 0, 1, 1 };
    byte ret = 0;

    for (int i = 7; i >= 0; i--) {
        if ( (parity(x & v[i]) + v2[i]) % 2 ) {
            //cout << i << endl;
            ret += 1 << (7-i);
        }
    }

    //cout << int(ret) << endl;

    //exit(0);

    return ret;
}

byte AESMath::exp(byte i) const {
    /*
     * Aufgabe 5c
     */
    return exp_table[i];
}

byte AESMath::inv(byte b) const {
    /*
     * Aufgabe 6
     */
    if (b == 0) return 0;
    // multiplikative inverse: g^i * ? = g^1
    // m-1-i
    // 1) aus b i bekommen. 2)  g^m-1-i berechnen
    return exp(256-1-log(b));
}

byte AESMath::log(byte b) const {
    /*
     * Aufgabe 5d
     */
  return log_table[b];
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
    if (a == 0 || b == 0) return 0;
    return exp( (log(a) + log(b)) % 255 );
}

byte AESMath::parity(byte b) {
    /*
     * Aufgabe 7
     */
    byte cnt = 0;
    for (int i = 0; i < 8; i++) {
        if ((b & (1 << i)) != 0) cnt++;
    }
    return cnt % 2;
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
    return sbox[b];
}

byte AESMath::invSBox(byte b) const {
    /*
     * Aufgabe 9d
     */
    return inv_sbox[b];
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
