#include <cassert>
#include <iomanip>
#include "DESCipher.h"

DESCipher::DESCipher() {
}

DESCipher::~DESCipher() {
}


void DESCipher::computeKeySchedule(const byte *key, bool encmode) {
/******************************************************************************
 * Aufgabe 5
 ******************************************************************************/

    // TODO: decrypt mode
    assert(encmode);

    byte roundKeySchedule[16][48];

    //int shifts[] = {1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28};
    // TODO: no idea why we need 1 less shifts here
    int shifts[] = {0, 1, 3, 5, 7, 9, 11, 13, 14, 16, 18, 20, 22, 24, 26, 27};

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 48; j++) {
            byte jPC2 = pc2[j];
            byte jLS;
            if (jPC2 <= 28) {
                jLS = (jPC2 + shifts[i]) % 28;
            } else {
                jLS = 28 + ((jPC2 - 28 + shifts[i]) % 28);
            }
            roundKeySchedule[i][j] = pc1[jLS];
            //cout << (int)roundKeySchedule[i][j] << " ";
        }
        //cout << endl << endl;
    }

    for( size_t i = 0; i < 16; i++ )
    {
        for( size_t j = 0; j < 48; j++ )
        {
            setBit( key_schedule[i], 6, j, getBit( key,8,roundKeySchedule[i][j] -1 ) );
        }
    }
}


byte DESCipher::computeSBox(byte id, byte line, byte col) {
    /******************************************************************************
     * Aufgabe 6a
     ******************************************************************************/
    assert(id < 8);
    assert(line < 4);
    assert(col < 16);

    return sbox[id][line * 16 + col];
}


int DESCipher::decrypt
        (
                const byte *cipher_text,
                int cipher_len,
                const byte *key,
                int key_len,
                byte *plain_text,
                int plain_len
        ) {
    /******************************************************************************
     * Aufgabe 8
     ******************************************************************************/

    assert(cipher_len % block_len == 0);
    assert(plain_len >= cipher_len);
    assert(key_len == 8);

    return plain_len;
}


int DESCipher::encrypt
        (
                const byte *plain_text,
                int plain_len,
                const byte *key,
                int key_len,
                byte *cipher_text,
                int cipher_len) {
    /******************************************************************************
     * Aufgabe 8
     ******************************************************************************/

    assert(plain_len % block_len == 0);
    assert(cipher_len >= plain_len);
    assert(key_len == 8);

    computeKeySchedule(key, true);

    for (unsigned int i = 0; i < plain_len; i += block_len) {
        processBlock(plain_text+i, cipher_text);
    }


    return plain_len;
}


void DESCipher::feistel
        (
                const byte *l_in,
                const byte *r_in,
                const byte *key,
                byte *l_out,
                byte *r_out,
                int rnd
        ) {
    /******************************************************************************
     * Aufgabe 6c
     ******************************************************************************/
    for (int i = 0; i < 4; i++) {
        l_out[i] = r_in[i];
    }

     byte z[4];
     functionF(r_in, key, z, rnd);
     for (int i = 0; i < 4; i++) {
         r_out[i] = z[i] ^ l_in[i];
     }
}


void DESCipher::functionF
        (
                const byte *r_in,
                const byte *key,
                byte *r_out,
                int rnd
        ) {
    /******************************************************************************
     * Aufgabe 6b
     ******************************************************************************/

    byte expand[6];
    permutate(ev, 48, r_in, 32, expand, 48);

    for (int i = 0; i < 6; i++) {
        expand[i] ^= key[i];
    }

    byte b[8][6];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            b[i][j] = getBit(expand, 8, i * 6 + j);
        }
    }

    byte s[8];
    for (int i = 0; i < 8; i++) {
        byte line = 2 * b[i][0] + b[i][5];
        assert(line >= 0 && line <= 3);
        byte col = 8 * b[i][1] + 4 * b[i][2] + 2 * b[i][3] + b[i][4];
        assert(col >= 0 && col <= 15);
        s[i] = computeSBox(i, line, col);
    }

    byte z[4];
    z[0] = (s[0] << 4) + s[1];
    z[1] = (s[2] << 4) + s[3];
    z[2] = (s[4] << 4) + s[5];
    z[3] = (s[6] << 4) + s[7];
    permutate(pp, 32, z, 4, r_out, 4);
}


