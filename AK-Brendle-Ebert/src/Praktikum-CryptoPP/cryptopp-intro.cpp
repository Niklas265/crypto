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

// Convertiert/Encodiert Bytes in (param) src und gibt 
// den zur Basis 16 encodierten Hexstring zurück.
string toHex(string src) {
    string dst;
    StringSink* sink = new StringSink(dst);
    HexEncoder* encoder = new HexEncoder(sink);
    StringSource(src, true, encoder);
    return dst;
}

// Convertiert/Decodiert den Hexstring in (param) src und gibt 
// den decodierten String zurück.
string fromHex(string src) {
    string dst;
    StringSource(src, true,
        new HexDecoder(
            new StringSink(dst)
        )
    );
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
        // Convertiert die Bytes in src zu Base 16 encodiertem Hex.
        string dst = toHex(src);
        cout << src << " >  HexEncoder > " << dst << endl;
        // Der Hexstring dst dekodiert sollte der gleiche
        // wie das Original src sein.
        assert(src == fromHex(dst));

        src = "4963682062696E2065696E20537472696E6720696E204865786164657A696D616C6B6F64696572756E6721";
        // Convertiert einen auf Basis 16 encodierten Hexstring zu Bytes.
        string decoderDst = fromHex(src);
        cout << src.substr(0, 20) << "... > HexDecoder > " << decoderDst << endl;
        // Das Encodieren des Strings decoderDst sollte wieder wie src sein.
        assert(src == toHex(decoderDst));

        // Die Funktion readFromFile aus der Toolbox wurde geändert. 
        // Zuerst haben wir die Funktionalität entfernt, dass alle 
        // Zeichen zu Kleinbuchstaben umgewandelt werden, indem 
        // wir den Aufruf von 'tolower(c)' in readFromFile entfernt haben.
        // Zweitens haben wir als drittes Argument einen neuen bool 
        // hinzugefügt, welcher skipNewlines heißt. Dieses Argument ist 
        // standardmäßig auf'false' voreingestellt. Das Setzen von 
        // skipNewlines auf true entfernt Newlines ('\n') in 
        // der Funktionsausgabe.
        // I use the relative path with the assumption, that we are located in the 'bin'
        // folder when this program is started.
        // If this script is started from the Debug Folder with './bin/cryptopp-intro', we
        // need to use "../data/base64data.txt" instead. This can be automated by
        // evaluating argv[0], but we thought this was not expected.
        if (!Toolbox::readFromFile("../../data/base64data.txt", src, /*skipNewlines=*/true)) {
            // Opening the File failed. Continue anyway to execute the other tasks.
            cerr << "Reading from File ../../data/base64data.txt failed. Continuing." << endl;
        }

        cout << "File ../../data/base64data.txt Content: " << src << endl;

        dst = "";
        // Decode base 64 encoded data in src into dst
        StringSource(src, true,
            new Base64Decoder(
                new StringSink(dst)
            )
        );

        cout << "Decoded File Content: " << dst << endl;
    } catch(const Exception& e) {
        cerr << e.what() << endl;
        assert(false);
    }
}

