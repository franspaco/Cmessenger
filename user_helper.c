#include "user_helper.h"

void storeUser(char* username, char* ip, int port) {
    FILE *outputFile = fopen("iptable.txt", "a");
    fprintf(outputFile, "%s %s %d\n", username, ip, port);
}
