SPCServer
=========

This project provides a pair of applications, client and server, that are meant to provide the basic
functionality necessary to establish a connection and reliably exchange data between the two. This
includes an initial handshake mechanism and ongoing connection monitoring via heartbeat polling.

Up to 1028 client instances can connect to the server and communicate independently.

This basic framework provides a simple starting point for you to build upon. With minimal
effort, you can create a command/response protocol to exchange information between the client and
server. This is ideal for applications requiring multiple deployed agents that need to send data
about their environment to a central server where a potential action can be taken, which potentially
includes the server invoking a command on the agent itself.

The server component of this project is known as a "single process concurrent server", which means
it can handle up to 1028 concurrent client connections while running as a single process. It is not
a multi-threaded server. It is not ideal to use for applications that are meant to handle thousands
of simultaneous connections.

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

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
