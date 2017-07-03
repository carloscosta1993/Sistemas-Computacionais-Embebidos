#ifndef TASK_UI
#define TASK_UI

#include "../data_structures/intertask.h"

#define THREAD_PT 0
#define THREAD_CT 1

void init_interface_task_ui(void);
Request* get_cmd_task_ui(void);
int get_request_destination_task_ui(Request* request);
void clean_stdin();

#endif
