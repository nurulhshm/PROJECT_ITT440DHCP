/*
* Shih-Chieh Lin
* CSCE 5580 pogramming assignment 4
* 4/24/2017
* A server (cse01.cse.unt.edu)in the internet using UDP domain, and the port number is get from argument number 1
* demonstrate working of DHCP server using a client-server architecture.
* DHCP server offer three IP address to client, and client randomly choosing an IP address, 
* client randomly generated transaction ID in the beginning, after choose an IP address, transaction ID add 1  
* Reference:
* http://beej-zhtw.netdpi.net/09-man-manual/9-13-inet_ntoa-inet_aton-inet_addr
* http://stackoverflow.com/questions/26941426/how-to-check-if-string-contains-particular-character
* http://www.geeksforgeeks.org/program-to-validate-an-ip-address/
* http://stackoverflow.com/questions/1680365/integer-to-ip-address-c
*/
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
#define BUFLEN 512  //Max length of buffer

void die(char *s)
{
    perror(s);
    exit(1);
}

char* getgateway(char *input){
	printf("gateway: ");
	fgets(input, 256, stdin);
	return input;
}

char* getsubnet_mask(char *input1){
	printf("subnet_mask: ");
	fgets(input1, 256, stdin);
	return input1;
}

struct udp{	
	char ip[129];// ip address
	int ip1,ip2,ip3;//three IP address choice
	int id;// transaction ID
	int lifetime;// lifetime
};

/* return 1 if string contain only digits, else return 0 */
int valid_digit(char *ip_str)
{
	while (*ip_str) {
		if (*ip_str >= '0' && *ip_str <= '9')
			++ip_str;
		else
			return 0;
	}
	return 1;
}

/* return 1 if IP string is valid, else return 0 */
int is_valid_ip(char *ip_str)
{
	int i, num, dots = 0;
	char *ptr;

	if (ip_str == NULL)
		return 0;

	// See following link for strtok()
	// http://pubs.opengroup.org/onlinepubs/009695399/functions/strtok_r.html
	ptr = strtok(ip_str, ".");

	if (ptr == NULL)
		return 0;

	while (ptr) {
		/* after parsing string, it must contain only digits */
		if (!valid_digit(ptr))
			return 0;

		num = atoi(ptr);

		/* check for valid IP */
		if (num >= 0 && num <= 255) {
			/* parse remaining string */
			ptr = strtok(NULL, ".");
			if (ptr != NULL)
				++dots;
		}
		else
			return 0;
	}

	/* valid IP string must contain 3 dots */
	if (dots != 3)
		return 0;
	return 1;
}

uint32_t IPToUInt(char *ip) {
	int a5, b5, c5, d5;
	uint32_t addr = 0;

	if (sscanf(ip, "%d.%d.%d.%d", &a5, &b5, &c5, &d5) != 4)
		return 0;

	addr = a5 << 24;
	addr |= b5 << 16;
	addr |= c5 << 8;
	addr |= d5;
	return addr;
}

char * print_ip(uint32_t ip)
{
    unsigned char bytes[4]; 
		char *str = malloc(sizeof(char) * 512);
		char buf3[256]="";
		char buf2[256]="";
		char buf1[256]="";
		char buf0[256]="";
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;   
    //printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);  		
		sprintf(buf3,"%d",bytes[3]);
    strcat(str,buf3);		
		strcat(str,".");
		sprintf(buf2,"%d",bytes[2]);
		strcat(str,buf2);
		strcat(str,".");
		sprintf(buf1,"%d",bytes[1]);
		strcat(str,buf1);
		strcat(str,".");
		sprintf(buf0,"%d",bytes[0]);
		strcat(str,buf0);
    return str;
}

