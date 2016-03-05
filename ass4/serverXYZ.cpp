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

void redirectTo(std::string domainName,Email email){
    pid_t childProcess=fork();
    std::cout<<email.getFromId()<<"\n";
    if(childProcess==0){
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
        rst = connect (sfd, (struct sockaddr *) &srv_addr, addrlen);
        checkConnect(rst);
        printf("CONNECTION SUCCESSFUL!!\n");
        char buf[BUF_SIZE]={'\0'};

        //forward data to other server in the same way

        //SEND HELO
        std::string input;
        std::string fromDomain;
        std::vector<std::string> emailSplit=split(email.getFromId(),'@');
        fromDomain=emailSplit[1];
        input="HELO "+fromDomain;
        memset(buf,'\0',BUF_SIZE);
        strcpy(buf,input.c_str());
        rst=write(sfd,buf,input.size()+1);
        checkSend(rst);
        memset(buf,'\0',BUF_SIZE);
        rst=recv(sfd,buf,BUF_SIZE,0);
        printf(">");
        printf("%s\n",buf);
        //

        //send mail from
        input="MAIL FROM: ";
        input=input+email.getFromId();
        memset(buf,'\0',BUF_SIZE);
        strcpy(buf,input.c_str());
        printf("%s..\n",buf);
        rst=write(sfd,buf,input.size()+1);
        checkSend(rst);
        rst=recv(sfd,buf,BUF_SIZE,0);
        printf(">%s\n",buf);
        //

        //send Recipiant Email
        input="RCPT TO: "+email.getToId();
        memset(buf,'\0',BUF_SIZE);
        strcpy(buf,input.c_str());
        rst=write(sfd,buf,input.size()+1);
        checkSend(rst);
        rst=recv(sfd,buf,BUF_SIZE,0);
        printf(">%s\n",buf);
        //

        //send the data
        input="DATA";
        memset(buf,'\0',BUF_SIZE);
        strcpy(buf,input.c_str());
        rst=write(sfd,buf,input.size()+1);
        checkSend(rst);
        rst=recv(sfd,buf,BUF_SIZE,0);
        printf(">%s\n",buf);
        std::vector<std::string> data=split(email.getBody(),'}');
        std::cout<<email.getBody()<<"\n";
        for(int dataLength=1;dataLength<data.size();dataLength++){
            memset(buf,'\0',BUF_SIZE);
            std::cout<<data[dataLength]<<"\n";
            char buf_[256];
            strcpy(buf_,data[dataLength].c_str());
            //buf_[data[dataLength].length()];
            for(int i=data[dataLength].length();i<BUF_SIZE;i++){
                buf_[i]='\0';
            }
            printf(">%s\n",buf_);
            rst=write(sfd,buf_,256);
            checkSend(rst);
            sleep(1);
            if(dataLength==data.size()-1){
                break;
            }
            // rst=recv(sfd,buf,BUF_SIZE,0);
            // checkRecieve(rst);
            // printf("%s\n",buf);
            // memset(buf,'\0',BUF_SIZE);
        }
        //printf("herer\\n");
        rst=recv(sfd,buf,BUF_SIZE,0);
        printf(">%s\n",buf);
        //

        //quit
        rst=write(sfd,"QUIT",5);
        checkSend(rst);
        rst=recv(sfd,buf,BUF_SIZE,0);
        checkRecieve(rst);
        printf(">%s\n",buf);
        //
    }
    else{
        int status1=0;
        waitpid(childProcess,&status1,0);
    }
}

std::vector<Email> emailList[10001];
std::map<std::string,int> nameMapping;
int countOfName=0;

void fill(){
    nameMapping.clear();
    for(int index=0;index<10001;index++){
        emailList[index].clear();
    }
    countOfName=0;
    std::ifstream readFile("temporaryFilling.txt");
    std::string line;
    int index=0;
    system("ls emailData > temporaryFilling.txt");
    while(std::getline(readFile,line)){
        line="/home/sandeep/networksLab/ass4/emailData/"+line;
        int sizeOfName=line.size();
        if(line[sizeOfName-1]=='l' && line[sizeOfName-2]=='i'){
            Email email;
            email.deserialiseEmail(line);
            std::cout<<email.getToId()<<"\n";
            if(nameMapping.count(email.getToId())==0){
                nameMapping[email.getToId()]=countOfName;
                countOfName++;
                std::cout<<email.getToId()<<" "<<nameMapping[email.getToId()]<<"\n";
                emailList[nameMapping[email.getToId()]].push_back(email);
                std::cout<<"size->"<<emailList[nameMapping[email.getToId()]].size()<<"\n";
            }
            else{
                emailList[nameMapping[email.getToId()]].push_back(email);
            }
        }
    }
}

