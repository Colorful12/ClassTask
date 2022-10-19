#include<stdlib.h>
#include <stdio.h> /* for snpringf()*/


void popup(char task[]){
    int BUFSIZE = 1000;
    char buf[BUFSIZE];
    char first[] = "終了通知";

    snprintf(buf, BUFSIZE, "bash popup.sh %s %s", first, task);

    system(buf);
}