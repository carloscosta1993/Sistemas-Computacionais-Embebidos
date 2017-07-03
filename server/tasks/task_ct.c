/* External Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Internal Libraries */
#include "../data_structures/intertask.h"
#include "../data_structures/local_memory.h"
#include "../data_structures/time.h"
#include "task_ct.h"

#define DEBUG 1

int get_oper_task_ct(Request* request){
	if(strcmp("rc",request->instr)==0){return RC;}
	else if(strcmp("sc",request->instr)==0){return SC;}
	else if(strcmp("rtl",request->instr)==0){return RTL;}
	else if(strcmp("rp",request->instr)==0){return RP;}
	else if(strcmp("mpm",request->instr)==0){return MPM;}
	else if(strcmp("mti",request->instr)==0){return MTI;}
	else if(strcmp("ra",request->instr)==0){return RA;}
	else if(strcmp("dtl",request->instr)==0){return DTL;}
	else if(strcmp("aa",request->instr)==0){return AA;}
	else if(strcmp("rmm",request->instr)==0){return RMM;}
	else if(strcmp("trtl",request->instr)==0){return TR;}
	else {assert(1==0);return -1;}
}

char* request_message_task_ct(int oper, int* data){

	char *str=(char*) malloc(124*sizeof(char));

	switch(oper){
		case(RC):
			sprintf(str,"%d %d %d",SOM,RCLK,EOM); break;
		case(SC):
			sprintf(str,"%d %d %d %d %d %d",SOM,SCLK,data[0],data[1],data[2],EOM); break;
		case(RTL):
			sprintf(str,"%d %d %d",SOM,RRTL,EOM); break;
		case(RP):
			sprintf(str,"%d %d %d",SOM,RPAR,EOM); break;
		case(MPM):
			sprintf(str,"%d %d %d %d",SOM,MPMN,data[0],EOM); break;
		case(MTI):
			sprintf(str,"%d %d %d %d",SOM,MTIN,data[0],EOM); break;
		case(RA):
			sprintf(str,"%d %d %d",SOM,RALAR,EOM); break;
		case(DTL):
			sprintf(str,"%d %d %d %d %d",SOM,DATL,data[0],data[1],EOM); break;
		case(AA):
			sprintf(str,"%d %d %d %d",SOM,AALA,data[0],EOM); break;
		case(RMM):
			sprintf(str,"%d %d %d",SOM,TRMM,EOM); break;
		case(TR):
			sprintf(str,"%d %d %d",SOM,TRTL,EOM); break;
		default: 
			break;
	}
	return str;
}

void reply_message_task_ct(char* str, int* msg,int* i, int* start, int* start_msg, int* code, int* end, int* data){
	
	int oper;

	if((*i)==-1){(*i)=0;}
	if((*start)==-1){(*start)=1;}
	if((*start_msg)==-1){(*start_msg)=0;}
	if((*code)==-1){(*code)=0;}
	if((*end)==-1){(*end)=0;}

	oper = atoi(str);
	if((*start)==1){
 		/// CHECK <SOM> ///
 		assert(oper==SOM);
 		(*start)=0;}
 	else if((*start)==0 && (*end)==0 && (*code)==0){
 		 /// CHECK <MSG> + <DATA> ///
 		if((*msg)==-1){(*msg)=oper;}
 		assert(oper!=EOM);
 		switch((*msg)){
 			case(SCLK):
 			case(MPMN):
 			case(MTIN):
 			case(DATL):
 			case(AALA):
 				(*code)=1; break;
 			case(RRTL):
 				if((*start_msg)==0){
 					(*start_msg)=1;}
 				else{
 					data[(*i)++]=oper;}
 				if((*i)==2){(*end)=1;};
 				break;
 			case(RCLK):
 			case(RPAR):
 			case(RALAR):
 				if((*start_msg)==0){
 					(*start_msg)=1;}
 				else{
 					data[(*i)++]=oper;}
 				if((*i)==3){(*end)=1;};
 				break;
 			case(TRTL):
 			case(NALA):		
 				if((*start_msg)==0){
 					(*start_msg)=1;}
 				else{
 					data[(*i)++]=oper;}
 				if((*i)==5){(*end)=1;};
 				break;
 			 case(TRMM):	
				if((*start_msg)==0){
 					(*start_msg)=1;}
 				else{
 					data[(*i)++]=oper;}
 				if((*i)==20){(*end)=1;};
 				break;
			default: 
				assert(1==0);
				break;}}
	else if((*start)==0 && (*end)==0 && (*code)==1){
		assert(oper==CMD_OK);
		(*end)=1;}
 	else if((*start)==0 && (*end)==1){
 		/// CHECK <EOM> ///
 		assert(oper==EOM);}

}
 
char* rc(int* data){
	char* str = (char*) malloc(128*sizeof(char));
	sprintf(str,"<< READ CLOCK >> \n-- [ TIME: %02d:%02d:%02d ] --\n",data[0],data[1],data[2]);
	return str;
}

void sc(int* data){
	printf("<< SET CLOCK >> \n-- [ TIME: %02d:%02d:%02d ] --\n",data[0],data[1],data[2]);

}

