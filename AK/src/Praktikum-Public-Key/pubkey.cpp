#include <iostream>
#include <set>
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace std;

void integerExercise() {
	/*********************************************************************
	 * Aufgabe 2.
	 *********************************************************************/

    Integer a = Integer(23975);
    Integer b = Integer("12345678900987654321");
    Integer c = Integer::Power2(10);

    cout << "(a + c) * b % 50001 = " << (a + c) * b % 50001 << endl;
}

void eeaExercise() {
	/*********************************************************************
	 * Aufgabe 3.
	 *********************************************************************/

    PublicKeyAlgorithmBox pb;
    Integer d, x, y;
    pb.EEA(Integer(2987), Integer(1279865), d, x, y);
    cout << "d: " << d << " x: " << x << " y: " << y << endl;
    pb.EEA(Integer(78845945), Integer(2503417846), d, x, y);
    cout << "d: " << d << " x: " << x << " y: " << y << endl;
    pb.EEA(Integer(57913313), Integer(173739939), d, x, y);
    cout << "d: " << d << " x: " << x << " y: " << y << endl;
}

void invExercise() {
	/*********************************************************************
	 * Aufgabe 4.
	 *********************************************************************/

    PublicKeyAlgorithmBox pb;
    Integer a_inv;
    pb.multInverse(Integer(10353), Integer(820343), a_inv);
    assert(a_inv == Integer(21711));
    assert(!pb.multInverse(Integer(10353), Integer(820344), a_inv));
    pb.multInverse(Integer(562312), Integer(57913313), a_inv);
    cout << a_inv << endl;
    assert(a_inv == Integer(53494466));
}

void modexpExercise() {
	/*********************************************************************
	 * Aufgabe 5.
	 *********************************************************************/

	// TODO: Aufhabe b), Wie unterscheiden sich Blockung und Non-Blocking
	//  Devices. probably ask google

	PublicKeyAlgorithmBox pb;
	assert(pb.modularExponentation(Integer(2), Integer(100000), Integer(23)) == Integer(12));
    assert(pb.modularExponentation(Integer(2343947997), Integer(765), Integer(111)) == Integer(105));
}

void randExercise() {
	/*********************************************************************
	 * Aufgabe 6.
	 *********************************************************************/

    Integer a;
    BlockingRng blockingRng;
    a.Randomize(blockingRng, 128);
    cout << "128 Bit Zufallszahl: " << a << endl;

    NonblockingRng nonblockingRng;
    a.Randomize(nonblockingRng, 1024);
    cout << "1024 Bit Zufallszahl: " << a << endl;
}

void millerRabinExercise() {
	/*********************************************************************
	 * Aufgabe 7.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	Integer a = Integer("279226292160650115722581212551219487007");
	assert(pb.millerRabinTest(a, 100));
	Integer b = Integer("247278711133334795867191516244139839983");
    assert(!pb.millerRabinTest(b, 100));
    Integer c = Integer("192172622525902080249109244057747132167");
    assert(!pb.millerRabinTest(c, 100));
    Integer d = Integer("177387942943728133030691912202779547031");
    assert(pb.millerRabinTest(d, 100));
}

int main(int argc, char** argv) {

	integerExercise();
	eeaExercise();
	invExercise();
	modexpExercise();
	randExercise();
	millerRabinExercise();

	return 0;

}
