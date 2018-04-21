
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

    char buffer[SOCK_BUFF_SIZE];

    int connection_fd;
    
    // Check the correct arguments
    if (argc != 3) {
        usage(argv[0]);
    }

    // Get username
    char* uname = getUsername();

    
    // Start the client
    connection_fd = connectSocket(argv[1], argv[2]);
    
    // Send hello message
    sendCodeStr(connection_fd, C_START, uname);

    packet_t resp;
    if(!readPacket(connection_fd, &resp)){
        printf("Could not log in!\n");
        exit(0);
    }
    else if(resp.code != REQ_OK){
        printf("Error!\nUsername taken!\n");
        exit(0);
    }else{
        // Start ncurses
        startNcurses();

        // Get screen size
        int stdscr_h, stdscr_w;
        getmaxyx(stdscr, stdscr_h, stdscr_w);

        // Create GUI struct and save screen size to it
        GUI_t gui;
        gui.stdscr_h = stdscr_h;
        gui.stdscr_w = stdscr_w;

        // Initialize GUI: draw windows, borders and all that
        initGUI(&gui);

        clientLoop(&gui, connection_fd);

        // End ncurses
        endNcurses();
    }
    // Free username memory
    free(uname);
    // Close the socket
    close(connection_fd);
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