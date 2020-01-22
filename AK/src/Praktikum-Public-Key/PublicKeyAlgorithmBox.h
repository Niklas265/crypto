#ifndef _PublicKeyAlgorithmBox_h_
#define _PublicKeyAlgorithmBox_h_

#include <vector>
#include "integer.h"

using namespace std;
using namespace CryptoPP;

/**
 * TODO
 */
class PublicKeyAlgorithmBox {
public:

	unsigned int computeConvergents(const Integer& a, const Integer& b,
			vector<Integer>& c, vector<Integer>& d);

	Integer euklid(const Integer& a, const Integer& b, vector<Integer>& q);

	/**
	 * EEA realisiert den Erweiterten Algorithmus von Euklid. Dabei wird der
	 * größte gemeinsame Teiler von a und b berechnet und in d gespeichert.
	 * Zusätzlich gilt, dass der größte gemeinsame Teiler von a und b das
	 * kleinste positive Element der Menge {ax + by | x, y ∈ Z} aller
	 * Linearkombinationen von a und b ist. Mit dem EEA wird zusätzlich dieses
	 * x und y berechnet und in (param) x und (param) y abgespeichert.
	 *
	 * @param a Ein ganze Zahl > 0 als Integer
	 * @param b Ein ganze Zahl > 0 als Integer
	 * @param d Diese Methode setzt d auf den größten gemeinsamen Teiler
	 * von a und b.
	 * @param x Diese Methode setzt x auf das x der oben erläuterten
	 * Linearkombination von  {ax + by | x, y ∈ Z}.
	 * @param y Diese Methode setzt y auf das y der oben erläuterten
	 * Linearkombination von {ax + by | x, y ∈ Z}.
	 * @return True, wenn d = 1, also wenn gcd(a,b)=1, sonst False.
	 */
	bool EEA(const Integer& a, const Integer& b, Integer& d,
			Integer& x, Integer& y);

	/**
	 * modularExponentation implementiert die modulare Exponentation. Dabei
	 * wird die Funktion a^b mod n berechnet und das Ergebnis wird über
	 * den Rückgabewert als Integer zurückgelifert.
	 *
	 * @param a a für a^b mod n als Integer.
	 * @param b b für a^b mod n als Integer.
	 * @param n c für a^b mod n als Integer.
	 * @return Das Ergebnis aus a^b mod n als Integer.
	 */
	Integer modularExponentation(const Integer& a, const Integer& b,
			const Integer& n);

	/**
	 * multInverse berechnet das multiplikative Inverse von a modulo n.
	 *
	 * @param a Das a, für welches das multiplikative Inverse a mod n berechnet
	 * wird. a ist ein Integer.
	 * @param n Das n, für welches das multiplikative Inverse a mod n berechnet
	 * wird. n ist ein Integer.
	 * @param a_inv a_inv ist ein Integer, in welchem diese Funktion das
	 * multiplikative Inverse von a modulo n speichert, wenn ein solches
	 * multiplikatives Inverses existiert.
	 * @return True, wenn ein multiplikatives Inverses von a mod n  existiert.
	 * False, wenn nicht.
	 */
	bool multInverse(const Integer& a, const Integer& n, Integer& a_inv);

    /**
     * Der Algorithmus witness ist eine abgewandelte Form der modularen Exponentiation, die für die übergebenen
     * Parameter a und n den Term  a^(n-1) (mod n) berechnet. Diese Berechnung soll den Satz von Fermat überprüfen.
     * Der Satz
     * von Fermat sagt aus: Ist n eine Primzahl, dann gilt für alle a in Z Stern n, dass: a^(n-1) kongruent zu 1
     * (mod n). Wenn das Ergebnis dieser Berechnung ungleich 1 ist, dann ist die Zahl definitiv keine Primzahl und a
     * kann als Solider Beweis dafür angesehen werden, dass n keine Primzahl ist. Ein solider Beweis ist eine Zahl a
     * in Z Stern n, die beweist, dass n eine zusammengesetzte Zahl ist. Sei n also eine beliebige ungerade natürliche
     * Zahl, die größer 1 ist. Dann werden r und u so gewählt, dass n-1 = 2^r * u ist. Dann ist a ein solider Beweis,
     * falls: 1. a^u nicht kongruent zu +1 oder -1 (modulo n) und 2. Für alle i=1 bis r-1 gilt: a ^(2^i * u) ist
     * nicht kongruent zu -1 (modulo n). Wenn n eine beliebige, zusammengesetzte Zahl ist, die keine Primzahlpotenz ist,
     * dann sind mindestens die Hälfte aller Zahlen in Z Stern n solide Beweise dafür, dass n eine zusammengesetzte
     * Zahl ist. Ist das Ergebnis der Berechung des Fermat-Kriteriums a^(n-1) (mod n) jedoch gleich 1,
     * dann ist es möglich, dass die Zahl eine Primzahl ist. Es existieren jedoch auch Carmichael Zahlen. Für diese
     * seltenen, zusammengesetzten Zahlen gilt: Sei c eine Charmichael Zahl, dann gilt für alle a in Z Stern c, dass
     * a^(n-1) kongruent zu 1 (modulo n). Deshalb reicht der Satz von Fermat allein nicht aus, um eine Primzahl
     * zu erkennen.
     * Deshalb wird während der Berechnung von a^(n-1) parallel dabei nach einer Lösung für die Gleichung x^2 kongruent
     * zu 1 (mod n) gesucht, die verschieden zu 1 und -1 (also n-1) ist. Wird eine solche Zahl gefunden, dann ist a ein
     * Solider Beweis dafür, dass n keine Primzahl ist. Diese Schlussfolgerung ist möglich, da wenn n eine
     * Primzahl ungleich 2 ist, dann sind 1 und -1 die einzigen Quadratwurzeln von 1 (mod n) in Z Stern n.
     * @param a Zahl in Z Stern n, für die überprüft wird, ob sie ein soldier Beweis dafür ist, dass n eine
     *  zusammengesetzte Zahl ist
     * @param n Zahl, für die getestet werden soll, ob sie eine Primzahl ist.
     * @return True, wenn n keine Primzahl ist (a also ein Solider Beweis ist), ansonsten False
     */
	bool witness(const Integer& a, const Integer& n);

