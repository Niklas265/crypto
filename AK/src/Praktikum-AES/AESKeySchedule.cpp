#include <iostream>
#include <iomanip>
#include <sstream>
#include "AESKeySchedule.h"

using namespace std;

AESKeySchedule::AESKeySchedule(const AESMath &aesmath, bool debug_mode)
        : debug_mode(debug_mode), math((const AESMath *) &aesmath), key_schedule(), nk(0),
          nb(4), nr(0), r_con(11,0) {

    /*
     * Aufgabe 10
     */

    r_con[0] = 0;

    // RC[i] = x (i.e. ‘02’) •(RC[i-1]) = x^(i-1)
    word a = 1;
    for (int i = 1; i < 11; i++) {
        r_con[i] = a << 24;
        a = aesmath.mul(a, 2);
    }
}

AESKeySchedule::~AESKeySchedule() {}

size_t AESKeySchedule::getNrOfRounds() const {
    return nr;
}

const word* AESKeySchedule::getRoundKey(size_t round) {
    assert(round<=nr);
    return &key_schedule[4*round];
}

string AESKeySchedule::formatRoundKey(size_t round) {
    assert(round<=nr);
    ostringstream os;

    for (size_t i=0; i<nb; i++) {
        os << hex << setw(8) << setfill('0') << key_schedule[4*round+i];
    }
    return os.str();
}

word AESKeySchedule::rotWord(word w) const {
    /*
     * Aufgabe 11
     */

    return (w << 8) + (w >> 24);
}

bool AESKeySchedule::setKey(const vector<byte>& key) {
    /*
     * Aufgabe 13
     */

    nk = key.size() * 8 / 32;   // Nk = Number of 32-bit words comprising the Cipher Key

    // KA-04 Folie 29
    assert(nb == 4);
    if (nk == 4) nr = 10;
    else if (nk == 6) nr = 12;
    else if (nk == 8) nr = 14;
    else return false; // Schlüssellänge != 128, 192, 256

    for (int i = 0; i < nk; i++) {
        word w = 0;
        for (int j = 0; j < 4; j++) {
            w <<= 8;
            w += key[4*i+j];
        }
        key_schedule.push_back(w);
    }

    if(debug_mode) {
        cout << " i|" << setw(15) << setfill(' ')
             << "temp|" << setw(15) << setfill(' ')
             << "rotWord|" << setw(15) << setfill(' ')
             << "subWord|" << setw(15) << setfill(' ')
             << "Rcon[i/Nk]|" << setw(15) << setfill(' ')
             << "Xor with Rcon|" << setw(15) << setfill(' ')
             << "w[i-Nk]|" << setw(15) << setfill(' ')
             << "w[i]" << endl;
    }

    // TODO: Delete, version before supporting debug output
    /*for (int i = nk; i < nb * (nr+1); i++) {
        word temp = key_schedule[i-1];
        if (i % nk == 0) {
            temp = subWord(rotWord(temp)) ^ r_con[i / nk];
        } else if (nk > 6 && i % nk == 4) {
            temp = subWord(temp);
        }
        key_schedule.push_back(key_schedule[i-nk] ^ temp);
        cout << dec << i << ' ' << hex << (key_schedule[i-nk] ^ temp) << endl;
    }*/

    for (int i = nk; i < nb * (nr+1); i++) {
        word temp, firsttemp, afterRotWord, afterSubWord, afterRconiDivnk,
             afterXORwithRcon, afterwIminusNk, aftertempXORwIminusNk;
        firsttemp = key_schedule[i-1];
        temp = firsttemp;
        if (i % nk == 0) {
            afterRotWord = rotWord(temp);
            afterSubWord = subWord(afterRotWord);
            afterRconiDivnk = r_con[i / nk];
            afterXORwithRcon = afterSubWord ^ afterRconiDivnk;
            temp = afterXORwithRcon;
        } else if (nk > 6 && i % nk == 4) {
            afterSubWord = subWord(temp);
            temp = afterSubWord;
        }
        afterwIminusNk = key_schedule[i-nk];
        aftertempXORwIminusNk = afterwIminusNk ^ temp;
        key_schedule.push_back(aftertempXORwIminusNk);

        if(debug_mode) {
            cout << dec << setw(2) << setfill(' ') << i << "|      ";
            cout << hex << setw(8) << setfill('0') << firsttemp << "|      ";
            if (i % nk == 0) {
                cout << hex << setw(8) << setfill('0') << afterRotWord << "|      ";
                cout << hex << setw(8) << setfill('0') << afterSubWord << "|      ";
                cout << hex << setw(8) << setfill('0') << afterRconiDivnk << "|      ";
                cout << hex << setw(8) << setfill('0') << afterXORwithRcon << "|      ";
            } else if (nk > 6 && i % nk == 4) {
                cout << string(15, ' ') << hex << setw(8) << setfill('0') << afterSubWord
                     << string(30, ' ') << "|      ";
            } else {
                cout << string(60, ' ');
            }
            cout << hex << setw(8) << setfill('0') << afterwIminusNk << "|      ";
            cout << hex << setw(8) << setfill('0') << aftertempXORwIminusNk << endl;
        }

    }

    return true;
}

word AESKeySchedule::subWord(word w) const {
    /*
     * Aufgabe 12
     */
    word ret = 0;

    for (int i = 3; i >= 0; i--) {
        ret <<= 8;
        ret += math->sBox(w >> i*8);
    }

    return ret;
}
