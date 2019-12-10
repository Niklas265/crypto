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
    for (int i = 0; i < 16; i++) {
        if (cntr[i] == 0xff) {
            cntr[i] = 0;
            if (i == 15) {
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
    byte ret;
    getBlock(&ret, 1);
    byte mask = 0x1;
    ret &= mask;
    return ret;
}

byte FortunaGenerator::getByte() {
    byte ret;
    getBlock(&ret, 1);
    return ret;
}

void FortunaGenerator::generateBlocks(byte* buffer, unsigned int n) {
    if (!seeded || getTimeStamp() - last_reseed > 500) {
        byte seed[32];
        string urandomPath = "/dev/urandom";
        ifstream urandStream(urandomPath, ios::binary);
        urandStream.read((char*)seed,32);
        urandStream.close();
        reseed(seed,32);
    }

    // AES Implementierung für die Verschlüsselung aus der CryptoPP-Bibliothek
    // im ECB Modus
    ECB_Mode<AES>::Encryption aes_enc;

    for (int i = 0; i < n; i++) {
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
    unsigned int num16Blocks;
    if (n >= 2) {
        num16Blocks = (n-1) / 16 + 1;
    } else {
        num16Blocks = 1;
    }

    byte* blockBuffer = (byte*)malloc(num16Blocks * 16);

    if(blockBuffer == NULL) {
        cerr << "Allocating Memory failed." << endl;
        return;
    }

    generateBlocks(blockBuffer, num16Blocks);

    for (unsigned int i = 0; i < n; i++) {
        buffer[i] = blockBuffer[i];
    }

    free(blockBuffer);
}

uint32_t FortunaGenerator::getTimeStamp() const {
	struct timeval  tv;

	gettimeofday(&tv,NULL);
	return tv.tv_sec*1000 + tv.tv_usec/1000;
}

