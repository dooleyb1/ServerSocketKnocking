# ServerSocketKnocking
C++ program that connects to a server on a TCP socket and logs the servers response to an HTTP GET request that you send it via the socket.

## Setup

To begin with, clone the repo.
```
git clone https://github.com/dooleyb1/DirectoryTree
```

Compile and build the code.
```
make
```

## Example (Public Webpage)

```
./knock -h www.tcd.ie -p 80 -f response.txt
```

This will send an HTTP GET request to the webpage _[www.tcd.ie](http://www.tcd.ie)_ at the socket listening at port 80 and log the response to the file _[response.txt](https://github.com/dooleyb1/ServerSocketKnocking/blob/master/response.txt)_.

## Example (Local Server)

First run the c++ server program which establishes a server on _localhost_ listening on the port 8888.

```
./server 

Socket created on port 8888

Waiting for incoming connections...

```

Then run the c++ knock program and use it to send an HTTP GET request to the host _localhost_ at port 8888. This example logs the response to the terminal rather than a file.

```
./knock -h localhost -p 8888
```

Server will then notice recieve the connection request and send back a response (defined in server.cpp). If you look back at the knock program the response from the server should have logged to the terminal.

```
Hello knock, this is a response from server acting on localhost:8888 - whatsup
```

## Parameters 

* **h, host:** the DNS name or IP address of the host to contact
* **p, port:** the TCP port on the server to which to connect

* **w, web:** make an HTTP GET request for the "/" resource
* **f, file:** store any output received from socket in file 

* **H, help:** produce a usage message such as that below
* **?, help:** produce a usage message such as that below

## Important

* The host input MUST be an IPv4 or IPv6 address or a Fully Qualified Domain Name (FQDN)
* The port MUST be an integer between 1 and 65535
* The mandatory parameters are the host and port.  
