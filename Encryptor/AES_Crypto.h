#ifndef _AES_CRYPTO_H_
#define _AES_CRYPTO_H_

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
#include <string>
using std::string;
#include <cstdlib>
using std::exit;
#include "assert.h"

#include"cryptopp/osrng.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/files.h"
#include "cryptopp/filters.h"
#include "cryptopp/aes.h"
#include "cryptopp/ccm.h"
using namespace CryptoPP;

class AES_Encryptor
{
private:
  CBC_Mode<AES>::Encryption encryptEngine;
  CBC_Mode<AES>::Decryption decryptEngine;
  AutoSeededRandomPool prng;
  CryptoPP::byte* _key;
  size_t _size;
  CryptoPP::byte _iv[AES::BLOCKSIZE];
  void SetKey();  

public: 
  AES_Encryptor(string AESKeyfile); 
  // Generate new iv and return
  string getNewIV();
  void setIV(byte* iv);
  // encrypt string
  string encrypt(string plain);
  // decrypt string
  string decrypt(string cipher);
};

#endif
