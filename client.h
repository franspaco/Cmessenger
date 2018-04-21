

#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <panel.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
// poll
#include <sys/poll.h>
// Custom libraries
#include "protocol.h"
#include "sockets.h"
#include "fatal_error.h"
#include "rw_list/rw_list.h"

// UI sizes definitions
#define LIST_WIDTH 25
#define TEXTBOX_HEIGHT 5

// Max allowed message length
//  (Must be smaller than SOCK_BUFF_SIZE !! )
#define USER_BUFF_SIZE 512

typedef struct chat_struct{
    WINDOW* win;
    PANEL* panel;
    long id;
    char uname[UNAME_LENGTH];
} chat_t;

typedef struct window_with_border{
    WINDOW* border;
    WINDOW* content;
} win_box_t;

typedef struct GUI_struct {
    win_box_t input;
    win_box_t list;
    WINDOW* content_box;
    int stdscr_h;
    int stdscr_w;
}GUI_t;

void startNcurses();
void endNcurses();

void init_win_box(win_box_t* win, int width, int height, int startx, int starty);
WINDOW* create_win(int width, int height, int startx, int starty);
WINDOW* create_win_box(int width, int height, int startx, int starty);

void initGUI(GUI_t* gui);

void clientLoop(GUI_t* gui, int fd);

void purgeStdin();
char* getUsername();


#endif