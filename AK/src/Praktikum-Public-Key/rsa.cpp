/*
 * rsa.cpp
 */

#include <iostream>
#include "PublicKeyAlgorithmBox.h"
#include "RSAEncryptor.h"
#include "RSADecryptor.h"
#include "RSAAttack.h"
#include "RSAOracle.h"

using namespace std;

// #rsaParameters()
void rsaParameters() {
	/*********************************************************************
	 * Aufgabe 15.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	// p q e und d werden von der Methode berechnet.
	// p q und d bilden den privaten Schlüssel. e ist Teil des öffentlichen
	// Schlüssels. Das n, der zweite Teil des öffentlichen Schlüssels, kann
	// effizient mit p * q berechnet werden.
	Integer p, q, e, d;
	// 256 ist die bitlen, die maximale Bitgröße der zu generierenden
	// Primzahlen p und q.
	// 30 ist der Qualitätsparameter s. s ist das s in 1-2^-s, welches die
	// Fehlerwahrscheinlichkeit dafür angibt, dass p keine Primzahl ist. Das
	// gleiche gilt für q.
	pb.generateRSAParams(p, q, e, d, 256, 30);
	// e ∈ {1, 2, . . . , ϕ(n) − 1} muss wahr sein.
	Integer phiN = (p-1)*(q-1);
	assert(e >= 1 && e <= phiN-1);
	// e und ϕ(n) müssen teilerfremd sein
	assert(Integer::Gcd(e, phiN) == 1);
	// d = e^−1 mod ϕ(n) muss wahr sein.
    pb.multInverse(e, phiN, d);
	assert((e * d) % phiN == 1);
    cout << "p: " << p << " q: " << q << " e: " << e << " d: " << d << endl;
}

// #rsaDemo()
void rsaDemo() {
	/*********************************************************************
	 * Aufgabe 16.
	 *********************************************************************/

	// Der RSA Schlüssel für das Beispiel ist vorgegeben.
    // p q und d bilden den privaten Schlüssel. e ist Teil des öffentlichen
    // Schlüssels. Das n, der zweite Teil des öffentlichen Schlüssels, kann
    // effizient mit p * q berechnet werden.
	Integer p = Integer("15192846618168946907");
    Integer q = Integer("10041530829891794273");
    Integer e = Integer("141290156426204318982571851806193576543");
    Integer d = Integer("73707354481439936713886319521045114527");

    // x ist der zu verschlüsselnde Integer.
    Integer x = Integer("79372353861768787619084471254314002875");
    // Es wird erwartet, dass x mit dem obigen Schlüssel zu y verschlüsselt
    // wird. Es wird ebenfalls erwartet, dass y entschlüsselt wieder x ergibt.
    Integer y = Integer("47915958473033255778832465116435774510");

    // Die gleichen Checks aus der rsaParameters Funktion von oben.
    Integer phiN = (p-1)*(q-1);
    assert(e >= 1 && e <= phiN-1);
    assert(Integer::Gcd(e, phiN) == 1);
    PublicKeyAlgorithmBox pb;
    pb.multInverse(e, phiN, d);
    assert((e * d) % phiN == 1);

    // RSAEncryptor bietet Methoden an, um eine Zahl mit dem RSA Kryptosystem
    // zu verschlüsseln.
    RSAEncryptor rsaEncryptor(p * q, e);
    Integer outy;
    // Verschlüsselt die Zahl x mit dem öffentlichen Schlüssel (n, e) und
    // speichert das Ergebnis in outy ab.
    rsaEncryptor.compute(x, outy);
    // Es wird erwartet, dass x mit dem obigen Schlüssel zu y verschlüsselt
    // wird.
    cout << "RSA Verschlüsselung: " << x << " > " << outy << endl;
    assert(outy == y);

    // RSADecryptor bietet Methoden an, um eine Zahl mit dem RSA Kryptosystem
    // zu entschlüsseln. Es wird erwartet, dass alle drei Methoden die
    // verschlüsselte Zahl zur ursprünglichen, entschlüsselten Zahl x
    // entschlüsseln.
    RSADecryptor rsaDecryptor(p, q, d);
    Integer outx, outgarner, outcrt;
    rsaDecryptor.compute(outy, outx);
    cout << "RSA Entschlüsselung: " << outy << " --compute--> " << outx << endl;
    assert(outx == x);
    rsaDecryptor.crt(outy, outcrt);
    cout << "RSA Entschlüsselung: " << outy << " --CRT--> " << outcrt << endl;
    assert(outcrt == x);
    rsaDecryptor.garner(outy, outgarner);
    cout << "RSA Entschlüsselung: " << outy << " --Garner--> " << outgarner << endl;
    assert(outgarner == x);
}

