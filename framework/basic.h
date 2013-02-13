#ifndef __BASIC_H__
#define __BASIC_H__

// basic libs
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h> // contains memset(), oddly enough

// socket includes
#include <sys/types.h>   // Types used in sys/socket.h and netinet/in.h
#include <netinet/in.h>  // Internet domain address structures and functions
#include <sys/socket.h>  // Structures and functions used for socket API
#include <netdb.h>       // Used for domain/DNS hostname lookup
#include <arpa/inet.h>	 // Used for inet_ntop()

// more includes for select(), signal stuff, etc.
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <signal.h>

// stl containers and other C++/C-ish stuff
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>

#endif