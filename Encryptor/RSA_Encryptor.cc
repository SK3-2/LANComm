#include "RSA_Crypto.h"

RSA_Encryptor::RSA_Encryptor(){    
    pdm = PairedDeviceManager::getInstance();
}

bool RSA_Encryptor::encrypt(string deviceID, string& plain, string& cipher) {   
  publicKey.Load(StringSource(pdm->getPublicKey(deviceID),true).Ref());
  encryptEngine = new RSAES_OAEP_SHA_Encryptor(publicKey);  
  try { 
    StringSource(plain, true, new PK_EncryptorFilter( rng, *encryptEngine,
	  new StringSink( cipher ) ) );
    return true;
  } catch( CryptoPP::Exception& e )  {
    cerr << "Caught Exception during RSA Encryption... "<<endl;
    cerr << e.what() <<endl;
    return false;
  }  
}

RSA_Encryptor::~RSA_Encryptor(){
  delete encryptEngine;
}
