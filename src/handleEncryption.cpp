#include "handleEncryption.h" 
#include "ESP8266TrueRandom.h"
#include <Crypto.h>
#include <CryptoLegacy.h>
#include <AES.h>
#include <CBC.h>

CBC<AES128> aesCBC;

uint8_t aes_key[16];
char aes_iv[16];

void printArray(uint8_t input[], uint length){
  for(uint i= 0; i< length; i++) {
    Serial.print(input[i], HEX);
    Serial.print((char*)",");
  }
  Serial.println();
}

void setupKey(){
  if(aesCBC.setKey(aes_key, sizeof(aes_key))) Serial.println("Key setup was successful");
  else Serial.println("Key setup failed");
}

char* decryptToChar(char* input, char* iv){
  uint8_t convertedInput[sizeof(input)];
  for(uint i = 0; i<sizeof(input); i++) convertedInput[i] = (uint8_t)input[i];
  uint8_t convertedIV[16];
  for(uint i = 0; i<sizeof(input); i++) convertedIV[i] = (uint8_t)iv[i];

  aesCBC.setIV(convertedIV, sizeof(convertedIV));
  uint8_t* decryptedRaw;
  aesCBC.decrypt(decryptedRaw, convertedInput, sizeof(convertedInput));

  uint range = sizeof(decryptedRaw)-decryptedRaw[sizeof(decryptedRaw)-1];
  char decrypted[range];
  for(uint i = 0; i<sizeof(range); i++) decrypted[i] = (char)decryptedRaw[i];
  Serial.println("decrypted message: "+ (String)decrypted);
  return decrypted;
}

char* encryptFromChar(char* input){
  uint8_t convertedInput[((sizeof(input)/16)+1)*16];
  for(uint i = 0; i<sizeof(input); i++) convertedInput[i] = (uint8_t)input[i];
  uint paddingValue = 16-sizeof(convertedInput)%16;
  for(uint i = sizeof(input); i < sizeof(convertedInput); i++) convertedInput[i] = paddingValue;
  uint8_t iv[16];
  for(uint i = 0; i<16; i++){
    iv[i] = (uint8_t)ESP8266TrueRandom.random(0,255);
    aes_iv[i] = (char)iv[i];
  }

  aesCBC.setIV(iv, 16);
  uint8_t encryptedRaw[sizeof(convertedInput)];
  aesCBC.encrypt(encryptedRaw, convertedInput, sizeof(convertedInput));
  
  char encrypted[sizeof(input)];
  for(uint i = 0; i<sizeof(input); i++) encrypted[i] = (char)encryptedRaw[i];

  Serial.print("IV: ");
  printArray(iv, 16);
  Serial.print("encrypted message: ");
  printArray(encryptedRaw, sizeof(encryptedRaw));

  char result[sizeof(aes_iv)+sizeof(encrypted)];
  strcpy(result, aes_iv);
  strcpy(result, encrypted);
  return result;
}