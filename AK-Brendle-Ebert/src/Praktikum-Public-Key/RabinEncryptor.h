/*
 * RabinEncryptor.h
 */

#ifndef RABINENCRYPTOR_H_
#define RABINENCRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

/***
 * RabinEncryptor stellt Methoden bereit, um eine Zahl in einem Integer
 * mit dem Rabin Kryptosystem zu verschlüsseln. Es gibt zusätzlich die
 * Möglichkeit, einen verschlüsselnden zu markieren.
 * Markiert bedeutet, dass das padding an das Ende der zu verschlüsselnden
 * Zahl angehangen wird und dann erst wird "x||padding" verschlüsselt.
 * Der Grund dafür ist, dass es bei der Entschlüsselung einer verschlüsselten
 * Zahl 4 verschiedene Möglichkeiten gibt und nur eine davon ist der
 * ursprüngliche Klartext. Da das padding beiden Parteien bekannt ist, erhält
 * die zu entschlüsselnde Partei durch das Padding einen Hinweis darauf,
 * welches der 4 entschlüsselten Möglichkeiten er originale Klartext sein
 * könnte. Durch das Padding kann ebenfalls ein Angriff mit frei wählbarem
 * Geheimtext abgewehrt werden.
 */
class RabinEncryptor {
private:
    /**
     * n soll das Produkt zweier Rabin Primzahlen sein.
     * Dies wird jedoch nicht überprüft.
     */
	Integer n;
	/**
	 * offset stellt die Zahl dar, mit der ein zu verschlüsselnder Text/Zahl
     * multipliziert werden muss, damit das padding auf dieses Produkt addiert
     * werden kann und in Dezimaldarstellung die letzten x Ziffern belegt,
     * wobei x die Anzahl der Ziffern des paddings in Dezimaldarstellung ist.
     * Zum Beispiel, wenn padding = 987 ist, dann ist offset = 1000.
     * Ein zu verschlüsselnder Geheimtext a = 123 kann dann mit
     * a * offset + padding = 123987 markiert werden.
	 */
	Integer offset;
    /**
     * offset stellt die Zahl dar, mit der ein zu verschlüsselnder Text/Zahl
     * multipliziert werden muss, damit das padding auf dieses Produkt addiert
     * werden kann und in Dezimaldarstellung die letzten x Ziffern belegt,
     * wobei x die Anzahl der Ziffern des paddings in Dezimaldarstellung ist.
     * Zum Beispiel, wenn padding = 987 ist, dann ist offset = 1000.
     * Ein zu verschlüsselnder Geheimtext a = 123 kann dann mit
     * a * offset + padding = 123987 markiert werden.
     */
	Integer padding;

public:
    /***
     * Konstruktor der RabinEncryptor Klasse, welcher die Klassenvariblen
     * n, offset, und padding setzt.
     *
     * @param n n soll das Produkt zweier Rabin Primzahlen als Integer sein.
     * @param padding Ist der Wert, mit dem der zu verschlüsselnde Text markiert
     * wird, wenn mit der compute2 Methode verschlüsselt wird. Markiert
     * bedeutet, dass das padding an das Ende des zu verschlüsselten Texts
     * angehangen wird.
     */
	RabinEncryptor(const Integer& n, const Integer& padding=Integer("987654321"));
	virtual ~RabinEncryptor();

	/***
	 * compute verschlüsselt die Zahl in x mit dem Rabin Kryptosystem und
	 * speichert die verschlüsselte Zahl ohne padding/Markierung in y ab.
	 *
	 * @param x Zu verschlüsselnde Zahl als Integer. x muss kleiner als n sein.
	 * @param y In y wird die verschlüsselte Zahl als Integer geschrieben.
	 * @return True, wenn erfolgreich verschlüsselt wurde, false wenn nicht.
	 */
	bool compute(const Integer& x, Integer& y);

    /***
     * compute2 verschlüsselt die Zahl in x mit dem Rabin Kryptosystem und
     * speichert die verschlüsselte Zahl mit padding/Markierung in y ab.
     * Markiert bedeutet, dass das padding an das Ende der zu verschlüsselnden
     * Zahl angehangen wird und dann erst wird "x||padding" verschlüsselt.
     *
	 * @param x Zu verschlüsselnde Zahl als Integer. x muss kleiner als n sein.
	 * @param y In y wird die zu verschlüsselnde Zahl mit padding/Markierung als
     * Integer geschrieben.
	 * @return True, wenn erfolgreich verschlüsselt wurde, false wenn nicht.
     */
	bool compute2(const Integer&x, Integer& y);
};

#endif /* RABINENCRYPTOR_H_ */
