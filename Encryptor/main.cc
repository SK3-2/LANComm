#include "AES_Encryptor.cc"

#include <iostream>
using std::cout;
using std::endl;

#include <string>

int main(void){
  CryptoPP::byte key[AES::DEFAULT_KEYLENGTH], iv[AES::BLOCKSIZE];
  CryptoPP::AutoSeededRandomPool prng;
  prng.GenerateBlock(key, sizeof(key));
  prng.GenerateBlock(iv, sizeof(iv));

  AES_Encryptor aes1(key,sizeof(key));
  AES_Encryptor aes2(key,sizeof(key),iv);

  string test = "hello wolrd";
  cout<<"plain text: "<<test<<endl;
  string cipher = aes1.encrypt(test);
  cout<<"cipher: "<< cipher <<endl;


  string recovered = aes1.decrypt(cipher);
  cout<<"recovered: "<< recovered <<endl;

  cout<<"getNewIV: "<<aes2.getNewIV()<<endl;


  return 0;
}
