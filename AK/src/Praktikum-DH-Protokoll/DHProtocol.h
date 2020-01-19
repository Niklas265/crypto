/*
 * DHProtocol.h
 */

#ifndef DHPROTOCOL_H_
#define DHPROTOCOL_H_

#include <iostream>
#include <integer.h>
#include <socketft.h>

using namespace std;
using namespace CryptoPP;

class DHProtocol {
protected:
	/*
	 *  Module of the Diffie-Hellman key exchange (must be prime)
	 */
	Integer p;
	/*
	 * Prime q such that p = 2*q + 1
	 */
	Integer q;
	/*
	 * Generator of Z^*_p
	 */
	Integer alpha;

	bool serverBob(Socket c);

	bool serverOscar(Socket c, const string& host, unsigned int port);

public:
	DHProtocol(const string& param_file);

	void alice(const string& host, unsigned int port);

	void bob(unsigned int port);

	void oscar(unsigned int inport, const string& host, unsigned int port);
};

#endif /* DHPROTOCOL_H_ */
