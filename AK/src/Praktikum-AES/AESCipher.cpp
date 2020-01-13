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

    // AES ist eine Blockchiffre mit Blocklänge 16 Bytes. Der zu verschlüsselnde
    // Klartext muss eine größe eines vielfachen von 16 haben. Ansonsten wird
    // false zurückgegeben, was einen Fehler signalisiert.
    if (plain_text.size() % 16 != 0) {
        return false;
    }

    // Der vector ist im grunde ein Array. Dieses Array wird auf die größe de
    // plain_textes gesetzt, damit der verschlüsselte Klartext dort gespeichert
    // werden kann.
    cipher_text.resize(plain_text.size());

    // In jedem Schleifendurchlauf wird ein Block (16 Bytes) verschlüsselt.
    for(int i = 0; i < plain_text.size(); i+= 16) {
        // an encryptBlock werden der Reihe nach 16 Bytes des plain_texts
        // übergeben. Auch die Position, an dem die 16 verschlüsselten Bytes
        // geschrieben werden sollen, werden übergeben.
        encryptBlock(&plain_text[i], &cipher_text[i]);
    }

    return true;
}

bool AESCipher::decrypt(const vector<byte>& cipher_text, vector<byte>& plain_text) {
    /*
     * Aufgabe 24c
     */

    // AES ist eine Blockchiffre mit Blocklänge 16 Bytes. Der zu entschlüsselnde
    // Ciphertext muss eine größe eines vielfachen von 16 haben. Ansonsten wird
    // false zurückgegeben, was einen Fehler signalisiert.
    if (cipher_text.size() % 16 != 0) {
        return false;
    }

    // Der vector ist im grunde ein Array. Dieses Array wird auf die größe de
    // cipher_texts gesetzt, damit der Klartext dort gespeichert
    // werden kann.
    plain_text.resize(cipher_text.size());

    // In jedem Schleifendurchlauf wird ein Block (16 Bytes) verschlüsselt.
    for(int i = 0; i < cipher_text.size(); i+= 16) {
        // an decryptBlock werden der Reihe nach 16 Bytes des cipher_text
        // übergeben. Auch die Position, an dem die 16 entschlüsselten Bytes
        // geschrieben werden sollen, werden übergeben.
        decryptBlock(&cipher_text[i], &plain_text[i]);
    }

    return true;
}

// TODO

bool AESCipher::process(const vector<byte>& in, vector<byte>& out, bool mode) {
    /*
     * Aufgabe 24a
     */

    if (mode == Encryption) {
        return encrypt(in, out);
    }
    else {
        return decrypt(in, out);
    }
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

    state.set(cipher_text);
    debugMessage(0, "iinput " + state.format());
    state.addKey(key_schedule.getRoundKey(key_schedule.getNrOfRounds()));
    debugMessage(0, "ik_sch " + key_schedule.formatRoundKey(key_schedule.getNrOfRounds()));

    // final round included in loop

    for (int i = 1; i < key_schedule.getNrOfRounds(); i++) {
        debugMessage(i, "istart " + state.format());
        state.invShiftRows();
        debugMessage(i, "is_row " + state.format());
        state.invSubBytes();
        debugMessage(i, "is_box " + state.format());
        state.addKey(key_schedule.getRoundKey(key_schedule.getNrOfRounds()-i));
        debugMessage(i, "ik_sch " + key_schedule.formatRoundKey(key_schedule.getNrOfRounds()-i));
        debugMessage(i, "ik_add " + state.format());
        state.invMixColumns();
    }

    debugMessage(key_schedule.getNrOfRounds(), "istart " + state.format());
    state.invShiftRows();
    debugMessage(key_schedule.getNrOfRounds(), "is_row " + state.format());
    state.invSubBytes();
    debugMessage(key_schedule.getNrOfRounds(), "is_box " + state.format());
    state.addKey(key_schedule.getRoundKey(0));
    debugMessage(key_schedule.getNrOfRounds(), "ik_sch " + key_schedule.formatRoundKey(0));

    state.get(plain_text);
    debugMessage(key_schedule.getNrOfRounds(), "ioutput " + state.format());
}

void AESCipher::encryptBlock(const byte *plain_text, byte *cipher_text) {
    /*
     * Aufgabe 22
     */
    state.set(plain_text);
    debugMessage(0, "input " + state.format());
    state.addKey(key_schedule.getRoundKey(0));
    debugMessage(0, "k_sch " + key_schedule.formatRoundKey(0));

    for(int i = 1; i < key_schedule.getNrOfRounds(); i++) {
        debugMessage(i, "start " + state.format());
        state.subBytes();
        debugMessage(i, "s_box " + state.format());
        state.shiftRows();
        debugMessage(i, "s_row " + state.format());
        state.mixColumns();
        debugMessage(i, "m_col " + state.format());
        state.addKey(key_schedule.getRoundKey(i));
        debugMessage(i, "k_sch " + key_schedule.formatRoundKey(i));
    }

    // Final Round
    debugMessage(key_schedule.getNrOfRounds(), "start " + state.format());
    state.subBytes();
    debugMessage(key_schedule.getNrOfRounds(), "s_box " + state.format());
    state.shiftRows();
    debugMessage(key_schedule.getNrOfRounds(), "s_row " + state.format());
    state.addKey(key_schedule.getRoundKey(key_schedule.getNrOfRounds()));
    debugMessage(key_schedule.getNrOfRounds(), "k_sch " + key_schedule.formatRoundKey(key_schedule.getNrOfRounds()));
    state.get(cipher_text);
    debugMessage(key_schedule.getNrOfRounds(),"output " + state.format());
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
