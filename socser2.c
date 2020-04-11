#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFLEN 503
#define PORT 8885

void die(char *s)
{
	perror(s);
	exit(1);
}

int main(void)
{
	struct sockaddr_in si_me, si_other;
	
	int s, i,j, slen = sizeof(si_other) , recv_len=0, len=0;
	char buf[BUFLEN];
	
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) 
	{
		die("socket");
	}
	
	memset((char *) &si_me, 0, sizeof(si_me));  //set memory to 0
	
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT); //converts the unsigned short integer hostshort from host byte order to network byte order.
	si_me.sin_addr.s_addr = htonl(INADDR_ANY); //converts the unsigned integer hostlong from host byte order to network byte order. 
	
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)  // bind a name si_me to a socket s
	{
		die("bind");
	}
	//memset(buf,0,503);
	char fname[20];
	FILE *fp;
	
	recv_len = recvfrom(s, buf, 20, 0, (struct sockaddr *) &si_other, &slen); //recieve file name & return the length of the message written to the buffer pointed to by the buf argument.
	
	char fna[100];
	
	strcpy(fna,buf);	//copy file name in fna
	
	memset(buf,0,503);
	recv_len = recvfrom(s, buf, 20, 0, (struct sockaddr *) &si_other, &slen); //recieve file size
	
	len=AF_INET;
	si_other.sin_port = htons(PORT);
	len=sizeof(fna);
	
	for(j=len-1;j>=0;j--) //Append 1 next to file name to create new file.
	{
		if(fna[j]=='.')
		{
			fna[j-1]='1';
		}
	}
	unsigned long mm = atoi(buf);  //converts the string argument str to an integer (type int).
	
	fp=fopen(fna,"wb");  //open file for writing
	int itr=1;
	memset(buf,0,503);
	
	recv_len = recvfrom(s, buf, (mm%503), 0, (struct sockaddr *) &si_other, &slen);
	
	fwrite(buf,(mm%503), 1, fp);  // writes data from the array pointed to, by buf to the given stream fp.
	
	memset(buf,0,503);
	fclose(fp);
	close(s);

	return 0;
}
