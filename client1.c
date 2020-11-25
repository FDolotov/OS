#include <stdio.h>
#include <sys/msg.h> // для msgget
#include <string.h> // для strlen

// #include <sys/ipc.h>

#ifndef MSGMAX
#define MSGMAX 1024
#endif

struct msgbuf {
    long mtype; 
    char text[MSGMAX];
};


int main()
{

    int fd = msgget(234, IPC_CREAT | 0666 );
    //message 1
    char cmd[]= "pwd";
    FILE *f;//= popen (cmd, "r");

    if ((f = popen(cmd, "r")) == NULL)
    {
        printf("Unable to open file\n");
        getchar();
        return 0;
    }

    struct msgbuf buf1;
    buf1.mtype = 1;


    fgets (buf1.text, sizeof(buf1.text), f);
    //printf("%s",buf1.text);
    //printf("\n");
    pclose (f);

    if (fd == -1 || msgsnd(fd, &buf1, strlen(buf1.text)+1, IPC_NOWAIT) == -1)
    {
        perror("\033[1;31m[ERROR] First message is not sent\033[0m");
    }
    else
    {   
        printf("\e[38;5;82m[INFO] First message sent!\033[0m\n");        
    }


    //message 2
    char cmd2[]= "grep -rw -m 1 \"define\" | tr \":\" \" \" | awk '{print $1}'| tr \"\n\" \" \"";

    if ((f = popen(cmd2, "r")) == NULL)
    {
        printf("Unable to open file\n");
        getchar();
        return 0;
    }

    struct msgbuf buf2;
    buf2.mtype = 2;


    fgets (buf2.text, sizeof(buf2.text), f);
    //printf("%s",buf2.text);
    //printf("\n");
    pclose (f);


    if (fd == -1 || msgsnd(fd, &buf2, strlen(buf2.text)+1, IPC_NOWAIT) == -1)
    {
        perror("\033[1;31m[ERROR] Second message is not sent\033[0m");
    }
    else
    {   
        printf("\e[38;5;82m[INFO] Second message sent!\033[0m\n");        
    }

    return 0;
}
