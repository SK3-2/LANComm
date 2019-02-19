#include "cryptopp/rsa.h"
using CryptoPP::RSA;
using CryptoPP::RSAES_OAEP_SHA_Encryptor;
using CryptoPP::RSAES_OAEP_SHA_Decryptor;

#include "cryptopp/sha.h"
using CryptoPP::SHA1;

#include "cryptopp/files.h"
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include "cryptopp/filters.h"
using CryptoPP::PK_EncryptorFilter;
using CryptoPP::StringSink;
using CryptoPP::StringSource;

#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <exception>
using std::exception;

#include <assert.h>

class RSA_Encryptor{
  
private:
  RSA::PublicKey PublicKey;
  RSAES_OAEP_SHA_Encryptor* encryptEngine; 
  string keyFile_public = "publicKey";
  PublicKeyStore* pkStore;


public: 
  RSA_Encryptor(const PublicKeyStore* pks)
  {
    pkStore = pks; 
  }

  string encrypt(string deviceID, string plain)
  {   
    pubKey = pkStore->find(deviceID);
    encryptEngine = new RSAES_OAEP_SHA_Encryptor(pubkey);
    string cipher;

    try
    { 
      StringSource(plain, true,
	          new PK_EncryptorFilter( rng, *encryptEngine,
	          new StringSink( cipher )
	)
     );
    } catch( CryptoPP::Exception& e )
    {
      cerr << "Caught Exception... "<<endl;
      cerr << e.what() <<endl;
     }
    delete encryptEngine;
    return cipher;
  };

    
  ~RSA_Encryptor(){
  }
   
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

