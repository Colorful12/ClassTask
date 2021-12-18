#include <stdio.h>
#include <time.h> /* for time() */

void Time(double addtime){
    time_t timer;
    struct tm *s_tm;

    printf("addtime : %lf\n",addtime);
    /*  現在時刻の取得＆tm構造体の取得 */
    time(&timer);
    s_tm = localtime(&timer);
    
    printf("開始時刻 : %d時 %d分 %d秒\n", s_tm->tm_hour, s_tm->tm_min, s_tm->tm_sec);

    /* 終了時間の計算&tm構造体の取得 */
    timer += addtime;
    s_tm = localtime(&timer);

    printf("終了時刻 : %d時 %d分 %d秒\n", s_tm->tm_hour, s_tm->tm_min, s_tm->tm_sec);
    
    return;
}