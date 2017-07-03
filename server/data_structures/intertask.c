/* External Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Internal Libraries */
#include "intertask.h"

Request* new_Request(char* instr, int* data){
	Request* request = (Request*) malloc(sizeof(Request));
	request->instr=strdup(instr);
	if(data!=NULL){request->data=data;}
	else{request->data=NULL;}
	return request;
}

void free_Request(Request* request){
	if(request!=NULL){
		if(request->instr!=NULL){free(request->instr);}
		if(request->data!=NULL){free(request->data);}
		free(request);}
}

void print_Request(Request* request, int valid){
	char *str=(char*) malloc(128*sizeof(char));
	int i;

	if(valid){
		str=strcpy(str,"");
		for(i=0;i<6;i++){
			if(request->data[i]==-1){break;}
			snprintf(str,128,"%s %d",str,request->data[i]);}
		if(i==0){
			printf("<< Request = [%s] >>\n", request->instr);}
		else{
			printf("<< Request = [%s,%s] >>\n", request->instr, str);}}
	else{
		printf("<< Request = INVALID >> | -u to see usage \n");}
	free(str);
}
