#ifndef INTERTASK
#define INTERTASK

typedef struct Request {char *instr; int *data;} Request;

Request* new_Request(char*, int*);
void free_Request(Request*);
void print_Request(Request*, int);

#endif