void EncryptionExercise_Part1() {
    byte key[16];
    string encoded_key = "08a8cbfe7a3d1262c8abc3d1197dfefe";
    // Converting base 16 encoded string to bytes.
    // The result is saved in byte array.
    // The conversion is required because we need to pass bytes
    // to the AES object.
    StringSource(encoded_key, true,
        new HexDecoder(
            new ArraySink(key, 16)
        )
    );

    string plain_text = "Streng geheime Botschaft";
    string cypher_text;

    // AES Implementation for Encryption from the CryptoPP lib
    ECB_Mode<AES>::Encryption aes_enc;

    // Using the 16 Bytes in the 'key' array for future encoding
    aes_enc.SetKey(key, sizeof(key));

    // Encrypts the content of plain_text with AES in ECB Mode
    // with they key in the key array. The HexEncoder is used
    // to convert the bytes from the AES output to human
    // readable hex code.
    StringSource(plain_text, true,
        new StreamTransformationFilter(aes_enc,
            new HexEncoder(
                new StringSink(cypher_text)
            )
        )
    );

    cout << plain_text << " > AES Encrypt > " << cypher_text << endl;

    ECB_Mode<AES>::Decryption aes_dec;

    // Key is used for both encryption and decryption.
    aes_dec.SetKey(key, sizeof(key));

    string p;

    // During Encoding we converted the bytes to hex.
    // HexDecoder is required and used to convert the hex back to bytes.
    StringSource(cypher_text, true,
        new HexDecoder(
            new StreamTransformationFilter(aes_dec,
                new StringSink(p)
            )
        )
    );

    cout << cypher_text << " > AES Decrypt > " << p << endl;
    assert(plain_text == p);
}

void EncryptionExercise_Part2() {
    string cypher_text;

    string src;
    if (!Toolbox::readFromFile("../../data/aescipher.txt", src, true)) {
        cerr << "Reading from File ../../data/aescipher.txt failed. Continuing." << endl;
    }

    cout << "File ../../data/aescipher.txt Content: " << src << endl;

    // The SetKeyWithIV function requires the key and IV to be
    // bytes and saved in a byte array.
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

    // AES Implementation for Decryption from the CryptoPP lib
    CTR_Mode<AES>::Decryption aes_crt_dec;

    // The bytes in the key array and iv array are used in the
    // future decoding
    aes_crt_dec.SetKeyWithIV(key, sizeof(key), iv);

    string dst;
    // The source is a base64 encoded string. The Base64Decoder is used
    // to convert the encoded string to bytes. This is required for the
    // AES StreamTransformationFilter.
    StringSource(src, true,
        new Base64Decoder(
            new StreamTransformationFilter(aes_crt_dec,
                 new StringSink(dst)
            )
        )
    );

    cout << src.substr(0, 20) << "... > AES Decrypt > " << dst << endl;
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
        // Task 3a) und 3b)
        EncryptionExercise_Part1();

        // Task 3c)
        EncryptionExercise_Part2();
    } catch(const Exception& e) {
        // This branch is executed is an Exception is raised in the
        // try Block, for example if CryptoPP threw an Error/Exception
        // during AES encoding or decoding
        // Prints the reason for the Exception and Exits the Program
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
    // SHA256 Implementation from the CryptoPP lib
    SHA256 hash;
    // Hashes the message string with CryptoPP's implementation
    // of SHA256 and saves the Hash
    // in the 'digest' string
    StringSource(message, true,
        new HashFilter(hash,
            new StringSink(digest)
        )
    );

    // The SHA256 Hash in hash is saved in hex encoded
    // format in the dst string. This way we can
    // print the Hash in human readable format.
    string dst;
    StringSink* sink = new StringSink(dst);
    HexEncoder* encoder = new HexEncoder(sink);
    StringSource(message, true, encoder);

    //Prints the hex encoded SHA256 Hash of message
    cout << "Message Hash: " << dst << endl;

    // HASH_AT_BEGIN: Specifies that the SHA256 Hash in
    // 'digest' should be at the beginning of the string,
    // the message is concatenated right after 'digest'
    // PUT_RESULT: Specifies that the result of the verification
    // is saved in the 'result' variable
    word32 flags = HashVerificationFilter::HASH_AT_BEGIN
                   | HashVerificationFilter::PUT_RESULT;

    // Evaluates if the digest is a SHA256 Hash of message
    // hash.Restart() restarts the internal state of the SHA256 hash
    // The result is saved as a boolean in the 'result' variable
    bool result;
    hash.Restart();
    StringSource(digest + message, true,
        new HashVerificationFilter(hash,
            new ArraySink((byte*)&result, sizeof(result)),
                flags
        )
    );

    cout << "Verification " << (result ? "successful" : "failed") << endl;

    assert(result);

    // b: The variable result will be set to false in HashVerificationFilter()/ArraySink

    // c: Replace the 'HashVerificationFilter::HASH_AT_BEGIN'
    //           with 'HashVerificationFilter::HASH_AT_END'.
}

