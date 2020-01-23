/*
 * RSAEncryptor.h
 */

#ifndef RSAENCRYPTOR_H_
#define RSAENCRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

/**
 * RSAEncryptor bietet die Funktion, eine Zahl mit dem RSA Kryptosystem
 * zu verschlüsseln. TODO: kann noch was zu RSA allgemein sagen, z.b. wann
 * es sicher ist etc...
 */
class RSAEncryptor {
private:
    /**
     * Der öffentliche Teil n des Schlüssels, welcher aus n = p * q berechnet
     * wird, wobei p und q prim sein sollten, das wird aber nicht überprüft.
     * n wird im Konstruktor dieser Klasse gesetzt.
     */
	Integer n;
	/**
	 * Der öffentliche Teil e des Schlüsseln, mit dem ein zu verschlüsselnder
	 * Text verschlüsselt wird. e wird im Konstruktor dieser Klasse gesetzt.
	 */
	Integer e;

public:
    /***
     * Konstruktor der RSAEncryptor Klasse, welche den öffentlichen Teil
     * des Schlüssels, n und e, setzt. Die Übergabeargumente werden den
     * Klassenvariablen n und e zugewiesen.
     * @param n Der bei der Ver- und Entschlüsselung verwendete Modul n,
     * welcher das Produkt von zwei Primzahlen p und q ist, wobei p != q. Das
     * allerdings kann nicht und wird nicht überprüft. n ist ein Integer und
     * Teil des öffentlichen Schlüssels.
     * @param e Der bei der Verschlüsselung verwendete Exponent e als Integer.
     * e ist ebenfalls Teil des öffentlichen Schlüssels. Weiter sollte gelten,
     * dass e * d ≡ 1 (mod ϕ(n)), wobei d ein Teil des privaten Schlüssels ist
     * und in der Klasse RSADecryptor gesetzt wird und somit zum entschlüsseln
     * verwendet wird.
     */
	RSAEncryptor(const Integer& n, const Integer& e);
	virtual ~RSAEncryptor();

	/***
	 * compute verschlüsselt die Zahl x mit dem RSA Kryptosystem und dem
	 * öffentlichen Schlüssel n und e. Speziell wird x^e mod n berechnet.
	 * Das Ergebnis ist das verschlüsselte x, welches im Parameter y
	 * gespeichert wird. Die Zahl x muss < n sein, weil sonst x nicht Teil
	 * des Klartextraums ist.
	 * @param x Die zu verschlüsselnde Zahl als Integer.
	 * @param y In y wird bei Erfolg die verschlüsselte Zahl x als Integer
	 * gespeichert.
	 * @return True, wenn x verschlüsselt worden ist, false wenn nicht.
	 */
	bool compute(const Integer& x, Integer& y) const;
};

#endif /* RSAENCRYPTOR_H_ */
