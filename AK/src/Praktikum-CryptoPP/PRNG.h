/*
 * PRNG.h
 *
 *  Created on: 13.04.2011
 *      Author: chkarg
 */

#ifndef PRNG_H_
#define PRNG_H_

#include "integer.h" // byte

using namespace CryptoPP;

class PRNG {
public:
    /**
	 * Generiert ein (pseudo) zufälliges Bit und gibt dieses in Form
	 * eines bools zurück.
	 */
	virtual bool getBit() = 0;

    /**
     * Generiert ein (pseudo) zufälliges Byte und gibt dieses in Form
     * eines byte Wertes zurück. Die möglichen erzeugten Werte des Bytes
     * sind gleichverteilt.
     */
	virtual byte getByte();

    /**
     * Generiert ein (pseudo) zufälliges Array von n (param) Bytes.
     * Die möglichen erzeugten Werte der Bytes sind gleichverteilt
     * und werden an die von 'buffer' (param) verweisende Stelle geschrieben.
     */
	virtual void getBlock(byte* buffer, unsigned int n);

    /**
     * Generiert eine Zufallszahl, die eine Größe von size (param) Bytes hat.
     * Die möglichen erzeugten Werte der Zufallszahl sind gleichverteilt
     * und werden in Form eines Integers zurückgegeben.
     */
	virtual Integer getInteger(unsigned int size);

    /**
     * Generiert eine Zufallszahl, die eine Größe von size (param) Bytes hat.
     * Die möglichen erzeugten Werte der Zufallszahl sind gleichverteilt
     * und werden in Form eines Integers zurückgegeben.
     */
	virtual Integer getInteger(const Integer& m);
};

#endif /* PRNG_H_ */
