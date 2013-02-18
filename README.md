General
=======

This is an implementation of an HTTP caching proxy based loosely upon the skeleton set forth by the upstream parent of this fork. The proxy is built upon a modular framework that abstracts as much as possible the handling of particular protocols from the operation of the server itself (specifically, listening for incoming client connections and passing them off to protocol-specific handlers.)

The project is designed to be reusable, and while the framework is currently in its infancy it can conceivably be extended in various ways. At the moment, the reference SocketServer implementation uses a forking model (fork()ing with each new incoming client, then spawning threads to handle each clients' specific requests), but SocketServer could be extended to support a select()-based model in order to minimize the number of processes required.

The project is broadly divided into the following classes:

*SocketServer:* Represents the server itself; initialized by specifying the port on which connections should be received. Protocol-specific "adapters" (described below) can be attached to the server via SocketServer.attach(adapter). Once an adapter is attached, its callback functions will be called by SocketServer when appropriate events occur; the callbacks are passed an instance of a Client class that wraps the connecting client's socket, through which the adapter communicates. Multiple adapters can be attached to the server, and their callbacks will be invoked in the order that they were attached. Returning a value greater than 0 from any callback indicates that no further adapters in the adapter chain should be invoked for that event.

*ProtocolAdapter:* Represents the implementation of a particular task over a given application-level protocol, e.g. HTTP proxying. At the moment, only onConnect(client) is available, which handles the entire session with a particular connecting client.

*ProxyAdapter:* Subclass of ProtocolAdapter, attached to SocketServer. Represents the task of proxying HTTP requests for incoming HTTP clients. This implementation spawns a separate thread for each GET request, but join()s immediately after the thread is created. Once a given amount of time has passed (30 seconds in this build) without a response from the upstream server, the GET thread returns a "504 Gateway Timeout". The proxy supports conditional GETs via the If-Modified-Since header. For purposes of resource efficiency, the ProxyAdapter allows only ten upstream connections per connecting client.

*Cacher:* Represents the cache where previous requests to upstream servers are stored. The cache elements and metadata (i.e. the original host, port, path, modified and expiration dates, etc.) are stored in a sqlite database for ease of retrieval. As a result, the maximum size of a cached element is the maximum size of a blob in sqlite3, ~1GB at the time of writing. Cache elements are manipulated via the CacheEntry class, which provides accessors and convenience methods for manipulating and persisting the cached element it represents.
(Using the "libdbapi" SQLite wrapper library, courtesy of http://code.google.com/p/libdbapi/.)

*Clients:* There are two client classes: Client, which wraps a handle to a BSD socket and provides convenience methods for sending and recieving using standard C++ container classes, and HttpClient, which adds additional functionality for communicating with HTTP servers and clients specifically. Note that HttpClient does not inherit from Client -- instead, it receives a Client reference in its constructor and uses it to proxy requests between the caller and the underlying Client object. This was designed in order to maintain the separation between SocketServer and any particular protocol, granted that SocketServer creates Client instances and should not be explicitly aware that an Adapter uses HTTP.

Build Directions (specific to UCLA lnxsrv)
===================

Due to restrictions on lnxsrv, several steps necessary to compile and run (if you're using this skeleton):

1. The following lines should be added to your ~/.bashrc

export PATH=/usr/local/cs/bin:$PATH
export LD_LIBRARY_PATH=/u/cs/grad/yingdi/boost/lib:/usr/local/lib64/:$LD_LIBRARY_PATH

2. To configure environment

./waf configure

If there are any errors, please email me.

3. To build/rebuild the code

./waf

4. All compiled executables are located in build/, so you can run them as this:

build/http-proxy

