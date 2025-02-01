# 42 School - Webserv

Webserv is an HTTP server written in __C++98__, designed to be compliant with __HTTP/1.1__ specifications. It manages multiple simultaneous connections and supports __GET__, __POST__ and __DELETE__ methods.

## Features
- Handle multiple ports and virtual hosts
- Support HTTP methods: __GET__, __POST__, __DELETE__
- Custom error pages support
- CGI support for executing dynamic scripts
- Configurable via configuraton file inspired by Nginx

## Installation
### Compilation

```make
make		# Compiles the project
```

### Cleaning
```make
make clean	# Removes object files
make fclean	# Removes object files and the executable
make re 	# Recompiles the entire project
```

### Usage
```sh
./webserv [configuration_file]
```
If no configuration file is provided, a default one will be used (config/webserv.conf)

## Configuration
### Writing a Configuration File
The configuration file is structured similarly to Nginx, using blocks and directives to define the server's behavior. Each server block specifies settings such as listening ports, domain names, and routes. The configuration is hierarchical, with nested directives for fine-grained control.
Each directive must end with a semicolon `;`.

### Blocks
- __server:__ Defines a server block that listens on a specific port
- __location:__ Specifies route handling
---
### Directives
#### listen
```
Syntax:		listen <port>;
Default:	80
Context: 	server
```
Specifies the port number the server will listen on.
_The valid range is beetwen __1__ and __65535__ Ports below __1024__ requires sudo privileges._

---
#### server_name
```
Syntax:		server_name [name];
Default:	"_"
Context: 	server
```
Defines a name for the server.

---
#### client_max_body_size
```
Syntax:		client_max_body_size <size>[k|m|g];
Default:	1m
Context: 	server, location
```
Sets the maximum allowed size of the client request body.

_Valid Range must be any positive integer followed by a size unit (k, m, g). The value will be converted to bytes must not exceed __MAX_INT__._

---
#### error_page
```
Syntax:		error_page <code> <uri>;
Default:	None (uses built-in error responses)
Context: 	server
```
Configures custom error pages for specific HTTP status codes.

---
#### default_server
```
Syntax:		default_server;
Default:	None (first defined host:port will be the default)
Context: 	server
```
Defines whether the server block should act as the default for a given host:port. This means that if multiple server blocks listen on the same host:port, the default one will handle requests that do not match any other explicitly defined server block.

_If this flag is omitted, no server will be explicitly marked as the default, and the first declared server block for that host:port will be automatically designated as the default._
