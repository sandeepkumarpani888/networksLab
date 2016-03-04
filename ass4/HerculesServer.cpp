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
#include "email.h"

int port_num = 23465;
#define BUF_SIZE 3000

int main ()
{
    pid_t child_tcp=fork();
    if(child_tcp==0){
        int rst; // Return status of functions.
        int cfd; // File descriptor for the client.
        /***************DESERIALISE****************/
        std::vector<News> Academic;
        fill_academic(Academic);

        std::vector<News> nAcademic;
        fill_nacademic(nAcademic);
        /**************** Create a socket. *******************************/
        int sfd; // Socket file descriptor.
        sfd = socket (AF_INET, SOCK_STREAM, 0); /* AF_INET --> IPv4,
                    * SOCK_STREAM --> TCP Protocol, 0 --> for the protocol. */
        if (sfd == -1) 
        {
            perror ("Server: socket error");
            exit (1);
        }
        printf ("Socket fd = %d\n", sfd);
        
        
        
        /***************** Assign an address to the server **************/
        struct sockaddr_in srv_addr, cli_addr; // Addresses of the server and the client.
        socklen_t addrlen = sizeof (struct sockaddr_in); // size of the addresses.
        
        // Clear the two addresses.
        memset (&srv_addr, 0, addrlen);
        memset (&cli_addr, 0, addrlen);

        // Assign values to the server address.
        srv_addr.sin_family = AF_INET; // IPv4.
        srv_addr.sin_port   = htons (28912); // Port Number.
        
        rst = inet_pton (AF_INET, "127.50.30.11", &srv_addr.sin_addr); /* To 
                                  * type conversion of the pointer here. */
        if (rst <= 0)
        {
            perror ("Server Presentation to network address conversion.\n");
            exit (1);
        }    
        
        
        
        /****************** Bind the server to an address. ***************/
        rst = bind (sfd, (struct sockaddr *) &srv_addr, addrlen); /* Note
            * the type casting of the pointer to the server's address. */
        if (rst == -1)
        {
            perror ("Server: Bind failed");
            exit (1);
        }

        for(int numberOfIterations=0;numberOfIterations<2;numberOfIterations++){
            printf("Max connections allowed to wait: %d\n",SOMAXCONN);
            rst=listen(sfd,6);
            if(rst==-1){
                perror("Server: Listen Failed");
                exit(1);
            }

            printf("Waiting___waiting\n");
            cfd = accept (sfd, (struct sockaddr *) &cli_addr, &addrlen);
            if(cfd==-1){
                perror("Server: Accept Failed");
                exit(1);
            }
            /***********INTERACTION IO*************/
            pid_t childProcess=fork();
            if(childProcess==0){
                printf("Entering child at interation number->%d\n",numberOfIterations);
                char buf[BUF_SIZE]={'\0'};
                rst=recv(cfd,buf,BUF_SIZE,0);
                printf("Recieved message from client-|%s|\n",buf);
            }
        }
    }
    else{
        //qwe
    }