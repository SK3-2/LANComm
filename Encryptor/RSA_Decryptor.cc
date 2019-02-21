#include "RSA_Crypto.h"


//Constructor
RSA_Decryptor::RSA_Decryptor(string privKeyFile, string pubKeyFile){       
  if(!LoadKey(privKeyFile,pubKeyFile)){      
    cout<<"Error - Cannot load private key."<<endl;      
    exit(-1);
  }    
  decryptEngine = new RSAES_OAEP_SHA_Decryptor(privateKey);
}

//Destructor
RSA_Decryptor::~RSA_Decryptor(){
  delete decryptEngine;
}

//Decryptor for string-type cipher
string RSA_Decryptor::decrypt(string cipher){  
  string recovered;
  try { 
    StringSource(cipher,true,new PK_DecryptorFilter( rng, *decryptEngine,
	  new StringSink( recovered )	) );
  }
  catch( CryptoPP::Exception& e ) {
    cerr << "Caught Exception during RSA decryption... "<<endl;
    cerr << e.what() <<endl;
  }
  return recovered;
}

//Load Keys for RSA decryption 
bool RSA_Decryptor::LoadKey(const string& privKeyFile,const string& pubKeyFile){
  try{
    privateKey.Load(FileSource( privKeyFile.c_str(), true, NULL, true ).Ref());      
  } catch (const CryptoPP:: Exception& e) {
    return false;
  }
  try{
    publicKey.Load(FileSource( pubKeyFile.c_str(), true, NULL, true ).Ref());
  } catch (const CryptoPP::Exception& e){
    publicKey.AssignFrom(privateKey);      
  }
  return true;
}

//Return public key 
string RSA_Decryptor::getPublicKey (){
  string publicKeyString;
  try{
    publicKey.Save(StringSink(publicKeyString).Ref());
  } catch ( const CryptoPP::Exception& e){
    cerr << e.what() <<endl;
  }
  return publicKeyString;
}


