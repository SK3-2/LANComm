#include "cryptopp/rsa.h"
using CryptoPP::RSA;

#include "cryptopp/files.h"
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

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

const char* privKey_file = "privateKey.dat";
const char* pubKey_file = "publicKey.dat";
RSA::PrivateKey privateKey;
RSA::PublicKey publicKey;
bool loadKey();
void saveKey();
void generateKey();

int main(void){
  if(loadKey()){          
    cout<<"Key-pair already exist!"<<endl;
    return 0;
  }
  cout<<"Creating new private key...";      
  generateKey();
  cout<<"Done!"<<endl;
  return 0;
}

bool loadKey(){
  try{
    privateKey.Load(FileSource(privKey_file, true, NULL, true ).Ref());    
  } catch (const CryptoPP::Exception& e) {
    cout<<"Private key doesn't exist. Key loading is failed."<<endl;	
    return false;
  }
  try{    
    publicKey.Load(FileSource(pubKey_file, true, NULL, true ).Ref());
  } catch (const CryptoPP::Exception& e) {
    publicKey.AssignFrom(privateKey);
    saveKey();      
  }
  return true; 
}

void saveKey(){
  privateKey.Save(FileSink(privKey_file,true).Ref());
  publicKey.Save(FileSink(pubKey_file, true).Ref());
}

void generateKey(){
  AutoSeededRandomPool rng;
  privateKey.GenerateRandomWithKeySize(rng, 1024);           
  publicKey.AssignFrom(privateKey);
  saveKey();
}
      
