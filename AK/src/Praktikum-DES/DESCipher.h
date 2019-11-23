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

  DESCipher();

  ~DESCipher();

  void computeKeySchedule(const byte *key, bool encmode=true);

  byte computeSBox(byte id, byte line, byte col);

  virtual int decrypt(const byte* cipher_text, int cipher_len, 
                      const byte* key, int key_len,
                      byte* plain_text, int plain_len);

  virtual int encrypt(const byte* plain_text, int plain_len, 
                      const byte* key, int key_len,
                      byte* cipher_text, int cipher_len);

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
