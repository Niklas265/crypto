/*
 * RSADecryptor.h
 */

#ifndef RSADECRYPTOR_H_
#define RSADECRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

/**
 * Die Klasse RSADecryptor bietet Methoden an, um einen mit RSA verschlüsselten
 * Geheimtext zu entschlüsseln. Der Geheimtext muss mit dem Öffentlichen
 * Schlüssel n und e verschlüsselt worden sein, wobei gelten muss, dass
 * n = p * q und dass e * d ≡ 1 (mod ϕ(n)). Die Sicherheit von RSA
 * beruht auf dem Faktorisierungsproblem und dem Invertieren der modularen
 * Potenzfunktion. Für keines der beiden Probleme ist ein Polynomialzeit
 * Algorithmus bekannt. Könnten diese Probleme in effizienter Laufzeit gelöst
 * werden, dann ist RSA nicht mehr sicher.
 *
 * Zusätzlich hängt die Sicherheit von der Qualität der Schlüssel und der
 * geheimhaltung des privaten Schlüssels ab. Mit dem aktuellen Stand der
 * Technik werden mindestens 2048 Bit pro p und q vorgeschlagen.
 * RSA ist ein Public Key Kryptosystem.
 */
class RSADecryptor {
private:
    /**
     * p ist ein Teil des privaten Schlüssels. p muss eine Primzahl sein,
     * das wird von dieser Klasse allerdings nicht überprüft. Es muss p != q
     * gelten. Das wird allerdings ebenfalls nicht überprüft.
     */
	Integer p;
    /**
     * q ist ein Teil des privaten Schlüssels. q muss eine Primzahl sein,
     * das wird von dieser Klasse allerdings nicht überprüft. Es muss
     * p != q gelten. Das wird allerdings ebenfalls nicht überprüft.
     */
	Integer q;
    /**
     * d ist ein Teil des privaten Schlüssels. d muss das mulitplikative Inverse
     * von e mod ϕ(n) sein. Das wird von dieser Klasse allerdings nicht
     * überprüft. e ist Teil des Öffentlichen Schlüssels.
     */
	Integer d;
	/**
	 * n ist ein Teil des öffentlichen Schlüssels. n wird durch p * q im
	 * Konstruktor berechnet. n wird sowohl bei der Ver- als auch bei der
	 * Entschlüsselung verwendet und gibt die Größe des Klartextraums und
	 * Geheimtextraums an, der Z_n ist.
	 */
	Integer n;

public:
    /***
     * Konstruktor der RSAEncryptor Klasse, welche den privaten Teil
     * des Schlüssels, p, q und d setzt. Die Übergabeargumente werden den
     * Klassenvariablen p, q und d zugewiesen. Das n wird ebenfalls durch
     * p * q berechnet. n ist ein Teil des öffentlichen Schlüsseln, und
     * gibt mit Z_n den Klartext- und Geheimtextraum an. Die Parameter
     * werden nicht auf Korrektheit hin überprüft. Also wird nicht überprüft
     * ob p und tatsächlich mit einer ausreichenden Wahrscheinlichkeit
     * Primzahlen sind und ebenfalls nicht, ob p == q ist.
     *
     * @param p p ist ein Teil des privaten Schlüssels. p muss eine Primzahl
     * sein, das wird allerdings nicht überprüft. p ist ein Integer.
     * @param q q ist ein Teil des privaten Schlüssels. q muss eine Primzahl
     * sein, das wird allerdings nicht überprüft. q ist ein Integer.
     * @param d
     */
	RSADecryptor(const Integer& p, const Integer& q, const Integer& d);
	virtual ~RSADecryptor();

	/***
	 * compute entschlüsselt den Integer y mit dem Öffentlichen Teil n und
	 * dem Privaten Teil d. Bei dieser Implementation der Entschlüsselung
	 * wird die zu entschlüsselnde Zahl y mit y^d mod n entschlüsselt.
	 * Das Ergebnis ist das entschlüsselte y, welches im Parameter x
	 * gespeichert wird. Die Zahl y muss < n sein, weil sonst y nicht Teil
	 * des Geheimtextraums ist.
	 * @param y Die zu entschlüsselnde Zahl als Integer.
	 * @param x Wenn entschlüsselt werden kann, dann wird das entschlüsselte
	 * y in x als Integer gespeichert.
	 * @return True, wenn entschlüsselt werden konnte, false wenn nicht.
	 */
	bool compute(const Integer& y, Integer& x) const;

    /***
     * compute entschlüsselt den Integer y mit dem Öffentlichen Teil n und
     * dem Privaten Teil d. Bei dieser Implementation der Entschlüsselung
     * wird die zu entschlüsselnde Zahl y mit dem Chinesischen Restsatz
     * entschlüsselt. Das ist möglich, weil die Faktoren p und q für n zum
     * einen bekannt sind, und zum anderen weil gcd(p,q) = 1 ist, was eine
     * Vorbedingung für den CRT ist.
     * Das Ergebnis ist das entschlüsselte y, welches im Parameter x
     * gespeichert wird. Die Zahl y muss < n sein, weil sonst y nicht Teil
     * des Geheimtextraums ist.
     * @param y Die zu entschlüsselnde Zahl als Integer.
     * @param x Wenn entschlüsselt werden kann, dann wird das entschlüsselte
     * y in x als Integer gespeichert.
     * @return True, wenn entschlüsselt werden konnte, false wenn nicht.
     */
	bool crt(const Integer& y, Integer& x) const;

    /***
     * compute entschlüsselt den Integer y mit dem Öffentlichen Teil n und
     * dem Privaten Teil d. Bei dieser Implementation der Entschlüsselung
     * wird die zu entschlüsselnde Zahl y mit Garners Verfahren
     * entschlüsselt.
     * Das Ergebnis ist das entschlüsselte y, welches im Parameter x
     * gespeichert wird. Die Zahl y muss < n sein, weil sonst y nicht Teil
     * des Geheimtextraums ist.
     * @param y Die zu entschlüsselnde Zahl als Integer.
     * @param x Wenn entschlüsselt werden kann, dann wird das entschlüsselte
     * y in x als Integer gespeichert.
     * @return True, wenn entschlüsselt werden konnte, false wenn nicht.
     */
	bool garner(const Integer& y, Integer& x) const;
};

#endif /* RSADECRYPTOR_H_ */
