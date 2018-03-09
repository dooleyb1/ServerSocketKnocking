#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>		
#include <fstream>
#include <iostream>

using namespace std;
#define BUFSIZE 4096

//Globals
bool hostflag, portflag, helpflag, webflag, fileflag;
fstream f;

//Function for reading and writing data recievedfrom socket
static int readSocket(int socket, char *buffer);

//Usage function
void usage(int n)
{
  string usageString("usage: ./knock -h host -p port [-H] [-w] [-f file]");

  cout << usageString << "\n";
  exit(0);
}

int main(int argc, char **argv)
{
    //Initialise all flags to false
    hostflag = portflag = helpflag = webflag = fileflag = false;
    
    //Initialise variables
    char host[500];
    char resource_path[500];
    char request[1000];
    struct hostent *server;
    struct protoent *pr;
    struct sockaddr_in serveraddr;
    int port;
    int result;

	int n,i,j;
	//Process command line args
	for(n=i=1;i<argc;i=n) {
		n++;
		//If arg starts with '-' do this
		if (argv[i][0] == '-' && argv[i][1]) {
		  for(j=1;argv[i][j];j++) {
			switch(argv[i][j]) {
			case 'h':
			  hostflag = true;
			  memcpy(host, argv[i+1], strlen(argv[i+1])+1);
			  break;
			case 'p':
			  portflag = true;
			  port = atoi(argv[i+1]);
			  break;
			case 'H':
			  helpflag = true;
			  break;
			case 'w':
			  webflag = true;
			  memcpy(resource_path, argv[i+1], strlen(argv[i+1])+1);	
			  break;
			case 'f':
			  fileflag = true;
			  f.open(argv[i+1],ios::out);
			  break;
			case '?':
			  helpflag = true;
			  break;
			default:
			  //If no flag has been set, then invalid input 
			  if(!hostflag && !portflag && !helpflag && !webflag && !fileflag) {
				  fprintf(stderr,"knock: Invalid argument -`%c'.\n",argv[i][j]);
				  usage(1);
				  exit(1);
				  break;
			  }
			}
		  }
		}
		//If arg doesn't start with '-' do this
		else if(argv[i][0] != '-') {
			for(j=0;argv[i][j];j++) {
	    		if (!strcmp("host", argv[i])) {
	     		  hostflag = true;
			  memcpy(host, argv[i+1], strlen(argv[i+1])+1);
	     		  break;
	    		}
	    		if (!strcmp("port",argv[i])) {
	    		  portflag = true;
			  port = atoi(argv[i+1]);
	     		  break;
	    		}
			if (!strcmp("help",argv[i])) {
	    		  helpflag = true;
	     		  break;
	    		}
			if (!strcmp("web",argv[i])) {
	    		  webflag = true;
			  memcpy(resource_path, argv[i+1], strlen(argv[i+1])+1);
	     		  break;
	    		}
			if (!strcmp("file",argv[i])) {
	    		  fileflag = true;
			  f.open(argv[i+1],ios::out);	
	     		  break;
	    		}
			//If no flag has been set, then invalid input 
			else if(!hostflag && !portflag && !helpflag && !webflag && !fileflag) {
			   fprintf(stderr,"knock: Invalid argument -`%s'.\n",argv[i]);
			   usage(1);
			   exit(1);
			   break;
			 }
			}
		}
	}
	
	//Handle invalid input or helpflag
	if(!hostflag || helpflag) {
		if(helpflag)
			usage(1);
		else {
			printf("No host defined\n");
			usage(1);
		}
	}
	
	else{
	//Create socket 
	pr = getprotobyname("tcp");
	
	int tcpSocket = socket(AF_INET, SOCK_STREAM, pr->p_proto);
     
    	if (tcpSocket < 0)
        	printf("\nError opening socket");
    	else
        
	//Get IP Address using DNS
    	server = gethostbyname(host);
     
    	if (server == NULL)
    	{
        	printf("gethostbyname() failed\n");
    	}
 
	//Format server address
    	bzero((char *) &serveraddr, sizeof(serveraddr));
    	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    	serveraddr.sin_port = htons(port);
	
	//Connect to socket
	if (connect(tcpSocket, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
        	printf("\nError Connecting");
 
	//Format HTTP GET Request 
	if(webflag)
    		sprintf(request, "GET /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n", 		resource_path, host);

	else
		sprintf(request, "GET / HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n", host);
		
	//Send HTTP GET Request
	if( send(tcpSocket , request , strlen(request) , 0) < 0)
    	{
        	printf("Send failed");
        	return 1;
    	}
	
	//Receive response and write to file
	char* buf = new char[100000];
	bzero(buf, 100000);
	readSocket(tcpSocket, buf);
    	return 0;
  }
}

//Function to handle socket responses of specific lengths
static int readSocket(int socket, char *buffer)
{
    ssize_t bytes_read = 0;
    bytes_read = recv(socket, buffer, BUFSIZE - 1, 0);
    int i=0;
    while (bytes_read > 0)
    {
		//Null terminate buffer
	        buffer[bytes_read] = 0; 
		//If fileflag set, write to file 
		if(fileflag)
		{
		    f<<buffer;
		    bytes_read = recv(socket, buffer, BUFSIZE - 1, 0);
		}
		//Otherwise print buffer to console
		else
		{
	            printf("\n%s\n", buffer);	
	            bytes_read = recv(socket, buffer, BUFSIZE - 1, 0);	
		}	
    }
    return 0;
}

