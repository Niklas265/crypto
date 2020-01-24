/*
 * RabinDecryptor.h
 */

#ifndef RABINDECRYPTOR_H_
#define RABINDECRYPTOR_H_

#include <vector>
#include "integer.h"

using namespace CryptoPP;
using namespace std;

/***
 * RabinDecryptor stellt Methoden bereit, um eine Zahl in einem Integer
 * mit dem Rabin Kryptosystem zu entschlüsseln. Es gibt zusätzlich die
 * Möglichkeit, über einen mit dem padding endenden verschlüsselten klartext
 * aus den 4 möglichen, aus der entschlüsselung resultierenden, Klartexten
 * zu bestimmen.
 * Markiert bedeutet, dass das padding an das Ende der zu verschlüsselnden
 * Zahl angehangen wird und dann erst wird "x||padding" verschlüsselt.
 * Der Grund dafür ist, dass es bei der Entschlüsselung einer verschlüsselten
 * Zahl 4 verschiedene Möglichkeiten gibt und nur eine davon ist der
 * ursprüngliche Klartext. Da das padding beiden Parteien bekannt ist, erhält
 * die zu entschlüsselnde Partei durch das Padding einen Hinweis darauf,
 * welches der 4 entschlüsselten Möglichkeiten er originale Klartext sein
 * könnte. Durch das Padding kann ebenfalls ein Angriff mit frei wählbarem
 * Geheimtext abgewehrt werden.
 * Das Rabin Kryptosystem ist ein Public Key Kryptosystem.
 */
class RabinDecryptor {
private:
    /**
     * p ist eine Rabin Primzahl. Dabei gilt, dass p ≡ 3 (mod 4) ist.
     * p ist Teil des privaten Schlüssels. p * q = n und p != q müssen gelten.
     */
	Integer p;
    /**
     * q ist eine Rabin Primzahl. Dabei gilt, dass q ≡ 3 (mod 4) ist.
     * q ist Teil des privaten Schlüssels. p * q = n und p != q müssen gelten.
     */
	Integer q;
    /**
     * offset stellt die Zahl dar, mit der ein zu verschlüsselnder Text/Zahl
     * multipliziert werden muss, damit das padding auf dieses Produkt addiert
     * werden kann und in Dezimaldarstellung die letzten x Ziffern belegt,
     * wobei x die Anzahl der Ziffern des paddings in Dezimaldarstellung ist.
     * Zum Beispiel, wenn padding = 987 ist, dann ist offset = 1000.
     * Ein zu verschlüsselnder Geheimtext a = 123 kann dann mit
     * a * offset + padding = 123987 markiert werden. Weiter kann mit
     * y % offset == padding, wobei y ein möglicher Klartext ist,
     * erkannt werden, ob dieser mögliche Klartext mit dem padding endet.
     * Das kann ein Hinweis darauf sein, dass der Klartext markiert worden ist.
     * Es kann aber auch sein, dass einer der möglichen Klartexte nur zufällig
     * mit dem padding endet.
     */
	Integer padding;
    /**
     * offset stellt die Zahl dar, mit der ein zu verschlüsselnder Text/Zahl
     * multipliziert werden muss, damit das padding auf dieses Produkt addiert
     * werden kann und in Dezimaldarstellung die letzten x Ziffern belegt,
     * wobei x die Anzahl der Ziffern des paddings in Dezimaldarstellung ist.
     * Zum Beispiel, wenn padding = 987 ist, dann ist offset = 1000.
     * Ein zu verschlüsselnder Geheimtext a = 123 kann dann mit
     * a * offset + padding = 123987 markiert werden. Weiter kann mit
     * y % offset == padding, wobei y ein möglicher Klartext ist,
     * erkannt werden, ob dieser mögliche Klartext mit dem padding endet.
     * Das kann ein Hinweis darauf sein, dass der Klartext markiert worden ist.
     * Es kann aber auch sein, dass einer der möglichen Klartexte nur zufällig
     * mit dem padding endet.
     */
	Integer offset;

public:
    /***
     * Konstruktor der RabinDecryptor Klasse, welcher die Klassenvariblen
     * p, q, offset, und padding setzt.
     *
     * @param p p soll eine Rabin Primzahl sein.
     * @param q q soll eine Rabin Primzahl sein.
     * @param padding Ist ein Wert, mit dem ein zu verschlüsselnde Text markiert
     * wurde. Markiert bedeutet, dass das padding an das Ende des zu
     * verschlüsselten Texts angehangen worden ist.
     */
	RabinDecryptor(const Integer& p, const Integer& q, const Integer& padding=Integer("987654321"));
	virtual ~RabinDecryptor();

	/***
	 * compute Entschlüsselt den Geheimtext y und speichert die 4 möglichen
	 * Klartexte im Vektor xv ab. Es wird nicht nach einem padding in den
	 * möglichen Klartexten hin überprüft. Zu beachten gilt, dass 4
	 * mögliche Klartexte zurückgelifert werden, und diese Methode nicht
	 * bestimmt, welches der 4 möglichen Klartexte der tatsächliche Klartext
	 * ist.
	 * @param y y ist der zu entschlüsselnde Geheimtext bzw. die zu
	 * entschlüsselnde Zahl als Integer.
	 * @param xv xv ist ein Vektor von Integern, in welche bei erfolgreicher
	 * entschlüsselung die 4 möglichen Klartexte abgespeichert werden.
	 * @return True, wenn das entschlüsseln erfolgreich war, false wenn nicht.
	 */
	bool compute(const Integer& y, vector<Integer>& xv);
    /***
     * compute Entschlüsselt den Geheimtext y und speichert eines der 4
     * möglichen Klartexte im Integer x ab. Es wird nicht nach einem padding in
     * den möglichen Klartexten hin überprüft.
     * @param y y ist der zu entschlüsselnde Geheimtext bzw. die zu
     * entschlüsselnde Zahl als Integer.
     * @param x x ist ein Integern, in welchem bei erfolgreicher
     * entschlüsselung eines der 4 möglichen Klartexte abgespeichert wird.
     * @return True, wenn das entschlüsseln erfolgreich war, false wenn nicht.
     */
	bool compute(const Integer& y, Integer& x);

    /***
     * compute2 Entschlüsselt den Geheimtext y, wodurch 4 mögliche Klartexte
     * entstehen. Zusätzlich zu compute überprüft compute2, ob eines dieser
     * 4 Klartexte in Dezimaldarstellung mit dem padding endet. Das padding
     * wird als markierung gesehen, welches den Klartext markiert. compute2
     * speichert bei Erfolg den Klartext ohne padding im Integer x.
     * @param y y ist der zu entschlüsselnde Geheimtext bzw. die zu
     * entschlüsselnde Zahl als Integer.
     * @param x x ist ein Integer, in welchem bei Erfolg der entschlüsselte
     * Klartext ohne padding gespeichert wird.
     * @return True, wenn das Entschlüsseln erfolgreich war, false wenn nicht.
     */
	bool compute2(const Integer& y, Integer& x);
};

#endif /* RABINSYSTEM_H_ */
