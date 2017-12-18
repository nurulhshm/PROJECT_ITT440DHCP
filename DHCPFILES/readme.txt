/*
* Shih-Chieh Lin
* CSCE 5580 pogramming assignment 4
* 4/24/2017
* A server (cse01.cse.unt.edu)in the internet using UDP domain, and the port number is get from argument number 1
* demonstrate working of DHCP server using a client-server architecture.
* DHCP server offer three IP address to client, and client randomly choosing an IP address, 
* client randomly generated transaction ID  from 0 to 99 in the beginning, after choose an IP address, transaction ID add 1  
* Reference:
* http://beej-zhtw.netdpi.net/09-man-manual/9-13-inet_ntoa-inet_aton-inet_addr
* http://stackoverflow.com/questions/26941426/how-to-check-if-string-contains-particular-character
* http://www.geeksforgeeks.org/program-to-validate-an-ip-address/
* http://stackoverflow.com/questions/1680365/integer-to-ip-address-c
* http://stackoverflow.com/questions/33603139/create-random-numbers-using-the-current-time-of-the-machine-any-idea
* http://stackoverflow.com/questions/12380758/c-error-function-returns-address-of-local-variable
*/

How to run:

gcc server.c
./a.out 1234
input gateway:192.168.1.1
subnet_mask: 255.255.255.250
(both of them will show input address is valid or not)
Waiting for data...


gcc client.c
./a.out 1234

result:
DHCP server display:
 client IP address: 0.0.0.0
 transaction ID: 77
 lifetime: 0

--client do not have ip address from DHCP server!--
DHCP send ip1: 192.168.1.2
DHCP send ip2: 192.168.1.3
DHCP send ip3: 192.168.1.4
Waiting for data...
 client IP address: 192.168.1.3
 transaction ID: 78
 lifetime: 3600
--client choice ip:192.168.1.3 from DHCP server--


client display:
client IP address: 0.0.0.0
client transaction ID: 77
client lifetime: 0
DHCP server give ip:
192.168.1.2,
192.168.1.3,
192.168.1.4
client choice IP address:192.168.1.3

----------------
Enter message :
client IP address: 192.168.1.3
client transaction ID: 78
client lifetime: 3600
DHCP server give ip:
192.168.1.2,
192.168.1.3,
192.168.1.4
client choice IP address:192.168.1.3
Waiting for data...

you can open different client or use the same client and enter

gcc client.c
./a.out 1234

another client display:
client IP address: 0.0.0.0
client transaction ID: 19
client lifetime: 0
DHCP server give ip:
192.168.1.2,
192.168.1.4,
192.168.1.5
client choice IP address:192.168.1.5

----------------
Enter message :
client IP address: 192.168.1.5
client transaction ID: 20
client lifetime: 3600
DHCP server give ip:
192.168.1.2,
192.168.1.4,
192.168.1.5
client choice IP address:192.168.1.5


they will get different IP address from the DHCP server.

Some bug:
If the available of IP address is less than three, there will have some duplicate address DHCP server offer happened.  