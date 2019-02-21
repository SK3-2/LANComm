#include "RSA_Crypto.h"

PublicKeyStore* PublicKeyStore::uniqueStore = nullptr;

//Private Constructor & Destructor
PublicKeyStore::PublicKeyStore() {}
PublicKeyStore::~PublicKeyStore() {}

//Static getInstance functinon
PublicKeyStore* PublicKeyStore::getInstance(){
  if (uniqueStore == nullptr){
    uniqueStore = new PublicKeyStore;
  }
  return uniqueStore;
}

//Shows all items in the list
void PublicKeyStore::listAll(){
  std::cout<<"Enrolled device list"<<std::endl;
  int i = 0;
  for(const auto& n : keyMap){
    std::cout <<"["<<i<<"] deviceID: "<<n.first<<std::endl;
    ++i;
  }
}

//Add device
void PublicKeyStore::addDevice(string deviceID, string pubKey){    
  keyMap[deviceID] = pubKey;
}

//Remove device
void PublicKeyStore::deleteDevice(string deviceID){
  keyMap.erase(deviceID);
}

//Get public key for given deviceID
string PublicKeyStore::getPubKey(string _deviceID){
  if(keyMap.find(_deviceID) == keyMap.end()){
    return NULL;
  }
  return keyMap[_deviceID];    
}



