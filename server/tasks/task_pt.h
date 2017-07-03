#ifndef TASK_PT
#define TASK_PT

#include "../data_structures/intertask.h"
#include "../data_structures/local_memory.h"

#define IRL 0
#define LRD 1
#define LRA 2
#define DRD 3
#define DRA 4
#define CPT 5
#define MPT 6
#define PR 7

int get_oper_task_pt(Request*);

char* irl(RingRegBuffer*, RingRegBuffer*);
char* lrd(RingRegBuffer*, int*);
char* lra(RingRegBuffer*, int*);
char* drd(RingRegBuffer*);
char* dra(RingRegBuffer*);
char* cpt(int);
char* mpt(int*, int*);
char* pr(RingRegBuffer*,int*);

#endif