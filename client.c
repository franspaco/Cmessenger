
#include "client.h"

/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
///                               PRIVATE FUNCTIONS
/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

int pollSocket(int fd){
    // Poll struct
    struct pollfd test_fds[1];
    // Timeout: 50ms
    int timeout = 50;
    // Set up the poll struct
    test_fds[0].fd = fd;
    test_fds[0].events = POLLIN;
    // Do poll
    return poll(test_fds, 1, timeout);
}

int deleteLast(WINDOW* win){
    int cx = 0;
    int cy = 0;
    getyx(win, cy, cx);
    if(cx > 0){
        mvwdelch(win, cy, cx-1);
    }
    else if(cy > 0){
        int winscr_h, winscr_w;
        getmaxyx(win, winscr_h, winscr_w);
        wmove(win, cy-1, winscr_w-1);
        wdelch(win);
    }
    else {
        // We reached 0,0: reprint buffer in case something is not being shown
        return 1;
    }
    return 0;
}

chat_t* create_chat(GUI_t* gui, long id, char* name){
    chat_t* temp = malloc(sizeof(chat_t));
    temp->win = create_win(gui->stdscr_w - LIST_WIDTH - 2, gui->stdscr_h - TEXTBOX_HEIGHT - 2, LIST_WIDTH + 1, 0 + 1);
    // Activate scroll lock on win
    scrollok(temp->win, TRUE);
    // Clear screen
    wclear(temp->win);
    wrefresh(temp->win);

    temp->panel = new_panel(temp->win);
    temp->id = id;
    temp->name = name;
    return temp;
}

void redraw_list(WINDOW* list, rw_list_t* chat_list, int selected){
    // Draw list
    int indx = 0;
    wclear(list);
    rw_rdlock(chat_list);
    rw_list_node_t* curr_ptr = chat_list->root;
    while(curr_ptr != NULL){
        if(selected == indx++){
            wattron(list, COLOR_PAIR(1));
            wprintw(list, "%s<-\n", ((chat_t*)curr_ptr->data)->name);
            wattroff(list, COLOR_PAIR(1));
        }
        else{
            wprintw(list, "%s\n", ((chat_t*)curr_ptr->data)->name);
        }
        curr_ptr = curr_ptr->next;
    }
    rw_unlock(chat_list);
    wrefresh(list);
}

/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
///                                   PUBLIC FUNCTIONS
/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

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
    initscr();      // Start ncurses mode
    keypad(stdscr, TRUE);   // Receive every key
    noecho();       // No echoing input
    cbreak();       // No buffering
    curs_set(0);    // Cursor invisible
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

    // Make border and content boxes for input and list
    init_win_box(&(gui->input), stdscr_w - LIST_WIDTH, TEXTBOX_HEIGHT, LIST_WIDTH, LINES - TEXTBOX_HEIGHT);
    init_win_box(&(gui->list), LIST_WIDTH, stdscr_h, 0, 0);

    // Create just the border
    gui->content_box = create_win_box(stdscr_w - LIST_WIDTH, stdscr_h - TEXTBOX_HEIGHT, LIST_WIDTH, 0);

    // Activate scroll lock on input
    scrollok(gui->input.content, TRUE);

    // Make input have a nice color
    wattron(gui->input.content, COLOR_PAIR(5));

    // Make sure the colors are correct
    wbkgd(gui->input.border, COLOR_PAIR(7));
    wrefresh(gui->input.border);
    wbkgd(gui->list.border, COLOR_PAIR(7));
    wrefresh(gui->list.border);
    wbkgd(gui->content_box, COLOR_PAIR(7));
    wrefresh(gui->content_box);
}

