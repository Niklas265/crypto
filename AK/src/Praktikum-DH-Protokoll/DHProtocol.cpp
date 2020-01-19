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
    // Generate ya Integer
    FortunaGenerator f;
    Integer a = f.getInteger(p);
    Integer ya = a_exp_b_mod_c(alpha, a, p);

    // Encode ya Integer into String
    MessageEncoder messageEncoder;
    messageEncoder.append(ya);

    // Create Socket for Bob
    Socket client;
    client.Create();
    client.Connect(host.c_str(), port);

    // Send ya String to Bob
    string yaEncoded = messageEncoder.getString(0);
    client.Send((byte*) &(yaEncoded[0]), yaEncoded.size());

    // Recv yb String from Bob
    byte* buffer = new byte[BUFFERSIZE];
    int len = client.Receive(buffer, BUFFERSIZE);
    // Decode yb String to yb Integer
    Integer yb;
    yb.Decode(buffer, len);

    // calculate shared key
    Integer ka = a_exp_b_mod_c(yb, a, p);

    cout << "ka = " << ka << endl;
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
    // Generate yb Integer
    FortunaGenerator f;
    Integer b = f.getInteger(p);
    Integer yb = a_exp_b_mod_c(alpha, b, p);

    // Encode yb Integer into String
    MessageEncoder messageEncoder;
    messageEncoder.append(yb);

    // Send yb String to Alice
    string ybEncoded = messageEncoder.getString(0);
    client.Send((byte*) &(ybEncoded[0]), ybEncoded.size());

    // Recv ya String from Alice
    byte* buffer = new byte[BUFFERSIZE];
    int len = client.Receive(buffer, BUFFERSIZE);
    // Decode ya String to ya Integer
    Integer ya;
    ya.Decode(buffer, len);

    // calculate shared key
    Integer kb = a_exp_b_mod_c(ya, b, p);

    cout << "kb = " << kb << endl;
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
    // (param) host und (param) port sind für bob's server

    FortunaGenerator f;

    // Setup for Alice ---------------------------------------------------------

    // Generate yb Integer for alice
    Integer b = f.getInteger(p);
    Integer yb = a_exp_b_mod_c(alpha, b, p);

    // Encode yb Integer into String
    MessageEncoder messageEncoder;
    messageEncoder.append(yb);

    // Send yb String to Alice
    string ybEncoded = messageEncoder.getString(0);
    alice.Send((byte*) &(ybEncoded[0]), ybEncoded.size());

    // Recv ya String from Alice
    byte* buffer = new byte[BUFFERSIZE];
    int len = alice.Receive(buffer, BUFFERSIZE);
    // Decode ya String to ya Integer
    Integer ya_alice;
    ya_alice.Decode(buffer, len);

    // calculate shared key
    Integer kb_alice = a_exp_b_mod_c(ya_alice, b, p);

    cout << "kb_alice = " << kb_alice << endl;


    // Setup for Bob -----------------------------------------------------------

    // Generate ya Integer
    Integer a = f.getInteger(p);
    Integer ya = a_exp_b_mod_c(alpha, a, p);

    // Encode ya Integer into String
    messageEncoder.clear();
    messageEncoder.append(ya);

    // Create Socket for Bob
    Socket client;
    client.Create();
    client.Connect(host.c_str(), port);

    // Send ya String to Bob
    string yaEncoded = messageEncoder.getString(0);
    client.Send((byte*) &(yaEncoded[0]), yaEncoded.size());

    // Recv yb String from Bob
    len = client.Receive(buffer, BUFFERSIZE);
    // Decode yb String to yb Integer
    Integer yb_bob;
    yb_bob.Decode(buffer, len);

    // calculate shared key
    Integer ka_bob = a_exp_b_mod_c(yb_bob, a, p);

    cout << "ka_bob = " << ka_bob << endl;

}

