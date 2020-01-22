#include <iostream>
#include <set>
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace std;

void integerExercise() {
	/*********************************************************************
	 * Aufgabe 2.
	 *********************************************************************/

	// Integer der CryptoPP lib werden erzeugt
    Integer a = Integer(23975);
    // Wenn ein Integer auf eine Zahl gesetzt werden soll, die größer ist
    // als die elementaren Datentypen, dann muss die Zahl als String kodiert
    // an den Konstruktor für Integer übergeben werden. Ansonsten kommt es
    // zu einem Überlauf des elementaren Datentyps.
    Integer b = Integer("12345678900987654321");
    // Berechnet 2^10 und speichert das Ergebnis im Integer c ab.
    Integer c = Integer::Power2(10);

    // mit diesen Integern kann wie mit den elementaren Datentypen int und
    // long gerechnet werden.
    cout << "(a + c) * b % 50001 = " << (a + c) * b % 50001 << endl;
}

void eeaExercise() {
	/*********************************************************************
	 * Aufgabe 3.
	 *********************************************************************/

    PublicKeyAlgorithmBox pb;
    Integer d, x, y;
    // Berechnet das d, x und y für verschiedene a und b mit dem Erweiterten
    // Algorithmus von Euklid. a wird an EEA als erstes Argument übergeben
    // und b an EEA als zweites Argument. d, x und y werden nach jeder der drei
    // Beispiele über stdout ausgegeben.
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
    // Berechnet das berechnet das multiplikative Inverse
    // von 10353 modulo 820343. Falls ein solches existiert, dann wird es in
    // a_inv gespeichert.
    pb.multInverse(Integer(10353), Integer(820343), a_inv);
    // Es wird erwartet, dass ein solches multiplikative Inverse existiert und
    // das dieses 21711 ist.
    assert(a_inv == Integer(21711));
    // Es wird erwartet, dass kein multiplikatives Inverses von
    // 10353 modulo 820344 existiert. In diesem Fall liefert multInverse()
    // False zurück.
    assert(!pb.multInverse(Integer(10353), Integer(820344), a_inv));
    // Berechnet das multiplikatives Inverses von 562312 modulo 57913313.
    // Es wird erwartet, dass dieses 53494466 ist.
    pb.multInverse(Integer(562312), Integer(57913313), a_inv);
    assert(a_inv == Integer(53494466));
}

void modexpExercise() {
	/*********************************************************************
	 * Aufgabe 5.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	// modularExponentation berechnet a^b mod n, wobei a als erstes, b als
	// zweites und n als drittes Argument an modularExponentation übergeben
	// werden.
	// Berechnet 2^100000 % 23 und überprüft ob das Ergebnis gleich 12 ist.
	assert(pb.modularExponentation(Integer(2), Integer(100000), Integer(23)) == Integer(12));
    // Berechnet 2343947997^765 % 111 und überprüft ob das Ergebnis gleich 105 ist.
	assert(pb.modularExponentation(Integer(2343947997), Integer(765), Integer(111)) == Integer(105));
}

void randExercise() {
	/*********************************************************************
	 * Aufgabe 6.
	 *********************************************************************/

    // Aufgabe 6 b):
    // Bei den Zufallszahlengeneratoren handelt es sich um Pseudozufallszahlen-
    // generatoren. Diese erzeugen scheinbar zufällig aussehende Bitfolgen
    // basierend auf einem Seed. Damit der generierte Bitstream noch mehr
    // zufällig aussieht, sollte dieser Seed immer wieder mal geändert werden.
    // Die Pseudozufallszahlen dev/random und dev/urandom von Linux verwenden
    // dafür einen 'entropy pool'. Wird ein neuer Seed gebraucht, dann kann
    // dieser aus dem pool entnommen werden. Das Betriebssystem fügt dem
    // pool Seeds aus verschiedenen scheinbar zufälligen Quellen wie zum
    // Beispiel die Verzögerung zwischen dem Drücken von Tasten. Dieser Pool
    // kann auch leer werden. Wenn dieser Pool leer ist, dann blockt ein
    // BlockingRng wie dev/random, also wartet bis er ein Bit zurückgibt,
    // hält praktisch die Ausführung an, bis zusätzliche Werte dem pool
    // hinzugefügt werden. Das Gerät /dev/random ist eine
    // veraltete Schnittstelle, die auf eine Zeit zurückgeht, in der den
    // kryptographischen Primitiven, die bei der Implementierung von
    // /dev/urandom verwendet wurden, kein großes Vertrauen entgegengebracht
    // wurde. Ein NonblockingRng blockt nicht und gibt auch Bits zurück, auch
    // wenn der pool leer ist und er eigentlich neue seeds haben möchte.
    // Mittlerweise wird, außer vielleicht kurz nach dem ersten Booten,
    // den kryptographischen Primitiven wie denen in dev/urandom mehr Vertrauen
    // entgegengebracht, wodurch auch NonblockingRngs für Dinge wie
    // Kryptographie als ausreichend sicher gelten.

    Integer a;
    BlockingRng blockingRng;
    // Randomize wird eine Referenz auf eine Klasse vom Typ
    // RandomNumberGenerator übergeben. BlockingRng und NonblockingRng
    // erben von RandomNumberGenerator und sind somit für Randomize geeignet.
    a.Randomize(blockingRng, 128);
    cout << "Erzeuge 128 Bit Zufallszahl: " << a << endl;

    NonblockingRng nonblockingRng;
    a.Randomize(nonblockingRng, 1024);
    cout << "Erzeuge 1024 Bit Zufallszahl: " << a << endl;
}

void millerRabinExercise() {
	/*********************************************************************
	 * Aufgabe 7.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	Integer a = Integer("279226292160650115722581212551219487007");
	// Mit dem Miller Rabin Test wird überprüft, ob a mit
	// Fehlerwahrscheinlichkeit s eine Primzahl ist.
	// Die zu testende Zahl a wird als erstes Argument an millerRabinTest()
	// übergeben.
	// Als Fehlerwahrscheinlichkeit wird 2^−100 über s, dem zweiten Argument,
	// gewählt.
	// millerRabinTest liefert True zurück, wenn mit der Fehlerwahrscheinlichkeit
	// angenommen wird, dass es sich bei a um eine Primzahl handelt.
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
