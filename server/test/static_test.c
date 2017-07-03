/* External Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Internal Libraries */
#include "../data_structures/local_memory.h"
#include "../data_structures/intertask.h"
#include "../tasks/task_ui.h"
#include "../tasks/task_pt.h"
#include "../tasks/task_ct.h"

//////////////////////// GLOBAL VARIABLES ////////////////////////
RingRegBuffer* RDAT; // Data Registers
RingRegBuffer* RALA; // Alarm Registers
int dataTransfPeriod; // Data Transfer Period

void init(){
	RDAT = new_RingRegBuffer(50);
	RALA = new_RingRegBuffer(50);
	dataTransfPeriod = 0;
}

int main(){
	
	char* str; 

	Request* request;
	int task,oper,write_oper,read_oper;
	char *io_request_message;
	char *io_reply_message = (char*) malloc(128*sizeof(char));
	int* io_data = (int*) malloc(5*sizeof(char));

	int i, assync=0;

	for(i=0;i<5;i++){io_data[i]=-1;}

	tm time; 
	int temp, lumin;

	init();

	///// FILL DATA BUFFER /////
	time=new_tm(16,34,0); temp=30; lumin=1;
	add_RingRegBuffer(RDAT,time,temp,lumin);
	add_RingRegBuffer(RALA,time,temp,lumin);
	time=new_tm(16,34,1); temp=32; lumin=0;
	add_RingRegBuffer(RDAT,time,temp,lumin);	
	add_RingRegBuffer(RALA,time,temp,lumin);
	time=new_tm(16,34,2); temp=33; lumin=2;
	add_RingRegBuffer(RDAT,time,temp,lumin);
	add_RingRegBuffer(RALA,time,temp,lumin);	
	time=new_tm(16,34,3); temp=29; lumin=3;
	add_RingRegBuffer(RDAT,time,temp,lumin);
	add_RingRegBuffer(RALA,time,temp,lumin);	
	time=new_tm(16,34,4); temp=31; lumin=5;
	add_RingRegBuffer(RDAT,time,temp,lumin);
	add_RingRegBuffer(RALA,time,temp,lumin);

	init_interface_task_ui();

	while(1){
		request=get_cmd_task_ui();
		if(request!=NULL){
			task=get_request_destination_task_ui(request);

			switch(task){
				case(THREAD_PT):
					oper = get_oper_task_pt(request);
					switch(oper){
						case(IRL):
							str = irl(RDAT,RALA);
							break;
						case(LRD):
							str = lrd(RDAT,request->data); 
							break;
						case(LRA):
							str = lra(RALA,request->data); 
							break;
						case(DRD):
							str = drd(RDAT); 
							break;
						case(DRA):
							str = dra(RALA);
							break;
						case(CPT):
							str = cpt(dataTransfPeriod);
							break;
						case(MPT):
							str = mpt(&dataTransfPeriod,request->data);
							break;
						case(PR):
							str = pr(RDAT,request->data);
							break;
						}
					printf("%s",str);
					free(str);
					break;
				case(THREAD_CT):

					write_oper = get_oper_task_ct(request);
					io_request_message = request_message_task_ct(write_oper, request->data);
					free(io_request_message);

					sprintf(io_reply_message,"%d %d %d %d %d %d",SOM,RCLK,2,2,2,EOM);
					read_oper = reply_message_task_ct(io_reply_message,io_data);

					switch(read_oper){
						case(SCLK):
						case(MPMN):
						case(MTIN):
						case(DATL):
						case(AALA):
							str = "-- [ SUCCESSFULL OPERATION ] -- \n"; break;
						case(RCLK):
							str = rc(io_data); break;
						case(RRTL):
							str = rtl(io_data); break;
						case(RPAR):
							str = rp(io_data); break;
						case(RALAR):
							str = ra(io_data); break;
						case(TRMM):
							str = rmm(io_data); break;
						case(TRTL):
							assync=1; tr(io_data,RDAT); break;	
						case(NALA):
							assync=1; naa(io_data,RALA); break;
						default: 
							break;}

					printf("%s",str);
					free(io_reply_message);
					free(str);
					break;
				default:
					break;}}
	}

	return 0;
}