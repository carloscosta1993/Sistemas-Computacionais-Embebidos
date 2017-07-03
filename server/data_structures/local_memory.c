/* External Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Internal Libraries */
#include "time.h"
#include "local_memory.h"

#define MAXBUFFERSIZE 100

Register* new_Register(tm time,int temp,int lumin){

	Register* reg = (Register*) malloc(sizeof(Register));
	reg->time=time;
	assert(temp>=0 && temp<=50);
	assert(lumin>=0 && lumin<=5);
	reg->temp=temp;
	reg->lumin=lumin;

	return reg;
}

tm get_time_Register(Register* reg){
	return reg->time;
}

int get_temp_Register(Register* reg){
	return reg->temp;
}

int get_lumin_Register(Register* reg){
	return reg->lumin;
}

char* toString_Register(Register* reg){
	char *str = (char*) malloc(128*sizeof(char));
	sprintf(str,"[ TIME: %02d:%02d:%02d TEMP:%d LUMIN:%d ]\n",reg->time.hour,reg->time.min,reg->time.sec,reg->temp,reg->lumin);
	return str;
}

void free_Register(Register* reg){
	free(reg);
}

RingRegBuffer* new_RingRegBuffer(int size){

	RingRegBuffer* buf;
	int i;

	assert(size>0 && size<MAXBUFFERSIZE);
	buf = (RingRegBuffer*) malloc (sizeof(RingRegBuffer));
	buf->size = size;
	buf->array = (Register**) malloc(size*sizeof(Register*));
	for(i=0;i<size;i++){
		buf->array[i]=NULL;}
	buf->idxNextRead=0;
	buf->idxNextWrite=0;
	return buf;
}

void add_RingRegBuffer(RingRegBuffer* buf,tm time,int temp,int lumin){

	assert(buf!=NULL);
	Register* reg=new_Register(time,temp,lumin);
	buf->array[buf->idxNextWrite++]=reg;
}

void deleteAll_RingRegBuffer(RingRegBuffer* buf){

	int i;
	assert(buf!=NULL);
	for(i=0;i<buf->size;i++){
		if(buf->array[i]!=NULL){
			free_Register(buf->array[i]);
			buf->array[i]=NULL;}
	}
	buf->idxNextWrite=0;
	buf->idxNextRead=0;
}

char* toString_RingRegBuffer(RingRegBuffer* buf,int n,int i){
	
	int count=1, start, k;
	char *str1 = (char*) malloc(128*buf->size*sizeof(char)), *str2;

	assert(n<buf->size);
	assert(i<buf->size);
	assert(buf!=NULL);
	str1=strcpy(str1,"");
	if(i==-1){
		start=buf->idxNextRead;}
	else{
		start=(buf->idxNextWrite+i)%buf->size;}
	for(k=start;k!=buf->idxNextWrite;k=(k+1)%buf->size){
		if(buf->array[k]!=NULL){
			str2=toString_Register(buf->array[k]);
			sprintf(str1,"%s%s",str1,str2);
			free(str2);}
		if(i==-1){
			buf->idxNextRead=(buf->idxNextRead+1)%buf->size;}
		if(count==n){break;}
		else{count++;}
	}
	return str1;
}

char* info_RingRegBuffer(RingRegBuffer* buf){
	char *str = (char*) malloc(128*sizeof(char));
	assert(buf!=NULL);
	sprintf(str,"[ Size:%d, nrd:%d, nwd:%d ]",buf->size,buf->idxNextRead%buf->size,buf->idxNextWrite%buf->size);
	return str;
}

void free_RingRegBuffer(RingRegBuffer* buf){
	int i;
	assert(buf!=NULL);
	for(i=0;i<buf->size;i++){
		if(buf->array[i]!=NULL){
			free_Register(buf->array[i]);}
	}
	free(buf->array);
	free(buf);
}

//////////////////////// FILE DEBUGGER ////////////////////////
// int main(){

// 	char* str;

// 	RingRegBuffer* buf = new_RingRegBuffer(50);
// 	tm time = new_tm(10,33,10);
// 	add_RingRegBuffer(buf,time,10,2);
// 	time = new_tm(10,33,10);
// 	add_RingRegBuffer(buf,time,11,3);
// 	str=toString_RingRegBuffer(buf,3,-1);
// 	printf("%s",str);
// 	free(str);
// 	time = new_tm(10,34,10);
// 	add_RingRegBuffer(buf,time,11,3);
// 	time = new_tm(10,35,10);
// 	add_RingRegBuffer(buf,time,11,3);
// 	deleteAll_RingRegBuffer(buf);
// 	str=toString_RingRegBuffer(buf,10,1);
// 	printf("%s",str);
// 	free(str);
// 	free_RingRegBuffer(buf);
// 	return 0;
// }

