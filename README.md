
# Cmessenger

A simple messaging app made in C.

## Installation

### Dependencies

* Cmake
* GCC (Tested in gcc version 5.4.0)
* Pthread library
* Ncurses library
* Panel library (generally included with ncurses)

To install ncurses in ubuntu run:

```sudo apt-get install libncurses-dev```

### Instructions

1. Clone Git repository
2. Create a folder called "build" inside
3. Inside build run: `cmake ..`   This will detect your compiler an generate a makefile.
4. Run `make`  To compile the source files into the executables.

## Running the program

When the software is build it produces two separate executables:

* msg_server
* msg_client

As the names imply, one is the server and the other one is the client.

### Running the server
To run the server simply execute:
```./msg_server <port_number>```

Where <port_number> is the port the server will listen to.

The server can be closed using Ctrl+C, this will automatically notify all clients and close the server.

### Running the client
To run the server simply execute:
```./msg_client <server_host> <port_number>```

Where <server_host> is the host of the server you are connecting to,
and <port_number> is the port the server is listening to.

The client will first ask for a username to connect to the server. 
**Warning: no spaces allowed!**

If the username has not been taken the client will change to chat mode.

![Cmessenger client UI](https://franspaco.com/resources/Cmessenger.png)

In here, the open chats are displayed on the left pane, the chats themselves are displayed in the central panel, and the user types in the panel at the bottom.

The active chat can be switched with the up/down keys.

Use F4 to open a new chat, enter the username of the other user and press Enter.

Chatting is as simple as typing and pressing enter to send.

To close the client you can use F1 or Crtl+C, the server will be notified.

