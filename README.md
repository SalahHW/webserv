# 42 School - Webserv

Webserv is a 42 school project that requires implementing an HTTP server written in __C++98__, designed to be compliant with __HTTP/1.1__ specifications. It manages multiple simultaneous connections and supports __GET__, __POST__ and __DELETE__ methods.

## Features
- Handle multiple ports and virtual hosts
- Support HTTP methods: __GET__, __POST__ and __DELETE__
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
_If no configuration file is provided, the server will use the __default configuration file__ located at `config/webserv.conf`_

## Configuration
### Writing a Configuration File
The configuration file is structured similarly to Nginx, using blocks and directives to define the server's behavior. Each server block specifies settings such as listening ports, domain names, and routes. The configuration is hierarchical, with nested directives for fine-grained control.
Each directive must end with a semicolon `;`.

### Blocks:
- __server:__ 
The server block defines the configuration for an entire server instance. It specifies settings for a server that listens on a specific port and handles incoming HTTP requests. Within this block, you configure directives such as the listening port, server name, error pages, and default server settings. Essentially, this block sets the overall behavior for a virtual host.
- __location:__ 
The location block is nested within a server block and is used to define configurations for specific routes or URIs. It allows for fine-grained control over how requests to particular paths are handled. Within a location block, you can override or extend server-level directives (such as client_max_body_size or custom error pages) to apply unique rules and behavior for that route.

#### Example
```nginx
server {
	listen 8080;
	server_name localhost;
	client_max_body_size 1m;

	# This location block applies to the root URL "/"
	location / {
		# Override client_max_body_size for the root path
		client_max_body_size 2m;
	}

	# This location block applies to URLs starting with "/images/
	location /images {
		# Enable autoindex to list files in the /images directory
		autoindex on;
	}
}
```
In this example:
- The __server__ block sets up a server listening on port `8080` with a specified server name and a maximum request body size of  `1m`.
- The first __location__ block, matching the root path `/`, overrides the `client_max_body_size` to `1m` specifically for requests to `/`.
- The second __location__ block, matching `/images`, enables directory listing with the `autoindex` directive.
---
### Directives:
##### listen
```
Syntax:		listen <port>;
Default:	80
Context: 	server
```
Specifies the port number the server will listen on.  

_The valid range is beetwen __1__ and __65535__ Ports below __1024__ requires sudo privileges._

---
##### server_name
```
Syntax:		server_name [name];
Default:	"_"
Context: 	server
```
Defines a name for the server.

---
##### error_page
```
Syntax:		error_page <code> <uri>;
Default:	None (uses built-in error responses)
Context: 	server
```
Configures custom error pages for specific HTTP status codes.  

_For each HTTP status code, add a separate `error_page` directive on a new line._  

Example:
```nginx
server {
	error_page 404 /error404.html;
	error_page 500 /error500.html;
}
```

---
##### default_server
```
Syntax:		default_server;
Default:	None (first defined host:port will be the default)
Context: 	server
```
Defines whether the server block should act as the default for a given host:port. This means that if multiple server blocks listen on the same host:port, the default one will handle requests that do not match any other explicitly defined server block.  

_If this flag is omitted, no server will be explicitly marked as the default, and the first declared server block for that host:port will be automatically designated as the default._

---
##### client_max_body_size
```
Syntax:		client_max_body_size <size>[k|m|g];
Default:	1m
Context: 	server, location
```
Sets the maximum allowed size of the client request body.  

_Valid Range must be any positive integer followed by a size unit (k, m, g). The value will be converted to bytes must not exceed __MAX_INT__._

---
##### client_time_out
```
Syntax:		client_time_out <time[s]>;
Default:	60s
Context:	server, location
```
Specifies the maximum duration (in seconds) the server will wait for a client to send data. If the client fails to send data within the specified time frame, the connection is terminated.

---
##### client_body_temp_path
```
Syntax:		client_body_temp_path <path>;
Default:	/client_temp
Context:	server, location
```
Specifies the directoriy where the server will store temporary files for client request bodies

---
##### root
```
Syntax:		root <directory>;
Default:	/www
Context:	location
```
Specifies the root directory from which the server will serve files. All file paths in request will be resolved relative to this directory,

Example:
```nginx
location /images {
	root /var/www/example;
	# Requests to "/images" will be served from /var/www/example/images
}
```
In this example, a request to `/images/picture.jpg` will be served from the file located at `/var/www/example/images/picture.jpg`.

---
##### index
```
Syntax:		index <file>;
Default:	index.html
Context:	location
```
Specifies a file to be used as the default index when a directory is requested.

---
##### autoindex
```
Syntax:		autoindex <on|off>;
Default:	off
Context:	location
```
Specifies whether the server should generate a directory listing when a directory is requested and no index file is found.
- When set to `on` the server will display a generated listing of the directory contents.
- When set to `off`, directory listings are disabled.

---
##### return
```
Syntax:		return <status code> <uri>;
Default:	None
Context:	location
```
The `return` directive immediately sends an HTTP response with the specified status code and a redirect to the provided URI. In this implementation, both the status code and the URI are required.

Example:
```nginx
location /old-page {
	return 301 /new-page;
}
```
In this example, when a client requests `/old-page`, the server responds with a 301 status code and redirect the client to `/new-page`.

---
##### deny (TO BE UPDATED)
```
Syntax:		deny all;
Default:	None
Context:	limit_except (within location)
```
Specifies that access is denied for all clients.

Example:
```nginx
location /secure {
	limit_except GET POST {
		deny all;
	}
}
```
In this example, the `/secure` location allows only the GET and POST methods. All other methods are denied access by the `deny all;` directive within the `limit_except` block.

---
##### allowed_methods (TO BE IMPLEMENTED)
Aims to replace deny directive above, wich is too difficult to use.