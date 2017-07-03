#ifndef TASK_CT
#define TASK_CT

#define SOM 0xFD
#define EOM 0xFE

#define RCLK 0xC0
#define SCLK 0xC1
#define RRTL 0xC2
#define RPAR 0xC3
#define MPMN 0xC4
#define MTIN 0xC5
#define RALA 0xC6
#define DATL 0xC7
#define AALA 0xC8
#define TRTL 0xC9
#define TRMM 0xCA
#define NALA 0xCB

#define CMD_OK 0
#define CMD_ERROR 0xFF

char* reply_message_serial_com(int oper, int* data);

#endif