//#sqrtExercise()
void sqrtExercise() {
	/*********************************************************************
	 * Aufgabe 17.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	Integer s;
	// Suche Kleinste nicht negative Quadratwurzel s mit
	// s² = 3157242151326374471752634944
	pb.sqrt(Integer("3157242151326374471752634944"), s);
	cout << "Kleinste nicht negative Quadratwurzel von 3157242151326374471752634944: "
	     << s << endl;
	assert(s == Integer("56189341972712"));
    // Suche Kleinste nicht negative Quadratwurzel s mit
    // s² = 11175843681943819792704729
    pb.sqrt(Integer("11175843681943819792704729"), s);
    cout << "Kleinste nicht negative Quadratwurzel von 11175843681943819792704729: "
         << s << endl;
    assert(s == Integer("3343029117723"));
    // Suche Kleinste nicht negative Quadratwurzel s mit
    // s² = 3343229819990029117723. Es wird erwartet, dass eine solche Zahl s
    // nicht existiert.
    assert(!pb.sqrt(Integer("3343229819990029117723"), s));
    cout << "Es wird erwartet, dass es keine Kleinste nicht negative "
         << "Quadratwurzel von 3343229819990029117723 gibt." << endl;
}

// #factorizingAttack()
void factorizingAttack() {
	/*********************************************************************
	 * Aufgabe 18.
	 *********************************************************************/

	// Beispiel für die Attacke auf RSA, bei der das ϕ(n) bekannt ist.
	RSAAttack rsaAttack;
    Integer p, q;
    // n ist das erste Argument, ϕ(n) das zweite.
    // Bei Erfolg liefert factorizeN die Faktoren p und q von n im 3. und 4.
    // Parameter zurück.
    rsaAttack.factorizeN(Integer("127869459623070904125109742803085324131"),
                         Integer("127869459623070904102412837477002840200"),
                         p, q);
    // Es wird erwartet, dass n erfolgreich faktorisiert werden konnte.
    assert(p * q == Integer("127869459623070904125109742803085324131"));
}

// #euklidExercise()
void euklidExercise() {
	/*********************************************************************
	 * Aufgabe 19.
	 *********************************************************************/
    PublicKeyAlgorithmBox pb;
    vector<Integer> q;
    Integer d = pb.euklid(39, 112, q);
    assert(d == 1);
    vector<Integer> expectedResult = {Integer("0"), Integer("2"), Integer("1"),
                                   Integer("6"), Integer("1"), Integer("4")};
    assert(q == expectedResult);
}

// #convergentsExercise()
void convergentsExercise() {
	/*********************************************************************
	 * Aufgabe 20.
	 *********************************************************************/

    PublicKeyAlgorithmBox pb;
    vector<Integer> c, d;
    pb.computeConvergents(Integer(39), Integer(112), c, d);
    vector<Integer> expectedC = {1, 0, 1, 1, 7, 8, 39};
    vector<Integer> expectedD = {0, 1, 2, 3, 20, 23, 112};
    assert(c == expectedC);
    assert(d == expectedD);
}

// #wienerAttack()
void wienerAttack() {
	/*********************************************************************
	 * Aufgabe 21.
	 *********************************************************************/

	RSAAttack rsaAttack;
	Integer p, q;
	rsaAttack.wienerAttack(Integer("224497286580947090363360894377508023561"),
	                       Integer("163745652718951887142293581189022709093"),
	                       p, q);
	assert(p * q == Integer("224497286580947090363360894377508023561"));
}

// #oracleExercise()
void oracleExercise() {
	/*********************************************************************
	 * Aufgabe 22.
	 *********************************************************************/

	Integer p = Integer("16015510136412338011");
	Integer q = Integer("12177032305856164321");
	Integer d = Integer("946975621");

	RSAOracle rsaOracle(p, q, d);
	assert(rsaOracle.half(Integer("116415012259126332853105614449093205668")));
    assert(!rsaOracle.half(Integer("74304303162215663057995326922844871006")));
    assert(!rsaOracle.half(Integer("102949691974634609941445904667722882083")));
    assert(rsaOracle.half(Integer("42549620926959222864355800078420537413")));
}

// #halfAttack()
void halfAttack() {
	/*********************************************************************
	 * Aufgabe 23.
	 *********************************************************************/

	Integer p = Integer("12889769717276679053");
	Integer q = Integer("17322528238664264177");
	Integer e = Integer("55051594731967684255289987977028610689");
    Integer d = Integer("149154082258429024247010774747829057473");
    Integer x = Integer("167092961114842952923160287194683529938");

	RSAAttack rsaAttack;
	RSAOracle rsaOracle(p, q, d);
	Integer result = rsaAttack.halfAttack(p * q, e, x, rsaOracle);

	RSADecryptor rsaDecryptor(p, q, d);
	Integer y;
	rsaDecryptor.compute(x, y);
    assert(y == result);
    cout << "Decrypted Message: " << result << endl;
}

// #main()
int main() {
	rsaParameters();
	rsaDemo();
	sqrtExercise();
	factorizingAttack();
	euklidExercise();
	convergentsExercise();
	wienerAttack();
	oracleExercise();
	halfAttack();
	return 0;
}

