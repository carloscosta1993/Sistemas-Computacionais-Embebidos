/* External Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Internal Libraries */
#include "task_ui.h"
#include "../data_structures/intertask.h"

#define BUFFERSIZE 50
#define MAXINPUTARGS 10
#define DEBUG 0

const char* cmds_ct[] = {"rc","sc","rtl","rp","mpm","mti","ra","dtl","aa","rmm"};
const char* cmds_pt[] = {"irl","lrd","lra","drd","dra","cpt","mpt","pr"};

int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace(*s))
      return 0;
    s++;}
  return 1;
}

int is_number(const char* s){
	int i;
	int length = strlen(s);
   for (i=0;i<length;i++){
        if (!isdigit(s[i])){
        		return 0;}}
   return 1;
}

void init_interface_task_ui(){
  printf("\n----------------------------------------------------------------------------------");   
  printf("\n|                           Welcome to ECOS WEATHER STATION                            |");   
  printf("\n..................................................................................");    
  printf("\n| Usage: << rc >> - Read Clock                                                    |");
  printf("\n|        << sc [h m s] >> - Set Clock                                             |");
  printf("\n|        << rtl >> - Read current temperature and luminosity                      |");
  printf("\n|        << rp >> - Read parameters                                               |");
  printf("\n|        << mpm [p] >> - Set monitoring period                                    |");
  printf("\n|        << mti [t] >> - Set inactivity time                                      |");
  printf("\n|        << ra >> - Read alarms                                                   |");
  printf("\n|        << dtl [t l] >> - Set alarm thresholds                                   |");
  printf("\n|        << aa [A] >> - Activate/Deactivate alarms                                |");
  printf("\n|        << rmm >> - Read maximum and minimum                                     |");
  printf("\n|        << irl >> - Read local memory info                                       |");
  printf("\n|        << lrd [n [i]] >> - Read data registers                                  |");
  printf("\n|        << lra [n [i]] >> - Read alarm registers                                 |");
  printf("\n|        << drd >> - Delete data registers                                        |");
  printf("\n|        << dra >> - Delete alarm registers                                       |");
  printf("\n|        << cpt >> - Read transference period                                     |");
  printf("\n|        << mpt >> - Set transference period                                      |");
  printf("\n|        << pr [h1 m1 s1 [h2 m2 s2]] >> - Infer Statistics                        |");
  printf("\n|        << exit >> - Exit Application                                            |");
  printf("\n----------------------------------------------------------------------------------\n");   
}

