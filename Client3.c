#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
//#include <string.h>

#define SOCKET_SERVER_PATH "/home/fedor/Desktop/osi_task3/socket_server.sock"
#define SOCKET_CLIENT_PATH "/home/fedor/Desktop/osi_task3/socket_client.sock"

int main()
{
	remove(SOCKET_CLIENT_PATH);
	
	struct sockaddr_un client_addr;

	bzero((void*)&client_addr, sizeof(client_addr));

	client_addr.sun_family = AF_UNIX;
	strcpy(client_addr.sun_path, SOCKET_CLIENT_PATH);

	int fd = socket(AF_UNIX,SOCK_DGRAM, 0);
	if (fd == -1)
	{
		perror("\33[1;31m[ERROR] Unable to open socket\033[0m");
		exit(1);
	}

	
	if (bind(fd, (struct sockaddr * )&client_addr, sizeof(client_addr)) != 0)
	{
		perror("\33[1;31m[ERROR] Unable to set socket name\033[0m");
		exit(1);
	}

	char buf[1024];
	recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, (socklen_t *)sizeof(client_addr));
	
	char buf2[1024];
	char cmd[] = "ls -a | cut -c1 | tr \".\" \" \" | wc -w";
	
	FILE *f ;
	if((f = popen(cmd, "r")) == NULL)
	{
		perror("\33[1;31m[ERROR] Unable to open file\033[0m");
		exit(1);
	}
	else
	{
		fgets(buf2, sizeof(buf2), f);
		fclose(f);
	}
//	char cmd2[] = ("ADD=$((");
//	char plus = "+";
//	strcat(cmd2, buf);
//	strcat(cmd2, plus);
//	strcat(cmd2, buf2);

	//int w_dot = (int)&buf2; //files wothout .
	//int dot = all - (all - w_dot); //files with .

	strcpy(client_addr.sun_path, SOCKET_SERVER_PATH);
	sendto(fd, buf2, sizeof(buf2), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
//	printf("%s\n", cmd2);	

	remove(SOCKET_CLIENT_PATH);

	close(fd);

	return(0);

}
