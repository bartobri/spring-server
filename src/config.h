#ifndef CONFIG_H
#define CONFIG_H 1

/*
 * The DEFAULT_HOST configuration sets the default host used by both the
 * client and server.
 * 
 * Some examples of valid options are:
 * 
 * Empty String    e.g. ""
 * IP Address      e.g. "192.168.0.1"
 * Hostname        e.g. "localhost"
 * 
 * The server uses this string to determine which network interface to
 * bind to. An empty string causes the server to listen for
 * incoming connections on all network interfaces. When set as an IP
 * address or a hostname, the server will only allow incoming connections
 * on the associated interface.
 * 
 * The client uses this string to determine which address to connect to
 * when starting. If set to an empty string, the use of the -h
 * command line option is required to set the hostname.
 * 
 * This needs to be a string literal. If you exclude the double-quotes,
 * bad things will happen.
 * 
 * The use of the -h command line option will override this setting.
 */
#define DEFAULT_HOST ""

/*
 * The DEFAULT_PORT configuration sets the default port used by both the
 * client and server. The server will use this value when listening for
 * incoming connections. The client will use this value when attempting
 * to connect to the server.
 * 
 * The use of the -p command line option will override this setting.
 */
#define DEFAULT_PORT 51717

/*
 * The COMMAND_LIMIT configuration sets the limit for the number of commands
 * that you can build programmatic responses for. The default setting of
 * 30 should be sufficient for most cases. If you require more, increase
 * this value as necessary.
 */
#define COMMAND_LIMIT 30

/*
 * The MAX_COMMAND_SIZE configuration tells the client and server the
 * maximum number of characters that a command string may contain. If you
 * send a command string with a length that is greater than this setting,
 * the software will try to handle it appropriately, but you risk
 * truncating a portion of the payload data as a result. It is better to
 * simply increase this configuration as needed. Though this software can
 * handle up to 99 command string characters, it is suggested you keep it
 * at a reasonable limit of 10 or less for optimal memory consumption.
 * Never exceed a 99 or you risk potential memory fault errors.
 */
#define MAX_COMMAND_SIZE 5

/*
 * The MAX_PAYLOAD_SIZE configuration sets the maximum number of characters
 * that can be contained in the payload portion of a transmission. If you
 * need to send a length of data longer than this value, increase as
 * necessary. Physical memory limitations may apply.
 */
#define MAX_PAYLOAD_SIZE 2000

/*
 * The PERIODIC_SECONDS configuration sets the interval, in seconds, at
 * which the periodic functions are executed. Since both the server and
 * client execute as a single process, delays in periodic execution can
 * occur if other programmed functions are executing for a period of time
 * greater than this setting.
 */
#define PERIODIC_SECONDS 5

/*
 * The PERIODIC_LIMIT configuration sets the limit for the number of
 * periodic functions you can create. The default setting of 30 should be
 * sufficient for most cases. If you require more, increase this value as
 * necessary.
 */
#define PERIODIC_LIMIT 30

/*
 * The IDLE_SECONDS configuration sets number of seconds a client can be
 * idle before the server will automatically disconnect them. This is to
 * protect the server from maintaining connections with clients that have
 * unexpectedly disconnected, such as in the event of a crash or power failure.
 * 
 * In order to prevent a functioning client from being disconnected during
 * long periods of time when it not required to communicate with the server,
 * it is suggested to set up a periodic function that sends a heartbeat
 * to the server. In this case, be sure that this value is not set lower
 * than the PERIODIC_SECONDS value.
 */
#define IDLE_SECONDS 10

/*
 * The SERVER_LOG_NAME configuration sets the name of the file that the
 * server logs information to. Change it as needed. This must be a string
 * literal. If you exclude the double-quotes, bad things will happen.
 */
#define SERVER_LOG_NAME "server_log.txt"

/*
 * The CLIENT_LOG_NAME configuration sets the name of the file that the
 * client logs information to. Change it as needed. This must be a string
 * literal. If you exclude the double-quotes, bad things will happen.
 */
#define CLIENT_LOG_NAME "client_log.txt"

/*
 * The LOG_FILE_PATH configuration sets the path to the directory that
 * contains the log files. This can be a relative path or an absolute path.
 * A relative path is relative to the executing user's home directory.
 * The HOME environmental variable must be set for relative paths.
 * 
 * The client and server will attempt to create any directories in the path
 * that do not exist.
 * 
 * This must be a string literal. If you exclude the double-quotes, bad
 * things will happen.
 */
#define LOG_FILE_PATH ".config/spring_server/"

/*
 * The INPUT_QUEUE_SIZE configuration sets the input queue size.
 * 
 * Sometimes it is necessary to send multiple back-to-back data transmissions
 * from the client or server in a single instance. In this case, the
 * receiving operating system may concatenate them together as one long
 * string of data before the client or server application has a chance to
 * read it from the socket file descriptor. When this happens, the client
 * or server will deconstruct the data into multiple command/payload pairs
 * and treat them separately as intended. During this deconstruction, it
 * places them in an "input queue". This configuration sets the maximum
 * number of command/payload pairs it can parse from the socket file
 * descriptor in a single instance.
 * 
 * It is not likely that you will have to change this value.
 */
#define INPUT_QUEUE_SIZE 10

#endif
