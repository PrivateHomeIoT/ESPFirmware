#ifndef jsonHandler_h
#define jsonHandler_h

struct Function;
struct Port;

extern char ssid[33];
extern char password[65];

extern Port configuredports[];

void saveData();
void loadData();

#endif
