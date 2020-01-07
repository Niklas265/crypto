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
    return 0;
}

bool AESKeySchedule::setKey(const vector<byte>& key) {
    /*
     * Aufgabe 13
     */
    return false;
}

word AESKeySchedule::subWord(word w) const {
    /*
     * Aufgabe 12
     */
    return 0;
}
