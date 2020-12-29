#ifndef handleJSON_h
#define handleJSON_h

struct Function;
struct Port;

extern char ssid[33];
extern char password[65];
extern bool firstBoot;

extern Port configuredports[];

void saveData();
void loadData();

#endif
