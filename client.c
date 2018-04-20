
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

void startNcurses(){
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    cbreak();
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED,     COLOR_BLACK);
        init_pair(2, COLOR_GREEN,   COLOR_BLACK);
        init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4, COLOR_BLUE,    COLOR_BLACK);
        init_pair(5, COLOR_CYAN,    COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE,   COLOR_BLACK);
        refresh();
    }
    bkgd(COLOR_PAIR(7));
    refresh();
}

void endNcurses(){
    endwin();
}

void init_win_box(win_box_t* win, int width, int height, int startx, int starty){
    win->border = create_win_box(width, height, startx, starty);
    win->content = create_win(width - 2, height - 2, startx + 1, starty + 1);
}

WINDOW* create_win(int width, int height, int startx, int starty){
    WINDOW* local_win;
    local_win = newwin(height, width, starty, startx);
    wrefresh(local_win);
    return local_win;
}

WINDOW* create_win_box(int width, int height, int startx, int starty){
    WINDOW* local_win;
    local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);
    wrefresh(local_win);
    return local_win;
}

void initGUI(GUI_t* gui){
    int stdscr_h = gui->stdscr_h;
    int stdscr_w = gui->stdscr_w;
    init_win_box(&(gui->input), stdscr_w - LIST_WIDTH, TEXTBOX_HEIGHT, LIST_WIDTH, LINES - TEXTBOX_HEIGHT);
    init_win_box(&(gui->list), LIST_WIDTH, stdscr_h, 0, 0);
    gui->content_box = create_win_box(stdscr_w - LIST_WIDTH, stdscr_h - TEXTBOX_HEIGHT, LIST_WIDTH, 0);
    wbkgd(gui->input.border, COLOR_PAIR(7));
    wrefresh(gui->input.border);
    wbkgd(gui->list.border, COLOR_PAIR(7));
    wrefresh(gui->list.border);
    wbkgd(gui->content_box, COLOR_PAIR(7));
    wrefresh(gui->content_box);
}

void clientLoop(GUI_t* gui, int fd){
    rw_list_t* chat_list = rw_list_init();
    int ch = 0;
    while(1){
        ch = getch();
    }

    free(chat_list);
}