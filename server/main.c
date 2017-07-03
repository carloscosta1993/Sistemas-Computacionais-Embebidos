/* External Libraries */
#include <cyg/io/io.h>
#include <cyg/kernel/kapi.h>
#include <cyg/hal/drv_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Internal Libraries */
#include "data_structures/time.h"
#include "data_structures/local_memory.h"
#include "data_structures/intertask.h"
#include "tasks/task_ui.h"
#include "tasks/task_pt.h"
#include "tasks/task_ct.h"

#define PRIOR_UI 10
#define PRIOR_PT_SYNC 3
#define PRIOR_PT_ASSYNC 3
#define PRIOR_CT_READ 1
#define PRIOR_CT_WRITE 2

#define DEBUG 0

//////////////////////// GLOBAL VARIABLES ////////////////////////
 
#define STACKSIZE 4096
char stack[5][STACKSIZE];

////// SHARED LOCAL MEMORY //////
RingRegBuffer* RDAT; // Data Registers
RingRegBuffer* RALA; // Alarm Registers
int dataTransfPeriod; // Data Transfer Period
int io_error=0; 

////// THREADS //////
cyg_thread ui_T, pt_sync_T;
cyg_thread ct_write_T, ct_read_T, pt_assync_T;

cyg_handle_t ui_H, pt_sync_H;
cyg_handle_t ct_write_H, ct_read_H, pt_assync_H;

////// MESSAGE QUEUES //////
cyg_handle_t mbox_PT_H, mbox_CT_H, mbox_UI_H;
cyg_mbox mbox_PT_M, mbox_CT_M, mbox_UI_M;

////// EVENT FLAGS //////
cyg_flag_t regtransf_F;
cyg_flag_value_t regtransf_F_V;

////// IO //////
cyg_io_handle_t io_handle;
Cyg_ErrNo err;

////// MUTEX //////
cyg_mutex_t myPrintf;
cyg_mutex_t shared_memory_1, shared_memory_2;

////// ALARM //////
cyg_handle_t regtransf_C_H, regtransf_A_H, system_clock_H;
cyg_alarm regtransf_A;

//////////////////////// ALARM HANDLER ////////////////////////
void regtransf(cyg_handle_t regtransf_A_H, cyg_addrword_t data){	
	cyg_flag_setbits(&regtransf_F,0x01);
}

//////////////////////// TASK - USER INTERFACE ////////////////////////
void thread_UI(){

	Request* request=NULL;
	char* str;
	int task;

	cyg_mutex_lock(&myPrintf);
	init_interface_task_ui();
	printf("\nEnter Command: ");
	cyg_mutex_unlock(&myPrintf);

	while(1){
		////// (BLOCK UNTIL) GET USER INPUT //////
		request = get_cmd_task_ui();

		if(request!=NULL){
			task = get_request_destination_task_ui(request);
			////// PUT MESSAGE IN QUEUE //////
			switch(task){
				case(THREAD_PT):
					cyg_mbox_put(mbox_PT_H,request); 
					break;
				case(THREAD_CT):
					cyg_mbox_put(mbox_CT_H,request); 
					break;
				default:
					assert(1==0);
					break;}

			////// GET MESSAGE FROM QUEUE //////
			if(DEBUG){printf("\n<< WAIT FOR CMD >>\n");}			
			str = (char*) cyg_mbox_timed_get(mbox_UI_H,cyg_current_time()+5*100); // change for timeout adn build different message
		}
		/// PRINT SCREEN ///
		cyg_mutex_lock(&myPrintf);
		if(request!=NULL){
			if(str==NULL){	
				printf(" << Request = TIMEOUT >>\n ");}
			else{
				printf("%s",str);
				free(str);}}
		else{
			printf("<< Request = INVALID >> | -u to see usage \n");}
		printf("Enter Command: ");
		cyg_mutex_unlock(&myPrintf);

	}
}

