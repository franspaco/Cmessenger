#include "user_helper.h"

// https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c
char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void storeUser(char* username, int fd) {
    FILE *outputFile = fopen(concat(concat("userData/", username), ".txt"), "w");
    fprintf(outputFile, "%s %d\n", username, fd);
    fclose(outputFile);
}

int userExists(char* username) {
	//TODO(osanseviero): Check if user exists
	//FILE *outputFile = fopen("iptable.txt", "a");
	return 0;
}