bool DESCipher::getBit(const byte *array, int array_len, int pos) const {
    int bytepos, bitpos;
    byte b;

    assert(array_len > 0);
    assert(pos >= 0);

    bytepos = pos / 8;
    bitpos = 7 - (pos % 8);
    if (bytepos < array_len) {
        b = 0x01;
        b = b << bitpos;
        // out << "(" << dec << bytepos << "," << bitpos << "," << hex << (short)b << ") ";
        return ((array[bytepos] & b) == b);
    } else {
        return false;
    }

}


void DESCipher::permutate
        (
                const byte *p,
                int p_len,
                const byte *in_array,
                int in_len,
                byte *out_array,
                int out_len
        ) const {
    int i;

    assert(in_len > 0);
    assert(p_len <= out_len * 8);

    for (i = 0; i < p_len; i++) {
        setBit(out_array, out_len, i, getBit(in_array, in_len, p[i] - 1));
    }

}


void DESCipher::printBitField(const byte *bytefield, int len, int block_len) const {
    int i, bit_len;

    bit_len = 8 * len;
    for (i = 0; i < bit_len; i++) {
        cout << getBit(bytefield, len, i);
        if ((i + 1 < bit_len) && ((i + 1) % block_len == 0)) {
            cout << " ";
        }
    }
}


void DESCipher::processBlock(const byte *in_block, byte *out_block) {
/******************************************************************************
 * Aufgabe 7
 ******************************************************************************/

    byte tmp[8];
    permutate(ip, 64, in_block, 64, tmp, 64);

    cout << endl << endl <<  "init " << endl;
    printBitField(tmp, 8); cout << endl;

    for (int i = 0; i < 16; i++) {
        byte t[8];
        feistel(tmp, tmp+4, key_schedule[i], t, t+4, i);
        cout << endl << endl << "round " << i << endl;
        //printBitField(key_schedule[i], 6); cout << endl;
        printBitField(t, 8); cout << endl;
        for (int j = 0; j < 8; j++) {
            tmp[j] = t[j];
        }
    }



    for (int i = 0; i < 64; i++) {
        setBit(out_block, 64, i, getBit(tmp, 64, fp[i] - 1));
        cout << i << ' ' << getBit(tmp, 64, fp[i]+1) << endl;
    }
}


void DESCipher::setBit
        (
                byte *array,
                int array_len,
                int pos,
                bool value
        ) const {
    int bytepos, bitpos;
    byte b;

    assert(array_len > 0);
    assert(pos >= 0);
    assert(pos < 8 * array_len);

    bytepos = pos / 8;
    bitpos = 7 - (pos % 8);
    b = 0x01;
    b = b << bitpos;
    //cout << "(" << dec << bytepos << "," << bitpos << "," << hex << (short)b << ") ";
    if (value == true) {
        array[bytepos] |= b;
    } else {
        b = b ^ 0xff;
        array[bytepos] &= b;
    }

}

void DESCipher::getKeySchedule(byte *key_schedule2) {
    for (size_t i = 0; i < 16; i++) {
        for (size_t j = 0; j < 6; j++) {
            key_schedule2[i * 6 + j] = key_schedule[i][j];
        }
    }
}


// #ip
byte DESCipher::ip[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
};


// #fp
byte DESCipher::fp[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
};


// #ev
byte DESCipher::ev[48] = {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
};


// #pc1
byte DESCipher::pc1[] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,

        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
};

// #pc2
byte DESCipher::pc2[] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
};

// #sbox
byte DESCipher::sbox[8][64] = {
        /* S1 */
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,

        /* S2 */
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,

        /* S3 */
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,

        /* S4 */
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,

        /* S5 */
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,

        /* S6 */
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,

        /* S7 */
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,

        /* S8 */
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};


// #pp
byte DESCipher::pp[32] = {
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25
};


// #total_rot
byte DESCipher::total_rot[16] = {
        1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28
};
