/* External Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

/* Internal Libraries */
#include "../data_structures/intertask.h"
#include "../data_structures/local_memory.h"
#include "../data_structures/time.h"
#include "task_pt.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct ProcessParams {int count; int acc_temp; int acc_lumin; int max_temp; int max_lumin; int min_temp; int min_lumin;} ProcessParams;

ProcessParams new_ProcessParams(){
	ProcessParams params = {0,0,0,INT_MIN,INT_MIN,INT_MAX,INT_MAX};
	return params;
}

void update_ProcessParams(ProcessParams* params,int temp,int lumin){
	params->count++;
	params->acc_temp+=temp;
	params->acc_lumin+=lumin;
	params->max_temp=MAX(params->max_temp,temp);
	params->min_temp=MIN(params->min_temp,temp);
	params->max_lumin=MAX(params->max_lumin,lumin);
	params->min_lumin=MIN(params->min_lumin,lumin);
}

int get_oper_task_pt(Request* request){
	if(strcmp("irl",request->instr)==0){return IRL;}
	else if(strcmp("lrd",request->instr)==0){return LRD;}
	else if(strcmp("lra",request->instr)==0){return LRA;}
	else if(strcmp("drd",request->instr)==0){return DRD;}
	else if(strcmp("dra",request->instr)==0){return DRA;}
	else if(strcmp("cpt",request->instr)==0){return CPT;}
	else if(strcmp("mpt",request->instr)==0){return MPT;}
	else if(strcmp("pr",request->instr)==0){return PR;}
	else {assert(1==0);return -1;}
}

// Request* request get_cmd_assync_task_pt(){

// }

char* irl(RingRegBuffer* buf1, RingRegBuffer* buf2){
	char *str = (char*) malloc(256*sizeof(char));
	char *str1, *str2;
	str1 = info_RingRegBuffer(buf1);
	str2 = info_RingRegBuffer(buf2);
	sprintf(str,"<< LOCAL MEMORY INFO >> \n [ RDAT: %s, RALAM %s ] \n",str1,str2);
	free(str1); free(str2);
	return str;
}

char* lrd(RingRegBuffer* buf, int* data){
	char *str = (char*) malloc(256*sizeof(char));
	char *str1;
	int n = data[0];
	int i = data[1];
	str1 = toString_RingRegBuffer(buf,n,i);
	sprintf(str,"<< READ FROM DATA BUFFER >>\n%s",str1);
	free(str1);
	return str;
}

char* lra(RingRegBuffer* buf, int* data){
	char *str = (char*) malloc(256*sizeof(char));
	char *str1;
	int n = data[0];
	int i = data[1];
	str1 = toString_RingRegBuffer(buf,n,i);
	sprintf(str,"<< READ FROM ALARM BUFFER >>\n%s",str1);
	free(str1);
	return str;
}

char* drd(RingRegBuffer* buf){
	char *str = (char*) malloc(256*sizeof(char));
	deleteAll_RingRegBuffer(buf);
	sprintf(str,"<< DELETED CONTENT OF DATA REGISTERS >>\n");
	return str;
}

char* dra(RingRegBuffer* buf){
	char *str = (char*) malloc(256*sizeof(char));
	deleteAll_RingRegBuffer(buf);
	sprintf(str,"<< DELETED CONTENT OF ALARM REGISTERS >>\n");
	return str;
}

char* cpt(int dataTransfPeriod){
	char *str = (char*) malloc(256*sizeof(char));
	if(dataTransfPeriod==0){
		sprintf(str,"<< READ DATA TRANSFER PERIOD >>\n -- [ DESACTIVATED ] --\n");}
	else{	
		sprintf(str,"<< READ DATA TRANSFER PERIOD >>\n -- [ TRANSF. PERIOD: %d sec ] --\n", dataTransfPeriod);}
	return str;
}

char* mpt(int* dataTransfPeriod, int* data){
	char *str = (char*) malloc(256*sizeof(char));
	*dataTransfPeriod=data[0];
	sprintf(str,"<< SET DATA TRANSFER PERIOD >>\n -- [ TRANSF. PERIOD: %d sec ] --\n", *dataTransfPeriod);
	return str;
}

char* pr(RingRegBuffer* buf, int* data){

	char *str = (char*) malloc(256*sizeof(char));

	tm timeStart, timeEnd;

	float mean_temp=INT_MAX, mean_lum=INT_MAX;
	int max_temp=INT_MAX, max_lumin=INT_MAX, min_temp=INT_MIN, min_lumin=INT_MIN;

	int i, oper=0, updateFg;
	ProcessParams params=new_ProcessParams();

	if(data[0]!=-1 && data[1]!=-1 && data[2]!=-1){
		oper++; timeStart = new_tm(data[0],data[1],data[2]);
		if(data[3]!=-1 && data[4]!=-1 && data[5]!=-1){
			oper++; timeEnd = new_tm(data[3],data[4],data[5]);}}

	for(i=buf->idxNextWrite+1;i!=buf->idxNextWrite;i=(i+1)%buf->size){
		if(buf->array[i]!=NULL){
			tm time = get_time_Register(buf->array[i]);
			int temp = get_temp_Register(buf->array[i]);
			int lumin = get_lumin_Register(buf->array[i]);
			updateFg=0;
			switch(oper){
				case(0):
					updateFg=1;
					break;
				case(1):
					if(isGreaterOrEqual_tm(time,timeStart) &&
					   isGreaterOrEqual_tm(timeEnd,time)){
						updateFg=1;}
					break;
				case(2):
					if(isGreaterOrEqual_tm(time,timeStart)){
						updateFg=1;}
					break;
				default:
					break;
			}
			if(updateFg){update_ProcessParams(&params,temp,lumin);}}}

	if(params.count>0){
		mean_temp = params.acc_temp/params.count; mean_lum = params.acc_lumin/params.count;
		max_temp = params.max_temp; max_lumin = params.max_lumin;
		min_temp = params.min_temp; min_lumin = params.min_lumin;}

	sprintf(str,"<< PROCESS DATA REGISTERS >>\n[ MEAN TEMP. = %f, MAX TEMP. = %d, MIN TEMP. = %d ]\n[ MEAN LUMIN. = %f, MAX LUMIN. = %d, MIN LUMIN. = %d ]\n",mean_temp,max_temp,min_temp,mean_lum,max_lumin,min_lumin);

	return str;
}

