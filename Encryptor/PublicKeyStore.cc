#include <iostream>

#include <string>
using std::string;

#include <unordered_map>

class PublicKeyStore{

private:
  std::unordered_map<std::string, std::string> keyMap;
  PublicKeyStore() {}
  ~PublicKeyStore() {}
  PublicKeyStore(const PublicKeyStore& s){}

  static PublicKeyStore* uniqueStore;

public:
  static PublicKeyStore* getInstance(){
    if (uniqueStore == nullptr){
      uniqueStore = new PublicKeyStore;
    }
    return uniqueStore;
  }

  void listAll(){
    std::cout<<"Enrolled device list"<<std::endl;
    int i = 0;
    for(const auto& n : keyMap){
      std::cout <<"["<<i<<"] deviceID: "<<n.first<<std::endl;
      ++i;
    }
  }

  void addDevice(string deviceID, string pubKey){    
    keyMap[deviceID] = pubKey;
  }

  void deleteDevice(string deviceID){
    keyMap.erase(deviceID);
  }

  string find(string deviceID){
    retrun keyMap.find(deviceID);
  }
};

PublicKeyStore* PublicKeyStore::uniqueStore = nullptr;

/*
int main(void)
{
  auto pks = PublicKeyStore::getInstance();
  pks->PublicKeyStore::addDevice("iphone6S","asdfghj2134560!@#%$$^%");
  pks->PublicKeyStore::listAll();

  return 0;
}
*/


