Spring Server
=============

**Table of Contents**

1. [About](#about)
2. [Demo Videos](#demo-videos)
3. [Download and Build](#download-and-build)
4. [Usage](#usage)
5. [Customizing](#customizing)
6. [Example](#example)
7. [License](#license)

About
-----

The goal of this project is to provide a simple and robust framework that
enables the swift creation of client-server applications.

Two executables, `client` and `server`, are provided with this project. They
come pre-equipped to easily connect and exchange data over a TCP/IP
network socket.

The `server` component can independently manage up to 1028 concurrent client
connections. It can automatically detect and disconnect inactive clients.

Both the `client` and `server` components provide hooks to execute
custom-written functions. They also provide tools to send, receive, or log
data from within those functions.

Through the development of custom-written functions, the `client` and
`server` can carry out virtually any task that can be written in C.

Knowledge of C is required.

Demo Videos
-----------

##### Custom Function Demo (13:54)

How to customize the client and server with custom functions:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=mYfXCqQFgjM" target="_blank"><img src="http://img.youtube.com/vi/mYfXCqQFgjM/0.jpg" alt="Custom Functions" width="240" height="180" border="10" /></a>

Download and Build
------------------

In order to download and build this program, you will need to have git,
gcc, and make installed. Install them from your package manager if not
already installed.

```
$ which make
/usr/bin/make

$ which gcc
/usr/bin/gcc

$ which git
/usr/bin/git
```

Next download and build the server and client apps:
```
git clone https://github.com/bartobri/spring-server.git
cd spring-server
make
```

The resulting binaries will be located in the `spring-server/bin/` directory.

Usage
-----

```
$ bin/server                # Run with default bindings and port
$ bin/server -h localhost   # Specify the hostname or IP of the interface to bind to
$ bin/server -p 8880        # Specify the port number to listen on

$ bin/client -h localhost            # Specify hostname to connect to
$ bin/client -h localhost -p 8880    # Specify host and port number to connect to
```

If you don't specify a hostname or port number, the server and client
will use the DEFAULT_HOST and DEFAULT_PORT settings contained in the config.h
header file. If DEFAULT_HOST is set to a null string, the server will
bind to all available network interfaces, while the client will require
the use of the `-h` command line argument to set the hostname or IP address
that it should connect to.

Note that if you run these programs without any customizations, the client
and server programs can successfully connect but will not exchange data or
perform tasks. Since no data is being exchanged, the server will disconnect
the client after about 10-15 seconds due to inactivity (this is configurable).

Customizing
-----------

There are only 3 files that you ever need to modify in order to customize
the client and server components to do virtually anything you want them to do.

1. **config.h** - Contains global settings used by both the client and server.
2. **server.c** - Contains custom function definitions for the server.
3. **client.c** - Contains custom function definitions for the client.

95% of customizing will be through the creation of custom functions for
the client and server to execute.

##### Defining Custom Functions

There are 4 classes of custom functions. Each function class is
executed differently.

1. **Connect Function** - Executed when a new connection is made.
2. **Disconnect Function** - Executed when a connection is terminated.
3. **Periodic Function** - Executed at a timed interval (configured in config.h).
4. **Command Function** - Executed in response to a command sent from the client or server.

Custom functions should be defined with specific parameters and return
values. Each function type has it's own macro to help with this. Below
is how each of the four function classes should be defined using these
macros.

```
CONNECT_FUNCTION(function_name) {
	(void)socket;
	
	// Code here
}

DISCONNECT_FUNCTION(function_name) {
	(void)socket;
	
	// Code here
}

PERIODIC_FUNCTION(function_name) {
	// Code here
}

COMMAND_FUNCTION(function_name) {
	(void)socket;
	(void)payload;
	
	// Code here
}
```

These macros expand into a traditional function definition with the
expected parameters and return values. Be sure to replace
"function_name" with a unique function name.

Connect and Disconnect functions are passed the integer value for the
socket that connected or disconnected, defined as `int socket`.

Command functions are passed the integer value of the socket that sent
the command, and a string pointer optionally containing a string of
characters referred to as the payload. They are defined as `int socket`
and `char *payload`.

Periodic functions are not passed any parameters.

All functions have a return value of `void`.

##### Executing Custom Functions

Once functions are defined inside the server.c and client.c files using
the templates above, you must specify when they are to be executed. This
is done inside the server_init() and client_init() functions which are
defined inside the client.c and server.c files respectively, which should
be just below where you defined your custom functions.

```
void server_init(void) {
    set_connect_function(&function_name);
    set_disconnect_function(&function_name);
    add_periodic_function(&function_name);
    add_command_function("cmnd", &function_name);
}

```

Only one connect and disconnect function can be set.

Multiple periodic functions can be set. They are executed in the order of which
they were added inside the server/client init function. The max allowed
is configured in config.h (see PERIODIC_LIMIT).

Multiple command functions can also be set, but each function must be
paired with a unique command string when it is set. The command string
tells the client or server to execute the function when it receives the
command when reading data from a network socket.

Commands are sent from the client or server using a tool provided with
this framework. More on tools in the next section.

##### Function Tools

Several tools are provided with this framework to make it simple to
exchange data, and perform other common tasks, from within your custom
functions.

```
/*
 * The write_socket() function writes the command (char *command) and
 * payload (char *payload) data to the socket (int s).
 */
int write_socket(int s, char *command, char *payload);

/*
 * The close_socket() function closes the network socket (int s) and
 * ensures that necessary cleanup tasks are executed.
 */
void close_socket(int s);

/*
 * The main_socket() function returns the integer value for the main
 * socket file descriptor. For the client, the main socket is that which
 * communicates to the server. For the server, the main socket is that
 * which listens for new client connections.
 */
int main_socket(void);

/*
 * The next_socket() function provides a iteration tool for traversing the
 * list of all active socket connections. Each call to next_socket()
 * returns the integer value for the next socket file descriptor in the
 * list. When the iteration reaches the end of the list, a negative
 * integer value is returned.
 */
int next_socket(void);

/*
 * The reset_next_socket() function sets an internally maintained list
 * position pointer to the start of the socket list. This function
 * should be called whenever you want the next_socket() function to
 * return the first socket in the socket list, such as the start of a
 * full socket list traversal.
 */
void reset_next_socket(void);

/*
 * The terminate() function tells the current client or server program
 * to terminate execution after the current function has returned
 * execution to the calling function.
 */
void terminate(void);

/*
 * The write_log() function write an entry to the log file. Uses printf
 * formatting and parameters.
 */
write_log(char *format_string, ...);

/*
 * The print_log() function is the same as write_log() but also prints
 * the formatted string to stdout.
 */
print_log(char *format_string, ...);
```

Example
-------

An example is provided that demonstrates the creation and triggering of one
of each of the four classes of custom functions. The customized client
and server examples are located in the following directory.

```
spring-server/src/example
```

You can compile and run them from the spring-server directory:

```
// Make the example client and server
$ make example

// Run the example server and client
$ bin/example_server
$ bin/example_client -h localhost
```

This is the process flow that they will execute:

1. The server component has a connect function defined and loaded, called "say_hello". When the client connects, this function is executed and sends the command "hello" to the new client.
2. The client component has a command function defined and loaded, called "receive_hello". It has associated the command "hello" with this function inside the client_init() code block. When the server sends the "hello" command to the client, the client executes the receive_hello function. Inside the function, it prints a message to stdout acknowledging the receipt of the command.
3. The client component has a periodic function defined and loaded. This is executed at the interval defined in config.h for PERIODIC_SECONDS, which is 5 seconds by default. The function sends the "beat" command to the server for the first 5 intervals, and stops sending it every interval thereafter.
4. The server component receives the "beat" command from the client, but does not have any command functions associated with it, so no function is executed as a response. But the server still acknowledges the receipt of data from the client by not auto-disconnecting it during the time it is receiving the periodic "beat" commands.
5. Once the client stops sending "beat" commands to the server, the server waits for a period of time defined in config.h for IDLE_SECONDS and then disconnects the client due to inactivity.
6. The server component has a disconnect function defined and loaded. When it disconnects the client, it executes this function. This function uses print_log() to add an entry to the logfile and print it to stdout.

License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
