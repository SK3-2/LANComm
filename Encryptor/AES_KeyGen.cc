#include <iostream>
using std::cout;
using std::endl;

#include"cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include "cryptopp/filters.h"
using CryptoPP::ArraySource;

#include "cryptopp/files.h"
using CryptoPP::FileSink;

#include "cryptopp/aes.h"
using CryptoPP::AES;

using CryptoPP::byte;

const char* secretKey_file = "secretKey.dat";
size_t _size = AES::DEFAULT_KEYLENGTH;
AutoSeededRandomPool prng;
byte* _Key;

int main(int argc, char* argv[]){
  cout<<"Creating new AES key size of ";
  if (argc > 1){
    _size = atoi(argv[1]);
    _Key = new byte[_size];
    cout<<_size<<"..."<<endl;;
  }
  else{  
    _Key= new byte[_size];
    cout<<_size<<"(default)..."<<endl;
  }
  
  prng.GenerateBlock(_Key,_size);
 //string str(reinterpret_cast<char *>(_Key),sizeof(_Key));  
  cout<<"AES Key is sucessfully created! -> ./"<<secretKey_file<<endl;    
  ArraySource aesKeySource(_Key, _size, true, new FileSink(secretKey_file));  
  delete[] _Key;
  return 0;
}

