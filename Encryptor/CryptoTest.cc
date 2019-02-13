#include <iostream>
using std::cout;
using std::endl;

#include "cryptopp/integer.h"
#include "cryptopp/rsa.h"
#include "cryptopp/osrng.h"


using namespace CryptoPP;


int main(int, char **) {
  Integer i;
  cout<< "i: "<< i<<endl;

  unsigned int modulusBits = 1024;
  AutoSeededRandomPool rng;
 

  InvertibleRSAFunction privkey; 
  privkey.Initialize(rng,modulusBits, 17);





  return 0;
}