//////////////////////// TASK - SYNCRONOUS PROCESSING TASK ////////////////////////
void thread_PT_sync(){

	Request* request;
	char* str;
	int oper;

	while(1){
		////// GET MESSAGE FROM QUEUE //////
		request = (Request*) cyg_mbox_get(mbox_PT_H);

		////// TASK ACTION //////
		/// LOCK SHARED MEMORY ///
		oper = get_oper_task_pt(request);
		if((IRL<=oper && oper<=DRA) || PR==oper){
			cyg_mutex_lock(&shared_memory_1);}
		else if(CPT<=oper && oper<=MPT){
			cyg_mutex_lock(&shared_memory_2);}
		else{assert(1==0);}
		switch(oper){
			case(IRL):
				str = irl(RDAT,RALA); break;
			case(LRD):
				str = lrd(RDAT,request->data); break;
			case(LRA):
				str = lra(RALA,request->data); break;
			case(DRD):
				str = drd(RDAT); break;
			case(DRA):
				str = dra(RALA); break;
			case(CPT):
				str = cpt(dataTransfPeriod); break;
			case(MPT):
				str = mpt(&dataTransfPeriod,request->data);
				if(dataTransfPeriod>0){
					cyg_alarm_enable(regtransf_A_H);
					cyg_alarm_initialize(regtransf_A_H,cyg_current_time()+dataTransfPeriod*100,dataTransfPeriod*100);} 
				else{
					cyg_alarm_disable(regtransf_A_H);}
				break;
			case(PR):
				str = pr(RDAT,request->data); break;
			default: 
				break;
		}
		/// UNLOCK SHARED MEMORY ///
		if((IRL<=oper && oper<=DRA) || PR==oper){
			cyg_mutex_unlock(&shared_memory_1);}
		else if(CPT<=oper && oper<=MPT){
			cyg_mutex_unlock(&shared_memory_2);}
		else{assert(1==0);}

		////// PUT MESSAGE IN QUEUE //////
		cyg_mbox_put(mbox_UI_H,str);
	}
}

//////////////////////// TASK - ASSYNCRONOUS PROCESSING TASK ////////////////////////
void thread_PT_assync(){

	Request* request;

	if(dataTransfPeriod>0){
		cyg_alarm_initialize(regtransf_A_H,cyg_current_time()+dataTransfPeriod*100,dataTransfPeriod*100);}

	while(1){
		////// WAIT FOR ALARM EVENT //////
		regtransf_F_V = cyg_flag_wait(&regtransf_F,0x01,CYG_FLAG_WAITMODE_AND | CYG_FLAG_WAITMODE_CLR);
		////// PUT MESSAGE IN QUEUE //////
		if(DEBUG){printf("\n<< ASSYNCRONOUS PROCESSING TASK REQUEST >>\n");}
		request=new_Request("trtl",NULL);
		cyg_mbox_put(mbox_CT_H,request);
	}
}

//////////////////////// TASK - COMMUNICATION TASK WRITE ////////////////////////
void thread_CT_write(){

	Request* request;
	char *request_message;
	cyg_uint32 request_len;
	int write_oper;

	while(1){
		////// (BLOCK UNTIL) GET MESSAGE FROM QUEUE //////
		request = (Request*) cyg_mbox_get(mbox_CT_H);

		////// TASK ACTION //////
		write_oper = get_oper_task_ct(request);
		
		cyg_mutex_lock(&myPrintf);
		switch(write_oper){
			case(SCLK):
				sc(request->data); break;
			case(MPMN):
				mpm(request->data); break;
			case(MTIN):
				mti(request->data); break;
			case(DATL):
				dtl(request->data); break;
			case(AALA):
				aa(request->data); break;
			default: 
				break;
			}
		cyg_mutex_unlock(&myPrintf);

		request_message = request_message_task_ct(write_oper,request->data);
		free_Request(request);
		request_len=strlen(request_message)+1;

		////// IO WRITE //////
		err=cyg_io_write(io_handle,request_message,&request_len);
		if(DEBUG){printf("<< MESSAGE SENT >>\n -- CONTENT:%s LENGTH:%d >>\n",request_message,request_len);}
		if(ENOERR!=err){io_error=1;}
		free(request_message);
	}
}

