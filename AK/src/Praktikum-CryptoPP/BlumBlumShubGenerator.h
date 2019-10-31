#ifndef _BlumBlumShubGenerator_h_
#define _BlumBlumShubGenerator_h_

#include "integer.h" // Integer
#include "PRNG.h"

using namespace CryptoPP;

/**
 * Der BlumBlumShubGenerator ist eine Klasse für das
 * Erzeugen von (pseudo) zufälligen Bits. Die erzeugten Bits
 * hängen von den Parametern n (mod n) und s (seed) ab.
 * Das heißt, dass bei verwendung der gleichen Parametern die gleiche
 * Zufallszahlenfolge erzeugt wird.
*/
class BlumBlumShubGenerator : public PRNG {
private:
	Integer n;
	Integer s;

public:
    /**
     * Einziger Konstruktor der BlumBlumShubGenerator Klasse.
     * Der Übergabeparameter n ist vom Typ Integer und
     * sollte das Produkt von zwei Primzahlen sein.
     * Der Übergabeparameter seed ist vom Typ Integer und
     * sollte nicht 0 oder 1 sein.
     */
	BlumBlumShubGenerator(const Integer& n, const Integer& seed);

	/**
	 * Generiert ein (pseudo) zufälliges Bit und gibt dieses in Form
	 * eines bools zurück.
	 */
	virtual bool getBit();

};

#endif
