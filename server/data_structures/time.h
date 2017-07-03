#ifndef TIME
#define TIME

typedef struct tm {int hour; int min; int sec;} tm;
tm new_tm(int,int,int);
int isGreaterOrEqual_tm(tm time1, tm time2);

#endif