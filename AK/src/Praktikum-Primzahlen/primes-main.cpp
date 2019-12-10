/*
 * primes.cpp
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "FortunaGenerator.h"
#include "MillerRabinTest.h"

using namespace std;

void millerRabin() {
	cout << "Miller-Rabin Test"
		 << "================="
		 << endl << endl;
}

void fortuna() {
	cout << "Fortuna PRNG"
		 << "============"
		 << endl << endl;

	FortunaGenerator fortunaGenerator;
	for (int i = 0; i < 16; i++) {
	    fortunaGenerator.cntr[i] = 0xff;
	}
	fortunaGenerator.cntr[15] = 0xf;

	byte b[2] = {0x42, 0x43};

	fortunaGenerator.reseed(b, 2);

    byte* blockBuffer = (byte*)malloc(3);
    blockBuffer[0] = 0;
    blockBuffer[1] = 0;
    blockBuffer[2] = 0;
    fortunaGenerator.getBlock(blockBuffer, 3);

    printf("%x %x %x", blockBuffer[0], blockBuffer[1], blockBuffer[2]);

    free(blockBuffer);

    for (int i = 0; i < 10; i++) {
        cout << (int)fortunaGenerator.getBit() << endl;
    }

    //----------------------
    // Aufgabe 2f
    byte seed[8] = {0x7a, 0xb2, 0xaf, 0x1a, 0x45, 0x61, 0xab, 0xf8};
    fortunaGenerator.reseed(seed, 8);
    byte* buf = (byte*)malloc(1000);
    if (buf == NULL) {
        cout << "Malloc failed" << endl;
        return;
    }
    fortunaGenerator.getBlock(buf, 1000);
    for (int i = 0; i < 1000; i++) {
        cout << (int)buf[i] << ' ';
    }
    cout << endl;


}

int main() {
	cout << "Generierung von Primzahlen" << endl
		 << "=========================="
		 << endl << endl;

	fortuna();
	//millerRabin();

	return 0;
}