//////////////////////// TASK - COMMUNICATION TASK READ ////////////////////////
void thread_CT_read(){

	char reply_char, buff[6];
	cyg_uint32 reply_len=1;
	int *io_data;

	char *str;
	int assync=0, read_oper, j;

	int i, start, start_msg, code, end;

	while(1){
		////// IO READ //////
		io_data = (int*) malloc(20*sizeof(int));
		/// INITIALIZE READ VARIABLES ///
	read_oper=-1;i=-1;start=-1;start_msg=-1;code=-1;end=-1;
		while(1){
			j=0;
			memset(&buff[0],0,sizeof(buff));
			err=cyg_io_read(io_handle,&reply_char,&reply_len);
			if(ENOERR!=err){assert(1==0);}
			while(reply_char!=' '&&reply_char!='\0'){
				buff[j++]=reply_char;
				err=cyg_io_read(io_handle,&reply_char,&reply_len);
				if(ENOERR!=err){assert(1==0);}
			}
			//if(DEBUG){printf("<< MESSAGE RECEIVED >>\n -- CONTENT:%s >>\n",buff);}
			reply_message_task_ct(buff,&read_oper,&i,&start,&start_msg,&code,&end,io_data);			
			if(reply_char=='\0'){break;}
		}
		
		////// TASK ACTION //////
		/// LOCK SHARED MEMORY ///
		if(read_oper==NALA || read_oper==TRTL){
			cyg_mutex_lock(&shared_memory_1);}
		if(read_oper==NALA){
			cyg_mutex_lock(&myPrintf);}

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
			case(TRTL):
				assync=1; tr(io_data,RDAT); break;
			case(TRMM):
				str = rmm(io_data); break;	
			case(NALA):
				assync=1; naa(io_data,RALA); fflush(stdout); break;
			default: 
				break;}

		////// PUT MESSAGE IN QUEUE //////
		if(assync==0){
			if(io_error==0){
				cyg_mbox_put(mbox_UI_H,str);}
			else{assert(1==0);}
		}else{
			if(read_oper==NALA){printf("Enter Command: ");}
			assync=0;		
		}
		
		/// UNLOCK SHARED MEMORY ///
		free(io_data);
		if(read_oper==NALA || read_oper==TRTL){
			cyg_mutex_unlock(&shared_memory_1);}
		if(read_oper==NALA){
			cyg_mutex_unlock(&myPrintf);}
	}
}

//////////////////////// INITIALIZATION ////////////////////////
void init(){

	////// (A) CREATE MESSAGE QUEUES //////
	cyg_mbox_create(&mbox_UI_H,&mbox_UI_M);
	cyg_mbox_create(&mbox_PT_H,&mbox_PT_M);
	cyg_mbox_create(&mbox_CT_H,&mbox_CT_M);

	////// (B) CREATE MUTEX //////
	cyg_mutex_init(&myPrintf); 
	cyg_mutex_init(&shared_memory_1);
	cyg_mutex_init(&shared_memory_2);

	////// (C) CREATE THREADS //////
	cyg_thread_create(PRIOR_UI,thread_UI,0,"thread_UI", 
		     		 (void*) stack[0],STACKSIZE,
		     		 &ui_H, &ui_T);
	cyg_thread_create(PRIOR_PT_SYNC,thread_PT_sync,0,"thread_PT_sync", 
		     		 (void*) stack[1],STACKSIZE,
		     		 &pt_sync_H, &pt_sync_T);
	cyg_thread_create(PRIOR_PT_ASSYNC,thread_PT_assync,0,"thread_PT_assync", 
			 		 (void*) stack[2],STACKSIZE,
			 		 &pt_assync_H, &pt_assync_T);
	cyg_thread_create(PRIOR_CT_READ,thread_CT_write,0,"thread_CT_read", 
		     		 (void*) stack[3],STACKSIZE,
		     		 &ct_write_H, &ct_write_T);
	cyg_thread_create(PRIOR_CT_WRITE,thread_CT_read,0,"thread_CT_write", 
		     		 (void*) stack[4],STACKSIZE,
		     		 &ct_read_H, &ct_read_T);

	////// (D) CREATE ALARM CLOCK //////	
	system_clock_H = cyg_real_time_clock();
	cyg_clock_to_counter(system_clock_H,&regtransf_C_H);
	cyg_alarm_create(regtransf_C_H,regtransf,(cyg_addrword_t) NULL,&regtransf_A_H,&regtransf_A);

	////// (E) EVENT FLAG BITS //////
	cyg_flag_init(&regtransf_F);

	////// (F) IO OPEN //////
	err=cyg_io_lookup("/dev/ser0",&io_handle);
	if(ENOERR!=err){		
		if(DEBUG){printf("++ Error in IO READ ++\n");}}

	////// (G) INITIALIZE SHARED LOCAL MEMORY  //////
	RDAT = new_RingRegBuffer(50);
	RALA = new_RingRegBuffer(50);
	dataTransfPeriod = 0;
}

int main(){

	init();
		
	cyg_thread_resume(ui_H);
	cyg_thread_resume(pt_sync_H);
	cyg_thread_resume(pt_assync_H);
	cyg_thread_resume(ct_write_H);
	cyg_thread_resume(ct_read_H);
	
	return 0;
}
