#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

#ifndef MSGMAX
#define MSGMAX 2048
#endif

struct mbuf
{
    long mtype; 
    char text[MSGMAX];
};

int main() 
{
    printf("\e[38;5;82m[INFO] Server started\033[0m\n");
    int fd = msgget(234, 0);
    if (fd == -1)
        perror("\033[1;31m[ERROR] Message querry is unavailable\033[0m");

    struct mbuf buf;
    struct msqid_ds ps;
    struct msqid_ds bytes;

    if (msgrcv(fd, &buf, MSGMAX, 1, IPC_NOWAIT) == -1)
        perror("\033[1;31m[ERROR] First message is not recieved\033[0m");
    else
        printf("First message:\n%s", buf.text);

    //message 2
    char one[MSGMAX]; 
    struct mbuf buf2;

    char cmd[] = "ls -rl "; 
    char cmd2[] = "| awk '{print $6$7$8\" \"$9}' | sort | awk '{print $2}' | tr \"\n\" \" \"";

    if (msgrcv(fd, &buf2, MSGMAX, 2, IPC_NOWAIT) == -1)
    {
        perror("\033[1;31m[ERROR] Second message is not recieved\033[0m");
    }
    else
    {
        printf("Second message:\n");
        //char *kek;
        //kek = buf2.text;
        strcat(buf2.text, cmd2);
        strcat(cmd, buf2.text);
        FILE *f= popen(cmd, "r");
        fgets(one, MSGMAX, f);
        printf("%s\n",one); 
    }

    if(msgctl(fd, IPC_STAT, &ps)==0)
    {
        printf("ID of the last process in msgsnd(): %d\n", ps.msg_lspid);
    } 

    if(msgctl(fd, IPC_STAT, &bytes)==0)
    {
        printf("Max length of message querry in bytes: %ld\n", bytes.msg_qbytes);
    }
// ps | tail -1 | awk '{print $1}'

    if (msgctl(fd, IPC_RMID, NULL) == -1)
        perror("\033[1;31m[ERROR] Meaasge querry is not deleted\033[0m");

return 0;
}
