#ifndef _DESCipher_h_
#define _DESCipher_h_

#include "BlockCipher.h"

class DESCipher : public BlockCipher {
private:
    static byte ip[64];

    static byte fp[64];

    static byte ev[48];

    static byte pc1[];

    static byte pc2[];

    static byte sbox[8][64];

    static byte pp[32];

    static byte total_rot[16];

    byte key_schedule[16][6];

public:
    /**
     * Leerer Konstruktor für die DESCipher Klasse
     */
    DESCipher();

    /**
     * Leerer Destruktor für die DESCipher Klasse
     */
    ~DESCipher();

    /*
     * Berechnet die Rundenschlüssel und speichert diese in key_schedule
     * ab. Es werden 16 Rundenschlüsselt berechnet, wobei jeder
     * Rundenschlüssel 48 Bit groß ist.
     *
     * @param key ein konstanter byte Zeiger. Dabei handelt es sich um
     * den 64 Bit großen Schlüssel.
     *
     * @param encmode gibt an, ob die Rundenschlüssel für das
     * Verschlüsseln (encmode=true) oder das Entschlüsseln (encmode=false)
     * erstellt werden sollen. encmode ist standardmäßig auf true gesetzt.
     */
    void computeKeySchedule(const byte *key, bool encmode=true);

    /*
     * computeSBox liefert ein Arrayelement zurück. Bei dem
     * Array handelt es sich um das sbox 2D Array. Das Element wird über
     * id, line und col bestimmt.
     *
     * DES verwendet 8 S-Boxen für Permutationen. Jede S-Box ist eine
     * nicht lineare Abbildung von {0,1}⁶ nach {0,1}⁴. Das bedeutet, dass
     * die S-Box einen 6 Bit Wert auf einen 4 Bit Wert abbildet/zuweist.
     * Der param id gibt an, welche der 8 S-Boxen verwendet werden soll.
     *
     * Es gibt 64 verschiedene Werte für {0,1}⁶. Jede S-Box ist deshalb
     * ein 64 Elemente großes Array. Jedes Element ist ein Byte. Da auf
     * {0,1}⁴ abgebildet wird, werden nur die unteren 4 Bits des Bytes
     * gesetzt. Die S-Box kann auch als 4x16 Matrix dargestellt werden.
     * Die ersten 16 Elemente des 64 Elementearrays bilden die erste
     * Zeile der 4x16 Matrix, die darauffolgenden 16 Elemente an
     * Index 16 bis Index 31 des 64 Elementearrays bilden die zweite
     * Zeile der 4x16 Matrix und so weiter.
     * Die param line, die Zeile, und param col, die Spalte, bestimmen
     * das Element Matrix bzw. des 64 Elementearrays.
     *
     * @param id gibt an, welche SBox bzw. welches Array für die Berechnung
     * verwendet werden soll. Es gibt 8 verschiedene SBoxen/Arrays in sbox.
     * Valide Werte von id sind also 0,1,...,7.
     *
     * @param line gibt die Zeile der 4x16 S-Box Matrix an. Da es sich
     * intern um Arrays handelt fängt man hier ab Index 0 an. Valide
     * Werte sind deshalb 0,1,2 und 3.
     *
     * @param col gibt die Spalte der 4x16 S-Box Matrix an. Da es sich
     * intern um Arrays handelt fängt man hier ab Index 0 an. Valide
     * Werte sind deshalb 0,1,2,...,15.
     *
     * @return Der Wert eines Elements der S-Boxen. Der Rückgabewert ist
     * vom Typ byte, allerdings werden nur die unteren 4 Bits gesetzt sein.
     */
    byte computeSBox(byte id, byte line, byte col);

    /*
     * decrypt entschlüsselt cipher_len Bytes ab cipher_text
     * mit dem key_len großen Schlüssel key und speichert den
     * entschlüsselten Text im plain_len großen plain_text
     * Speicherblock ab.
     *
     * @param cipher_text zeigt auf einen Speicherblock, in dem der
     * mindestens cipher_len Byte große verschlüsselte zu entschlüsselnde Text
     * gespeichert ist.
     *
     * @param cipher_len gibt die Größe des cipher_text in Byte an. cipher_len
     * muss ein vielfaches von der durch BlockCipher spezifizierten
     * block_len sein.
     *
     * @param key zeigt auf einen Speicherblock, in dem der 8 Byte
     * große Key gespeichert ist. Dieser Key wird für das entschlüsseln
     * verwendet. Speziell werden aus diesem Key die 16 Rundenschlüssel
     * berechnet.
     *
     * @param key_len gibt die Größe des key in Byte an. key_len muss
     * gleich 8 sein.
     *
     * @param plain_text zeigt auf einen mindestens plain_len großen
     * Speicherblock, in dem der entschlüsselte cipher_text gespeichert
     * wird.
     *
     * @param plain_len gibt die Größe des plain_text in Byte an.
     * Um das komplette Ergebnis zu speichern muss plain_len
     * größer als cipher_len sein.
     */
    virtual int decrypt(const byte* cipher_text, int cipher_len,
                      const byte* key, int key_len,
                      byte* plain_text, int plain_len);

    /*
     * encrypt verschlüsselt plain_len Bytes ab plain_text
     * mit dem key_len großen Schlüssel key und speichert den
     * verschlüsselten Text im cipher_len großen cipher_text
     * Speicherblock ab.
     *
     * @param plain_text zeigt auf einen Speicherblock, in dem der
     * mindestens plain_len Byte große zu verschlüsselnde Text
     * gespeichert ist.
     *
     * @param plain_len gibt die Größe des plain_text in Byte an. plain_len
     * muss ein vielfaches von der durch BlockCipher spezifizierten
     * block_len sein.
     *
     * @param key zeigt auf einen Speicherblock, in dem der 8 Byte
     * große Key gespeichert ist. Dieser Key wird für das verschlüsseln
     * verwendet. Mit diesem key kann der resultierende cipher_text
     * auch wieder entschlüsselt werden.
     * Speziell werden aus diesem Key die 16 Rundenschlüssel berechnet.
     *
     * @param key_len gibt die Größe des key in Byte an. key_len muss
     * gleich 8 sein.
     *
     * @param cipher_text zeigt auf einen mindestens cipher_len großen
     * Speicherblock, in dem der verschlüsselte plain_text gespeichert
     * wird.
     *
     * @param cipher_len gibt die Größe des cipher_text in Byte an.
     * Um das komplette Ergebnis zu speichern muss cipher_len größer
     * als plain_len sein.
     */
    virtual int encrypt(const byte* plain_text, int plain_len,
                      const byte* key, int key_len,
                      byte* cipher_text, int cipher_len);

    /*
     *
     */
    void processBlock(const byte* in_block, byte* out_block);

    void feistel(const byte* l_in,
               const byte* r_in,
               const byte* key,
               byte* l_out,
               byte* r_out,
               int rnd=0);

    void functionF(const byte* r_in,
                 const byte* key,
                 byte* r_out,
                 int rnd=0);

    bool getBit(const byte* array, int array_len, int pos) const;

    void permutate(const byte* p, int p_len,
                 const byte* in_array, int in_len,
                 byte* out_array, int out_len) const;

    void printBitField(const byte* bytefield, int len, int block_len=8) const;

    void setBit(byte* array, int array_len, int pos, bool value) const;

    void getKeySchedule( byte* keySchedule );

};

#endif
