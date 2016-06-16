SPCServer
=========

This project provides a pair of applications that can be used for bootstrapping your client/server
application. To be explicit, two binaries are provided with this project: a server and a client.

Out of the box, these applications are meant to provide the minimal functionality required to
establish a connection and reliably exchange data between the two. This amounts to nothing more than
a simple handshake and ongoing heartbeat monitoring throughout the life of the connection.

This basic functionality provides a starting point for you to build upon by creating a set of
commands and other kinds of data that can be exchanged between the two.

The server component of this project is known as a "single process concurrent server", which means
it can handle up to 1028 concurrent client connections while running as a single process. It is not
a multi-threaded server. It is not ideal to use for applications that are meant to handle thousands
of simultaneous connections.

The client component is a simple binary that can connect with the server, execute the handshake, and
respond to heartbeat polling from the server.

This project is an ideal starting point for other projects that wish to have a client application
running on multiple machines, all communicating with a central server via a set of commands that you
develop.

Installation
------------

To build both the server and client apps:
```
git clone https://github.com/bartobri/SPCServer.git
cd SPCServer
make
```

To build just the server app:
```
git clone https://github.com/bartobri/SPCServer.git
cd SPCServer
make server
```

To build just the client app:
```
git clone https://github.com/bartobri/SPCServer.git
cd SPCServer
make client
```

The resulting binaries will be located in the SPCServer/bin directory. From there you can copy them
to your preferred bin directory.

Usage
-----

To run the server:

```
bin/server
```

The server listens on port 51717 by default. To override the port, use the `-p` option:

```
bin/server -p 51718
```

To run the client:

```
bin/client <hostname> <port>
```

License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the MIT License (MIT). See [LICENSE](LICENSE) for more details.
