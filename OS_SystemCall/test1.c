#include<stdlib.h>
#include <stdio.h> /* for snpringf()*/


void test1(void){
    int BUFSIZE = 1000;
    char buf[BUFSIZE];
    char first[] = "終了通知";
    char second[] = "課題1の終了時間です";
    snprintf(buf, BUFSIZE, "bash test.sh %s %s", first, second);

    system(buf);
}