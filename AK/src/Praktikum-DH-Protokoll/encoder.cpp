/*
 * encoder.cpp
 */
#include <iostream>
#include "MessageEncoder.h"
#include <integer.h>

using namespace std;
using namespace CryptoPP;

void encoderTest() {
	cout << "MessageEncoderTest" << endl <<
			"==================" << endl;

	MessageEncoder me;

    me.append(Integer("10892"));
    me.append("Hallo Welt!");
    me.append(Integer("459111231"));

    cout << "Encoded Data: " << me.encode() << endl;

    assert(me.encode() == "2A8C#48616C6C6F2057656C7421#1B5D7B3F");

    me.decode(me.encode());

    Integer i = me.getInteger(0);
    string s = me.getString(1);
    Integer j = me.getInteger(2);

    assert(i == Integer("10892"));
    assert(s == "Hallo Welt!");
    assert(j == Integer("459111231"));

    me.clear();

    me.append(Integer("98921323668113423232"));
    me.append("Kryptografie macht Spass!");
    me.append("Crypto++ ist eine Kryptografie-Bibliothek");
    me.append(Integer("456291260721096"));

    cout << "Encoded Data: " << me.encode() << endl;

    me.decode("6DB03BF70E#4963682062696E2065696E65205A65696368656E666F6C67652C20646965#207369636820617573207A776569205465696C656E207A7573616D6D656E7365747A74");

    cout << me.getInteger(0) << endl;
    cout << me.getString(1) << endl;
    cout << me.getString(2) << endl;

    cout << me.verify("ahVoh4naghie", "937A6984EE71C06E808AB996BF76C246AC29E198B6637B02C37ABDE92D6A46C8") << endl;
    assert(me.verify("ahVoh4naghie", "937A6984EE71C06E808AB996BF76C246AC29E198B6637B02C37ABDE92D6A46C8"));
}

int main(int argc, char **argv) {
	encoderTest();
	return 0;
}
