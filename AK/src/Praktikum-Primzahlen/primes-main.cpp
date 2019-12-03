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
}

int main() {
	cout << "Generierung von Primzahlen" << endl
		 << "=========================="
		 << endl << endl;

	//fortuna();
	//millerRabin();

	return 0;
}
