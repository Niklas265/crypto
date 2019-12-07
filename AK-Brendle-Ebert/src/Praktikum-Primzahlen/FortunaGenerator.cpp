/*
 * FortunaGenerator.cpp
 *
 *  Created on: 20.04.2011
 *      Author: chkarg
 */

#include <iostream>
#include <fstream>
#include "filters.h"
#include "sha.h"
#include "aes.h"
#include "modes.h"
#include "hex.h"
#include "files.h"
#include "FortunaGenerator.h"

using namespace std;

FortunaGenerator::FortunaGenerator() {
	for (int i = 0; i < sizeof(key); i++) {
		key[i] = 0;
	}

	for (int i = 0; i < sizeof(cntr); i++) {
		cntr[i] = 0;
	}

	seeded = false;
	last_reseed=0;
}

bool FortunaGenerator::incCntr() {
    // ctr is 16 char array
    for (int i = 15; i >= 0; i--) {
        if (cntr[i] == 0xff) {
            cntr[i] = 0;
            if (i == 0) {
                return false;
            }
        } else {
            cntr[i] += 1;
            break;
        }
    }

    return true;
}

void FortunaGenerator::reseed(byte* seed, unsigned int size) {
    byte keySeedConcat[32+size];

    incCntr();

    seeded = true;

    for (int i = 0; i < 32; i++) {
        keySeedConcat[i] = key[i];
    }
    for (int i = 0; i < size; i++) {
        keySeedConcat[32+i] = seed[i];
    }

    SHA256 hash;
    // Der String in message wird mit CryptoPP's implementation von SHA256
    // gehasht. Der Hash wird im 'digest' String abgespeichert.
    ArraySource(keySeedConcat, 32+size, true,
        new HashFilter(hash,
            new ArraySink(key, 32)
        )
    );

    last_reseed = getTimeStamp();
}

bool FortunaGenerator::getBit() {
}

byte FortunaGenerator::getByte() {
}

void FortunaGenerator::generateBlocks(byte* buffer, unsigned int n) {

    if (sizeof(buffer) < 16*n) {
        cerr << "TODO" << endl;
        return;
    }

    if (cntr == 0) {
        cerr << "TODO" << endl;
    }

    for (int i = 0; i < n; i++) {
        // AES Implementierung für die Verschlüsselung aus der CryptoPP-Bibliothek
        // im ECB Modus
        ECB_Mode<AES>::Encryption aes_enc;

        // Verwendung der 16 Bytes des 'key'-Arrays für zukünftige Verschlüsselung
        // mit dem AES Objekt
        aes_enc.SetKey(key, sizeof(key));

        // Verschlüsselt den Inhalt von plain_text mit AES im ECB-Modus
        // Der HexEncoder wird verwendet, um die Bytes der AES Ausgabe in
        // menschenlesbaren Hex-Code zu konvertieren.
        ArraySource(cntr, 16, true,
            new StreamTransformationFilter(aes_enc,
                new ArraySink(buffer + 16*i, 16)
            )
        );

        incCntr();
    }
}

void FortunaGenerator::getBlock(byte* buffer, unsigned int n) {
    int num = n /
}

uint32_t FortunaGenerator::getTimeStamp() const {
	struct timeval  tv;

	gettimeofday(&tv,NULL);
	return tv.tv_sec*1000 + tv.tv_usec/1000;
}

