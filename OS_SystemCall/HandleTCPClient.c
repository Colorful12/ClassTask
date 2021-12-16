#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>    /* for strlen() */
#include <unistd.h> /*for sleep()*/
#include <stdlib.h>
#define RCVBUFSIZE 100   /* Size of receive buffer */

void DieWithError(char *errorMessage); 

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE], taskBuffer[RCVBUFSIZE], timeBuffer[RCVBUFSIZE];  /* Buffer for echo string */
    int recvMsgSize, taskSize, timeSize;   /* Size of received message */
    double time; /* stoppint time */

    char bikkuri[] = "!";
    
    if ((taskSize = recv(clntSocket, taskBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    if (send(clntSocket, bikkuri, strlen(bikkuri), 0) != strlen(bikkuri))
            DieWithError("send() failed");
    if ((timeSize = recv(clntSocket, timeBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
   
    time = strtod(timeBuffer, NULL)*60;
    sleep(time); /* 指定された時間スリープする */

    if (send(clntSocket, taskBuffer, taskSize, 0) != taskSize)
        DieWithError("send() failed");

    close(clntSocket);    /* Close client socket */
}