CryptoTest:
	g++ -DNDEBUG -g3 -O2 -Wall -Wextra -o CryptoTest CryptoTest.cc -l:libcryptopp.a

Encryptor:
	g++ -DNDEBUG -g3 -O2 -Wall -Wextra -o Encryptor Encryptor.cc -l:libcryptopp.a

AES:
	g++ -dndebug -g3 -o2 -Wall -Wextra -o aes_encryptor main.cc aes_encryptor.cc -l:libcryptopp.a

RSA:
	g++ -dndebug -g3 -o2 -Wall -Wextra -o rsa_decryptor RSA_Encryptor.cc -l:libcryptopp.a

all:
	Encryptor

clean:
	rm Encryptor

