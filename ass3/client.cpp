#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <time.h>

#define DEST "10.117.11.125"
#define SRC "10.117.11.171"
#define MOD 1000000007

//#define DEST "127.0.0.1"
#define port_serv 23466

//#define SRC "127.0.0.1"
#define port_client 23466

// RTLP header's structure
struct rtlpheader {
    unsigned short int rtlp_srcport;
    unsigned short int rtlp_destport;
    int rtlp_seqnum;
    int rtlp_acknum;
    unsigned int rtlp_chksum;
    unsigned short int rtlp_type;
};

//rtlp_type=-1 SYN
//rtlp_type=-2 ACK+SYN
//rtlp_type=-3 ACk
//rtlp_type=-4 DATA
//rtlp_type=-5 FIN

unsigned int csum(char *packet)
{
    struct iphdr *ip = (struct iphdr *)packet; 
    struct rtlpheader *rtlp = (struct rtlpheader *)&packet[20];
    int *temp=(int *)&packet[40];
    unsigned int hash=0;
    hash=rtlp->rtlp_srcport;
    hash=hash*(MOD)+rtlp->rtlp_destport;
    hash=hash*(MOD)+rtlp->rtlp_seqnum;
    hash=hash*(MOD)+rtlp->rtlp_acknum;
    hash=hash*(MOD)+*temp;
    return hash;       
}

struct sockaddr_in daddr,saddr;

void correct(){
    daddr.sin_family = AF_INET;
    saddr.sin_family = AF_INET;
    daddr.sin_port = 0; /* not needed in SOCK_RAW */
    saddr.sin_port = 0; /* not needed in SOCK_RAW */
    inet_pton(AF_INET, SRC, (struct in_addr *)&saddr.sin_addr.s_addr);
    inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);
    memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
    memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero));
}

