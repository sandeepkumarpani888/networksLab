#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "train.hpp"
#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <string.h> // For memset.
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().
#include <sys/select.h> // For select().
#include <bits/stdc++.h>

int port_num = 10041;
#define BUF_SIZE 300
int cfd[1001];
char buf[BUF_SIZE]={'\0'};

void check_select(int rst){
    if(rst==-1){
        perror("SELECT FAILED:");
        exit(1);
    }
}

struct query{
    int people_count;
    int elderly_count;
    int time_h;
    int time_m;
    int time_s;
    std::vector<BerthPref> berth;
    int coach;
    int id;
    bool operator<(query other) const{
        if(id!=other.id){
            return id<other.id;
        }
        if(people_count!=other.people_count){
            return people_count<other.people_count;
        }
        else{
            if(elderly_count!=other.elderly_count){
                return elderly_count<other.elderly_count;
            }
            else{
                if(time_h!=other.time_h){
                    return time_h<other.time_h;
                }
                else{
                    if(time_m!=other.time_m){
                        return time_m<other.time_m;
                    }
                    else{
                        return time_s<other.time_s;
                    }
                }
            }
        }
    }
};

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
    //BLACKBOX START
    for(int i=0;i<1001;i++){
        cfd[i]=-1;
    }
    int rst; // Return status of functions.

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
    srv_addr.sin_port   = htons (port_num); // Port Number.
    
    rst = inet_pton (AF_INET, "127.10.12.1", &srv_addr.sin_addr); /* To 
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

    /***************** listen (): Mark the socket as passive. *******/
    printf ("Max connections allowed to wait: %d\n", SOMAXCONN);
    rst = listen (sfd, 1);
    if (rst == -1)
    {
        perror ("Server: Listen failed");
        exit (1);
    }
    //BLACKBOX END
    /**************** Select (): **************************************/
    //printf ("Going to sleep\n");
    //sleep (10); // For testing purposes.
    //printf ("Out of sleep\n");
    Train t1(12321,"Superfast Exp");
    Train t2(12301,"Rajdhani Exp");
    t1.add_coaches(3,Coach::Class::AC,Coach::SType::SIX,9);
    t1.add_coaches(12,Coach::Class::SLEEPER,Coach::SType::SIX,9);
    t2.add_coaches(13,Coach::Class::AC,Coach::SType::FOUR,9);

    fd_set rd, wr, er,dfd,active;
    int count=0;
    struct timeval tm;
    FD_ZERO(&rd);
    FD_ZERO(&wr);
    FD_ZERO(&er);
    FD_ZERO(&dfd);
    FD_ZERO(&active);
    FD_SET(sfd,&active); 
    std::vector<query> train1;
    std::vector<query> train2;
    int counter1=0;
    while(1)
    {
        counter1++;
        printf("-------  IN ITERATION ------%d\n",counter1);
        std::vector<query> Q1;
        std::vector<query> Q2;
        printf("Train name->%s\n",t1.name().c_str());
        printf("AC (%d/72)\t\t SLEEPER (%d/72)\n",t1.get_booked_seats(Coach::Class::AC),t1.get_booked_seats(Coach::Class::SLEEPER));
        printf("Train name->%s\n",t2.name().c_str());
        printf("AC (%d/56)\t\t SLEEPER (%d/0)\n",t2.get_booked_seats(Coach::Class::AC),t2.get_booked_seats(Coach::Class::SLEEPER));   
        FD_ZERO(&rd);
        FD_SET(0,&rd);
        FD_SET(sfd,&rd);
        for(int i=0;i<1001;i++){
            if(cfd[i]!=-1){
                printf("putting->%d\n",cfd[i]);
                FD_SET(cfd[i],&rd);
            }
        }
        tm.tv_sec=0;
        tm.tv_usec = 10;
        printf("entering again\n");
        rst = select (100, &rd,NULL,NULL,&tm);
        check_select(rst);
        if(FD_ISSET(0,&rd)){
            int temp;
            scanf("%d",&temp);
            if(temp==1){
                printf("Train name->%s\n",t1.name().c_str());
                printf("AC (%d/72)\t\t SLEEPER (%d/72)\n",t1.get_booked_seats(Coach::Class::AC),t1.get_booked_seats(Coach::Class::SLEEPER));
                printf("Train name->%s\n",t2.name().c_str());
                printf("AC (%d/56)\t\t SLEEPER (%d/0)\n",t2.get_booked_seats(Coach::Class::AC),t2.get_booked_seats(Coach::Class::SLEEPER));   
            }
        }
        if(FD_ISSET (sfd, &rd))
        {
            printf("HERE COMES A NEW CONNECTION\n");
            int temp=accept(sfd,(struct sockaddr *)&cli_addr,&addrlen);
            send(temp,"WELCOME TO RESERVATION PORTAL",BUF_SIZE,0);
            printf("SENT MESSAGE\n");
            for(int i=0;i<1001;i++){
                if(cfd[i]==-1){
                    cfd[i]=temp;
                    printf("ADDED TO THE LIST\n");
                    break;
                }
            } 
        }

        for(int i=0;i<1001;i++){
            if(FD_ISSET(cfd[i],&rd)){
                rst=recv(cfd[i],buf,BUF_SIZE,0);
                printf("recieved->%s\n",buf);
                if(strcmp(buf,"quit")==0){
                    cfd[i]=-1;
                    printf("closed connection %d\n",cfd[i]);
                    continue;
                }
                std::string buffer_(buf);
                std::vector<std::string> break_query=split(buffer_,',');

                //PARSING QUERY
                query temp_q;
                std::istringstream ss(break_query[1]);
                int train_id;
                ss>>train_id;
                printf("train id->%d ",train_id);
                int coach_type;
                ss.str(break_query[2]);
                std::string coach_;
                ss>>coach_;
                printf("coach_->%s",break_query[2].c_str());
                if(break_query[2][0]=='A'){
                    coach_type=(int)Coach::Class::AC;
                }
                else{
                    coach_type=(int)Coach::Class::SLEEPER;
                }
                printf("coach_type->%d ",coach_type);
                int num_people;
                std::string num_;
                std::istringstream ss1(break_query[3]);
                ss1>>num_people;
                printf("num_people->%d ..%s",num_people,break_query[3].c_str());
                std::vector<std::string> list_b=split(break_query[4],'-');
                std::vector<BerthPref> berth_;
                for(int i=0;i<(int)list_b.size();i++){
                    BerthPref temp;
                    if(list_b[i]=="SL"){
                        temp.type=Berth::Btype::SL;
                        temp.coach_type=coach_type;
                    }
                    else if(list_b[i]=="SU"){
                        temp.type=Berth::Btype::SU;
                        temp.coach_type=coach_type;
   
                    }
                    else if(list_b[i]=="LB"){
                        temp.type=Berth::Btype::LB;
                        temp.coach_type=coach_type;
                    }
                    else if(list_b[i]=="MB"){
                        temp.type=Berth::Btype::MB;
                        temp.coach_type=coach_type;
                    }
                    else if(list_b[i]=="UB"){
                        temp.type=Berth::Btype::UB;
                        temp.coach_type=coach_type;
                    }
                    else if(list_b[i]=="NA"){
                        temp.type=Berth::Btype::NA;
                        temp.coach_type=coach_type;
                    }
                    berth_.push_back(temp);
                }
                int time_h1;
                int time_m1;
                int time_s1;
                std::vector<std::string> temp_array=split(break_query[6],':');
                for(int i=0;i<3;i++){
                    std::istringstream ss(temp_array[i]);
                    int temp;
                    ss>>temp;
                    if(i==0){
                        time_h1=temp;
                    }
                    else if(i==1){
                        time_m1=temp;
                    }
                    else if(i==2){
                        time_s1=temp;
                    }
                }
                printf("%d/%d/%d\n",time_h1,time_m1,time_s1);
                printf("\n\n");
                int elder_count=0;
                std::vector<std::string> temp_people=split(break_query[5],'-');
                for(int i=0;i<(int)temp_people.size();i++){
                    std::istringstream ss(temp_people[i]);
                    std::cout<<" "<<temp_people[i];
                    int temp1;
                    ss>>temp1;
                    printf("%d->",temp1);
                    if(temp1>=60){
                        elder_count++;
                    }
                }
                printf("ELDER PEOPLE->%d\n",elder_count);
                temp_q.people_count=num_people;
                temp_q.elderly_count=elder_count;
                temp_q.time_h=time_h1;
                temp_q.time_m=time_m1;
                temp_q.time_s=time_s1;
                temp_q.berth=berth_;
                temp_q.id=cfd[i];
                temp_q.coach=coach_type;
                if(train_id==12321){
                    Q1.push_back(temp_q);
                }
                else if(train_id==12301){
                    Q2.push_back(temp_q);
                }
                //COMLPETE

                /*if(rst==-1){
                    printf("error\n");
                    perror("Client : Recieve failed\n");
                    exit(1);
                }

                if(strcmp(buf,"quit")==0){
                    cfd[i]=-1;
                    printf("closed connection %d\n",i);
                    continue;
                }
                else{
                    //qwe here we need to query
                    send(cfd[i],"HI THERE",BUF_SIZE,0);
                }*/
            }
        }
        std::sort(Q1.begin(),Q1.end());
        std::sort(Q2.begin(),Q2.end());
        std::reverse(Q1.begin(),Q1.end());
        std::reverse(Q2.begin(),Q2.end());

        for(int i=0;i<(int)Q1.size();i++){
            std::vector<std::vector<Berth>::iterator> result;
            bool success=t1.book(Q1[i].berth, result);
            std::string answer;
            if (success){
                for(auto it:result){
                    std::cout<<" "<<it->get_coach_name()<<" "<<it->no<<" "<<it->get_berth_place()<<"\n";
                    answer=answer+" "+it->get_coach_name();
                    answer=answer+" "+std::to_string(it->no);
                    answer=answer+" "+it->get_berth_place();
                    answer=answer+"::";
                }
            }
            else{
                answer="FAILURE";
            }
            strcpy(buf,answer.c_str());
            send(Q1[i].id,buf,BUF_SIZE,0);
        }

        for(int i=0;i<(int)Q2.size();i++){
            std::vector<std::vector<Berth>::iterator> result;
            bool success=t2.book(Q2[i].berth, result);
            std::string answer;
            if (success){
                for(auto it:result){
                    std::cout<<" "<<it->get_coach_name()<<" "<<it->no<<" "<<it->get_berth_place()<<"\n";
                    answer=answer+" "+it->get_coach_name();
                    answer=answer+" "+std::to_string(it->no);
                    answer=answer+" "+it->get_berth_place();
                    answer=answer+":: ";
                }
            }
            else{
                answer="FAILURE";
            }
            strcpy(buf,answer.c_str());
            send(Q2[i].id,buf,BUF_SIZE,0);
        }
    }
    
    /****************** Close ****************************************/
    printf ("Closing connection...\n");
    rst = close (sfd); // Close the socket file descriptor.
    if (rst == -1)
    {
        perror ("Server close failed");
        exit (1);
    }
return 0;}