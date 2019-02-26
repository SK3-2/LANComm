#include "AES_Crypto.h"

//Constructor
AES_Module::AES_Module(string AESKeyfile) {
  string _key = LoadKey(AESKeyfile); 
  SetEngineWithKey(_key);
  cout<<"CBC_Mode_AES_Module is instantiated"<<endl;
}

string AES_Module::LoadKey(string AESKeyfile){
  string stringKey;
  FileSource ss(AESKeyfile.c_str(),true, new StringSink(stringKey));
  return stringKey;
}

void AES_Module::SetEngineWithKey(string _key){
  _size = _key.length();
  byte _keyByte[_size];
  memcpy(_keyByte, _key.c_str(), _size);  
  memset(_iv,0,AES::BLOCKSIZE); // zero initialize for iv
  encryptEngine.SetKeyWithIV(_keyByte, _size,_iv);
  decryptEngine.SetKeyWithIV(_keyByte, _size,_iv);
}

// encrypt string
void AES_Module::encrypt(string& plain, string& cipher){
  try{      
    StringSource s(plain, true, 
	new StreamTransformationFilter(encryptEngine, 
	  new StringSink(cipher)));
  } catch (const CryptoPP::Exception& e){
    cerr << e.what() << endl;
    exit(1);
  }
}

// decrypt string
bool AES_Module::decrypt(string& cipher, string& recovered){
  try{
    StringSource d(cipher, true,
	new StreamTransformationFilter(decryptEngine,
	  new StringSink(recovered)));
    return true;
  } catch (const CryptoPP::Exception& e){
    cerr << e.what() <<endl;
    return false;
    //exit(1);
  }
}  

// Generate new iv and return
string AES_Module::getNewIV(){
  prng.GenerateBlock(_iv,sizeof(_iv));    
  string str(reinterpret_cast<char *>(_iv),sizeof(_iv));
  return str;
}

//Set iv
/*void AES_Module::setIV(byte* iv){
  memcpy(_iv,iv,AES::BLOCKSIZE);
  SetKeyWithIV();
}
*/


