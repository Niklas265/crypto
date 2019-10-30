#include <cassert>
#include <iostream>
#include <string>
#include "filters.h" // StringSink
#include "hex.h" // HexEncoder
#include "sha.h"
#include "aes.h"
#include "modes.h"
#include "base64.h"
#include "files.h" // FileSink
#include "osrng.h" // BlockingRNG
#include "integer.h" // Integer
#include "nbtheory.h" // RabinMillerTest
#include "BlumBlumShubGenerator.h"
#include "../Praktikum-CMake/Polybius/Toolbox.h"

#include <math.h>

using namespace std;
using namespace CryptoPP;

string hexToASCII(string src) {
    string dst;
    StringSource(src, true, new HexDecoder(new StringSink(dst)));
    return dst;
}
 
void DataFlowExercise() {
    /*************************************************************************
     * Aufgabe 2.
     *************************************************************************/
    cout << endl
         << "Data Flow Exercise:" << endl
         << "===================" << endl << endl;
 
    string src = "Kryptographie macht Spaß!!!";

    try {
        string dst;
        StringSink* sink = new StringSink(dst);

        HexEncoder* encoder = new HexEncoder(sink);

        StringSource(src, true, encoder);

        cout << dst << " -> " << hexToASCII(dst) << endl;
        assert(src == hexToASCII(dst));

        src = "4963682062696E2065696E20537472696E6720696E204865786164657A696D616C6B6F64696572756E6721";
        cout << hexToASCII(src) << endl;

        // modified readFromFile a bit
        // 3rd arg is a bool called skipNewlines. setting skipNewlines to true skips newlines in the output
        Toolbox::readFromFile("../../data/base64data.txt", src, /*skipNewlines=*/true);

        cout << src << endl;

        dst = "";

        StringSource(src, true,
            new Base64Decoder(
                new StringSink(dst)
            )
        );

        cout << dst << endl;
    } catch(const Exception& e) {
        cerr << e.what() << endl;
        assert(false);
    }
}

void EncryptionExercise_Part1() {
    byte key[16];
    string encoded_key = "08a8cbfe7a3d1262c8abc3d1197dfefe";
    StringSource(encoded_key, true,
        new HexDecoder(
            new ArraySink(key, 16)
        )
    );

    string plain_text = "Streng geheime Botschaft";
    string cypher_text;

    ECB_Mode<AES>::Encryption aes_enc;

    aes_enc.SetKey(key, sizeof(key));

    StringSource(plain_text, true,
        new StreamTransformationFilter(aes_enc,
            new HexEncoder(
                new StringSink(cypher_text)
            )
        )
    );

    ECB_Mode<AES>::Decryption aes_dec;

    aes_dec.SetKey(key, sizeof(key));

    string p;

    StringSource(cypher_text, true,
        new HexDecoder(
            new StreamTransformationFilter(aes_dec,
                new StringSink(p)
            )
        )
    );

    cout << p << endl;
}

void EncryptionExercise_Part2() {
    string cypher_text;

    string src;
    Toolbox::readFromFile("../../data/aescipher.txt", src, true);

    StringSource(src, true,
        new Base64Decoder(
            new StringSink(cypher_text)
        )
    );

    cout << cypher_text << endl;

    byte key[16];
    string encoded_key = "47656865696D65725363686CC3BC7373";
    StringSource(encoded_key, true,
        new HexDecoder(
            new ArraySink(key, 16)
        )
    );

    byte iv[16];
    string encoded_iv = "496E697469616C6973696572756E6773";
    StringSource(encoded_iv, true,
         new HexDecoder(
             new ArraySink(iv, 16)
         )
    );

    CTR_Mode<AES>::Decryption aes_crt_dec;

    aes_crt_dec.SetKeyWithIV(key, sizeof(key), iv);

    string p;
    StringSource(cypher_text, true,
        new StreamTransformationFilter(aes_crt_dec,
             new StringSink(p)
        )
    );

    cout << p << endl;
}
 
void EncryptionExercise() {
    /*************************************************************************
     * Aufgabe 3.
     *************************************************************************/
    cout << endl
         << "Encryption Exercise:" << endl
         << "====================" << endl
         << endl;

    try {
        // 3a) und 3b)
        EncryptionExercise_Part1();

        // 3c)
        EncryptionExercise_Part2();
    } catch(const Exception& e) {
        cerr << e.what() << endl;
        assert(false);
    }
}
 
 
void HashExercise() {
    /*************************************************************************
     * Aufgabe 4.
     *************************************************************************/
    cout << endl
         << "Hash Exercise:" << endl
         << "==============" << endl
         << endl;
 
    string message = "Kryptographie macht immer noch Spaß!!!";
 
    string digest;
    SHA256 hash;
    StringSource(message, true,
        new HashFilter(hash,
            new StringSink(digest)
        )
    );

    word32 flags = HashVerificationFilter::HASH_AT_BEGIN
                   | HashVerificationFilter::PUT_RESULT;

    bool result;
    hash.Restart();
    StringSource(digest + message, true,
        new HashVerificationFilter(hash,
            new ArraySink((byte*)&result, sizeof(result)),
                flags
        )
    );

    assert(result);

    // b: result is false

    // c: HASH_AT_END
}
 
 
Integer modexp(const Integer &a, const Integer &b, const Integer &n) {
    /*************************************************************************
     * Aufgabe 5a.
     *************************************************************************/
    Integer d("1");

    for (int i = b.BitCount()-1; i >= 0; i--) {
        d = (d * d) % n;
        if (b.GetBit(i) == 1) {
            d = (d * a) % n;
        }
    }
 
    return d;
}
 
 
void IntegerExercise() {
    /*************************************************************************
     * Aufgabe 5bc.
     *************************************************************************/
    cout << endl
         << "IntegerExercise:" << endl
         << "================" << endl << endl;

    Integer res = modexp(Integer(371), Integer(18961551), Integer(2371));

    Integer res2 = a_exp_b_mod_c(Integer(371), Integer(18961551), Integer(2371));

    assert(res == res2);
}

void RNGExercise() {
    /*************************************************************************
     * Aufgabe 6cfhj.
     *************************************************************************/
    cout << endl
         << "RNGExercise:" << endl
         << "============" << endl << endl;

    Integer n = Integer("1252910265243849922375596598575099209083498535192739493227403") *
                Integer("1476222059624949757818143837507324048590620075519516306265283");

    BlumBlumShubGenerator bbs(n, Integer(42));

    for (int i = 0; i < 20; i++) {
        cout << i << ": " << int(bbs.getBit()) << endl;
    }

    for (int i = 0; i < 5; i++) {
        cout << i << ": " << int(bbs.getByte()) << endl;
    }

    byte array[100];

    bbs.getBlock(array, 100);

    cout << (int)array[99] << endl;

    cout << bbs.getInteger(200) << endl;


    Integer sum;

    for (int i = 0; i < 10000; i++)
        sum += bbs.getInteger(Integer(25));

    cout << sum / 10000 << endl;

    cout << bbs.getInteger(Integer(pow(2, 30))) << endl;
}
 
 
int main() {
    DataFlowExercise();
    EncryptionExercise();
    HashExercise();
    IntegerExercise();
    RNGExercise();
 
    return 0;
}