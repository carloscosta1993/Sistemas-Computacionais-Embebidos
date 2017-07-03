#ifndef LOCAL_MEMORY
#define LOCAL_MEMORY

#include "time.h"

typedef struct Register {tm time; int temp; int lumin;} Register;
typedef struct RingRegBuffer{Register** array; int size; int idxNextRead; int idxNextWrite;} RingRegBuffer;

Register* new_Register(tm,int,int);
tm get_time_Register(Register*);
int get_temp_Register(Register*);
int get_lumin_Register(Register*);
char* toString_Register(Register*);

RingRegBuffer* new_RingRegBuffer(int);
void add_RingRegBuffer(RingRegBuffer*,tm,int,int);
void deleteAll_RingRegBuffer(RingRegBuffer*);
char* info_RingRegBuffer(RingRegBuffer*);
char* toString_RingRegBuffer(RingRegBuffer*,int,int);

#endif