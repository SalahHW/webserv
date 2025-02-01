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