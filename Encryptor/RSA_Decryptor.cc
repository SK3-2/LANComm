#include "cryptopp/rsa.h"
using CryptoPP::RSA;
using CryptoPP::InvertibleRSAFunction;
using CryptoPP::RSAES_OAEP_SHA_Encryptor;
using CryptoPP::RSAES_OAEP_SHA_Decryptor;

#include "cryptopp/sha.h"
using CryptoPP::SHA1;

#include "cryptopp/files.h"
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include "cryptopp/filters.h"
using CryptoPP::PK_EncryptorFilter;
using CryptoPP::PK_DecryptorFilter;
using CryptoPP::StringSink;
using CryptoPP::StringSource;

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include "cryptopp/secblock.h"
using CryptoPP::SecByteBlock;

#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;
using CryptoPP::DecodingResult;

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <exception>
using std::exception;

#include <assert.h>

class RSA_Decryptor{
  
private:
  AutoSeededRandomPool rng;
  InvertibleRSAFunction parameters;
  RSA::PrivateKey PrivateKey;
  RSA::PublicKey PublicKey;
  RSAES_OAEP_SHA_Decryptor* decryptEngine; 
  string keyFile_private = "privateKey";
  string keyFile_public = "publicKey";

public: 
  RSA_Decryptor(){   
    
    if(!LoadKey(keyFile_private,PrivateKey)){      
      cout<<"Key Pair Loading Failed."<<endl;
      PrivateKey.GenerateRandomWithKeySize(rng, 1024);     
      cout<<"New paired keys are created."<<endl;      
      SaveKey(PrivateKey,keyFile_private); 
    }
    PublicKey.AssignFrom(PrivateKey);
    SaveKey(PublicKey,keyFile_public);
    
    decryptEngine = new RSAES_OAEP_SHA_Decryptor(PrivateKey);
  }
      
  ~RSA_Decryptor(){
    delete decryptEngine;
  }
   
  // Message
  string decrypt(string cipher){  
    string recovered;
    try { 
      StringSource(cipher, true,
        new PK_DecryptorFilter( rng, *decryptEngine,
  	new StringSink( recovered )
  	)
     );
    } catch( CryptoPP::Exception& e )
    {
      cerr << "Caught Exception... "<<endl;
      cerr << e.what() <<endl;
     }
    return recovered;
  };


  void SaveKey(const RSA::PublicKey& PublicKey, const string& filename){
    PublicKey.Save(
	FileSink(filename.c_str(), true).Ref());
  }

  void SaveKey(const RSA::PrivateKey& PrivateKey, const string& filename){
    PrivateKey.Save(
	FileSink(filename.c_str(), true).Ref());
  }

  bool LoadKey(const string& filename, RSA::PrivateKey& PrivateKey){
    try{
      PrivateKey.Load(
	FileSource( filename.c_str(), true, NULL, true ).Ref()
	);      
    } catch (const CryptoPP::Exception& e) {
      return false;
    }
    return true;
  }

  bool LoadKey( const string& filename, RSA::PublicKey& PublicKey){
    try{
      PublicKey.Load(
      FileSource( filename.c_str(), true, NULL, true ).Ref()
	);
    } catch (const CryptoPP:: Exception& e) {
      return false;
    }
    return true;
  }

};

int main(void){

  RSA_Decryptor rsa;
  string plain = "Hello world.";
  string recovered = rsa.decrypt(plain);
  cout<< recovered<<endl;
  return 0;
}
