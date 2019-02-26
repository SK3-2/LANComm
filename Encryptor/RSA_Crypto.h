#ifndef _RSA_CRYPTO_H
#define _RSA_CRYPTO_H

#include "cryptopp/rsa.h"
#include "cryptopp/files.h"
#include "cryptopp/filters.h"
#include "cryptopp/osrng.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/sha.h"
using namespace CryptoPP;

#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
#include <exception>
using std::exception;
#include <assert.h>
#include <unordered_map>

class PublicKeyStore{
private:
  std::unordered_map<std::string, std::string> keyMap;
  PublicKeyStore();
  ~PublicKeyStore();
  static PublicKeyStore* uniqueStore;

public:
  static PublicKeyStore* getInstance();
  void listAll();
  void addDevice(string deviceID, string pubkey);
  void deleteDevice(string deviceID);
  string getPubKey(string _deviceID);
};

class RSA_Decryptor{
private:
  static RSA::PrivateKey privateKey;
  static RSA::PublicKey publicKey;
  AutoSeededRandomPool rng;
  RSAES_OAEP_SHA_Decryptor* decryptEngine; 
  string keyFile_private = "privateKey";
  string keyFile_public = "publicKey";
public: 
  RSA_Decryptor(string privKeyFile, string pubKeyFile);      
  ~RSA_Decryptor();    
  string decrypt(string cipher);  
  bool LoadKey(const string& privKeyFile, const string& pubKeyFile);
  string getPublicKey ();
};

class RSA_Encryptor{
private: 
  RSA::PublicKey publicKey;
  RSAES_OAEP_SHA_Encryptor* encryptEngine;
  AutoSeededRandomPool rng; 
  string pubKeyFile = "publicKey";
  PairedDeviceManager* pdm;

public:
  RSA_Encryptor();
  ~RSA_Encryptor();
  string encrypt(string deviceID, string& plain, string& cipher);
};

#endif
