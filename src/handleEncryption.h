#ifndef handleEncryption_h
#define handleEncryption_h

#include <AESLib.h>
#include <Arduino.h>

extern byte aes_key[];
extern byte aes_iv[N_BLOCK];

char* decrypt(char* text);
char* encrypt(char* text);

#endif 