void clientLoop(GUI_t* gui, int fd){
    // Create the chat list
    rw_list_t* chat_list = rw_list_init();

    // Set up loopback
    chat_t* loopback = create_chat(gui, -1, "loopback");

    // Set up loopback as current chat
    chat_t* current = loopback;

    top_panel(current->panel);
    update_panels();
    doupdate();

    rw_list_push_back(chat_list, (void*)loopback);

    // TEST
    //TODO: REMOVE
    chat_t* test= create_chat(gui, 0, "test");
    rw_list_push_back(chat_list, (void*)test);

    // User input buffer
    char buffer[USER_BUFF_SIZE];
    buffer[0] = '\n';
    // Next position to store characters in the buffer
    int nextch = 0;

    // Current open connection
    int selected = 0;

    // Chat count
    int chat_n = 0;

    // New connection mode
    int new_conn = 0;

    // Variable for storing input
    int ch = 0;

    // To keep things simple
    WINDOW* list = gui->list.content;
    WINDOW* input = gui->input.content;

    // Set a timeout for getch to make it a non-blocking call 
    //  thus, allowing us to easily poll the socket
    timeout(50);

    //Draw chat list for the first time
    redraw_list(list, chat_list, selected);

    // Start infinite cycle asking for input
    while(1){

        ch = getch();

        // Get length of list
        chat_n = rw_list_length(chat_list);

        // If if the selected chat is >= than the number of chats
        // Switch it back to 0 (loopback). This could happen if a chat was closed
        if(selected >= chat_n){
            selected = 0;
        }

        // Input Timeout: no user input. Check the socket.
        if(ch == -1){
            int poll = pollSocket(fd);
            // If error was raised during the polling
            if(poll == -1){
                //TODO: handle this!
                break;
            }
            // If nothing happened: poll user and socket again
            else if(poll == 0){
                continue;
            }
            // Something was received
            else{
                packet_t packet;
                readPacket(fd, &packet);
                if(packet.code == S_QUIT){
                    wattron(current->win, COLOR_PAIR(1));
                    wprintw(current->win, "Server closing connection!\nPress any key to close...\n");
                    wattroff(current->win, COLOR_PAIR(1));
                    wrefresh(current->win);
                    timeout(-1);
                    getch();
                    break;
                }
                else if(packet.code == RCV_MSG){
                    wattron(current->win, COLOR_PAIR(1));
                    wprintw(current->win, "Recv: %li %s\n", packet.id, packet.msg);
                    wattroff(current->win, COLOR_PAIR(1));
                    wrefresh(current->win);
                }
            }
        }
        // F1 key or ESC
        else if(ch == KEY_F(1) || ch == 27){
            // Break cycle
            break;
        }
        // F4 key: new connection
        else if(ch == KEY_F(4)){
            wmove(gui->input.border, 0, 0);
            wattron(gui->input.border, COLOR_PAIR(1));
            wprintw(gui->input.border, "NEW CONNECTION - Enter usename: ");
            wattroff(gui->input.border, COLOR_PAIR(1));
            wrefresh(gui->input.border);
            new_conn = 1;
        }
        // Check if it is a printable character
        else if(isprint(ch)){
            // If the buffer still has space
            if(nextch < USER_BUFF_SIZE -2){
                buffer[nextch++] = ch;
                buffer[nextch] = '\0';
                waddch(input, ch);
                wrefresh(input);
            }
        }
        // If backspace: delete
        else if(ch == KEY_BACKSPACE){
            // If there's something to delete
            if(nextch > 0){
                buffer[--nextch] = '\0';
                // Delete and check if buffer still has content
                // If so, reprint buffer
                if(deleteLast(input) && nextch > 0){
                    wclear(input);
                    wprintw(input, "%s", buffer);
                }
                wrefresh(input);
            }
        }
        // If key down: next chat
        else if(ch == KEY_DOWN){
            if(selected < chat_n - 1){
                selected++;
                chat_t* next_chat;
                if(rw_list_get_element(chat_list, (void**)&next_chat, selected)){
                    current = next_chat;
                    top_panel(current->panel);
                    update_panels();
                    doupdate();
                }
            }
        }
        // If key down: previous chat
        else if(ch == KEY_UP){
            if(selected > 0){
                selected--;
                chat_t* next_chat;
                if(rw_list_get_element(chat_list, (void**)&next_chat, selected)){
                    current = next_chat;
                    top_panel(current->panel);
                    update_panels();
                    doupdate();
                }
            }
        }
        // Check if input is ENTER
        else if(ch == 10){
            // If normal send
            if(!new_conn){
                wattron(current->win, COLOR_PAIR(2));
                wprintw(current->win, "you: %li %s\n", current->id, buffer);
                wattroff(current->win, COLOR_PAIR(2));
                wrefresh(current->win);
                sendCodeIdStr(fd, SND_MSG, current->id, buffer);
            }
            // If new connection mode
            else {
                new_conn = 0;
                // Restore the box
                box(gui->input.border, 0 , 0);
                wrefresh(gui->input.border);
            }
            buffer[0] = '\0';
            nextch = 0;
            wclear(input);
            wrefresh(input);
        }
        else{
            // ?
        }

        //Draw list
        redraw_list(list, chat_list, selected);
    }
    free(loopback);
    free(chat_list);
}
