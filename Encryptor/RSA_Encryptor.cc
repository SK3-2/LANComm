#include "RSA_Crypto.h"


RSA_Encryptor::RSA_Encryptor(PublicKeyStore* pks){
  pkStore = pks; 
}

string RSA_Encryptor::encrypt(string deviceID, string plain) {   
  publicKey.Load(StringSource(pkStore->getPubKey(deviceID),true).Ref());
  encryptEngine = new RSAES_OAEP_SHA_Encryptor(publicKey);
  string cipher;
  try { 
    StringSource(plain, true, new PK_EncryptorFilter( rng, *encryptEngine,
	  new StringSink( cipher ) ) );
  } catch( CryptoPP::Exception& e )  {
    cerr << "Caught Exception during RSA Encryption... "<<endl;
    cerr << e.what() <<endl;
  }
  return cipher;
}

RSA_Encryptor::~RSA_Encryptor(){
  delete encryptEngine;
}
