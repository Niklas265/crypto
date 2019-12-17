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

Integer randomPrime(int l, unsigned int s) {
    if (l % 8 != 0) {
        cout << "For param l, only multiples of 8 supported." << endl;
        return Integer("0");
    }

    MillerRabinTest millerRabinTest;
    FortunaGenerator fortunaGenerator;
    byte buf[l/8];

    do {
        for (int i = 0; i < l/8; i++) {
            buf[i] = fortunaGenerator.getByte();
        }
    } while (!millerRabinTest.isPrime(&fortunaGenerator, Integer(buf, l/8), s));

    return Integer(buf, l/8);
}

void millerRabin() {
	cout << "Miller-Rabin Test"
		 << "================="
		 << endl << endl;

	MillerRabinTest millerRabinTest;
	Integer n("121399589920585397786647347761633");
	for (int i = 6; i < n; i++) {
	    cout << i << endl;
	    if (millerRabinTest.witness(Integer(i), n)) {
	        cout << "n zusammengesetzt mit Witness " << i << endl;
            break;
	    }
	}

    FortunaGenerator fortunaGenerator;

	for (int i = 3; i < 1000; i++) {
	    if (millerRabinTest.isPrime(&fortunaGenerator, Integer(i), 10)) {
	        cout << i << endl;
	    }
	}

	Integer check1 = Integer("249236727971798483921811058804041930204535828339");
	cout << check1 << ' ' << millerRabinTest.isPrime(&fortunaGenerator, Integer(check1), 50) << endl;

    Integer check2 = Integer("167376539261987877304745111417636335723391598799");
    cout << check2 << ' ' << millerRabinTest.isPrime(&fortunaGenerator, Integer(check2), 50) << endl;

    Integer check3 = Integer("207241657224647904979925877995751997643166331621");
    cout << check3 << ' ' << millerRabinTest.isPrime(&fortunaGenerator, Integer(check3), 50) << endl;

    Integer check4 = Integer("203681457354349143050092586190992078578082388049");
    cout << check4 << ' ' << millerRabinTest.isPrime(&fortunaGenerator, Integer(check4), 50) << endl;

    assert(millerRabinTest.boundedExponentation(Integer("7"), Integer("43"), Integer::Power2(121))
        == Integer("2183814375991796599109312252753832343"));

    assert(millerRabinTest.boundedExponentation(Integer("7"), Integer("43"), Integer::Power2(120))
           == Integer("1329227995784915872903807060280344577"));

    assert(millerRabinTest.searchBase(
            Integer("346158389003024161132823470747012309343112001349179903402673"),
            Integer("29")
        ) == Integer("113"));

    Integer e;
    Integer b;
    millerRabinTest.isPerfectPower(
        Integer("346158389003024161132823470747012309343112001349179903402673"),
        b, e);
    cout << "b: " << b << " e: " << e << endl;
    assert(b == Integer("113"));
    assert(e == Integer("29"));

    b = 0;
    e = 0;
    millerRabinTest.isPerfectPower(
            Integer("2183814375991796599109312252753832343"),
            b, e);
    cout << "b: " << b << " e: " << e << endl;

    b = 0;
    e = 0;
    millerRabinTest.isPerfectPower(
            Integer("1329227995784915872903807060280344577"),
            b, e);
    cout << "b: " << b << " e: " << e << endl;

    int primeSizes[] = {128, 512, 1024, 2048, 4096};
    for (int primeSize : primeSizes) {
        cout << primeSize << " Bit Prime: " << randomPrime(primeSize, 2) << endl;
    }
}

void fortuna() {
	cout << "Fortuna PRNG"
		 << "============"
		 << endl << endl;

	FortunaGenerator fortunaGenerator;
	/*for (int i = 0; i < 16; i++) {
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
    }*/

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
	millerRabin();

	return 0;
}
