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

class AES_Module
{
private:
  CBC_Mode<AES>::Encryption encryptEngine;
  CBC_Mode<AES>::Decryption decryptEngine;
  AutoSeededRandomPool prng;
  //CryptoPP::byte* _key;
  size_t _size;
  CryptoPP::byte _iv[AES::BLOCKSIZE];
  void SetEngineWithKey(string _key);  

public: 
  AES_Module(string AESKeyfile);   
  string LoadKey(string);
  
  // Generate new iv and return
  string getNewIV();
  void setIV(byte* iv);
  
  // encrypt string
  void encrypt(string& plain,string& cipher);
  
  // decrypt string
  bool decrypt(string& cipher,string& recovered);
};

#endif
