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
#define BUF_SIZE 256

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

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s,char delim){
    std::vector<std::string> elems;
    split(s,delim,elems);
    return elems;
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

    srv_addr.sin_port   = htons (28932); // Port Number.

    rst = inet_pton (AF_INET, "127.50.30.11", &srv_addr.sin_addr); /* To
                                  * type conversion of the pointer here. */
    if (rst <= 0)
    {
        perror ("Client Presentation to network address conversion.\n");
        exit (1);
    }
    //INITIAL PART NOT THAT IMPORTANT


    /***************** Connect to the server ************************/
    // rst = connect (sfd, (struct sockaddr *) &srv_addr, addrlen);
    // checkConnect(rst);
    // printf("CONNECTION SUCCESSFUL!!\n");

    /**************** Send-Receive messages ************************/
    for(int counter1=0;counter1<=0;counter1++){
        printf("Do you wish to send(1) or recieve email(2)\n");
        printf(">");
        //printf("lol\n");
        int opertaionType;
        scanf("%d",&opertaionType);
        if(opertaionType==1){
            printf("Please enter your email id?\n>");
            std::string fromId;
            std::string fromDomain;
            std::cin>>fromId;
            std::vector<std::string> emailSplit=split(fromId,'@');
            fromDomain=emailSplit[1];
            if(fromDomain=="abc.com"){
                //qwe
                // Clear the two addresses.
                memset (&srv_addr, 0, addrlen);

                // Assign values to the server address.
                srv_addr.sin_family = AF_INET; // IPv4.

                srv_addr.sin_port   = htons (28962); // Port Number.

                rst = inet_pton (AF_INET, "10.117.11.117", &srv_addr.sin_addr); /* To
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
            }
            else if(fromDomain=="xyz.com"){
                //qwe
                // Clear the two addresses.
                memset (&srv_addr, 0, addrlen);

                // Assign values to the server address.
                srv_addr.sin_family = AF_INET; // IPv4.

                srv_addr.sin_port   = htons (28962); // Port Number.

                rst = inet_pton (AF_INET, "10.117.11.81", &srv_addr.sin_addr); /* To
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
            }
            //get sender's email
            printf("Please enter the number of recipients\n>");
            int recipientCount;
            std::vector<std::string> recipientEmail;
            scanf("%d",&recipientCount);
            printf("Please enter the recipient emails one per line\n");
            for(int recpCount=0;recpCount<recipientCount;recpCount++){
                printf(">");
                std::string emailID;
                std::cin>>emailID;
                recipientEmail.push_back(emailID);
            }
            //got the list of recipient ids;
            printf("Enter the data you want to send ending with a single .\n");
            std::string lineInput;
            std::vector<std::string> data;
            while(true){
                std::getline(std::cin,lineInput);
                data.push_back(lineInput);
                if(lineInput[0]=='.' && lineInput.length()==1){
                    //data=data+".";
                    break;
                }
            }
            printf("sending data ?/?\n");
            
            //prepare the emails to be sent
            std::vector<Email> emailList;
            for(int recpCount=0;recpCount<recipientCount;recpCount++){
                Email email;
                email.setFromId(fromId);
                email.setToId(recipientEmail[recpCount]);
                //email.setBody(data);
                email.setTimeNow();
                emailList.push_back(email);
            }
            printf("%d-->\n",recipientCount);

            for(int recpCount=0;recpCount<recipientCount;recpCount++){
                char buf[BUF_SIZE]={'\0'};
                fd_set readfds;
                FD_ZERO(&readfds);
                FD_SET(sfd,&readfds);
               //send HELO
                std::string input;
                input="HELO "+fromDomain;
                strcpy(buf,input.c_str());
                //sleep(1);
                rst=send(sfd,buf,BUF_SIZE,0);
                printf("%s\n", buf);
                checkSend(rst);
                FD_ZERO(&readfds);
                FD_SET(sfd,&readfds);
                rst=select(sfd+1,&readfds,NULL,NULL,NULL);
                if(FD_ISSET(sfd,&readfds)){
                    rst=recv(sfd,buf,BUF_SIZE,0);
                }
                printf(">");
                printf("%s\n",buf);
                //

                //send mail from
                input="MAIL FROM: ";
                input=input+emailList[recpCount].getFromId();
                memset(buf,'\0',BUF_SIZE);
                strcpy(buf,input.c_str());
                //printf("%s..\n",buf);
                //sleep(1);
                rst=send(sfd,buf,BUF_SIZE,0);
                printf("%s\n", buf);
                checkSend(rst);
                FD_ZERO(&readfds);
                FD_SET(sfd,&readfds);
                rst=select(sfd+1,&readfds,NULL,NULL,NULL);
                if(FD_ISSET(sfd,&readfds)){
                    printf("here\n");
                    memset(buf,'\0',BUF_SIZE);
                    rst=recv(sfd,buf,BUF_SIZE,0);
                }
                printf(">%s\n",buf);
                //

                //send Recipiant Email
                input="RCPT TO: "+emailList[recpCount].getToId();
                memset(buf,'\0',BUF_SIZE);
                strcpy(buf,input.c_str());
                //sleep(1);
                rst=send(sfd,buf,BUF_SIZE,0);
                printf("%s\n", buf);
                checkSend(rst);
                FD_ZERO(&readfds);
                FD_SET(sfd,&readfds);
                rst=select(sfd+1,&readfds,NULL,NULL,NULL);
                if(FD_ISSET(sfd,&readfds)){
                    rst=recv(sfd,buf,BUF_SIZE,0);
                }
                printf(">%s\n",buf);
                //

                //send the data
                input="DATA";
                memset(buf,'\0',BUF_SIZE);
                strcpy(buf,input.c_str());
                //sleep(1);
                rst=send(sfd,buf,BUF_SIZE,0);
                checkSend(rst);
                FD_ZERO(&readfds);
                FD_SET(sfd,&readfds);
                rst=select(sfd+1,&readfds,NULL,NULL,NULL);
                if(FD_ISSET(sfd,&readfds)){
                    rst=recv(sfd,buf,BUF_SIZE,0);
                }
                printf(">%s\n",buf);
                for(int dataLength=1;dataLength<data.size();dataLength++){
                    strcpy(buf,data[dataLength].c_str());
                    //sleep(1);
                    rst=send(sfd,buf,BUF_SIZE,0);
                    checkSend(rst);
                }
                FD_ZERO(&readfds);
                FD_SET(sfd,&readfds);
                rst=select(sfd+1,&readfds,NULL,NULL,NULL);
                if(FD_ISSET(sfd,&readfds)){
                    rst=recv(sfd,buf,BUF_SIZE,0);
                }
                printf(">%s\n",buf);
                //

                //quit
                sleep(1);
                rst=send(sfd,"QUIT",BUF_SIZE,0);
                checkSend(rst);
                FD_ZERO(&readfds);
                FD_SET(sfd,&readfds);
                rst=select(sfd+1,&readfds,NULL,NULL,NULL);
                if(FD_ISSET(sfd,&readfds)){
                    rst=recv(sfd,buf,BUF_SIZE,0);
                }
                checkRecieve(rst);
                printf(">%s\n",buf);
                rst=close(sfd);
                checkClose(rst);
                sfd = socket (AF_INET, SOCK_STREAM, 0);
                addrlen = sizeof (struct sockaddr_in);
                memset (&srv_addr, 0, addrlen);
                srv_addr.sin_family = AF_INET; // IPv4.
                srv_addr.sin_port   = htons (28962); // Port Number.
                rst = inet_pton (AF_INET, "10.117.11.117", &srv_addr.sin_addr);
                rst = connect (sfd, (struct sockaddr *) &srv_addr, addrlen);
                checkConnect(rst);
                //
            }
            //
        }
        else if(opertaionType==2){
            char buf[BUF_SIZE]={'\0'};
            printf("Please enter your email id?\n>");
            std::string fromId;
            std::string fromDomain;
            std::cin>>fromId;
            std::vector<std::string> emailSplit=split(fromId,'@');
            fromDomain=emailSplit[1];
            if(fromDomain=="abc.com"){
                //qwe
                // Clear the two addresses.
                memset (&srv_addr, 0, addrlen);

                // Assign values to the server address.
                srv_addr.sin_family = AF_INET; // IPv4.

                srv_addr.sin_port   = htons (28962); // Port Number.

                rst = inet_pton (AF_INET, "10.117.11.117", &srv_addr.sin_addr); /* To
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
            }
            else if(fromDomain=="xyz.com"){
                //qwe
                // Clear the two addresses.
                memset (&srv_addr, 0, addrlen);

                // Assign values to the server address.
                srv_addr.sin_family = AF_INET; // IPv4.

                srv_addr.sin_port   = htons (28962); // Port Number.

                rst = inet_pton (AF_INET, "10.117.11.81", &srv_addr.sin_addr); /* To
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
            }
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(sfd,&readfds);
            std::string input;
            input="USER "+fromId;
            strcpy(buf,input.c_str());
            rst=send(sfd,buf,BUF_SIZE,0);
            checkSend(rst);
            FD_ZERO(&readfds);
            FD_SET(sfd,&readfds);
            rst=select(sfd+1,&readfds,NULL,NULL,NULL);
            if(FD_ISSET(sfd,&readfds)){
                rst=recv(sfd,buf,BUF_SIZE,0);
            }
            printf(">%s\n",buf);
            if(buf[0]=='-'){
                rst=close(sfd);
                checkClose(rst);
                return 0;
            }
            //Got the email id

            printf("Enter the password?\n>");
            std::string password;
            std::cin>>password;
            input="PASS "+password;
            strcpy(buf,input.c_str());
            rst=send(sfd,buf,BUF_SIZE,0);
            checkSend(rst);
            FD_ZERO(&readfds);
            FD_SET(sfd,&readfds);
            rst=select(sfd+1,&readfds,NULL,NULL,NULL);
            if(FD_ISSET(sfd,&readfds)){
                rst=recv(sfd,buf,BUF_SIZE,0);
            }
            printf(">%s\n",buf);
            if(buf[0]=='-'){
                rst=close(sfd);
                checkClose(rst);
                return 0;
            }
            //Got the password

            //select one of them
            while(true){

                //Get the list of all the emails present
                input="LIST";
                strcpy(buf,input.c_str());
                rst=send(sfd,buf,BUF_SIZE,0);
                checkSend(rst);
                FD_ZERO(&readfds);
                FD_SET(sfd,&readfds);
                rst=select(sfd+1,&readfds,NULL,NULL,NULL);
                if(FD_ISSET(sfd,&readfds)){
                    rst=recv(sfd,buf,BUF_SIZE,0);
                }
                printf(">%s\n",buf);
                int countOfEmails=0;
                while(true){
                    memset(buf,'\0',BUF_SIZE);
                    FD_ZERO(&readfds);
                    FD_SET(sfd,&readfds);
                    rst=select(sfd+1,&readfds,NULL,NULL,NULL);
                    if(FD_ISSET(sfd,&readfds)){
                        rst=recv(sfd,buf,BUF_SIZE,0);
                    }
                    sleep(1);
                    input=buf;
                    printf("%s..\n",buf);
                    if(strlen(buf) && buf[0]=='.'){
                        break;
                    }
                    else{
                        countOfEmails++;
                    }
                }
                //

                printf("Select the id of the email you want to read?\n>");
                int indexOfEmail=0;
                scanf("%d",&indexOfEmail);
                input="LIST "+std::to_string(indexOfEmail);
                memset(buf,'\0',BUF_SIZE);
                strcpy(buf,input.c_str());
                rst=send(sfd,buf,BUF_SIZE,0);
                checkSend(rst);
                sleep(1);
                FD_ZERO(&readfds);
                FD_SET(sfd,&readfds);
                rst=select(sfd+1,&readfds,NULL,NULL,NULL);
                if(FD_ISSET(sfd,&readfds)){
                    rst=recv(sfd,buf,BUF_SIZE,0);
                }
                printf("--->>%s\n",buf);
                //sleep(5);
                if(buf[0]=='+'){
                    printf("%s\n",buf);
                    while(true){
                        FD_ZERO(&readfds);
                        FD_SET(sfd,&readfds);
                        rst=select(sfd+1,&readfds,NULL,NULL,NULL);
                        if(FD_ISSET(sfd,&readfds)){
                            rst=recv(sfd,buf,BUF_SIZE,0);
                        }
                        input=buf;
                        std::cout<<"message+ "<<input<<"\n";
                        if(input.size()==1 && input[0]=='.'){
                            break;
                        }
                        sleep(1);
                    }
                }
                else{
                    printf("message- %s\n",buf);
                }
                printf("Do you wish to recieve more messages?\n");
                printf("Yes(1) or No(2)\n");
                int opertaionType;
                scanf("%d",&opertaionType);
                if(opertaionType==2){
                    break;
                }
            }
            //

            //quit
            rst=send(sfd,"QUIT",BUF_SIZE,0);
            checkSend(rst);
            FD_ZERO(&readfds);
            FD_SET(sfd,&readfds);
            rst=select(sfd+1,&readfds,NULL,NULL,NULL);
            if(FD_ISSET(sfd,&readfds)){
                rst=recv(sfd,buf,BUF_SIZE,0);
            }
            //
        }
        rst=close(sfd);
        checkClose(rst);
    }
}