Request* get_cmd_task_ui(){

	char buffer[BUFFERSIZE];
	char *tmp1, **tmp2;
	int count = 0, i;
	int* num = NULL;
	Request* request;
	
	tmp2 = (char**) malloc(MAXINPUTARGS*sizeof(char*));
	num = (int*) malloc(6*sizeof(int));
	for(i=0;i<6;i++){
		num[i]=-1;}

	fgets(buffer,BUFFERSIZE,stdin);
	tmp1=strtok(buffer," \n");
	while(tmp1!=NULL){
		if(!is_empty(tmp1)){
			tmp2[count++] = strdup(tmp1);}
		tmp1=strtok(NULL," \n");
	}

	switch(count){
		case(1):
			if(strcmp(tmp2[0],"rc")==0  || 
			   strcmp(tmp2[0],"rtl")==0 ||
			   strcmp(tmp2[0],"rp")==0  ||
			   strcmp(tmp2[0],"ra")==0  ||
			   strcmp(tmp2[0],"rmm")==0 ||
			   strcmp(tmp2[0],"irl")==0 ||
			   strcmp(tmp2[0],"drd")==0 ||
			   strcmp(tmp2[0],"dra")==0 ||
			   strcmp(tmp2[0],"cpt")==0 ||
			   strcmp(tmp2[0],"pr")==0){
				request = new_Request(tmp2[0],num);}
			else if(strcmp(tmp2[0],"exit")==0){
				exit(1);}
			else{
				return NULL;
			}
			break;
		case(2):
			if((strcmp(tmp2[0],"mpm")==0 ||
				 strcmp(tmp2[0],"mti")==0 ||
				 strcmp(tmp2[0],"mpt")==0) &&
				 is_number(tmp2[1])){
				num[0] = atoi(tmp2[1]);
				if(num[0]>=0 && num[0]<60){
					request = new_Request(tmp2[0],num);}
				else{
					return NULL;}}
			else if(strcmp(tmp2[0],"aa")==0){
				if(strcmp(tmp2[1],"A")==0){
					num[0]=1;
					request=new_Request(tmp2[0],num);}
				else if(strcmp(tmp2[1],"a")==0){
					num[0]=0;
					request=new_Request(tmp2[0],num);} 
				else{
					return NULL;
				}
			}
			else if((strcmp(tmp2[0],"lrd")==0 ||
				 strcmp(tmp2[0],"lra")==0) &&
				 is_number(tmp2[1])){
				num[0] = atoi(tmp2[1]);
				if(num[0]>0 && num[0]<50){
					request=new_Request(tmp2[0],num);}
				else{
					return NULL;}}
			else{
				return NULL;
			}
			break;
		case(3):
			if(strcmp(tmp2[0],"dtl")==0 && 
				is_number(tmp2[1]) && 
				is_number(tmp2[2])){
				num[0] = atoi(tmp2[1]);
				num[1] = atoi(tmp2[2]);
				if(num[0]>=0 && num[0]<50 &&
					num[1]>=0 && num[1]<=6){
					request = new_Request(tmp2[0],num);}
				else{
					return NULL;}}
			else if((strcmp(tmp2[0],"lrd")==0 || 
				      strcmp(tmp2[0],"lra")==0) && 
					   is_number(tmp2[1]) && 
				      is_number(tmp2[2])){
				num[0] = atoi(tmp2[1]);
				num[1] = atoi(tmp2[2]);
				if(num[0]>=0 && num[0]<=50 &&
					num[1]>=0 && num[1]<=50){
					request = new_Request(tmp2[0],num);}
				else{
				return NULL;}}
			else{
				return NULL;}
			break;
		case(4):
			if((strcmp(tmp2[0],"sc")==0 ||
				strcmp(tmp2[0],"pr")==0) && 
				is_number(tmp2[1]) && 
				is_number(tmp2[2]) && 
				is_number(tmp2[3])){
				num[0] = atoi(tmp2[1]);
				num[1] = atoi(tmp2[2]);
				num[2] = atoi(tmp2[3]);
				if(num[0]>=0 && num[0]<24 &&
					num[1]>=0 && num[1]<60 &&
					num[2]>=0 && num[2]<60){
					request = new_Request(tmp2[0],num);}
				else{
				return NULL;}}
			else{
				return NULL;}
			break;
		case(7):
			if(strcmp(tmp2[0],"pr")==0 && 
				is_number(tmp2[1]) && is_number(tmp2[2]) &&
				is_number(tmp2[3]) && is_number(tmp2[4]) &&
				is_number(tmp2[5]) && is_number(tmp2[6])){
				num[0] = atoi(tmp2[1]);
				num[1] = atoi(tmp2[2]);
				num[2] = atoi(tmp2[3]);
				num[3] = atoi(tmp2[4]);
				num[4] = atoi(tmp2[5]);
				num[5] = atoi(tmp2[6]);
				if(num[0]>=0 && num[0]<24 &&
					num[1]>=0 && num[1]<60 &&
					num[2]>=0 && num[2]<60 &&
					num[3]>=0 && num[3]<24 &&
					num[4]>=0 && num[4]<60 &&
					num[5]>=0 && num[5]<60){
					request = new_Request(tmp2[0],num);}
				else{
				return NULL;}}
			else{
				return NULL;}
			break;
		default:
			return NULL;
			break;}

	if(DEBUG){print_Request(request,1);}
	return request;
}

int get_request_destination_task_ui(Request* request){
	int i;
	for(i=0;i<8;i++){
		if(strcmp(cmds_pt[i],request->instr)==0){
			return THREAD_PT;}}
	return THREAD_CT;
}

void clean_stdin(){

	int c;
	do{
		c = getchar();
	}while(c!='\n'&&c != EOF);
}
