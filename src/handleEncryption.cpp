#include "handleEncryption.h" 
#include "ESP8266TrueRandom.h"
#include <Crypto.h>
#include <CryptoLegacy.h>
#include <AES.h>
#include <CBC.h>

CBC<AES128> aesCBC;

uint8_t aes_key[16];
char aes_iv[16];

String printArray(uint8_t input[], uint length){
  String result;
  for(uint i= 0; i< length; i++) {
    Serial.print(input[i], HEX);
    Serial.print((char*)",");
    result += (uint)input[i];
    result += ",";
  }
  Serial.println();
  return result;
}

void setupKey(){
  if(aesCBC.setKey(aes_key, sizeof(aes_key))) Serial.println("Key setup was successful");
  else Serial.println("Key setup failed");
}

char* decryptToChar(char* input, char* iv, uint length){
  uint8_t convertedInput[length];
  for(uint i = 0; i<length; i++) convertedInput[i] = (uint8_t)input[i];
  uint8_t convertedIV[16];
  for(uint i = 0; i<length; i++) convertedIV[i] = (uint8_t)iv[i];

  aesCBC.setIV(convertedIV, sizeof(convertedIV));
  uint8_t* decryptedRaw;
  aesCBC.decrypt(decryptedRaw, convertedInput, sizeof(convertedInput));

  uint range = sizeof(decryptedRaw)-decryptedRaw[sizeof(decryptedRaw)-1];
  char decrypted[range];
  for(uint i = 0; i<sizeof(range); i++) decrypted[i] = (char)decryptedRaw[i];
  Serial.println("decrypted message: "+ (String)decrypted);
  return decrypted;
}

char* encryptFromChar(char* input, uint length){
  Serial.println(length);
  uint ciphersize=((length/16)+1)*16;
  uint8_t convertedInput[ciphersize];
  for(uint i = 0; i<length; i++) convertedInput[i] = (uint8_t)input[i];
  uint paddingValue = 16-length%16;
  for(uint i = length; i < length; i++) convertedInput[i] = paddingValue;
  Serial.print("converted Input: ");
  printArray(convertedInput, ciphersize);
  uint8_t iv[16];
  for(uint i = 0; i<16; i++){
    iv[i] = (uint8_t)ESP8266TrueRandom.random(0,255);
    aes_iv[i] = (char)iv[i];
  }

  Serial.print("IV: ");
  String ivString = printArray(iv, 16);
  Serial.println(ivString);

  aesCBC.setIV(iv, 16);
  uint8_t encryptedRaw[ciphersize];
  aesCBC.encrypt(encryptedRaw, convertedInput, ciphersize);
  
  char encrypted[ciphersize];
  for(uint i = 0; i<length; i++) encrypted[i] = (char)encryptedRaw[i];

  Serial.print("encrypted message: ");
  String msgString = printArray(encryptedRaw, ciphersize);
  Serial.println(msgString);

  ivString += msgString;
  Serial.println(ivString);
  return (char*)ivString.c_str();
}