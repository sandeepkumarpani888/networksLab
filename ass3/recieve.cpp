#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/stdc++.h>

#define DEST "10.145.148.86"
#define SRC "10.146.7.116"
#define SRC_PORT 23145
#define DST_PORT 16423

int main(void)
{

    int s;
    struct sockaddr_in daddr,saddr;
    char packet[1000];
    /* point the iphdr to the beginning of the packet */
    struct iphdr *ip = (struct iphdr *)packet;

    if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }

    daddr.sin_family = AF_INET;
    saddr.sin_family = AF_INET;
    daddr.sin_port = htons(SRC_PORT); /* not needed in SOCK_RAW */
    saddr.sin_port = htons(DST_PORT); /* not needed in SOCK_RAW */
    inet_pton(AF_INET, SRC, (struct in_addr *)&saddr.sin_addr.s_addr);
    inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);
    memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
    memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero));
    //memset(packet, 'B', sizeof(packet));   /* payload will be all As */
    packet[20]='y';
    packet[21]='l';
    for(int i=22;i<sizeof(packet);i++){
        packet[i]='&';
    }
    printf("%d\n",sizeof(packet));

    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = htons(sizeof(iphdr));    /* 16 byte value */
    ip->frag_off = 0;       /* no fragment */
    ip->ttl = 64;           /* default value */
    ip->protocol = IPPROTO_RAW; /* protocol at L4 */
    ip->check = 0;          /* not needed in iphdr */
    ip->saddr = saddr.sin_addr.s_addr;
    ip->daddr = daddr.sin_addr.s_addr;

    for(int j=0;j<5;j++){
        sleep(1);
        printf("recieved from ip->saddr->%d to ip->daddr->%d\n",ip->saddr,ip->daddr);
        if (sendto(s, (char *)packet, sizeof(packet), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
            perror("packet send error:");
    
    }
    close(s);
    exit(EXIT_SUCCESS);
}