#include "httpd.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include "gen_html.h"
#include "process_httppost.h"

#define CONNMAX 5 

// Client request

char    *method,    // "GET" or "POST"
				*uri,       // "/index.html" things before '?'
				*qs,        // "a=1&b=2"     things after  '?'
				*prot;      // "HTTP/1.1"

char    *payload;     // for POST
int      payload_size;
//

static int listenfd, clients[CONNMAX];
//static void error(char *);
static void startServer(const char *);
static void respond(int);

typedef struct { char *name, *value; } header_t;
static header_t reqhdr[17] = { {(char*)"\0", (char*)"\0"} };
static int clientfd;

static char *buf;

// for debug
void print_clients() {
	for( int i = 0 ; i < CONNMAX ; i++ ) {
		printf( "%5d ", clients[i]  ); 
	}
	printf("\n");
	return;
}

void serve_forever(const char *PORT)
{
		struct sockaddr_in clientaddr;
		socklen_t addrlen;
		
		int slot=0;
		
		fprintf(stderr,
						"Server started %shttp://127.0.0.1:%s%s\n",
						"\033[92m",PORT,"\033[0m"
						);

		// Setting all elements to -1: signifies there is no client connected
		int i;
		for (i=0; i<CONNMAX; i++)
				clients[i]=-1;
		startServer(PORT);
		
		// Ignore SIGCHLD to avoid zombie threads
		signal(SIGCHLD,SIG_IGN);

		// ACCEPT connections
		pid_t pid;
		while (1)
		{
				addrlen = sizeof(clientaddr);
				clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

				if (clients[slot]<0)
				{
						perror("accept() error");
				}
				else
				{
						pid = fork();
						if( pid == -1 ) {
							perror("Can't fork()!");
						}
						else if ( pid == 0 ) // child
						{
								close(listenfd);
								respond(slot);
								exit(0);
						} else {  // parent
							close(clients[slot]);
							clients[slot] = -1;
						}
				}
				//print_clients(); // for debug

				while (clients[slot]!=-1) slot = (slot+1)%CONNMAX;
		}
}

//start server
void startServer(const char *port)
{
		struct addrinfo hints, *res, *p;

		// getaddrinfo for host
		memset (&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		if (getaddrinfo( NULL, port, &hints, &res) != 0)
		{
				perror ("getaddrinfo() error");
				exit(1);
		}
		// socket and bind
		for (p = res; p!=NULL; p=p->ai_next)
		{
				int option = 1;
				listenfd = socket (p->ai_family, p->ai_socktype, 0);
				setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
				if (listenfd == -1) continue;
				if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
		}
		if (p==NULL)
		{
				perror ("socket() or bind()");
				exit(1);
		}

		freeaddrinfo(res);

		// listen for incoming connections
		if ( listen (listenfd, 1000000) != 0 )
		{
				perror("listen() error");
				exit(1);
		}
}


// get request header
char *request_header(const char* name)
{
		header_t *h = reqhdr;
		while(h->name) {
				if (strcmp(h->name, name) == 0) return h->value;
				h++;
		}
		return NULL;
}

//client connection
void respond(int n)
{
		int rcvd;

		buf = (char*)malloc(65535);
		//memset( buf, '\0', sizeof( buf );
		rcvd=recv(clients[n], buf, 65535, 0);

		if (rcvd<0)    // receive error
				fprintf(stderr,("recv() error\n"));
		else if (rcvd==0)    // receive socket closed
				fprintf(stderr,"Client disconnected upexpectedly.\n");
		else    // message received
		{
				buf[rcvd] = '\0';

				method = strtok(buf,  " \t\r\n");
				uri    = strtok(NULL, " \t");
				prot   = strtok(NULL, " \t\r\n"); 

				fprintf(stderr, "\x1b[32m + [%s] %s\x1b[0m\n", method, uri);
				
				qs = strchr(uri, '?');
				if (qs)
				{
						*qs++ = '\0'; //split URI
				} else {
						qs = uri - 1; //use an empty string
				}

				header_t *h = reqhdr;
				char *t, *t2;
				while(h < reqhdr+16) {
						char *k,*v;
						//char *k,*v,*t; // ? eric) duplicated declaration of *t
						k = strtok(NULL, "\r\n: \t"); if (!k) break;
						v = strtok(NULL, "\r\n");     while(*v && *v==' ') v++;
						h->name  = k;
						h->value = v;
						h++;
						fprintf(stderr, "[H] %s: %s\n", k, v);
						t = v + 1 + strlen(v);
						if (t[1] == '\r' && t[2] == '\n') {
							t += 2; // two bytes for CR/LF
							break;
						}
				}
				t++; // now the *t shall be the beginning of user payload
				t2 = request_header("Content-Length"); // and the related header if there is  
				payload = t;
				payload_size = t2 ? atol(t2) : (rcvd-(t-buf));
				if( payload_size )
					fprintf(stderr,"[P] %s\n", payload);

				// bind clientfd to stdout, making it easier to write
				clientfd = clients[n];
				dup2(clientfd, STDOUT_FILENO);
				close(clientfd);

				// call router
				route();

				// tidy up
				fflush(stdout);
				shutdown(STDOUT_FILENO, SHUT_WR);
				close(STDOUT_FILENO);
		}

		// free buffer
		free( buf );

		//Closing SOCKET
		shutdown(clientfd, SHUT_RDWR);         //All further send and recieve operations are DISABLED...
		close(clientfd);
		clients[n]=-1;
}

void route()
{
		ROUTE_START()

		ROUTE_GET("/")
		{
				printf("HTTP/1.1 200 OK\r\n\r\n");
				//printf("Hello! You are using %s", request_header("User-Agent"));
				printf("%s", gen_html_from_stat_file() ); 
		}

		ROUTE_POST("/")
		{
				printf("HTTP/1.1 200 OK\r\n\r\n");
				//printf("Wow, seems that you POSTed %d bytes. \r\n", payload_size);
				//printf("Fetch the data using `payload` variable.");
				std::string body(payload);
				handle_with_body( body );
		}
	
		ROUTE_END()
}

/* eof */

