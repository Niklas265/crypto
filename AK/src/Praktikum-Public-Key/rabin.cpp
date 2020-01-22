/*
 * rabin.cpp
 */

#include <iostream>
#include <vector>
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"
#include "RabinDecryptor.h"
#include "RabinEncryptor.h"
#include "RabinAttack.h"

using namespace std;

// TODO: wahrscheinlich will er dass wir das im terminal ausgeben lassen damit
//  er es sieht

// #rabinParameters()
void rabinParameters() {
	/*********************************************************************
	 * Aufgabe 9.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	Integer p, q;
    pb.randomRabinPrime(p, 256, 50);
    pb.randomRabinPrime(q, 256, 50);
	//cout << "Erste  256 Bit Rabin Primzahl p: " << p << endl;
    //cout << "Zweite 256 Bit Rabin Primzahl q: " << q << endl;
}

// #sqrtModPrimeExercise()
void sqrtModPrimeExercise() {
	/*********************************************************************
	 * Aufgabe 10.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	vector<Integer> v;
	pb.modPrimeSqrt(Integer("400040001"), Integer("884249923643"), v);
	assert(v[0] == Integer("884249903642")  && v[1] ==  Integer("20001"));

    vector<Integer> v2;
    pb.modPrimeSqrt(Integer("644640535034"), Integer("868380007367"), v2);
    assert(v2[0] == Integer("868378777318") && v2[1] ==  Integer("1230049"));
}

// #rabinDemo()
void rabinDemo() {
	/*********************************************************************
	 * Aufgaben 11 und 12.
	 *********************************************************************/

    Integer p = Integer("728768879148869666628372866383");
    Integer q = Integer("1178365175275537416785439551531");
    // n = p * q
    RabinEncryptor rabinEncryptor = RabinEncryptor(p * q, 0);
    Integer y;
    rabinEncryptor.compute(Integer("234131892323212"), y);
    assert(y == Integer("54817743002848138258673996944"));

    RabinDecryptor rabinDecryptor = RabinDecryptor(p, q, 0);

    vector<Integer> v;
    rabinDecryptor.compute(y, v);
    //cout << v[0] << ' ' << v[1] << v[2] << ' ' << v[3] << endl;
    assert(v[0] == Integer("858755868013614750163033981532656431854843747105980613759161"));
    assert(v[1] == Integer("468253878381255984906252283528370901639136709523733710358252"));
    assert(v[2] == Integer("390501989632358765256781698004285530215707037816378795724121"));
    assert(v[3] == Integer("234131892323212"));

    Integer x1;
    rabinDecryptor.compute(y, x1);
    assert(x1 == Integer("858755868013614750163033981532656431854843747105980613759161"));

    // With Padding
    RabinEncryptor rabinEncryptorPadded = RabinEncryptor(p * q, Integer("987654321"));
    Integer yPadded;
    rabinEncryptorPadded.compute2(Integer("234131892323212"), yPadded);

    RabinDecryptor rabinDecryptorPadded = RabinDecryptor(p, q, Integer("987654321"));
    Integer x;
    rabinDecryptorPadded.compute2(yPadded, x);
    assert(x == Integer("234131892323212"));
}

// #rabinAttack()
void rabinAttack() {
	/*********************************************************************
	 * Aufgabe 13.
	 *********************************************************************/

    Integer p = Integer("728768879148869666628372866383");
    Integer q = Integer("1178365175275537416785439551531");

    RabinDecryptor rabinDecryptor(p, q, 0);

    RabinAttack rabinAttack;
    Integer f;
    int tries = rabinAttack.factorize(p * q, f, 5, rabinDecryptor);
    if (tries != -1) {
        cout << "Found a factor f: " << f << endl;
        assert(f == p || f == q);
    } else {
        cout << "Didnt find f after " << tries << " tries." << endl;
    }
}

// #main()
int main() {

	rabinParameters();
	sqrtModPrimeExercise();
	rabinDemo();
	rabinAttack();

	return 0;
}