void getLockOnDirectory(){
    struct flock fl = {F_WRLCK, SEEK_SET, 0, 0, 0};
    int fd;
    fl.l_type = F_RDLCK;
    if ((fd = open("directoryStatus.txt", O_RDWR)) == -1)
    {
        perror("open");
        exit(1);
    }
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    printf("GOT LOCK\n");
    //COMPLETED//
    fill();
    //FINISHED REMOVE THE LOCK//
    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &fl) == -1)
    {
        perror("fctl");
        exit(1);
    }
    printf("unlocked");
    //UNLOCK THE DIRECTORY
}


int main (){
    pid_t childTcp=fork();
    if(childTcp==0){
        int rst; // Return status of functions.
        int cfd; // File descriptor for the client.
        /***************DESERIALISE****************/
        //THIS NEEDS TO BE DONE
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
        srv_addr.sin_port   = htons (28942); // Port Number.
        
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
        char buf[BUF_SIZE]={'\0'};

        for(int numberOfIterations=0;numberOfIterations<=2;numberOfIterations++){
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
                printf("ENTERING CHILD with counter->%d\n",numberOfIterations);//debugging
                Email email;
                std::string data;
                
                rst=recv(cfd,buf,BUF_SIZE,0);
                checkRecieve(rst);
                printf(">%s\n",buf);
                if(buf[0]=='H'){
                    //SMTP operations
                    rst=write(cfd,"250 OK",BUF_SIZE);
                    //HELO

                    rst=recv(cfd,buf,BUF_SIZE,0);
                    checkRecieve(rst);
                    printf(">%s\n",buf);
                    data=buf;
                    std::vector<std::string> dataSplit=split(data,' ');
                    email.setFromId(dataSplit[2]);
                    printf("%s-->\n",email.getFromId());
                    std::cout<<email.getFromId()<<"\n";
                    rst=write(cfd,"250 OK",BUF_SIZE);
                    checkSend(rst);
                    //FROM EMAIL

                    rst=recv(cfd,buf,BUF_SIZE,0);
                    checkRecieve(rst);
                    printf(">%s\n",buf);
                    data=buf;
                    dataSplit=split(data,' ');
                    email.setToId(dataSplit[2]);
                    rst=write(cfd,"250 OK",BUF_SIZE);
                    checkSend(rst);
                    //TO EMAIL

                    rst=recv(cfd,buf,BUF_SIZE,0);
                    checkRecieve(rst);
                    printf(">%s\n",buf);
                    rst=write(cfd,"354 start mail input",BUF_SIZE);
                    checkSend(rst);
                    data="";
                    std::string input="";

                    while(true){
                        rst=recv(cfd,buf,BUF_SIZE,0);
                        checkRecieve(rst);
                        printf(">%s\n",buf);
                        std::string dataInput(buf);
                        if(dataInput[0]=='.' && dataInput.length()==1){
                            data=data+".";
                            break;
                        }
                        else{
                            data=data+dataInput+"}";
                        }
                    }
                    rst=write(cfd,"250 OK",BUF_SIZE);
                    checkSend(rst);
                    email.setBody(data);
                    //DATA RECIEVED

                    rst=recv(cfd,buf,BUF_SIZE,0);
                    checkRecieve(rst);
                    printf(">%s\n",buf);
                    rst=write(cfd,"221 service closed",BUF_SIZE);
                    checkSend(rst);
                    email.setTimeNow();
                    //email.serialiseEmail();
                    //QUIT

                    //checking for domain names
                    std::vector<std::string> domainFrom=split(email.getFromId(),'@');
                    std::vector<std::string> domainTo=split(email.getToId(),'@');
                    if(domainTo[1]!="xyz.com"){
                        //forward to correct server;
                        redirectTo(domainTo[1],email);
                    }
                    else{
                        email.serialiseEmail();
                    }
                }
                else{
                    std::string firstMessage(buf);
                    std::vector<std::string> messageSplit=split(firstMessage,' ');
                    std::string fromId=messageSplit[1];
                    std::cout<<"email id->>>"<<fromId<<"\n";
                    //FILL THE map and vector
                    getLockOnDirectory();

                    if(nameMapping.count(fromId)==0){
                        rst=write(cfd,"-NOT PRESENT",BUF_SIZE);
                        break;
                    }
                    else{
                        rst=write(cfd,"+GREEN TO GO",BUF_SIZE);
                    }
                    //checked the existence of user

                    //get the password
                    rst=recv(cfd,buf,BUF_SIZE,0);
                    checkRecieve(rst);
                    std::string data(buf);
                    std::cout<<"recieved ->"<<data<<"\n";
                    std::vector<std::string> dataSplit=split(data,' ');
                    if(dataSplit[1]=="carrot"){
                        rst=write(cfd,"+AND YOU GET ACCESS",BUF_SIZE);
                    }
                    else{
                        rst=write(cfd,"-Try gmail for a change",BUF_SIZE);
                        break;
                    }

                    while(true){
                        getLockOnDirectory();
                        //LIST all the emails
                        rst=recv(cfd,buf,BUF_SIZE,0);
                        checkRecieve(rst);
                        printf("---+>>%s\n",buf);
                        std::string input="+OK "+std::to_string(emailList[nameMapping[fromId]].size());
                        strcpy(buf,input.c_str());
                        rst=write(cfd,buf,BUF_SIZE);
                        checkSend(rst);
                        for(int index=0;index<=emailList[nameMapping[fromId]].size();index++){
                            std::string input="";
                            if(index==emailList[nameMapping[fromId]].size()){
                                for(int i=0;i<BUF_SIZE;i++){
                                    buf[i]='\0';
                                }
                                buf[0]='.';
                                printf("-->>>>%s\n",buf);
                                printf("-->index->%d\n",index);
                                rst=write(cfd,buf,BUF_SIZE);
                                checkSend(rst);
                                break;
                            }
                            int dataSize=emailList[nameMapping[fromId]][index].getBody().size();
                            index++;
                            input=""+std::to_string(index)+" "+std::to_string(dataSize);
                            index--;
                            strcpy(buf,input.c_str());
                            printf("-->>>%s\n",buf);
                            rst=write(cfd,buf,BUF_SIZE);
                            checkSend(rst);
                            printf("-->index->%d\n",index);
                            //sleep(1);
                        }

                        printf("entering loop\n");
                        rst=recv(cfd,buf,BUF_SIZE,0);
                        checkRecieve(rst);
                        printf("here====%s\n",buf);
                        input=buf;
                        if(buf[0]=='L'){
                            std::vector<std::string> dataSplit=split(input,' ');
                            std::stringstream ss1(dataSplit[1]);
                            int messaageId;
                            ss1>>messaageId;
                            printf("request for %d-%d\n",messaageId,emailList[nameMapping[fromId]].size());
                            if((messaageId!=0 && messaageId<=emailList[nameMapping[fromId]].size())){
                                std::string input_="+OK "+std::to_string(messaageId)+" "+std::to_string(emailList[nameMapping[fromId]][messaageId-1].getBody().size());
                                memset(buf,'\0',BUF_SIZE);
                                strcpy(buf,input_.c_str());
                                printf("messafe-->%s\n",buf);
                                printf("OKAY\n");
                                rst=write(cfd,buf,BUF_SIZE);
                                checkSend(rst);
                            }
                            else{
                                printf("not okay\n");
                                rst=write(cfd,"-ERROR",BUF_SIZE);
                                break;
                            }
                            messaageId--;
                            dataSplit=split(emailList[nameMapping[fromId]][messaageId].getBody(),'}');
                            printf("%d-->\n",dataSplit.size());
                            for(int index=1;index<dataSplit.size();index++){
                                memset(buf,'\0',BUF_SIZE);
                                if(index==dataSplit.size()-1){
                                    buf[0]='.';
                                }
                                else{
                                    strcpy(buf,dataSplit[index].c_str());
                                }
                                rst=write(cfd,buf,BUF_SIZE);
                                printf("message sent->%s..%d\n",buf,strlen(buf));
                                checkSend(rst);
                                sleep(1);
                            }
                        }
                        else if(buf[0]=='Q'){
                            rst=write(cfd,"+Signing off",BUF_SIZE);
                            checkSend(rst);
                            break;
                        }
                    }
                }
                //
                printf("Exiting child with counter->%d\n",numberOfIterations);
                return 0;
            }
        }
        rst=close(sfd); // Close the socket file descriptor.
        checkClose(rst);

    }
    else{
        int statusTcp=0;
        waitpid(childTcp,&statusTcp,0);
    }
return 0;}