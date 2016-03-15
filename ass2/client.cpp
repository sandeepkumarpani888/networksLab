#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <string.h> // For memset.
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().
#include <bits/stdc++.h>
#include <ctime>
#include "train.hpp"

int port_num = 10041;
#define BUF_SIZE 300
char buf[BUF_SIZE]={'\0'};

void check_close(int rst){
    if(rst==-1){
        perror("CLIENT CLOSE FAILED");
        exit(1);
    }
}

void check_send(int rst){
    if(rst==-1){
        perror("Client : Send failed");
        exit(1);
    }
}

std::vector<std::string> query;

int main ()
{
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
    srv_addr.sin_port   = htons (port_num); // Port Number.
    
    rst = inet_pton (AF_INET, "127.10.12.1", &srv_addr.sin_addr); /* To 
                              * type conversion of the pointer here. */
    if (rst <= 0)
    {
        perror ("Client Presentation to network address conversion.\n");
        exit (1);
    }    
    //READING FROM FILE
    std::ifstream infile("Booking.txt");
    std::string line;
    while(std::getline(infile,line)){
        std::istringstream iss(line);
        query.push_back(line);
    }
    for(int i=0;i<(int)query.size();i++){
        std::cout<<query[i]<<std::endl;
    }
    //DONE READING FROM FILE

    
    printf ("Trying to connect to server\n");
    /***************** Connect to the server ************************/
    rst = connect (sfd, (struct sockaddr *) &srv_addr, addrlen);
    if (rst == -1)
    {
        perror ("Client: Connect failed.");
        exit (1);
    }
    printf ("Connected to server\n");

    rst = recv (sfd, buf, BUF_SIZE, 0);
    printf("%s->\n",buf);
    FILE *ftr=fopen("booked1.csv","w");
    /**************** Send-Receive messages ************************/
    for(int i=0;i<(int)query.size();i++){

        //GETTING CURRENT TIME
        time_t t=time(0);
        struct tm *now=localtime(&t);
        query[i]=query[i]+",";
        query[i]=query[i]+std::to_string(now->tm_hour);
        query[i]=query[i]+":";
        query[i]=query[i]+std::to_string(now->tm_min);
        query[i]=query[i]+":";
        query[i]=query[i]+std::to_string(now->tm_sec);
        //END GETTING CURRENT TIME

        printf("sending request->%s\n",query[i].c_str());
        strcpy(buf,query[i].c_str());
        rst = send (sfd,buf, BUF_SIZE, 0);
        check_send(rst);
        printf ("Query sent successfully.\n");
        
        printf ("Waiting to receive the reply.\n");
        rst = recv (sfd, buf, BUF_SIZE, 0);
        if (rst == -1)
        {
            perror ("Client: Receive failed");
            exit (1);
        }
        printf ("Received message = %s\n", buf);
        fprintf(ftr,"%s\n",buf);
        sleep(5);
    }
    
    
    
    /****************** Close ****************************************/
    strcpy(buf,"quit");
    rst=send(sfd,buf,BUF_SIZE,0);
    check_send(rst);
    rst=close(sfd);
    check_close(rst);
    printf("Thank you for using the service\n");
return 0;}