/* External Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Internal Libraries */
#include "time.h"

tm new_tm(int hour,int min,int sec){
	assert(hour>=0 && hour<24);
	assert(min>=0 && min<60);
	assert(sec>=0 && sec<60);
	tm time = {hour, min, sec};
	return time;
}

int isGreaterOrEqual_tm(tm time1, tm time2){
	if(time1.hour>time2.hour){
		return 1;}
	else if(time1.hour<time2.hour){
		return 0;}
	else{
		if(time1.min>time2.min){
			return 1;}
		else if(time1.min<time2.min){
			return 0;}
		else{
			if(time1.sec>=time2.sec){
				return 1;}
			else{
				return 0;}}}	
}

// ////////////////////// FILE DEBUGGER ////////////////////////
// int main(){
// 	tm time1 = new_tm(12,10,02);
// 	tm time2 = new_tm(12,10,03);
// 	printf("GREATER: %d\n",isGreaterOrEqual_tm(time1,time2));
// 	return 0;
// }