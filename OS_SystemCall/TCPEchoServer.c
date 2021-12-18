#include "TCPEchoServer.h"  /* TCP echo server includes */
#include <sys/wait.h>       /* for waitpid() */


int main(int argc, char *argv[])
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    unsigned short echoServPort;     /* Server port */
    pid_t processID;                 /* Process ID from fork() */
    unsigned int childProcCount = 0; /* Number of child processes */ 

    if (argc != 2)
    {
        fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg:  local port */

    servSock = CreateTCPServerSocket(echoServPort);

    for (;;) 
    {
        clntSock = AcceptTCPConnection(servSock);
        
        if ((processID = fork()) < 0)
            DieWithError("fork() failed");
        else if (processID == 0)  /* 子プロセス */
        {
            close(servSock);  
            HandleTCPClient(clntSock);

            exit(0);           
        }

        childProcCount++; 

        while (childProcCount) /* ゾンビのお掃除 */
        {
            processID = waitpid((pid_t) -1, NULL, WNOHANG);  /* ノンブロッキング */
            if (processID < 0)  
                DieWithError("waitpid() failed");
            else if (processID == 0) 
                break;
            else
                childProcCount--; 
        }
    }
    
}