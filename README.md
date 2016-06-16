SPCServer
=========

This project provides a pair of applications that can be used for bootstrapping your client/server
application.

The Server
----------

The server component of this project is known as a "single process concurrent server", which means
it can handle up to 1028 concurrent client connections while running as a single process.

It is not a multi-threaded server. It is not ideal to use for applications that are meant to handle
thousands of simultaneous connections.

This server comes prepackaged with basic functionality you will likely need, such as client connection
monitoring via a heartbeat polling. The server will send a heartbeat to all clients and will expect
a response within a reasonable amount of time. If it does not get a response, the server will
disconnect the client.

The Client
----------

The client component is programmed to connect to the server and respond to heartbeat requests as to
not get disconnected.

License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the MIT License (MIT). See [LICENSE](LICENSE) for more details.
