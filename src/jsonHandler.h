#ifndef jsonHandler_h
#define jsonHandler_h

struct Function;
struct Port;
extern Port configuredports[];
extern char ssid[33];
extern char password[65];

void saveData();
void loadData();

#endif
