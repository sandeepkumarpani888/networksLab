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

#define DEST "10.117.11.171"
#define SRC "10.117.11.125"

//#define DEST "127.0.0.1"
#define port_serv 23466

//#define SRC "127.0.0.1"
#define port_client 23466

#define MOD 1000000007

// RTLP header's structure
struct rtlpheader {
    unsigned short int rtlp_srcport;
    unsigned short int rtlp_destport;
    int rtlp_seqnum;
    int rtlp_acknum;
    unsigned int rtlp_chksum;
    unsigned short int rtlp_type;
};

unsigned int csum(char *packet)
{
    struct iphdr *ip = (struct iphdr *)packet;
    struct rtlpheader *rtlp = (struct rtlpheader *)&packet[20];
    int *temp = (int *)&packet[40];
    unsigned hash = 0;
    hash = rtlp->rtlp_srcport;
    hash = hash * (MOD) + rtlp->rtlp_destport;
    hash = hash * (MOD) + rtlp->rtlp_seqnum;
    hash = hash * (MOD) + rtlp->rtlp_acknum;
    hash = hash * (MOD) + *temp;
    return hash;
}

int main(void)
{

    int s;
    struct sockaddr_in daddr, saddr;
    char packet[1000] ;
    srand(time(NULL));

    /* point the iphdr to the beginning of the packet */
    struct iphdr *ip = (struct iphdr *)packet;
    struct rtlpheader *rtlp = (struct rtlpheader *)&packet[20];
    if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }

    daddr.sin_family = AF_INET;
    saddr.sin_family = AF_INET;
    daddr.sin_port = 0; /* not needed in SOCK_RAW */
    saddr.sin_port = 0; /* not needed in SOCK_RAW */
    inet_pton(AF_INET, SRC, (struct in_addr *)&saddr.sin_addr.s_addr);
    inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);
    memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
    memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero));
    unsigned int fromlen = sizeof(saddr);
    unsigned int checker = -1;
    bool handshake = false;

    while(1) {
        printf("\nWAITING %u %u\n", ip->saddr, ip->daddr);
        if (recvfrom(s, (char *)&packet, sizeof(packet), 0, (struct sockaddr *)&saddr, &fromlen) < 0) {
            perror("packet recieve error:");
        }
        else if (!handshake) {
            printf("SUCCESSFULLY RECIEVED A PACKET FROM CLIENT");
            iphdr *ip = (struct iphdr *)packet;
            rtlpheader *rtlp = (struct rtlpheader *)&packet[20];
            printf("%d  |  %d  |  %d  |  %d  |  %d  \n", rtlp->rtlp_srcport, rtlp->rtlp_destport, rtlp->rtlp_seqnum, rtlp->rtlp_acknum, rtlp->rtlp_chksum);

            if (rtlp->rtlp_type == 1) {
                printf("of type SYN\n");
                rtlp->rtlp_acknum = rtlp->rtlp_seqnum;
                rtlp->rtlp_seqnum = rand() % 300 + 1;
                checker = rtlp->rtlp_seqnum;
                std::swap(ip->saddr, ip->daddr);
                std::swap(rtlp->rtlp_srcport, rtlp->rtlp_destport);
                rtlp->rtlp_type=2;
                if (sendto(s, (char *)packet, sizeof(packet), 0, (struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0) {
                    perror("SYN+ACK packet send error");
                }
                else {
                    printf("SYN+ACK PACKET SENT! with checked->%d\n", checker);
                    printf("%d  |  %d  |  %d  |  %d  |  %d   |   %d\n", rtlp->rtlp_srcport, rtlp->rtlp_destport, rtlp->rtlp_seqnum, rtlp->rtlp_acknum, rtlp->rtlp_chksum,rtlp->rtlp_type);
                }
            }
            else if (rtlp->rtlp_acknum == checker && rtlp->rtlp_type==8) {
                printf(" RECIEVED ACKNOWLEDGMENT\n");
                printf("%d  |  %d  |  %d  |  %d  |  %d  |   %d\n", rtlp->rtlp_srcport, rtlp->rtlp_destport, rtlp->rtlp_seqnum, rtlp->rtlp_acknum, rtlp->rtlp_chksum,rtlp->rtlp_type);
                handshake = true;
            }
            else {
                printf("ERROR RECIEVING HANDSHAKE ACKNOWLEDGMENT\n");
                printf("%d  |  %d  |  %d  |  %d  |  %d   |   %d\n", rtlp->rtlp_srcport, rtlp->rtlp_destport, rtlp->rtlp_seqnum, rtlp->rtlp_acknum, rtlp->rtlp_chksum,rtlp->rtlp_type);
            }
        }
        else if (handshake) {
            printf("handshaked\n");
            int *temp = (int*)&packet[40];
            //printf("ECHO REQUEST FROM CLIENT %d\n", *temp);
            //*temp = *temp + 1;
            iphdr *ip = (struct iphdr *)packet;
            rtlpheader *rtlp = (struct rtlpheader *)&packet[20];
            printf("%d-->>\n",rtlp->rtlp_type);
            if(rtlp->rtlp_type==4){
                //DATA TRANSFER
                printf("ECHO REQUEST FROM CLIENT %d\n",*temp);
                if (rtlp->rtlp_chksum != csum(packet)) {
                    continue;
                }
                std::swap(ip->saddr, ip->daddr);
                std::swap(rtlp->rtlp_srcport, rtlp->rtlp_destport);
                std::swap(rtlp->rtlp_seqnum, rtlp->rtlp_acknum);
                rtlp->rtlp_type=3;
                usleep(3500);
                if (sendto(s, (char *)packet, sizeof(packet), 0, (struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0) {
                    perror("ECHO REQUEST TO CLIENT FAILED\n");
                }
                else {
                    printf("ECHO REQUEST %d SENT TO CLIENT\n", *temp);
                }
                std::swap(ip->saddr, ip->daddr);
                std::swap(rtlp->rtlp_srcport, rtlp->rtlp_destport);
            }
            else if(rtlp->rtlp_type==5){
                //FINSIH SEQUENCE STARTS
                printf("SUCCESSFULLY RECIEVED A PACKET FROM CLIENT");
                printf("%d  |  %d  |  %d  |  %d  |  %d  \n", rtlp->rtlp_srcport, rtlp->rtlp_destport, rtlp->rtlp_seqnum, rtlp->rtlp_acknum, rtlp->rtlp_chksum);

                if (rtlp->rtlp_type == 5) {
                    printf("of type FIN\n");
                    iphdr *ip = (struct iphdr *)packet;
                    rtlpheader *rtlp = (struct rtlpheader *)&packet[20];
                    rtlp->rtlp_acknum = rtlp->rtlp_seqnum;
                    rtlp->rtlp_seqnum = rand() % 300 + 1;
                    checker = rtlp->rtlp_seqnum;
                    std::swap(ip->saddr, ip->daddr);
                    std::swap(rtlp->rtlp_srcport, rtlp->rtlp_destport);
                    rtlp->rtlp_type=2;
                    if (sendto(s, (char *)packet, sizeof(packet), 0, (struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0) {
                        perror("FIN+ACK packet send error");
                    }
                    else {
                        printf("FIN+ACK PACKET SENT! with checked->%d\n", checker);
                        printf("%d  |  %d  |  %d  |  %d  |  %d   |   %d\n", rtlp->rtlp_srcport, rtlp->rtlp_destport, rtlp->rtlp_seqnum, rtlp->rtlp_acknum, rtlp->rtlp_chksum,rtlp->rtlp_type);
                    }
                }
            }
            else if(rtlp->rtlp_type==9){
                printf(" RECIEVED ACKNOWLEDGMENT\n");
                printf("%d  |  %d  |  %d  |  %d  |  %d  |   %d\n", rtlp->rtlp_srcport, rtlp->rtlp_destport, rtlp->rtlp_seqnum, rtlp->rtlp_acknum, rtlp->rtlp_chksum,rtlp->rtlp_type);
                handshake = true;
                close(s);
                exit(EXIT_SUCCESS);
            }
            //FINISH SEQUENCE ENDS
        }
    }
    close(s);
    exit(EXIT_SUCCESS);
}