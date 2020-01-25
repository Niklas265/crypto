/*
 * RSAAttack.h
 */

#ifndef RSAATTACK_H_
#define RSAATTACK_H_

#include "integer.h"
#include "RSAOracle.h"

using namespace CryptoPP;

/**
 * TODO
 */
class RSAAttack {
public:
	RSAAttack();
	virtual ~RSAAttack();

	/***
	 * factorizeN berechnet die Faktoren p und q von n anhand von ϕ(n). Das
	 * bedeutet, dass wenn ϕ(n) bekannt ist, n gebrochen werden kann. n ist
	 * Teil des öffentlichen Schlüssels.
	 * Für die Attacke muss n und ϕ(n) von n bekannt sein.
     * (1) n = p * q und (2) ϕ(n) = (p-1) * (q-1).
     * Mit p und q kann RSA gebrochen werden, also es können verschlüsselte
     * Nachrichten entschlüsselt werden.
     * Indem man Gleichung (1) in Gleichung (2) mit p = n / q einsetzt:
     * ϕ(n) = (p-1) * (n/q - 1)
     * Diese Gleichung kann in eine Form gebracht werden, um p und q mit der
     * Mitternachtsformel zu berechnen:
     * 0 = p² + (ϕ(n) − n − 1)p + n
     * Bei der Mitternachtsformel ist a = 1, b = ϕ(n) - n - 1 und c = n
	 * @param n n ist ein Integer und Teil des öffentlichen zu brechenden
	 * Schlüssels.
	 * @param phi_n  ϕ(n), wobei  ϕ(n) = (p-1)*(q-1) ist.  ϕ(n) kann ohne
	 * Kenntniss von p und q nicht effizient berechnet werden.
	 * @param p In p wird bei Erfolg ein Faktor von n als Integer geschrieben.
	 * @param q In q wird bei Erfolg der andere Faktor von n als Integer
	 * geschrieben.
	 * @return True, wenn erfolgreich n faktorisiert wurde, false wenn nicht.
	 */
	bool factorizeN(const Integer& n, const Integer& phi_n, Integer& p, Integer& q) const;

	bool wienerAttack(const Integer& n, const Integer& e, Integer& p, Integer& q) const;

	Integer halfAttack(const Integer& n, const Integer& e, const Integer& y, RSAOracle& rsa_oracle) const;

};

#endif /* RSAATTACK_H_ */