char* rtl(int* data){
	char* str = (char*) malloc(128*sizeof(char));
	sprintf(str,"<< READ TEMPERATURE AND LUMINOSITY >> \n-- [ TEMP.: %d, LUMIN.: %d ] --\n",data[0],data[1]);
	return str;
}

char* rp(int* data){
	char* str = (char*) malloc(128*sizeof(char));
	sprintf(str,"<< READ PMON, TSOM AND TINA >> \n-- [ PMON: %d sec, TSOM: %d sec, TINA: %d sec] --\n",data[0],data[1],data[2]);
	return str;
}

void mpm(int* data){
	printf("<< SET PMON >> \n-- [ PMON: %02d ] --\n",data[0]);
}

void mti(int* data){
	printf("<< SET TINA >> \n-- [ TINA: %02d ] --\n",data[0]);
}

char* ra(int* data){
	char* str = (char*) malloc(128*sizeof(char));
	sprintf(str,"<< READ ALARM SETTING : ALAT, ALAL, ALAF >> \n-- [ ALAT: %d, ALAL: %d, ALAF: %d ] --\n",data[0],data[1],data[2]);
	return str;
}

void dtl(int* data){
	printf("<< SET ALARM THRESHOLD FOR TEMPERATURE AND LUMINOSITY >> \n-- [ ALAT: %d, ALAL: %d ] --\n",data[0],data[1]);
}

void aa(int* data){
	printf("<< ACTIVATE/DESACTIVATE ALARM >> \n-- [ ALAF: %d ] --\n",data[0]);
}

char* rmm(int* data){
	char* str = (char*) malloc(128*sizeof(char));
	sprintf(str,"<< READ REGISTERS WITH MAXIMUM AND MINIMUM TEMPERATURE AND LUMINOSITY >>\n -- [ MAX TEMP - TIME: %02d:%02d:%02d, TEMP.: %d, LUMIN.: %d ] --\n -- [ MIN TEMP - TIME: %02d:%02d:%02d, TEMP.: %d, LUMIN.: %d ] --\n -- [ MAX LUMIN - TIME: %02d:%02d:%02d, TEMP.: %d, LUMIN.: %d ] --\n -- [ MIN LUMIN - TIME: %02d:%02d:%02d, TEMP.: %d, LUMIN.: %d ] --\n" ,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9],data[10],data[11],data[12],data[13],data[14],data[15],data[16],data[17],data[18],data[19]);
	return str;
}

char* tr(int* data, RingRegBuffer* buf){
	char* str = (char*) malloc(128*sizeof(char));
	tm time = new_tm(data[0],data[1],data[2]);
	add_RingRegBuffer(buf,time,data[3],data[4]);
	str = "-- [ SUCCESSFULL OPERATION ] -- \n";
	return str;
}

void naa(int* data, RingRegBuffer* buf){
	tm time = new_tm(data[0],data[1],data[2]);
	add_RingRegBuffer(buf,time,data[3],data[4]);
	printf("\n<< FIRED NEW ALARM >> \n-- [ TIME: %02d:%02d:%02d, TEMP.: %d, LUMIN.: %d ]\n",data[0],data[1],data[2],data[3],data[4]);
}

//////////////////////// FILE DEBUGGER ////////////////////////
// int main(void){

// 	char *str, reply_message[64];
// 	int *io_data;

// 	int i;

// 	int read_oper, assync=0;

// 	RingRegBuffer* RDAT = new_RingRegBuffer(50);
// 	tm time = new_tm(10,33,10);
// 	add_RingRegBuffer(RDAT,time,10,2);
// 	time = new_tm(10,33,10);
// 	add_RingRegBuffer(RDAT,time,11,3);

// 	RingRegBuffer* RALA = new_RingRegBuffer(50);
// 	time = new_tm(11,33,10);
// 	add_RingRegBuffer(RALA,time,10,2);
// 	time = new_tm(11,33,10);
// 	add_RingRegBuffer(RALA,time,11,3);

// 	io_data = (int*) malloc(5*sizeof(int));
// 	for(i=0;i<5;i++){io_data[i]=-1;}
// 	sprintf(reply_message,"%d %d 31 12 5 1 %d",SOM,TRMM,EOM);

// 	read_oper = reply_message_task_ct(reply_message, io_data);

// 	switch(read_oper){
// 		case(SCLK):
// 		case(MPMN):
// 		case(MTIN):
// 		case(DATL):
// 		case(AALA):
// 			str = "-- [ SUCCESSFULL OPERATION ] -- \n"; break;
// 		case(RCLK):
// 			str = rc(io_data); break;
// 		case(RRTL):
// 			str = rtl(io_data); break;
// 		case(RPAR):
// 			str = rp(io_data); break;
// 		case(RALAR):
// 			str = ra(io_data); break;
// 		case(TRMM):
// 			str = rmm(io_data); break;
// 		case(TRTL):
// 			assync=1; tr(io_data,RDAT); break;	
// 		case(NALA):
// 			assync=1; naa(io_data,RALA); break;
// 		default: 
// 			break;
// 	}
// 	if(assync==0){printf("%s",str);}

// 	return 0;
// }


