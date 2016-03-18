#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <string.h> // For memset.
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().
#include <wait.h>
#include <fcntl.h>

int port_num = 24001;
#define BUF_SIZE 1024

int checkRecieve(int rst){
    if(rst==-1){
        perror("Collection error");
        exit(-1);
    }
}

int checkSend(int rst){
    if(rst==-1){
        perror("SENDING FAILED\n");
        exit(-1);
    }
}

int checkClose(int rst){
    if(rst==-1){
        perror("CLOSING FAILED");
        exit(-1);
    }
}

int checkConnect(int rst){
    if(rst==-1){
        perror("CONNECTION FAILED");
        exit(-1);
    }
}

int main(){
    //INITIAL PART...MORE LIKE A BLACK-BOX ;)
    int rst; // Return status of functions.
    int cfd; // File descriptor for the client.

    /**************** Create a socket. *******************************/
    int sfd; // Socket file descriptor.
    sfd = socket (AF_INET, SOCK_STREAM, 0); /* AF_INET --> IPv4,
                 * SOCK_STREAM --> TCP Protocol, 0 --> for the protocol. */
    if (sfd == -1)
    {
        perror ("Client: socket error");
        exit (1);
    }
    printf ("Socket fd = %d\n", sfd);
    /***************** Assign an address of the server **************/
    struct sockaddr_in srv_addr, cli_addr; // Addresses of the server and the client.
    socklen_t addrlen = sizeof (struct sockaddr_in); // size of the addresses.

    // Clear the two addresses.
    memset (&srv_addr, 0, addrlen);

    // Assign values to the server address.
    srv_addr.sin_family = AF_INET; // IPv4.
    srv_addr.sin_port   = htons (28962); // Port Number.

    rst = inet_pton (AF_INET, "127.0.0.1", &srv_addr.sin_addr); /* To
                                  * type conversion of the pointer here. */
    if (rst <= 0)
    {
        perror ("Client Presentation to network address conversion.\n");
        exit (1);
    }
    //INITIAL PART NOT THAT IMPORTANT


    /***************** Connect to the server ************************/
    rst = connect (sfd, (struct sockaddr *) &srv_addr, addrlen);
    checkConnect(rst);
    printf("CONNECTION SUCCESSFUL!!\n");

    char buf[BUF_SIZE]={'\0'};
    std::string message="Hello World from client";
    strcpy(buf,message.c_str());
    rst=send(sfd,buf,BUF_SIZE,0);
    checkSend(rst);
    rst=recv(sfd,buf,BUF_SIZE,0);
    checkRecieve(rst);
    printf("Message from server->%s\n",buf);
    close(sfd);
    checkClose(sfd);
}