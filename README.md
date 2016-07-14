Spring Server
=============

The goal of this project is to act as a springboard for applications that require the exchange of
data between a server and one or more clients over a TCP/IP network. Two programs are provided, client
and server, that together act as a communication framework upon which you can easily exchange data.

The server component can manage up to 1028 concurrent client connections.

Both the server and client components execute a simple handshake and utilize two-way heartbeat
polling to ensure that any unexpected disconnections are handled gracefully.

The heartbeat polling feature makes use of a simple command-response protocol that can be
customized and expanded upon.

Potential projects that may wish to use this repository as a jumping-off point range from a simple
poker game, to an infrastructure monitoring solution, to a Napster clone.

Since this project is coded in C, knowledge of C is required.

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

These applications already use a simple communication protocol for heartbeat polling and disconnecting. The easiest way to customize these programs is to build on this protocol.


License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
