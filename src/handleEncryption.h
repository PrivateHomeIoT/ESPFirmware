#ifndef handleEncryption_h
#define handleEncryption_h

#include <Arduino.h>
#include <Crypto.h>
#include <CryptoLegacy.h>
#include <AES.h>
#include <CBC.h>

extern uint8_t aes_key[16];
extern char aes_iv[16];

void setupKey();
char* decryptToChar(char* input, char* iv);
char* encryptFromChar(char* input);

#endif