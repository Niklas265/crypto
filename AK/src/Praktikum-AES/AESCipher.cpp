#include <cassert>
#include <iostream>
#include <iomanip>
#include "AESCipher.h"

using namespace std;

AESCipher::AESCipher(bool debug_mode) : debug_mode(debug_mode), aesmath(),
    key_schedule(aesmath, debug_mode), state(aesmath, debug_mode) {
}

bool AESCipher::encrypt(const vector<byte>& plain_text, vector<byte>& cipher_text) {
    /*
     * Aufgabe 24b
     */
  return false;
}

bool AESCipher::decrypt(const vector<byte>& plain_text, vector<byte>& cipher_text) {
    /*
     * Aufgabe 24c
     */
    return false;
}

bool AESCipher::process(const vector<byte>& in, vector<byte>& out, bool mode) {
    /*
     * Aufgabe 24a
     */
  return false;
}

bool AESCipher::setKey(const vector<byte>& key) {
    return key_schedule.setKey(key);
}

void AESCipher::debugMessage(size_t round, string msg) {
    if (debug_mode) {
        cout << "round[" << dec << setw(2) << setfill(' ') << round
            << "]." << msg << endl;
    }
}

void AESCipher::decryptBlock(const byte *cipher_text, byte *plain_text) {
    /*
     * Aufgabe 23
     */
}

void AESCipher::encryptBlock(const byte *plain_text, byte *cipher_text) {
    /*
     * Aufgabe 22
     */
}

vector<byte> AESCipher::toVector(const string& msg, size_t block_len) {
    assert((block_len>0) && (block_len<=32) );

    vector<byte> v;

    copy(msg.begin(), msg.end(), back_inserter(v));

    v.push_back(0);
    while (v.size() % block_len != 0) {
        v.push_back(1);
    }

    return v;
}

string AESCipher::toString(const vector<byte>& vec) {
    auto it = vec.begin();
    string r;

    while ((*it!=0) and (it!=vec.end())) {
        r.push_back(*it);
        it++;
    }
    return r;
}
