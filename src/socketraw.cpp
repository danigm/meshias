#include <sys/types.h>
#define __FAVOR_BSD
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <cstdlib>
char data[1024] = "";

using namespace std;

unsigned short csum(unsigned short *buf,int nwords)
{
    //this function returns the checksum of a buffer
    unsigned long sum;
    for (sum = 0; nwords > 0; nwords--){sum += *buf++;}
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short) (~sum);
}

int createRaw(int protocol_to_sniff)
{
    int raw_fd = socket(AF_INET, SOCK_RAW, protocol_to_sniff);
    if (raw_fd < 0)
    {
        cout << "ERROR creating raw socket\n";
        exit(1);
    }else{
        cout << "Raw Socket Created!        :-D\n";
        return raw_fd;
    }
}
int bindRaw(int socketToBind,sockaddr_in* sin)
{
    int err = bind(socketToBind,(struct sockaddr *)sin,sizeof(*sin));
    if (err < 0)
    {
        cout << "ERROR binding socket.\n";
        exit(1);
    }else{
        cout << "Bound socket!          :-D\n";
        return 0;
    }
}

int main(int argc,char* argv[])
{
    //create raw socket for binding
    int bindSocket = createRaw(17);
    
    //create structures
    struct sockaddr_in sin;
    unsigned char packetBuf[4096];
    
    //specify port to bind to
    bzero((char *)&sin,sizeof(sin));
    sin.sin_port = htons(55000);
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr= INADDR_ANY;
    
    //bind socket
    bindRaw(bindSocket,&sin);
    
    int tmp = 1;
    setsockopt(bindSocket, 0, IP_HDRINCL,&tmp,sizeof(tmp));
    
    //re-use socket structure
    //Details about where this custom packet is going:
    bzero((char *)& sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(55000);    //port to send packet to
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");   //IP to send packet to
    
    unsigned short buffer_size = sizeof(struct ip) + sizeof(struct tcphdr);//+ sizeof(data);
    cout << "Buffer size: " << buffer_size << endl;
    
    struct ip *IPheader = (struct ip *) packetBuf;
    struct tcphdr *TCPheader = (struct tcphdr *) (packetBuf + sizeof (struct ip));
    
    //Fill out IP Header information:
    IPheader->ip_hl = 5;
    IPheader->ip_v = 4;     //IPv4
    IPheader->ip_tos = 0;       //type of service
    IPheader->ip_len = htons(buffer_size);  //length
    IPheader->ip_id = htonl(54321);
    IPheader->ip_off = 0;
    IPheader->ip_ttl = 255; //max routers to pass through
    IPheader->ip_p = 17;     //tcp
    IPheader->ip_sum = 0;   //Set to 0 before calulating later
    IPheader->ip_src.s_addr = inet_addr("123.4.5.6");   //source IP address
    IPheader->ip_dst.s_addr = inet_addr("127.0.0.1");   //destination IP address
    
    //Fill out TCP Header information:
    TCPheader->th_sport = htons(55000); //source port
    TCPheader->th_dport = htons(55000);         //destination port
    TCPheader->th_seq = random();
    TCPheader->th_ack = 0;  //Only 0 on initial SYN 
    TCPheader->th_off = 0;
    TCPheader->th_flags = TH_SYN;   //SYN flag set
    TCPheader->th_win = htonl(65535);   //used for segmentation
    TCPheader->th_sum = 0;              //Kernel fill this out
    TCPheader->th_urp = 0;
    
    //Now fill out the checksum for the IPheader
    IPheader->ip_sum = 0; // csum((unsigned short *) packetBuf, IPheader->ip_len >> 1);
    cout << "IP Checksum: " << IPheader->ip_sum << endl;
    //create raw socket for sending ip packet
    int sendRaw = createRaw(6);
    if (sendRaw < 0)
    {
        cout << "ERROR creating raw socket for sending.\n";
        exit(1);
    }else{
        cout << "Raw socket created for sending!    :-D\n";
    }
    int sendErr = sendto(sendRaw,packetBuf,
        sizeof(packetBuf),0,(struct sockaddr *)&sin,sizeof(sin));
    
    if (sendErr < sizeof(packetBuf))
    {
        cout << sendErr << " out of " << sizeof(packetBuf) << " were sent.\n";
        exit(1);
    }else{
        cout << "<" << sendErr << "> Sent message!!!        :-D\n";
    }
    
    cout << "Sleeping for 2 seconds";
    sleep(1);
    cout << ".";
    sleep(1);
    cout << ".\n";
    char recvPacket[4096] = "";
    int newData = recv(bindSocket,recvPacket,sizeof(recvPacket),0);
    if (newData <=0)
    {
        cout << newData << " returned by recv! :(\n";
        exit(1);
    }
    else
    {
        printf("Data received: %d\n",newData);
        for(int i=0;i<newData;i++)
            printf("%c",recvPacket[i]);
        puts("");
        IPheader = (struct ip *) recvPacket;
        TCPheader = (struct tcphdr *) (recvPacket + sizeof (struct ip));
        printf("TTL: %d source_address: %s dest_address: %s s_port: %d d_port: %d\n",IPheader->ip_ttl,
                inet_ntoa(IPheader->ip_src),inet_ntoa(IPheader->ip_dst),
                    ntohs(TCPheader->th_sport),ntohs(TCPheader->th_dport));

        //cout << "<" << recvPacket << "> RECIEVE SUCCESSFULL!!  :-D\n";
    }
    
    return 0;   
}