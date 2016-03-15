
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/socket.h> // For the socket () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <string.h> // For memset.
#include <arpa/inet.h> // For inet_pton ().

int port_num = 24001;
#define BUF_SIZE 300000

void check_close(int rst){
    if(rst<0){
        perror("CLOSE FAILED\n");
        exit(1);
    }
}

void check_send(int rst){
    if(rst<0){
        printf("CLIENT : SENDTO FAILED\n");
        exit(1);
    }
    else{
        printf("Sent data=%d\n",rst);
    }
}

void check_recieve(int rst){
    if(rst<0){
        printf("RECIEVE FAILED\n");
        exit(1);
    }
}

int main () 
{
    int rst; // Return status of functions.
    int sfd; // Socket file descriptor.
    
    /***************** Create a socket *******************************/
    sfd = socket (AF_INET, SOCK_DGRAM, 0); /* AF_INET --> IPv4,
                * SOCK_DGRAM --> UDP Protocol, 0 --> for the protocol. */
    if (sfd == -1) 
    {
        perror ("Client_1.c socket error");
        exit (1);
    }
    printf ("Socket fd = %d\n", sfd);

    /****************** Send - receive messages **********************/
    while(1){
        int pass;
        printf("Enter password\n");
        char msg[200] = {'\0'};
        scanf("%s",msg);
        char msg_len = 200;
        int flags = 0;
        struct sockaddr_in dest_addr; 
        socklen_t addrlen = sizeof (struct sockaddr_in); 
        
        
        // Initializing destination address.
        memset (&dest_addr, 0, addrlen); // Initializes address to zero.
        dest_addr.sin_family = AF_INET;  // Address is in IPv4 format.
        dest_addr.sin_port   = htons (24021);  // Port number of the server.
        rst = inet_pton (AF_INET, "127.0.0.1", &dest_addr.sin_addr); /* Note
                * that third field should point to an in_addr (in6_addr). */
        if (rst <= 0)
        {
            perror ("Client Presentation to network address conversion.\n");
            exit (1);
        }        
        //BLACK BOX OVER

        /* Sending message to the server. */
        //PASSWORD SENDING
        rst = sendto (sfd, msg, 20, flags, (struct sockaddr *) &dest_addr, sizeof (struct sockaddr_in));
        check_send(rst);
        
        
        char buf[BUF_SIZE] = {'\0'};
        struct sockaddr_in sender_addr;
        socklen_t sender_len;
        //waiting for authenntication
        rst = recvfrom (sfd, buf, BUF_SIZE, flags, (struct sockaddr *) &sender_addr, &sender_len);
        if (rst < 0)
        {
            perror ("Client: couldn't receive");
            exit (1);
        }
        if(buf[0]=='1'){
            printf("YOU ARE AUTHORISED\n");
            printf("PLEASE ENTER THE DATE IN YEAR-MONTH-DAY FORMAT\n");
            char date_admin[300];
            scanf("%s",date_admin);
            rst=sendto(sfd,date_admin,300,flags,(struct sockaddr *)&dest_addr,sizeof(struct sockaddr_in));
            check_send(rst);
            rst=recvfrom(sfd,buf,BUF_SIZE,flags,(struct sockaddr *)&sender_addr,&sender_len);
            check_recieve(rst);
            printf("%s\n",buf);
            printf("Do you want to continue? 'Y' or 'n'\n");
            char response[2];
            scanf("%s",response);
            if(response[0]=='Y'){
                continue;
            }
            else{
                rst=close(sfd);
                check_close(rst);
                printf("Thank you!!\n");
                break;
            }
        }
        else{
            printf("AUTHORISATION FAILED\n");
                rst = close (sfd); // Close the socket file descriptor.
                check_close(rst);
                printf("Kicking you out!!\n");
        }

        //THIS prints the address
        const char *buf2=inet_ntop(AF_INET,(struct sockaddr *)&sender_addr,buf,BUF_SIZE);
        if (buf2==NULL)
        {
            perror ("Client: Conversion of sender's address to presentation failed");
            exit (1);
        }
        printf ("Servers address = %s\n", buf2);
        //THATS ALL
    }
}