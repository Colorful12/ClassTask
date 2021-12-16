#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define RCVBUFSIZE 100   /* Size of receive buffer */

void DieWithError(char *errorMessage);
void test1();

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    char task[100];
    double time;
    char timebuf[100];
    unsigned int taskLen;
    unsigned int timebufLen;
    int bytesRcvd;   /*最後にサーバーから受け取る文字列の長さ*/

    if ((argc < 2) || (argc > 3))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",
               argv[0]);
       exit(1);
    }



    printf("何までの時間を測定しますか？>> ");
    scanf("%s",task);
    printf("測る時間は何分ですか？>> ");
    scanf("%lf",&time);
    snprintf(timebuf, 100, "%lf", time); /* double型をchar型配列に変換*/

    servIP = argv[1]; /* server IP address  */

    if (argc == 3)
        echoServPort = atoi(argv[2]); 
    else
        echoServPort = 7;  /*指示がなければ7番ポート */

    /* ソケットの作成 */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* サーバーのアドレス構造体の作成 */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* 0埋め */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* サーバーとの接続の確立 */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");
 
    taskLen = strlen(task);
    timebufLen = strlen(timebuf);

    int check;
    char checkBuffer[RCVBUFSIZE];
    /* 文字列たちをサーバーに送信 */
    /* この辺もう少しきれいにする */
    
    if (send(sock, task, taskLen, 0) != taskLen)
        DieWithError("send() sent a different number of bytes than expected");
    if (( recv(sock, checkBuffer, RCVBUFSIZE - 1, 0)) <= 0){
        DieWithError("recv() failed or connection closed prematurely");
    }else if (strcmp(checkBuffer, "!")!=0)
        DieWithError("client chatched a different word from server. not \"!\"");
    if (send(sock, timebuf, timebufLen, 0) != timebufLen)
        DieWithError("send() sent a different number of bytes than expected");    


    if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    echoBuffer[bytesRcvd] = '\0';  
    test1(echoBuffer);
    printf("\n");    /* Print a final linefeed */

    close(sock);
    exit(0);
}