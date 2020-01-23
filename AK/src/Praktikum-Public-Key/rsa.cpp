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
	Integer p, q, e, d;
	pb.generateRSAParams(p, q, e, d, 256, 30);
	Integer phiN = (p-1)*(q-1);
	assert(e >= 1 && e <= phiN-1);
	assert(Integer::Gcd(e, phiN) == 1);
    pb.multInverse(e, phiN, d);
	assert((e * d) % phiN == 1);
    cout << "p: " << p << " q: " << q << " e: " << e << " d: " << d << endl;
}

// #rsaDemo()
void rsaDemo() {
	/*********************************************************************
	 * Aufgabe 16.
	 *********************************************************************/

	Integer p = Integer("15192846618168946907");
    Integer q = Integer("10041530829891794273");
    Integer e = Integer("141290156426204318982571851806193576543");
    Integer d = Integer("73707354481439936713886319521045114527");
    Integer x = Integer("79372353861768787619084471254314002875");
    Integer y = Integer("47915958473033255778832465116435774510");

    Integer phiN = (p-1)*(q-1);
    assert(e >= 1 && e <= phiN-1);
    assert(Integer::Gcd(e, phiN) == 1);
    PublicKeyAlgorithmBox pb;
    pb.multInverse(e, phiN, d);
    assert((e * d) % phiN == 1);

    RSAEncryptor rsaEncryptor(p * q, e);
    Integer outy;
    rsaEncryptor.compute(x, outy);
    assert(outy == y);

    RSADecryptor rsaDecryptor(p, q, d);
    Integer outx, outgarner, outcrt;
    rsaDecryptor.compute(outy, outx);
    cout << outx << endl;
    assert(outx == x);
    rsaDecryptor.crt(outy, outcrt);
    cout << "CRT " << outcrt << endl;
    assert(outcrt == x);
    rsaDecryptor.garner(outy, outgarner);
    assert(outgarner == x);
}

//#sqrtExercise()
void sqrtExercise() {
	/*********************************************************************
	 * Aufgabe 17.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	Integer s;
	pb.sqrt(Integer("3157242151326374471752634944"), s);
	assert(s == Integer("56189341972712"));
    pb.sqrt(Integer("11175843681943819792704729"), s);
    assert(s == Integer("3343029117723"));
    assert(!pb.sqrt(Integer("3343229819990029117723"), s));
}

// #factorizingAttack()
void factorizingAttack() {
	/*********************************************************************
	 * Aufgabe 18.
	 *********************************************************************/

	RSAAttack rsaAttack;
    Integer p, q;
    rsaAttack.factorizeN(Integer("127869459623070904125109742803085324131"),
                         Integer("127869459623070904102412837477002840200"),
                         p, q);
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

