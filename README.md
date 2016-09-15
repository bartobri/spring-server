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

* Log info to a text file.

* Server: Manage up to 1028 concurrent client connections.

* Server: Detect and disconnect inactive clients.

At the core of these programs is the ability to execute your custom
written functions. Custom functions allow you to make the client and
server do virtually anything within your ability to program. The framework
provides tools for you to automatically execute a function in response
to one of 4 events:

* When a successful connection is established.

* At a (configurable) timed interval.

* In response to the receipt of data.

* When a disconnection occurs.

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

##### TODO

Example
-------

##### TODO


License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
