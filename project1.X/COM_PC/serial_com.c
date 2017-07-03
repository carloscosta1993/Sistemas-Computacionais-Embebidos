/* External Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Internal Libraries */
#include "serial_com.h"

char* reply_message_serial_com(int oper, int data[]){

	char str[248];
    
	switch(oper){
		case(RCLK):
			sprintf(str,"%d %d %d %d %d %d",SOM,RCLK,data[0],data[1],data[2],EOM); break;
		case(SCLK):
			sprintf(str,"%d %d %d %d",SOM,SCLK,CMD_OK,EOM); break;
		case(RRTL):
			sprintf(str,"%d %d %d %d %d",SOM,RRTL,data[0],data[1],EOM); break;
		case(RPAR):
			sprintf(str,"%d %d %d %d %d %d",SOM,RPAR,data[0],data[1],data[2],EOM); break;
		case(MPMN):
			sprintf(str,"%d %d %d %d",SOM,MPMN,CMD_OK,EOM); break;
		case(MTIN):
			sprintf(str,"%d %d %d %d",SOM,MTIN,CMD_OK,EOM); break;
		case(RALA):
			sprintf(str,"%d %d %d %d %d %d",SOM,RALA,data[0],data[1],data[2],EOM); break;
		case(DATL):
			sprintf(str,"%d %d %d %d",SOM,DATL,CMD_OK,EOM); break;
		case(AALA):
			sprintf(str,"%d %d %d %d",SOM,AALA,CMD_OK,EOM); break;
		case(TRTL):
			sprintf(str,"%d %d %d %d %d %d %d %d",SOM,TRTL,data[0],data[1],data[2],data[3],data[4],EOM); break;
		case(TRMM):
			sprintf(str,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",SOM,TRMM,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9],data[10],data[11],data[12],data[13],data[14],data[15],data[16],data[17],data[18],data[19],EOM); break;
		case(NALA):
			sprintf(str,"%d %d %d %d %d %d %d %d",SOM,NALA,data[0],data[1],data[2],data[3],data[4],EOM); break;
		default: 
			break;
	}
	return str;
}