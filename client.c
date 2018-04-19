
#include "client.h"

void purgeStdin(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char* getUsername(){
    printf("Write your username: (max 10 characters)\n");
    char* uname = malloc(UNAME_LENGTH*sizeof(char));
    fgets(uname, UNAME_LENGTH, stdin);
    return uname;
}