int main(void)
{
    int limit1;
    printf("enter the number of times you want to ping the server\n");
    scanf("%d",&limit1);
    int s;
    int s_timeout;
    srand(time(NULL));
    char packet[1000] ;

    /* point the iphdr to the beginning of the packet */
    struct iphdr *ip = (struct iphdr *)packet; 
    struct rtlpheader *rtlp = (struct rtlpheader *)&packet[20];
    //printf("%d...%d...%d..%d\n",sizeof(struct rtlpheader),sizeof(int),sizeof(unsigned short int),sizeof(unsigned int));

    if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }

    s_timeout=s;
    struct timeval tv;
    tv.tv_sec=0;
    tv.tv_usec=4001;
    setsockopt(s_timeout,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv,sizeof(struct timeval));

    daddr.sin_family = AF_INET;
    saddr.sin_family = AF_INET;
    daddr.sin_port = 0; /* not needed in SOCK_RAW */
    saddr.sin_port = 0; /* not needed in SOCK_RAW */
    inet_pton(AF_INET, SRC, (struct in_addr *)&saddr.sin_addr.s_addr);
    inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);
    memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
    memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero)); 
    
    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = htons(40);    /* 16 byte value */
    ip->frag_off = 0;       /* no fragment */
    ip->ttl = 64;           /* default value */
    ip->protocol = IPPROTO_RAW; /* protocol at L4 */
    ip->check = 0;          /* not needed in iphdr */
    ip->saddr = saddr.sin_addr.s_addr;
    ip->daddr = daddr.sin_addr.s_addr;

    rtlp->rtlp_srcport = htons(port_serv);
    rtlp->rtlp_destport = htons(port_client);
    rtlp->rtlp_seqnum = rand()%300+1;
    rtlp->rtlp_acknum = -1;
    rtlp->rtlp_chksum = csum(packet);
    int index=sizeof(struct iphdr)+sizeof(struct rtlpheader);
    unsigned int checker=rtlp->rtlp_seqnum;
    unsigned int fromlen=sizeof(daddr);
    int rst=0;

    for(int k=0;k<=0;k++){
        sleep(1);
        rtlp->rtlp_srcport = htons(port_serv);
        rtlp->rtlp_destport = htons(port_client);
        rtlp->rtlp_seqnum = rand()%300+1;
        checker=rtlp->rtlp_seqnum;
        rtlp->rtlp_acknum = -1;
        correct();
        ip->saddr = saddr.sin_addr.s_addr;
        ip->daddr = daddr.sin_addr.s_addr;
        rtlp->rtlp_srcport = htons(port_serv);
        rtlp->rtlp_destport = htons(port_client);
        rtlp->rtlp_type=1;

        //std::swap(rtlp->rtlp_srcport,rtlp->rtlp_destport);
        //std::swap(ip->saddr,ip->daddr);

        unsigned int checker=rtlp->rtlp_seqnum;
        while(1){
            rtlp->rtlp_type=1;
            rtlp->rtlp_seqnum=checker;
            rtlp->rtlp_acknum = -1;
            correct();
            ip->saddr = saddr.sin_addr.s_addr;
            ip->daddr = daddr.sin_addr.s_addr;
            rtlp->rtlp_srcport = htons(port_serv);
            rtlp->rtlp_destport = htons(port_client);
            if(sendto(s,(char *)packet,sizeof(packet),0,(struct sockaddr *)&daddr,(socklen_t)sizeof(daddr))<0){
                perror("packet send error:");
            }
            else{
                printf("SUCCESSFULLY SENT A SYN PACKET\n");
                printf("ip->saddr=%u   ip->daddr=%u\n",ip->saddr,ip->daddr);
                printf("%d  |  %d  |  %d  |  %d  |  %d  |  %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum,rtlp->rtlp_acknum,rtlp->rtlp_chksum,rtlp->rtlp_type);
            }
            printf("///WAITING///\n");
            rst=0;
            if(recvfrom(s,(char *)&packet,sizeof(packet),0,(struct sockaddr *)&saddr,&fromlen)<0){
                perror("packet recieve error:");
            }
            else{
                printf("SUCCESSFULLY RECIEVED SYN+ACK WITH SERVER\n");
                iphdr *ip = (struct iphdr *)packet; 
                rtlpheader *rtlp = (struct rtlpheader *)&packet[20];
                correct();
                ip->saddr = saddr.sin_addr.s_addr;
                ip->daddr = daddr.sin_addr.s_addr;
                rtlp->rtlp_srcport = htons(port_serv);
                rtlp->rtlp_destport = htons(port_client);
                printf("%d  |  %d  |  %d  |  %d  |  %d  |   %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum,rtlp->rtlp_acknum,rtlp->rtlp_chksum,rtlp->rtlp_type);
                if(rtlp->rtlp_acknum==checker){
                    printf("success\n");
                    break;
                }
                else{
                    continue;
                }
            }
        }
        ip = (struct iphdr *)packet; 
        rtlp = (struct rtlpheader *)&packet[20];

        if(rtlp->rtlp_acknum==checker ){
            printf("SEND AN ACKNOWLEDGENMENT\n");
            rtlp->rtlp_acknum=rtlp->rtlp_seqnum;
            rtlp->rtlp_seqnum=checker;
            rtlp->rtlp_type=8;
            printf("%d  |  %d  |  %d  |  %d  |  %d  |   %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum,rtlp->rtlp_acknum,rtlp->rtlp_chksum,rtlp->rtlp_type);
            if(sendto(s,(char *)packet,sizeof(packet),0,(struct sockaddr *)&daddr,(socklen_t)sizeof(daddr))<0){
                perror("packet send error:");
            }
            else{
                printf("SEND ACKNOWLEDGENMENT!!\n");
            }
        }
        printf("HANDSAKE DONE!!\n");
        //sleep(7);
        while(recvfrom(s_timeout,(char *)&packet,sizeof(packet),0,(struct sockaddr *)&saddr,&fromlen)>0){
           //qwe
        }

        //SENDING DATA ie "ECHO REQUEST" 100
        int *temp=(int *)&packet[40];
        *temp=0;
        int counter=0;
        bool ackrecv=true;
        while(true){
            correct();
            ip->saddr = saddr.sin_addr.s_addr;
            ip->daddr = daddr.sin_addr.s_addr;
            rtlp->rtlp_srcport = htons(port_serv);
            rtlp->rtlp_destport = htons(port_client);
            rtlp->rtlp_type=4;
            
            //CHECKER
            if(ackrecv){
                 counter++;
                 while(recvfrom(s_timeout,(char *)&packet,sizeof(packet),0,(struct sockaddr *)&saddr,&fromlen)>0){
                    //qwe
                 }
                 rtlp->rtlp_seqnum+=sizeof(int);
            }
            else{
            }
            *temp=counter;
            rtlp->rtlp_chksum = csum(packet);
            int checker_=rtlp->rtlp_seqnum;
            
            //CHECKER
            if(sendto(s,(char *)packet,sizeof(packet),0,(struct sockaddr *)&daddr,(socklen_t)sizeof(daddr))<0){
                perror("packet send error:");
            }
            else{
                printf("ECHO REQUEST to SERVER %d\n",*temp);
            }
            if(recvfrom(s_timeout,(char *)&packet,sizeof(packet),0,(struct sockaddr *)&saddr,&fromlen)<0){
                ackrecv=false;
            }
            else{
                int *temp2=(int *)&packet[40];
                if(counter==*temp2 && rtlp->rtlp_acknum==checker_&& rtlp->rtlp_type==3){
                    printf("ACKNOWLEDGED FROM  SERVER -->>%d\n",*temp2);
                    ackrecv=true;
                    if(counter==limit1){
                        break;
                    }
                }
                else{
                    ackrecv=false;
                }
                rtlp->rtlp_seqnum=checker_;
            }
        }
        //DONE SENDING DATA

        while(recvfrom(s_timeout,(char *)&packet,sizeof(packet),0,(struct sockaddr *)&saddr,&fromlen)>0){
           //qwe
        }

        //FINISHING CONNECTION
        rtlp->rtlp_srcport = htons(port_serv);
        rtlp->rtlp_destport = htons(port_client);
        rtlp->rtlp_seqnum = rand()%300+1;
        checker=rtlp->rtlp_seqnum;
        rtlp->rtlp_acknum = -1;
        correct();
        ip->saddr = saddr.sin_addr.s_addr;
        ip->daddr = daddr.sin_addr.s_addr;
        rtlp->rtlp_srcport = htons(port_serv);
        rtlp->rtlp_destport = htons(port_client);
        rtlp->rtlp_type=5;
        while(1){
            rtlp->rtlp_type=5;
            rtlp->rtlp_seqnum=checker;
            rtlp->rtlp_acknum = -1;
            correct();
            ip->saddr = saddr.sin_addr.s_addr;
            ip->daddr = daddr.sin_addr.s_addr;
            rtlp->rtlp_srcport = htons(port_serv);
            rtlp->rtlp_destport = htons(port_client);

            if(sendto(s,(char *)packet,sizeof(packet),0,(struct sockaddr *)&daddr,(socklen_t)sizeof(daddr))<0){
                perror("packet send error:");
            }
            else{
                printf("SUCCESSFULLY SENT A FIN PACKET\n");
                printf("ip->saddr=%u   ip->daddr=%u\n",ip->saddr,ip->daddr);
                printf("%d  |  %d  |  %d  |  %d  |  %d  |  %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum,rtlp->rtlp_acknum,rtlp->rtlp_chksum,rtlp->rtlp_type);
            }
            printf("///WAITING///\n");
            rst=0;
            if(recvfrom(s,(char *)&packet,sizeof(packet),0,(struct sockaddr *)&saddr,&fromlen)<0){
                perror("packet recieve error:");
            }
            else{
                printf("SUCCESSFULLY RECIEVED FIN+ACK WITH SERVER\n");
                iphdr *ip = (struct iphdr *)packet; 
                rtlpheader *rtlp = (struct rtlpheader *)&packet[20];
                correct();
                ip->saddr = saddr.sin_addr.s_addr;
                ip->daddr = daddr.sin_addr.s_addr;
                rtlp->rtlp_srcport = htons(port_serv);
                rtlp->rtlp_destport = htons(port_client);
                printf("%d  |  %d  |  %d  |  %d  |  %d  |   %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum,rtlp->rtlp_acknum,rtlp->rtlp_chksum,rtlp->rtlp_type);
                if(rtlp->rtlp_acknum==checker){
                    break;
                }
                else{
                    continue;
                }
            }
        }
        ip = (struct iphdr *)packet; 
        rtlp = (struct rtlpheader *)&packet[20];

        if(rtlp->rtlp_acknum==checker){
            printf("SEND AN ACKNOWLEDGENMENT\n");
            rtlp->rtlp_acknum=rtlp->rtlp_seqnum;
            rtlp->rtlp_seqnum=checker;
            rtlp->rtlp_type=9;
            printf("%d  |  %d  |  %d  |  %d  |  %d  |   %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum,rtlp->rtlp_acknum,rtlp->rtlp_chksum,rtlp->rtlp_type);
            if(sendto(s,(char *)packet,sizeof(packet),0,(struct sockaddr *)&daddr,(socklen_t)sizeof(daddr))<0){
                perror("packet send error:");
            }
            else{
                printf("SEND ACKNOWLEDGENMENT!!\n");
            }
        }
        close(s);
        //DONE
    }

exit(EXIT_SUCCESS);}