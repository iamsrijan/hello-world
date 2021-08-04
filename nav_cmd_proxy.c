#include <stdio.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX_STRING 255 
#define NAV_PORT 3316 
#define MAX_CONN 10
#define NAV_TTY "/dev/pts/20"
  
int send_cmd_to_nav(char *cmd, char *out)
{
 	int fd = open(NAV_TTY, O_RDWR);   
    int ret;
    char cmd_buf[1024], buf[1024];

    if (fd < 0) {
        perror("Sim:");
        exit(1);
    }

    //ret = write(fd, "<_navw_curstate_>\n",18);
	sprintf(cmd_buf,"%s\n", cmd);
	printf("cmd: %s Len: %d\n", cmd, strlen(cmd)+1);
    ret = write(fd, cmd_buf, strlen(cmd_buf)+1);
    printf("ret: %d\n", ret);
    sleep(1);

#if 0
    ret = read(fd, buf, 255);
    printf("ret: %d\n", ret);
    printf("buf: %s\n", buf);
#else
    ret = read(fd, out, 255);
    printf("ret: %d\n", ret);
    printf("out: %s\n", out);
#endif

	close(fd);

}

/* Read Client API call and response */
void read_request_and_reply(int sockfd) 
{ 
    char buf[MAX_STRING]; 
    char out[MAX_STRING]; 
    int read_bytes, n; 

    for (;;) { 
        bzero(buf, MAX_STRING); 
  
        /* Read Client API request */
        read_bytes = read(sockfd, buf, sizeof(buf)); 
		printf("Read %d bytes\n", read_bytes);

		//if (read_bytes > 0) { //TODO: Need to implement select call
        	printf("Client Req: %s\n", buf); 

			/* pass the command to NAV controller and read response */
			send_cmd_to_nav(buf, out);
			printf("Response: %s Len: %d\n", out, strlen(out));
  
        	/* Send the response to client  */
        	write(sockfd, out, sizeof(out)); 
		//}
  
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buf, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
} 
  
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in srv_addr, cli_addr; 
  
    /* Create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Error: Unable to create socket\n"); 
        exit(0); 
    } 
    else {
        printf("Socket successfully created..\n"); 
	}

    bzero(&srv_addr, sizeof(srv_addr)); 
  
    /* Assign IP adress & Port */
    srv_addr.sin_family = AF_INET; 
    srv_addr.sin_port = htons(NAV_PORT); 
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  
    /* Bind */
    if ((bind(sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr))) != 0) { 
        printf("Socket bind failed!\n"); 
        exit(0); 
    } 
    else {
        printf("Bind succeeded.\n"); 
	}
  
    /* Listen */
    if ((listen(sockfd, MAX_CONN)) != 0) { 
        printf("Listen failed!\n"); 
        exit(0); 
    } 
    else {
        printf("Server listening..\n"); 
	}
    len = sizeof(cli_addr); 
  
    /* Accept the data packet from client */
    connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &len); 
    if (connfd < 0) { 
        printf("acccept failed!\n"); 
        exit(0); 
    } 
    else {
        printf("acccept succeeded.\n"); 
	}
  
    /* Communicate with the client */
    read_request_and_reply(connfd); 
  
    /* Close the socket */
	close(connfd);
    close(sockfd); 
} 
