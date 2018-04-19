

#include "client.h"

#include <signal.h>

static void finish(int sig);

void usage(char * program);

static void finish(int sig)
{
    endwin();

    /* do your non-curses wrapup here */

    exit(0);
}

int main(int argc, char * argv[]){

    /*
    int connection_fd;
    // Check the correct arguments
    if (argc != 3) {
        usage(argv[0]);
    }*/

    // Get username
    char* uname = getUsername();

    // Start the client
    //connection_fd = connectSocket(argv[1], argv[2]);
    



    // Start ncurses
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    cbreak();

    if (has_colors()) {
        start_color();
        /*
         * Simple color assignment, often all we need.  Color pair 0 cannot
         * be redefined.  This example uses the same value for the color
         * pair as for the foreground color, though of course that is not
         * necessary:
         */
        init_pair(1, COLOR_RED,     COLOR_BLACK);
        init_pair(2, COLOR_GREEN,   COLOR_BLACK);
        init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4, COLOR_BLUE,    COLOR_BLACK);
        init_pair(5, COLOR_CYAN,    COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE,   COLOR_BLACK);
    }

    int num = 0;
    int lol;
    
    while((lol = getch()) != KEY_F(1)){
        attrset(COLOR_PAIR(num % 7 +1));
        printw("TEXTO");
        //refresh();
        num++;
    }

    // End ncurses
    endwin();
    // Free username memory
    free(uname);
    // Close the socket
    //close(connection_fd);
    return 0;
}

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}