int setip1(int total, struct udp a, int *ptr){
	int num;
  for (num = 0; num < total; num++){
	  if (ptr[total * 1 + num] == 0){
		  a.ip1 = ptr[num];
		  ptr[total * 1 + num] = 1;//mark as send
		  break;
	  }
  }
	return a.ip1;
  //printf("send ip1: %x\n", a.ip1);//send three ip choice
}
int setip2(int total, struct udp a, int *ptr){
	int num;
	for (num = 0; num < total; num++){
		if (ptr[total * 1 + num] == 0){
			a.ip2 = ptr[num];
			ptr[total * 1 + num] = 1;//mark as send
			break;
		}
	}
	return a.ip2;
	//printf("send ip2: %x\n", a.ip2);//send three ip choice
}
int setip3(int total, struct udp a, int *ptr){
	int num;
	for (num = 0; num < total; num++){
		if (ptr[total * 1 + num] == 0){
			a.ip3 = ptr[num];
			ptr[total * 1 + num] = 1;//mark as send
			break;
		}
	}
	return a.ip3;
	//printf("send ip3: %x\n", a.ip3);//send three ip choice
}
/////////////////
int main(int argc, char *argv[])
{
    struct sockaddr_in si_me, si_other;
     
    int s, i, slen = sizeof(si_other) , recv_len;

		int portnumber;// port number from input
    char buf[BUFLEN];
		struct udp a;//send struct from server
		struct udp b;//receive struct from client
		memset((char *)&a, 0, sizeof(a));
		a.lifetime = 3600; // set lifetime 3600 secs correct
		memset((char *)&b, 0, sizeof(b));
		/////////////check if user enter port number
		if (argc < 2) {
			fprintf(stderr, "ERROR, no port provided\n");
			exit(1);
		}
		portnumber = atoi(argv[1]);// port number from input
		char gatewayin[256];//buffer for number user enter for gateway number
		char *gateway = //"192.168.1.1";
			getgateway(gatewayin);//correct
		struct sockaddr_in antelope;
		struct sockaddr_in antelope1;
		char *some_addr;
		char *some_addr1;
		
		inet_aton(gateway, &antelope.sin_addr);//convert string to struct in_addr IP address

		some_addr = inet_ntoa(antelope.sin_addr); // return IP back to string
		printf("input gateway:%s\n", some_addr);//check
		is_valid_ip(some_addr) ? printf("Valid input\n") : printf("Input is Not valid!\n");

		char subnetmaskin[256];//buffer for number user enter for subnet_mask
		char *subnet_mask = //"255.255.255.0";//
			getsubnet_mask(subnetmaskin);//correct
		//inet_aton(getsubnet_mask(subnetmaskin), &antelope1.sin_addr);//convert string to struct in_addr IP address
		inet_aton(subnet_mask, &antelope1.sin_addr);//convert string to struct in_addr IP address
		some_addr1 = inet_ntoa(antelope1.sin_addr); // return IP back to string
		printf("input subnet_mask:%s\n", some_addr1);
		is_valid_ip(some_addr1) ? printf("Valid input\n") : printf("Input is Not valid!\n");//check
		//printf("input subnet_mask:%c, %c, %c\n", some_addr1[0], some_addr1[1], some_addr1[2]);//testing

		//uint32_t ip8 = IPToUInt("192.168.1.2");//testing
		uint32_t ip9=IPToUInt(gateway);
		uint32_t mask9 = IPToUInt(subnet_mask);
		uint32_t net_lower = (ip9 & mask9);
		uint32_t net_upper = (net_lower | (~mask9));
		/////printf("low:%x, high:%x\n",net_lower,net_upper); //display lower and upper address 
		//printf("low+1:%x, high-1:%x\n", net_lower+1, net_upper-1);//testing
		//
		/*
		if (ip8>=net_lower && ip8<=net_upper){//testing
			printf("True!\n"); 
		}
		else
		{
			printf("False!\n");
		}
		*/
		//
		uint32_t ipp[5000];
		int j9;
		int total=net_upper-net_lower;//for malloc

		int j, j1, i1;						
                //zero out the structure
                //memset((int *)&ptr, 0, sizeof(ptr));//??

		int *ptr=calloc(total*3,sizeof(int));
		/////printf("total:%d\n",total);//display total range
		for (j9 = 1; j9<=net_upper - net_lower; j9++){
			ipp[j9] = net_lower + j9;
			//printf("%d: %x\t",j9,ipp[j9]);//correct testing
			ptr[j9 - 1] = ipp[j9];
			//print_ip(ipp[j9]);//print ip address	
			//printf("%s",ptr1);
			//printf("\n mon:%s\n",mo);			
			//printf("%d:%d\t", j9, (int)ipp[j9]);
		}
		//////////////// set gateway taken
		//printf("ip9: %x\n",ip9);//testing
		int i6;
		for (i6 = 0; i6 < total; i6++){
			if (ip9 >= ptr[i6]){// IP address equal orless than gateway can't use
				//printf("ptr[i6]: %x\n", ptr[i6]);//testing
				ptr[total*1+i6]=1;
				ptr[total*2+i6]=1;
			}
		}

		/* show all malloc in the beginning
                for (i1 = 0; i1 < 3; i1++) {
									if (i1 == 0){ printf("\nIP address:\n"); }
									else if(i1==1){printf("offer: 0 is able, 1 is unalbe\n");}
									else if(i1==2){printf("client took: 0 is not took, 1 is took\n");}

                        for (j1 = 0; j1 < total; j1++) {
                               printf("%x\t", *(ptr + total * i1 + j1));
			}
                        putchar('\n');
                        putchar('\n');
                }
								*/

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
		si_me.sin_port = htons(portnumber);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    //keep listening for data
		int num=0,num1;
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
				//try to receive some data, this is a blocking call
				//printf("a life: %d\n",a.lifetime) ;
				if ((recv_len = recvfrom(s, &b, sizeof(struct udp), 0, (struct sockaddr *) &si_other, &slen)) == -1)
				//if ((recv_len = recvfrom(s, &b, sizeof(b), 0, (struct sockaddr *) &si_other, &slen)) == -1)
				{
					die("recvfrom()");
				}
				//printf("b life: %d\n", b.lifetime);
        //print details of the client/peer and the data received
        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        //printf("Data: %s\n" , buf);
				//print details of the client/peer and the data received
				//printf("\nReceived packet from %s: %d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));//original
				//printf("\n client IP address:%s\n", inet_ntoa(si_other.sin_addr));
				printf("\n client IP address: %s\n transaction ID: %d\n lifetime: %d\n", b.ip, b.id, b.lifetime);
				///*  correct need change
				if (strcmp(b.ip, "0.0.0.0")==0){//check is client has ip or not
					//client do not has ip address from DHCP server
					printf("\n--client do not have ip address from DHCP server!--\n");//testing
					///////////////
					a.ip1=setip1(total,a,ptr);
					a.ip2=setip2(total, a, ptr);
					a.ip3=setip3(total, a, ptr);			
					//printf("DHCP send ip1: %x\n", a.ip1);
					printf("DHCP send ip1: %s\n", print_ip(a.ip1));
					printf("DHCP send ip2: %s\n", print_ip(a.ip2));
					printf("DHCP send ip3: %s\n", print_ip(a.ip3));
					
				}
				else{//client already has IP address from DHCP server					
					printf("--client choice ip:%s from DHCP server--\n", b.ip);
					int i8;
					for (i8 = 0; i8 < total; i8++){//set choiced address not able to be choice anymore
						if (IPToUInt(b.ip) == ptr[i8]){// IP address equal orless than gateway can't use
							//printf("ptr[i8]: %x\n", ptr[i8]);//testing
							ptr[total * 1 + i8] = 1;
							ptr[total * 2 + i8] = 1;
						}

					}
					for (i8 = 0; i8 < total; i8++){//free other two addresses not be choose 
					  if (ptr[total * 2 + i8] == 0){
						  ptr[total * 1 + i8] = 0;
					  }
				  }
					strcpy(a.ip, b.ip);//send back to client //DHCP ACK
				}
				a.id = b.id;
				//*/
				//now reply the client with the same data
				///* correct send
				if (sendto(s, &a, sizeof(a), 0, (struct sockaddr*) &si_other, slen) == -1)
				{
					die("sendto()");
				}
				//data behind
				/*
				printf("-------------all malloc data:-----------\n");
				for (i1 = 0; i1 < 3; i1++) {
					if (i1 == 0){ printf("IP address: store by integer\n"); }
					else if (i1 == 1){ printf("offer: 0 is able, 1 is unalbe\n"); }
					else if (i1 == 2){ printf("client took: 0 is not took, 1 is took\n"); }

					for (j1 = 0; j1 < total; j1++) {
						printf("%x\t", *(ptr + total * i1 + j1));
					}
					putchar('\n');
					putchar('\n');
				}
				*/
				///////////
				//char * sp;
				//sp=print_ip(ptr[0]);//testing
				//printf("%s", sp); // testing
        //now reply the client with the same data
        //if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        //{
        //    die("sendto()");
        //}
    } 
    close(s);
    return 0;
}
