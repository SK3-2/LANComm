#include <iostream>
using std::cout;
using std::endl;
using std::cerr;

#include"cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "cryptopp/aes.h"
using CryptoPP::AES;

#include "cryptopp/ccm.h"
using CryptoPP::CBC_Mode;

#include "assert.h"

using CryptoPP::byte;

class AES_Encryptor
{
private:
  CBC_Mode<AES>::Encryption encryptEngine;
  CBC_Mode<AES>::Decryption decryptEngine;

  AutoSeededRandomPool prng;
  CryptoPP::byte* _key;
  size_t _size;
  CryptoPP::byte _iv[AES::BLOCKSIZE];

  void SetKeyWithIV(){
    encryptEngine.SetKeyWithIV(_key, _size, _iv);
    decryptEngine.SetKeyWithIV(_key, _size, _iv);
    cout<<"CBC_Mode_AES_Encryptor is instantiated"<<endl;
  }

public:
  //Constructor
  AES_Encryptor(CryptoPP::byte* key, size_t size) {
    _key = key; 
    _size = size;
    prng.GenerateBlock(_iv, sizeof(_iv));
    SetKeyWithIV();
  }
  AES_Encryptor(CryptoPP::byte* key, size_t size, CryptoPP::byte* iv){
    memcpy(_iv,iv,AES::BLOCKSIZE);
    _key = key;
    _size = size;
   
    SetKeyWithIV();
  }

  //Set iv
  void setIV(byte* iv){
    memcpy(_iv,iv,AES::BLOCKSIZE);
    SetKeyWithIV();
  }

  // Generate new iv and return
  string getNewIV(){
    prng.GenerateBlock(_iv,sizeof(_iv));    
    string str(reinterpret_cast<char *>(_iv),sizeof(_iv));
    return str;
  }

  // encrypt string
  string encrypt(string plain){
    string cipher;
    try{      
      StringSource s(plain, true, 
	  new StreamTransformationFilter(encryptEngine, 
	    new StringSink(cipher)
	    )
	);
    } catch (const CryptoPP::Exception& e){
      cerr << e.what() << endl;
      exit(1);
    }
    return cipher;
  }

  // decrypt string
  string decrypt(string cipher){
    string recovered;
    try{
      StringSource d(cipher, true,
        new StreamTransformationFilter(decryptEngine,
        new StringSink(recovered)
        )
      );
    } catch (const CryptoPP::Exception& e){
      cerr << e.what() <<endl;
      exit(1);
    }
    return recovered;
  }  
};

