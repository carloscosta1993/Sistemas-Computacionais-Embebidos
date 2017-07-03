#ifndef TASK_CT
#define TASK_CT

#include "../data_structures/intertask.h"
#include "../data_structures/local_memory.h"

#define RC 0 
#define SC 1
#define RTL 2
#define RP 3
#define MPM 4
#define MTI 5
#define RA 6
#define DTL 7
#define AA 8
#define RMM 9
#define TR 10
#define NAA 11

#define SOM 0xFD
#define EOM 0xFE

#define RCLK 0xC0
#define SCLK 0xC1
#define RRTL 0xC2
#define RPAR 0xC3
#define MPMN 0xC4
#define MTIN 0xC5
#define RALAR 0xC6
#define DATL 0xC7
#define AALA 0xC8
#define TRTL 0xC9
#define TRMM 0xCA
#define NALA 0xCB

#define CMD_OK 0
#define CMD_ERROR 0xFF

int get_oper_task_ct(Request* request);
char* request_message_task_ct(int, int*);
void reply_message_task_ct(char*, int*, int*, int*, int*, int*, int*, int*);


char* rc(int*);
void sc(int*);
char* rtl(int*);
char* rp(int*);
void mpm(int*);
void mti(int*);
char* ra(int*);
void dtl(int*);
void aa(int*);
char* rmm(int*);
char* tr(int*,RingRegBuffer*);
void naa(int*,RingRegBuffer*);

#endif
