#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <string.h> // For memset.
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().
#include "news.h"
int port_num = 23465;
#define PORT 24017
#define IP "127.60.30.11"
#define BUF_SIZE 300000

using namespace std;

int check_recieve(int rst){
    if(rst==-1){
        perror("Collection error");
        exit(-1);
    }
}

int check_send(int rst){
    if(rst==-1){
        perror("SENDING FAILED\n");
        exit(-1);
    }
}

int check_close(int rst){
    if(rst==-1){
        perror("CLOSING FAILED");
        exit(-1);
    }
    else{
       // printf("CONNECTION CLOSED\n");
    }
}

int check_connect(int rst){
    if(rst==-1){
        printf("Client: Connect failed\\n");
        exit(1);
    }
}

int main ()
{
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
        srv_addr.sin_port   = htons (28912); // Port Number.
        
        rst = inet_pton (AF_INET, "127.50.30.11", &srv_addr.sin_addr); /* To 
                                  * type conversion of the pointer here. */
        if (rst <= 0)
        {
            perror ("Client Presentation to network address conversion.\n");
            exit (1);
        }
    //INITIAL PART NOT THAT IMPORTANT


    /***************** Connect to the server ************************/
    rst = connect (sfd, (struct sockaddr *) &srv_addr, addrlen);
    check_connect(rst);
    printf("CONNECTION SUCCESSFUL!!\n");
    printf("Welcome READER\n");
    
    /**************** Send-Receive messages ************************/
    while(true){
        //tells server that reporter is accessing
        char buf[BUF_SIZE] = {'\0'}; 
        rst=write(sfd,"rp",4);
        check_send(rst);
        rst = recv (sfd, buf, BUF_SIZE, 0);
        check_recieve(rst);
        printf ("%s\n", buf);
        //server pinging done
        
        //Selects the type of news to present
        int type;
        std::cin>>type;
        buf[0]='0'+type;
        rst=write(sfd,buf,2);
        rst=recv(sfd,buf,BUF_SIZE,0);
        std::cout<<buf;
        //selection done

        //get data
        printf("enter the details of the news article-\n");
        News khabar;
        printf("enter the id->\n");
        int id_;
        std::cin>>id_;
        cin.ignore();
        khabar.set_ID(id_);
        std::string title;
        std::string content;
        printf("enter the title->\n");
        std::getline(std::cin,title);
        khabar.set_title(title);
        printf("enter the content->\n");
        std::getline(std::cin,content);
        khabar.set_content(content);
        string datasend;
        datasend=std::to_string(khabar.get_ID())+",";
        datasend=datasend+khabar.get_title()+",";
        datasend=datasend+khabar.get_content();
        strcpy(buf,datasend.c_str());
        rst=write(sfd,buf,BUF_SIZE);
        //send data

        //confirmation
        rst=recv(sfd,buf,BUF_SIZE,0);
        check_recieve(rst);
        printf("%s\n",buf);
        //done

        //close socket
        rst=close(sfd);
        check_close(rst);
        //done

        printf("Do you wish to continue Y or n\n");
        char s[2];
        scanf("%s",s);
        if(s[0]=='Y'){
            printf("Connecting you again...Hold on\n");
            {
                sfd = socket (AF_INET, SOCK_STREAM, 0);
                addrlen = sizeof (struct sockaddr_in);
                memset (&srv_addr, 0, addrlen);
                srv_addr.sin_family = AF_INET; // IPv4.
                srv_addr.sin_port   = htons (28912); // Port Number.
                rst = inet_pton (AF_INET, "127.50.30.11", &srv_addr.sin_addr);
                rst = connect (sfd, (struct sockaddr *) &srv_addr, addrlen);
                check_connect(rst);
            }
            //Connecting again
        }
        else if(s[0]=='n'){
            printf("Thank you\n");
            return 0;
        }
    }
}

