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
}

int main() {
	cout << "Generierung von Primzahlen" << endl
		 << "=========================="
		 << endl << endl;

	fortuna();
	//millerRabin();

	return 0;
}
