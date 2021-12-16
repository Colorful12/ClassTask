#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>    /* for strlen() */
#define RCVBUFSIZE 100   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    char taskBuffer[RCVBUFSIZE];
    char timeBuffer[RCVBUFSIZE];
    int recvMsgSize, taskSize, timeSize;                    /* Size of received message */

    char bikkuri[] = "!";
    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    printf("%s\n",echoBuffer);
    if (send(clntSocket, bikkuri, strlen(bikkuri), 0) != strlen(bikkuri))
            DieWithError("send() failed");

    if ((taskSize = recv(clntSocket, taskBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    printf("%s\n", taskBuffer);
    if (send(clntSocket, bikkuri, strlen(bikkuri), 0) != strlen(bikkuri))
            DieWithError("send() failed");
            
    if ((timeSize = recv(clntSocket, timeBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    printf("%s\n",timeBuffer);

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        /* Echo message back to client */
        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");

        /* See if there is more data to receive */
        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithError("recv() failed");
    }

    close(clntSocket);    /* Close client socket */
}