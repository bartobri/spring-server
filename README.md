Spring Server
=============

This project provides a pair of applications, client and server, that are meant to provide the basic
functionality necessary to establish a connection and reliably exchange data between the two.
The server component can handle over one thousand simultaneous client connections. Both components
execute a simple handshake mechanism and utilize two-way heartbeat polling to ensure that any
unexpected disconnections are handled gracefully.

The aim of this project is to act as a springboard for network application projects ranging from a
simple poker game, to an infrastructure monitoring solution, to a Napster clone. This basic framework
provides a simple starting point to build upon. The existing command-response protocol, which
contains a command for heartbeat polling and a command for disconnecting, can easily be
expanded with a specialized protocol that suits your requirements.

The server component of this project is known as a "single process concurrent server", which means
it can handle up to 1028 concurrent client connections while running as a single process. It can, in
theory, scale beyond 1028 connections when deployed in a two-tier design, with a top tier
server instance coordinating with up to 1028 tier-two server instances, which in turn coordinate with
up to 1028 cient instances, accommodating up to 1056784 simultaneous connections.

Since this project is coded in C, some knowledge of C is required.

Installation
------------

To build both the server and client apps:
```
git clone https://github.com/bartobri/spring-server.git
cd spring-server
make
```

To build just the server app:
```
git clone https://github.com/bartobri/spring-server.git
cd spring-server
make server
```

To build just the client app:
```
git clone https://github.com/bartobri/spring-server.git
cd spring-server
make client
```

The resulting binaries will be located in the spring-server/bin directory. From there you can copy them
to your preferred bin directory.

Usage
-----

To run the server:

```
bin/server
```

The server listens on port 51717 by default. To override the port, use the `-p` option:

```
bin/server -p <port> 
```

To suppress server output, use the `-q` option:

```
bin/server -q
```

To run the client:

```
bin/client -h <hostname> -p <port>
```

Customizing
-----------

These applications already use a simple protocol for heartbeat polling and disconnecting. The easiest way to customize these programs is to expand on this protocol.


License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
