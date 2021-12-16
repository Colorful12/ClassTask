#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */
void test1();

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
    unsigned int taskLen;
    unsigned int timebufLen;
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() 
                                        and total bytes read */

    if ((argc < 3) || (argc > 4))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",
               argv[0]);
       exit(1);
    }

    char task[1000];
    double time;
    char timebuf[1000];

    printf("これから行う作業を入力してください。>> ");
    scanf("%s",task);
    printf("測る時間は何分ですか？>> ");
    scanf("%lf",&time);
    snprintf(timebuf, 1000, "%lf", time); /* double型をchar型配列に変換*/

    servIP = argv[1];             /* server IP address  */
    echoString = argv[2];         /* string to echo */

    if (argc == 4)
        echoServPort = atoi(argv[3]); 
    else
        echoServPort = 7;  /*指示がなければ7番ポート */

    /* ソケットの作成 */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");

    echoStringLen = strlen(echoString);          /* Determine input length */
    taskLen = strlen(task);
    timebufLen = strlen(timebuf);

    int aaa;
    char aaaBuffer[RCVBUFSIZE];
    /* 文字列たちをサーバーに送信 */
    /* この辺もう少しきれいにする */
    if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
        DieWithError("send() sent a different number of bytes than expected");
    if ((aaa = recv(sock, aaaBuffer, RCVBUFSIZE - 1, 0)) <= 0){
        DieWithError("send() sent a different number of bytes than expected");
    }else{printf("catch\n");}
    if (send(sock, task, taskLen, 0) != taskLen)
        DieWithError("send() sent a different number of bytes than expected");
    if ((aaa = recv(sock, aaaBuffer, RCVBUFSIZE - 1, 0)) <= 0){
        DieWithError("send() sent a different number of bytes than expected");
    }else{printf("catch\n");}    
    if (send(sock, timebuf, timebufLen, 0) != timebufLen)
        DieWithError("send() sent a different number of bytes than expected");    

    /* Receive the same string back from the server */
    totalBytesRcvd = 0;
    printf("Received: ");                /* Setup to print the echoed string */
    while (totalBytesRcvd < echoStringLen)
    {
        /* Receive up to the buffer size (minus 1 to leave space for
           a null terminator) bytes from the sender */
        if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
        echoBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
        printf("%s", echoBuffer);      /* Print the echo buffer */
        test1();
    }

    printf("\n");    /* Print a final linefeed */

    close(sock);
    exit(0);
}