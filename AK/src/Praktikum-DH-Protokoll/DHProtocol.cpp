/*
 * DHProtocol.cpp
 */
#include <cassert>
#include <fstream>
#include <socketft.h>
#include "FortunaGenerator.h"
#include "MessageEncoder.h"
#include "DHProtocol.h"

using namespace std;
using namespace CryptoPP;

#define BUFFERSIZE 20000

DHProtocol::DHProtocol(const string& param_file) {
	ifstream file;
	string data;
	MessageEncoder encoder;

	file.open(param_file.c_str());
	if (file.is_open() == false) {
		cout << "Could not open file: " << param_file << endl;
		assert(false);
	}

	file >> data;
	file.close();

	if (encoder.decode(data) == false) {
		cout << "Could not decode DH parameters." << endl;
		assert(false);
	}

	if (encoder.size() != 3) {
		cout << "Wrong number of DH parameters." << endl;
		assert(false);
	}

	q = encoder.getInteger(0);
	p = encoder.getInteger(1);
	alpha = encoder.getInteger(2);

	/*
	 cout << "q = " << q << endl;
	 cout << "p = " << p << endl;
	 cout << "alpha = " << alpha << endl;
	 */

}

void DHProtocol::alice(const string& host, unsigned int port) {
}

void DHProtocol::bob(unsigned int port) {
	cout << "Acting as Bob." << endl;

	Socket server, client;
	int len;

	server.Create();
	server.Bind(port);
	server.Listen();

	while (true) {
		try {

			if (server.Accept(client) == true) {
				serverBob(client);
			}
		} catch (Socket::Err& err) {
			cout << "Fehler: " << err.GetWhat() << endl;
		}
	}
	server.CloseSocket();
}

bool DHProtocol::serverBob(Socket client) {
}

void DHProtocol::oscar(unsigned int inport, const string& host, unsigned int port) {
	cout << "Acting as Oscar." << endl;

	Socket server, client;
	int len;

	server.Create();
	server.Bind(inport);
	server.Listen();

	while (true) {
		try {

			if (server.Accept(client) == true) {
				serverOscar(client, host, port);
			}
		} catch (Socket::Err& err) {
			cout << "Error: " << err.GetWhat() << endl;
		}
	}
	server.CloseSocket();
}

bool DHProtocol::serverOscar(Socket alice, const string& host, unsigned int port) {
}

