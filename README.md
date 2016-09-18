Spring Server
=============

**Table of Contents**

1. [About](#about)
2. [Download and Build](#download-and-build)
3. [Usage](#usage)
4. [Customizing](#customizing)
5. [Example](#example)
6. [License](#license)

About
-----

The goal of this project is to provide a simple and robust framework that
enables the swift creation of client-server applications. The framework
abstracts away the usual networking overhead by providing a pair of
client/server programs pre-written to manage all network-related
tasks. This allows the programmer to primarily focus on customization.

**What is provided:**

* Two programs: 'client' and 'server'

**What can they do:**

* Connect to each other over a TCP/IP network socket.

* Easily exchange data, evaluate, and react.

* Execute custom-written functions.

* Log data to a text file.

* Server: Manage up to 1028 concurrent client connections.

* Server: Detect and disconnect inactive clients.

Tools provided by the framework allow you to send data from within your
functions, analyse data received, log info to a text file, or terminate
a connection. A tool to loop over all active connections is also provided,
mostly for the server component, since the client only ever has one connection.

Potential projects that may wish to use this repository as a springboard
point range from a card game, to an infrastructure monitoring solution,
to a Napster clone.

Since this project is coded in C, knowledge of C is required.

See 'Customizing' for information about how to customize these programs
for your specific needs.

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

The resulting binaries will be located in the spring-server/bin directory.

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
and server programs can successfully connect but will not echange data or
perform and tasks. Since the client hasn't been customized to send any data,
the server will disconnect the client after about 10-15 seconds due to
inactivity (this is configurable).

Customizing
-----------

There are only 3 files that you ever need to modify in order to customize the client and server components.

1. **config.h** - Contains global settings used by both the client and server.
2. **server.c** - Contains custom function definitions for the server.
3. **client.c** - Contains custom function definitions for the client.

##### Defining Custom Functions

There are 4 classes of custom functions. Each class of function is
executed differently.

1. **Connect Function** - Executed when a new connection is made.
2. **Disconnect Function** - Executed when a connection is terminated.
3. **Periodic Function** - Executed at a timed interval (configured in config.h).
4. **Command Function** - Executed in response to a command sent from the client or server.

Because the client and server components expect the functions to be defined
with specific function parameters and return values, each function type
has it's own macro to simplify the process. below is how each of the four
function classes should be defined using the macros.

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
expected function parameters and return values. Be sure to replace
"function_name" with a unique function name.

Connect and Disconnect functions are passed the integer value for the
socket that connected or disconnected, defined as `int socket`.

Command functions are passed the integer value of the socket that sent
the command, and a string pointer optionally containing a data payload.
They are defined as `int socket` and `char *payload`.

Periodic functions are not passed any parameters.

All functions have a return value of `void`.

##### Executing Custom Functions

Once functions are defined using the templates above, you must tell the
client and server components when to execute them. This is done inside the
server_init() and client_init() functions which are defined inside the
client.c and server.c files respectively, which should be just below where
you defined your custom functions.

```
set_connect_function(&function_name);
set_disconnect_function(&function_name);
add_periodic_function(&function_name);
add_command_function("cmnd", &function_name);

```

Only one connect and disconnect function can be set.

Multiple periodic functions can be set. The are executed in the order of which
they were added inside the server/client init function. The max allowed
is configured in config.h (see PERIODIC_LIMIT).

Multiple command functions can also be set, but each function must be
paired with a unique command string when it is set. The command string
tells the client or server to execute the function associated with the
given command string when it received the command from over the network.

Commands are sent from the client or server using a tool provided with
this framework. More on this in the next section.

##### Function Tools

Several tools are provided with this framework to make it simple to
exchange data, and perform other common tasks, from within your custom
functions.

```
write_socket(int socket, char *command, char *payload);
```

Write the command and payload strings to the given socket. This will
initiate the function associated with the command on the receiving side.
The function will be passed the payload string for analysis, logging, etc.

```
void close_socket(int socket);
```

Close the given socket and terminate the connection. Note that this will
execute a disconnect function if one is defined.

```
int main_socket(void);
```

Return the integer value for the main socket. For the client, this returns
the socket for the server connection. For the server, this returns the
socket that is used to listen for new connections.

```
int next_socket(void);
```

Used for looping over all connected sockets. This is mainly provided for
the server component, so it can loop over the socket list for all connected
clients.

```
void reset_next_socket(void);
```

Used to set the next_socket() loop position to the start of the socket list.

```
void terminate(void);
```

Terminate the program. This causes the program to exit.

```
write_log(char *format_string, ...);
```

Write an entry to the log file. Uses printf formatting and parameters.

```
print_log(char *format_string, ...);
```

Same as write_log() but also prints the formatted string to stdout.

Example
-------

##### TODO


License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
