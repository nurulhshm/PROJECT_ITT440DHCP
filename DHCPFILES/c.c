/*
* Shih-Chieh Lin
* CSCE 5580 pogramming assignment 4
* 4/24/2017
* A server (cse01.cse.unt.edu)in the internet using UDP domain, and the port number is get from argument number 1
* demonstrate working of DHCP server using a client-server architecture.
* DHCP server offer three IP address to client, and client randomly choosing an IP address,
* client randomly generated transaction ID from 0 to 99 in the beginning, after choose an IP address, transaction ID add 1
* Reference:
* http://stackoverflow.com/questions/33603139/create-random-numbers-using-the-current-time-of-the-machine-any-idea
* http://stackoverflow.com/questions/12380758/c-error-function-returns-address-of-local-variable
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define SERVER "129.120.151.94"
#define BUFLEN 512  //Max length of buffer

void die(char *s)
{
    perror(s);
    exit(1);
}
 
struct udp{
	char ip[129];// ip address
	int ip1, ip2, ip3;//three IP address choice
	int id;// transaction ID
	int lifetime;// lifetime
};

char * print_ip(uint32_t ip)//convert int to string of IP address
{
	unsigned char bytes[4];
	char *str = malloc(sizeof(char) * 512);
	char buf3[256] = "";
	char buf2[256] = "";
	char buf1[256] = "";
	char buf0[256] = "";
	bytes[0] = ip & 0xFF;
	bytes[1] = (ip >> 8) & 0xFF;
	bytes[2] = (ip >> 16) & 0xFF;
	bytes[3] = (ip >> 24) & 0xFF;
	//printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);//testing  
	sprintf(buf3, "%d", bytes[3]);
	strcat(str, buf3);
	strcat(str, ".");
	sprintf(buf2, "%d", bytes[2]);
	strcat(str, buf2);
	strcat(str, ".");
	sprintf(buf1, "%d", bytes[1]);
	strcat(str, buf1);
	strcat(str, ".");
	sprintf(buf0, "%d", bytes[0]);
	strcat(str, buf0);
	//printf("string:%s\n", str);//testing show string not return
	//printf("string:%x\n", IPToUInt(str));
	return str;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

		int portnumber;
		struct udp a;//receive struct from server
		struct udp b;//send struct from client
		memset((char *)&a, 0, sizeof(a));
		memset((char *)&b, 0, sizeof(b));

		portnumber = atoi(argv[1]);//port number from input
		
		srand((unsigned)time(NULL));//create random number by current time
		int random = rand();
		strcpy(b.ip, "0.0.0.0");// set IP address to 0.0.0.0
		//printf("b.ip: %s\n",b.ip);//testing
		////b.id = random%100;//0-99
		////printf("b.id: %d\n", b.id);//testing
		////b.lifetime = 0;
		////printf("b.lifetime: %d\n", b.lifetime);//testing

		if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
		si_other.sin_port = htons(portnumber);
     
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
		int choiceip;
		b.id = random % 100;//0-99
		choiceip = random % 3;// three ip server given random pick one
		b.lifetime = 0;
    while(1)
    {      
					
				printf("client IP address: %s\n", b.ip);
				printf("client transaction ID: %d\n", b.id);//testing
				printf("client lifetime: %d\n", b.lifetime);
				//printf("b.lifetime: %d\n", b.lifetime);//testing
				//send the struct b		
				if (sendto(s, &b, sizeof(struct udp), 0, (struct sockaddr *) &si_other, slen) == -1)
				{
					die("sendto()");
				}
				
        //receive a reply and print it
       
				if (recvfrom(s, &a, sizeof(struct udp), 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            die("recvfrom()");
        }
				//printf("DHCP server give ip:%x, %x, %x\n",  a.ip1,a.ip2,a.ip3);// int of ip
				printf("DHCP server give ip:\n%s,\n%s,\n%s\n", print_ip(a.ip1), print_ip(a.ip2), print_ip(a.ip3));
				b.lifetime = a.lifetime;
				//printf("liftime: %d, %d",a.lifetime,b.lifetime);
				b.id = a.id + 1;// ID+1
				//randomly choice one IP address
				char * sp;				
				if (choiceip == 0){
					sp=print_ip(a.ip1);					
				}
				if (choiceip == 1){
					sp=print_ip(a.ip2);
				}
				if (choiceip == 2){
					sp=print_ip(a.ip3);
				}
				printf("client choice IP address:%s\n", sp); // testing
				
				//printf("b.ip:%s\n",b.ip);//testing
				//printf("new client IP adress: %s",b.ip);
				//printf("lifetime aft:%d, id:%d\n",b.lifetime,b.id);//testing	
				if (strcmp(a.ip,b.ip)==0){ break; }
				strcpy(b.ip, sp);
				printf("\n----------------\n");
    }
    close(s);
    return 0;
}
