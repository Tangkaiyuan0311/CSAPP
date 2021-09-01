#include "wrappers.h"

// perform the transaction with fd (connection fd)
void doit(int fd);

// process HTTP request headers, default behavior: ignore
void read_requesthdrs(rio_t* rp);

// parse URI from the request line into filename and optionally CGI program arguments
// return: 1(static content), 0(dynamic content)
int parse_uri(char* uri, char* filename, char* cgiargs);

// serve static content filename of filesize to connection fd
void serve_static(int fd, char* filename, int filesize);

// determine MIME type of filename by examining extention name
void get_filetype(char* filename, char* filetype);

// serve dynamic content of CGI program filename with cgiargs to connection fd
void serve_dynamic(int fd, char* filename, char* cgiargs);

// part of HTTP reponse header in case of error
void clienterror(int fd, char* cause, char* errnum, char* shortmsg, char* longmsg);

extern char **environ;

// iterative server loop
int main(int argc, char** argv) {
	int listenfd, connfd;
	char hostname[MAXLINE], port[MAXLINE];
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;

	// check command line args
	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}

	if ((listenfd = open_listenfd(argv[1])) < 0) {
		fprintf(stderr, "open_listenfd error\n");
		exit(1);
	}
	// server loop
	while (1) {
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);
		getnameinfo((SA*)&clientaddr, clientlen, 
				hostname, MAXLINE, 
				port, MAXLINE, 0);
		printf("accept connnection from (%s, %s)\n", hostname, port);
		doit(connfd); // process HTTP transaction
		Close(connfd);
	}
}


void doit(int fd) {
	int is_static;
	struct stat sbuf; // file metadata
	char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	char filename[MAXLINE], cgiargs[MAXLINE];
	rio_t rio;

	// read request line into (method, uri, version)
	Rio_readinitb(&rio, fd);
	Rio_readlineb(&rio, buf, MAXLINE);
	printf("Request line:\n");
	printf("%s", buf);
	sscanf(buf, "%s %s %s", method, uri, version);
	if (strcasecmp(method, "GET")) {
		clienterror(fd, method, "501", "Not implemented", "Tiny does not implement this method");
		return;
	}

	// read and ignore request headers
	read_requesthdrs(&rio);

	// parse URI for GET request
	is_static = parse_uri(uri, filename, cgiargs);
	if (stat(filename, &sbuf) < 0) { // get file metadata
		clienterror(fd, filename, "404", "Not Found", "Tiny couldn't find the file");
		return;
	}
	
	if (is_static) {
		// regular file?  read permission?
		if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) { 
			clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't read the file");
			return;
		}
		serve_static(fd, filename, sbuf.st_size);
	}
	else { 
		if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
            clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't run the CGI program");
            return;
        }
        serve_dynamic(fd, filename, cgiargs);
	}

}

void clienterror(int fd, char* cause, char* errnum, char* shortmsg, char* longmsg) {
	char buf[MAXLINE], body[MAXLINE];

	// build the HTTP response body
	sprintf(body, "<html><title>Tiny Error</title>");
	sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
	sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
	sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
	sprintf(body, "%s<hr><em>The Tiny Web Server</em>\r\n", body);

	// print HTTP response
	sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
	Rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf)); // write reponse line&headers
	Rio_writen(fd, body, strlen(body)); // write response body
}

// trivial, print request headers
void read_requesthdrs(rio_t* rp) {
	char buf[MAXLINE];
	Rio_readlineb(rp, buf, MAXLINE);
	while (strcmp(buf, "\r\n")) { // not terminating line
		Rio_readlineb(rp, buf, MAXLINE);
		printf("%s", buf);
	}
	return;
}

int parse_uri(char* uri, char* filename, char* cgiargs) {
	char* ptr;

	if (!strstr(uri, "cgi_bin")) { // uri denotes static content
		strcpy(cgiargs, "");
		// assume static content stored in current directory
		strcpy(filename, ".");
		strcat(filename, uri); // ./<filename>
		if (uri[strlen(uri)-1] == '/') // default extention
			strcat(filename, "home.html");
		return 1;
	}
	else { // dynamic content
		ptr = index(uri, '?'); // get args
		if (ptr) {
			strcpy(cgiargs, ptr+1);
			*ptr = '\0'; // make filename a string, separate filename and args
		}
		else
			strcpy(cgiargs, "");
		strcpy(filename, ".");
		strcat(filename, uri); // '\0' pointed by ptr denotes the end of filename
		// ./cgi_bin/...
		return 0;
	}
}

// supporting filetype of server
void get_filetype(char* filename, char* filetype) {
	if (strstr(filename, ".html"))
		strcpy(filetype, "text/html");
	else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
	else if (strstr(filename, ".png"))
        strcpy(filetype, "image/png");
	else if (strstr(filename, ".jpeg"))
        strcpy(filetype, "image/jpeg");
	else
        strcpy(filetype, "text/plain");
}

void serve_static(int fd, char* filename, int filesize) {
	int srcfd;
	char *srcp, filetype[MAXLINE], buf[MAXLINE];

	// construct and send response line&headers
	get_filetype(filename, filetype);
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
	sprintf(buf, "%sConnection: close\r\n", buf);
	sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
	sprintf(buf, "%sContent-type: %s\r\n", buf, filetype);
	Rio_writen(fd, buf, strlen(buf));
	printf("Response header: \n");
	printf("%s", buf);

	// send response body
	srcfd = Open(filename, O_RDONLY, 0);
	srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0); // error protection is ommited
	Close(srcfd);
	Rio_writen(fd, srcp, filesize);
	munmap(srcp, filesize);
}


void serve_dynamic(int fd, char* filename, char* cgiargs) {
	char buf[MAXLINE], *emptylist[] = {NULL};

	// HTTP response
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	Rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

	if (Fork() == 0) {
		// set only one CGI args
		setenv("QUERY_STRING", cgiargs, 1);
		Dup2(fd, STDOUT_FILENO); // redirect stdout to client connection fd
		Execve(filename, emptylist, environ);
	}
	Wait(NULL);
}











