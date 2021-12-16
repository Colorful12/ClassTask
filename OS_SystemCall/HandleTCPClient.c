#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>    /* for strlen() */
#include <unistd.h> /*for sleep()*/
#include <stdlib.h>
#define RCVBUFSIZE 1000   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    char taskBuffer[RCVBUFSIZE];
    char timeBuffer[RCVBUFSIZE];
    int recvMsgSize, taskSize, timeSize;                    /* Size of received message */
    double time;

    char bikkuri[] = "!";
    
    if ((taskSize = recv(clntSocket, taskBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    printf("%s\n", taskBuffer);
    if (send(clntSocket, bikkuri, strlen(bikkuri), 0) != strlen(bikkuri))
            DieWithError("send() failed");

    if ((timeSize = recv(clntSocket, timeBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    printf("%s\n",timeBuffer);

    time = strtod(timeBuffer, NULL)*60;
    sleep(time); /* 指定された時間スリープする */

    /* Send received string and receive again until end of transmission */
    while (taskSize > 0)      /* zero indicates end of transmission */
    {
        /* Echo message back to client */
        if (send(clntSocket, taskBuffer, taskSize, 0) != taskSize)
            DieWithError("send() failed");

        /* See if there is more data to receive */
        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithError("recv() failed");
    }

    close(clntSocket);    /* Close client socket */
}