// Calculates (a^b) % n and returns the result of this calculation
// as a Integer Objekt from the CryptoPP lib
// This calculation is according to the Pseudocode for
// 'Modulare Exponentiation vom Zahlentheorie-Vorkurs'
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

    // Berechnet (a^b) % n mit unserer Implementation
    // a = 1. argument; b = 2. arg; n = 3. arg
    Integer res = modexp(Integer(371), Integer(18961551), Integer(2371));

    // Berechnet (a^b) % n mit der Implementation der CryptoPP Bibliothek
    // a = 1. argument; b = 2. arg; n = 3. arg
    Integer res2 = a_exp_b_mod_c(Integer(371), Integer(18961551), Integer(2371));

    // Überprüfung, ob das Ergebnis der Crypto++ Funktion a_exp_b_mod_c mit
    // der von uns implementierten modexp Funktion übereinstimmt.
    assert(res == res2);

    Integer p(95957);
    Integer a(58788);
    // Da p eine ungerade Primzahl ist, gilt nach dem Euler Kriterium auf
    // Seite 43 der 'Zufallszahlengeneratoren' Folien, dass a ein
    // quadratischer Rest modulo p genau dann ist, wenn a^((p-1)/2) kongruent zu
    // 1 (mod p) ist.
    bool isQR = modexp(a, Integer((p-1) / 2), p) == 1;
    cout << "Die Zahl " << a << " ist " << (isQR ? "ein" : "kein") << " QR mod " << p << endl;
}

void RNGExercise() {
    /*************************************************************************
     * Aufgabe 6cfhj.
     *************************************************************************/
    cout << endl
         << "RNGExercise:" << endl
         << "============" << endl << endl;

    // Das n für den BBS wird aus dem Produkt von zwei Primzahlen gebildet.
    Integer n = Integer("1252910265243849922375596598575099209083498535192739493227403") *
                Integer("1476222059624949757818143837507324048590620075519516306265283");

    // Initialisierung des BlumBlumShubGenerators mit "mod n" und seed 42.
    BlumBlumShubGenerator bbs(n, Integer(42));

    cout << "Generierung von 20 Zufallsbits: ";
    for (int i = 0; i < 20; i++) {
        cout << int(bbs.getBit());
    }
    cout << endl << endl;

    cout << "Generierung von 5 Zufallsbytes: ";
    for (int i = 0; i < 5; i++) {
        cout << int(bbs.getByte()) << " ";
    }
    cout << endl << endl;

    byte array[100];
    bbs.getBlock(array, 100);
    cout << "Generierung von 100 Zufallsbytes: ";
    for (int i = 0; i < 100; i++) {
        cout << int(array[i]) << " ";
    }
    cout << endl << endl;

    cout << "Generierung einer Zufallszahl mit 200 Bytes: " << bbs.getInteger(200);
    cout << endl << endl;


    Integer sum;
    cout << "Generierung von 1000 gleichverteilten Integern aus der Menge {0,1,...,24}. ";
    for (int i = 0; i < 1000; i++)
        sum += bbs.getInteger(Integer(25));
    cout << "Generierte Zahlen summiert: " << sum
         << " Summiert sollte das irgendwo in der Nähe von " << 12 * 1000 << " sein." << endl << endl;

    cout << "Generierung einer Zufallszahl aus der Menge  {0, 1, . . . , 2^30 − 1}: "
         << bbs.getInteger(Integer(1 << 30)) << endl;
}
 
 
int main() {
    DataFlowExercise();
    EncryptionExercise();
    HashExercise();
    IntegerExercise();
    RNGExercise();
 
    return 0;
}
