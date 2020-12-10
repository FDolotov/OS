#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/un.h> // for sockaddr_un
#include <unistd.h>

#define SOCKET_SERVER_PATH "/home/fedor/Desktop/osi_task3/socket_server.sock"
#define SOCKET_CLIENT_PATH "/home/fedor/Desktop/osi_task3/socket_client.sock"

int main()
{
	remove(SOCKET_SERVER_PATH);
	
	struct sockaddr_un addr;
	bzero((void*)&addr, sizeof(addr));

	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SOCKET_SERVER_PATH);

	int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (fd == -1)
	{
		perror("\33[1;31m[ERROR] Unable to open socket\033[0m");
		exit(1);
	}

	int name = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (name != 0)
	{
		perror("\33[1;31m[ERROR] Unable to set socket name\033[0m");
		exit(1);
	}


	char buffer[1024];
	char cmd [] = "ls -a | wc -l";

	FILE *f;
	if ((f = popen(cmd, "r")) == NULL)
	{
		perror("\33[1;31m[ERROR] Unable to open file\033[0m");
		return 0;
	}
	else
	{
		fgets(buffer, sizeof(buffer), f);
		fclose(f);
	}

	strcpy(addr.sun_path, SOCKET_CLIENT_PATH);
	sendto(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, sizeof(addr));

	char result[1024];
	recvfrom(fd, result, sizeof(result), 0, (struct sockaddr *)&addr, (socklen_t *)sizeof(addr));

	printf("%s", result);

	remove(SOCKET_SERVER_PATH);

	close(fd);

	return(0);

}
