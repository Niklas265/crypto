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
    const Integer a = 102;
    const Integer b = 75;
    Integer d, x, y;
    pb.EEA(a, b, d, x, y);
    cout << "d: " << d << " x: " << x << " y: " << y << endl;
}

void invExercise() {
	/*********************************************************************
	 * Aufgabe 4.
	 *********************************************************************/
	// inv existeiert if eea() == true
    a * ? (mod n) = 1
}

void modexpExercise() {
	/*********************************************************************
	 * Aufgabe 5.
	 *********************************************************************/
}

void randExercise() {
	/*********************************************************************
	 * Aufgabe 6.
	 *********************************************************************/
}

void millerRabinExercise() {
	/*********************************************************************
	 * Aufgabe 7.
	 *********************************************************************/
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
