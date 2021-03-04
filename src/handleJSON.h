#ifndef handleJSON_h
#define handleJSON_h

extern char ssid[33];
extern char password[65];
extern bool firstBoot;

char* parseJSON();
char* generateJSON();
void saveData();
void loadData();

#endif