    /**
     * Die Methode millerRabinTest() implementiert den Miller-Rabin Primzahltest. Dieser Algorithmus ist ein Monte Carlo
     * Algorithmus. Ein Monte Carlo Algorithmus ist ein Algorithmus, der folgende Eigenschaften besitzt: Der
     * Algorithmus bekommt als Eingabe eine beliebig lange Bitfolge und gibt als Ergebnis 0 oder 1 zurück; der
     * Algorithmus hat Zugriff auf einen Zufallsgenerator, der Zufallsbits zurückliefert, die gleichverteilt sind; der
     * Algorithmus berechnet das korrekte Ergebnis, mit der Wahrscheinlichkeit von (1/2) + Ɛ, wobei Ɛ > 0.
     * Der Miller-Rabin Primzahltest besitzt einen einseitigen Fehler. Wird eine Zahl von ihm als eine zusammengesetzte
     * Zahl eingestuft, dann ist das Ergebnis zu 100 Prozent korrekt. Wird eine Zahl jedoch als Primzahl erkannt, dann
     * ist das Ergebnis mit einer Wahrscheinlichkeit von 1-2^(-s) korrekt. Diese Fehlerwahrscheinlichkeit wird durch
     * den Qualitätsparameter s beeiflusst, der angibt, wie viele zufällige Zahlen zwischen 1 und n-1 erzeugt werden
     * und daraufhin überprüft werden, ob sie ein Solider Beweis sind, dass n eine zusammengesetzte Zahl ist.Die
     * Laufzeit des Miller-Rabin Primzahltests beträgt O(log₂(n)³ * s). Eine Vorraussetzung für den Einsatz des
     * Miller-Rabin Primzahltests sind kryptographisch gute Zufallszahlen.
     * @param n Integer, für den untersucht werden soll, ob es sich um eine Primzahl oder um eine zusammengesetze
     * Zahl handelt
     * @param s Qualitätsparameter s, der angibt, wie oft die Schleife durchlaufen werden soll
     * @return True, wenn die Zahl eine Primzahl ist, False wenn die Zahl eine zusammengesetze Zahl ist
     */
	bool millerRabinTest(Integer& n, unsigned int s);

	Integer randomInteger(const Integer& n);

	unsigned int randomPrime(Integer &p, unsigned int bitlen, unsigned int s);

	/**
	 * randomRabinPrime generiert mit Fehlerwahrscheinlichkeit 2^-s eine
	 * Zufallsprimzahl mit folgenden Eigenschaften:
	 * - Die Binärdarstellung von p hat höchstens bitlen Bits.
	 * - p ist eine Rabin-Primzahl, ð, es gilt p ≡ 3 (mod 4).
	 * und speichert diese in p ab.
	 *
	 * @param p In p wird die generierte Rabin Primzahl abgespeichert.
	 * @param bitlen bitlen gibt die maximale Größte der zu erzeugenden Primzahl
	 * in Bits an.
	 * @param s s ist das s in 2^-s, welches die Fehlerwahrscheinlichkeit
	 * angibt, dass der zurückgeliferte Wert in p keine Primzahl ist.
	 * @return Es wird immer 0 zurückgelifert.
	 */
	unsigned int randomRabinPrime(Integer &p, unsigned int bitlen,
			unsigned int s);

	/**
	 * modPrimeSqrt berechnet die zwei Quadratwurzeln von y mod p, falls p
	 * kongruent zu 3 (mod 4) ist.
	 *
	 * @param y Das y bei y mod p, für welches die Quadratwurzeln berechnet
	 * werden. y ist ein Integer.
	 * @param p Das p bei y mod p, für welches die Quadratwurzeln berechnet
	 * werden. p ist ein Integer. Es werden nur Quadratwurzeln berechnet, wenn
	 * gilt, dass p kongruent zu 3 (mod 4) ist. Für ein richtiges Ergebnis muss
	 * p eine Primzahl sein.
	 * @param v v ist ein Vector aus Integern, in welchem die zwei
	 * Quadratwurzeln von y mod p gespeichert werden, falls p
	 * kongruent zu 3 (mod 4) ist.
	 *
	 * @return True, falls die Quadratwurzeln berechnet wurden, sonst False.
	 * Die Quadratwurzeln können von dieser Funktion berechnet werden, falls
	 * p kongruent zu 3 (mod 4) ist.
	 */
	bool modPrimeSqrt(const Integer& y, const Integer& p, vector<Integer>& v);

	bool sqrt(const Integer& x, Integer& s) const;

	void generateRSAParams(Integer& p, Integer& q, Integer& e, Integer& d,
			unsigned int bitlen = 256, unsigned int s = 30);
};

#endif
