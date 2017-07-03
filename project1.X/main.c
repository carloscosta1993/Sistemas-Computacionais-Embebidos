/*
 * File:   main.c
 * Author: Group 4 || Carlos Costa [84926], Miguel Ferreira [75471], Tomas Mendes [85063]
 *
 * Created on 20 April 2017
 */

// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = RCIO6      // Oscillator Selection bits (External RC oscillator, port function on RA6)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "XLCD/xlcd.h"
#include "COM_PC/serial_com.h"
#include "USART/usart.h"
#include "I2C/i2c.h"
#include "ADC/adc.h"


/******************** Global Variables ********************/
/** Global Parameters **/
volatile int PMON = 3, TSOM = 2, TINA = 10, ALAT = 30, ALAL = 4, ALAF = 0;
/** Timer1 Control **/
volatile int countClock=40, countMonitor=2, countTINA = 0, countSaveTime=0;
volatile int countAlarmTemperature=-1, countAlarmLuminosity=-1;
/** Clock & DisplayClock **/
volatile int hours = 2, minutes = 30, seconds = 40;
volatile int displayHours, displayMinutes, displaySeconds;
/** Temperature + Luminosity & Display Temperature + Luminosity **/
volatile int temperature = 0, luminosity = 0;
volatile int maxTemperature = -1, maxLuminosity = -1;
volatile int minTemperature = 100, minLuminosity = 10;
volatile int displayTemperature, displayLuminosity;
/** Alarm **/
volatile int iALTON = 0, wALTON=0, iALLON = 0, wALLON=0;
/** DisplayControl **/
volatile int updateDisplay = 0;
/** ExternalEEPROMControl **/
volatile unsigned char Addrhi = 0, Addrlo;
volatile int updateExternalEEPROM[4];
volatile unsigned char dataInEEEPROM[4][5];
/** IEEPROMControl **/
volatile int updateIEEPROM=0;
/** Sleep Mode **/
volatile int goToSleep=0;
/** Mode **/
volatile int mode = 0, pressedMode=0; // 0 - Normal Mode ; 1 - Modified Mode;
/** Status of Modified Mode **/
volatile int status = -1;

#define HOURS 0;
#define MINUTES 1;
#define SECONDS 2;
#define MINTEMPLUM 3;
#define MAXTEMPLUM 4;
#define THRTEMP 5;
#define THRLUM 6;
#define ALARM 7;

void Delay1KTCYx( unsigned long n )
{
//  _delay(n*1000);
    while(n--*1000);
}

void DelayFor18TCY( void )
{
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
}

void DelayExternalEEPROM(){
    int i;
    for(i=0;i<1000;i++){ //500
        DelayFor18TCY();
    }
}

void DelayPORXLCD( void )
{
  Delay1KTCYx(60);
  return;
}

void DelayXLCD( void )
{
  Delay1KTCYx(20);
  return;
}

void Delay10TCYx( unsigned long n )
{
  while(n--) _delay(10);
}

/******************** Project TREE ********************/

////////// (A) I/0 INTERFACE //////////
void displayInterface(int mode, int status);
void OnBuffer();
void OffBuffer();
int getLuminosity ();
unsigned char getTemperature ();
void writeEEEPROM(unsigned char, unsigned char, unsigned char);
unsigned char readEEEPROM(unsigned char, unsigned char);
void writeIEEPROM(unsigned char , unsigned char);
unsigned char readIEEPROM(unsigned char);

////////// (A.1) INTERRUPTION CALLS //////////
void timer1_isr ();  
void rb0_isr();
void ra4_isr();
void serial_com_isr();
////////// (A.2) INTERRUPTION VECTOR //////////
void interrupt ISR (void);
////////// (B) INITIALIZATION //////////
void global_init();
void LCD_init();
void RBO_init();
void buzzer_init();
void timer0_init();
void timer1_init ();
void TC74_init();
void ADC_init();
////////// (C) RESET //////////
void timer0_reset();

/******************** I/0 INTERFACE ********************/

void displayInterface(int mode, int status){

    ////// CLEAR MEMORY BEFORE ASSINGMENT ///////
    char tmp[3];
    
    /////////////// MAYBE CHANGE TO INCLUDE ONLY IN THE BEGINNING //////////////////
    // Reset Display
    while(BusyXLCD());
    WriteCmdXLCD(DOFF); 
        
    /** Select Clock Display Time **/
    if(mode==0){
        displayHours=hours; 
        displayMinutes=minutes; 
        displaySeconds=seconds;
        displayTemperature=temperature;
        displayLuminosity=luminosity;
       
        while(BusyXLCD());
        WriteCmdXLCD(CURSOR_OFF);  
    }
    else{
        while(BusyXLCD());
        WriteCmdXLCD(BLINK_ON); 
    }
    
    /*** Display Clock ***/
    while(BusyXLCD());
    SetDDRamAddr(0x01);
    tmp[0] = "\0";
    if(displayHours<10){sprintf(tmp,"0%d",displayHours);}
    else{sprintf(tmp,"%d",displayHours);}
    putsXLCD(tmp);
            
    while(BusyXLCD());
    SetDDRamAddr(0x03);
    putrsXLCD(":");

    while(BusyXLCD());
    SetDDRamAddr(0x04);
    tmp[0] = "\0";
    if(displayMinutes<10){sprintf(tmp,"0%d",displayMinutes);}
    else{sprintf(tmp,"%d",displayMinutes);}
    putsXLCD(tmp);
    
    while(BusyXLCD());
    SetDDRamAddr(0x06);
    putrsXLCD(":");
    
    while(BusyXLCD());
    SetDDRamAddr(0x07);
    tmp[0] = "\0";
    if(displaySeconds<10){sprintf(tmp,"0%d",displaySeconds);}
    else{sprintf(tmp,"%d",displaySeconds);}
    putsXLCD(tmp);  
        
    /** Display Max/Min Temperature + Luminosity **/
    while(BusyXLCD());
    SetDDRamAddr(0x0A);
    if(mode==1){
        putrsXLCD("mM");
    }
    putrsXLCD("  ");
    
    /*** Display Alarm ***/
    while(BusyXLCD());
    SetDDRamAddr(0x0C);
    if(wALTON==1 || mode==1){
        putrsXLCD("T");
    }
    else{
        putrsXLCD(" ");
    }
    while(BusyXLCD());
    
    /*** Display Alarm ***/
    while(BusyXLCD());
    SetDDRamAddr(0x0D);
    if(wALLON==1 || mode==1){
        putrsXLCD("L");
    }
    else{
        putrsXLCD(" ");
    }

    /*** Display Alarm Enabler ***/
    while(BusyXLCD());
    SetDDRamAddr(0x0F);
    if(ALAF){putrsXLCD("A");}
    else{putrsXLCD("a");}
    while(BusyXLCD());
    
    /*** Display Temperature & Luminosity ***/
    SetDDRamAddr(0x41);
    tmp[0] = "\0";
    switch(status){
        case 4: 
            if(!pressedMode){displayTemperature=temperature;}
            if(displayTemperature<10){sprintf(tmp,"0%d",displayTemperature);}
            else{sprintf(tmp,"%d",displayTemperature);}
            break;
        case 5:
            displayTemperature=temperature;
            if(ALAT<10){sprintf(tmp,"0%d",ALAT);}
            else{sprintf(tmp,"%d",ALAT);}
            break;
        default:
            if(displayTemperature<10){sprintf(tmp,"0%d",displayTemperature);}
            else{sprintf(tmp,"%d",displayTemperature);}
            break;
    }
    putsXLCD(tmp);  
    while(BusyXLCD());
    
    SetDDRamAddr(0x44);
    putrsXLCD("C");
    while(BusyXLCD());

    SetDDRamAddr(0x4D);
    putrsXLCD("L");
    while(BusyXLCD());
    
    SetDDRamAddr(0x4F);
    tmp[0] = "\0";
    switch(status){       
        case 4:
            if(!pressedMode){displayLuminosity=luminosity;}
            sprintf(tmp,"%d",displayLuminosity);
            break;
        case 5:
            if(!pressedMode){displayLuminosity=luminosity;}    
            sprintf(tmp,"%d",displayLuminosity);
            break;
        case 6:
            sprintf(tmp,"%d",ALAL);
            break;
        default:
            sprintf(tmp,"%d",displayLuminosity);
            break;
    }
    putsXLCD(tmp);  
    while(BusyXLCD());   
    
    /** Place Cursor in Modified Mode **/
    switch(status){
        case 0:SetDDRamAddr(0x02);break;
        case 1:SetDDRamAddr(0x05);break;
        case 2:SetDDRamAddr(0x08);break;
        case 3:SetDDRamAddr(0x0A);break;
        case 4:SetDDRamAddr(0x0B);break;
        case 5:SetDDRamAddr(0x0C);break;
        case 6:SetDDRamAddr(0x0D);break;
        case 7:SetDDRamAddr(0x0F);break;
        default:break;
    }
}

void OnBuffer(){
    CCP1CON = 0x0F;
}

void OffBuffer(){
    CCP1CON = ~CCP1CON & 0x0F;
}

int getLuminosity (void)
{
  int level, read;
  float result;
  
  OpenADC( ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_1ANA & ADC_12_TAD,
           ADC_CH0 & ADC_REF_VDD_VSS & ADC_INT_OFF, 0x0E );
  Delay10TCYx( 5 ); // Delay for 50TCY
  ConvertADC(); // Start conversion
  while( BusyADC() ); // Wait for completion
  read = ReadADC(); // Read result
  CloseADC(); // Disable A/D converter
    
  result = read/171;
  level = floor(result); 
  return level;

}

unsigned char getTemperature (void)
{
	unsigned char value;
do{
	IdleI2C();
	StartI2C(); IdleI2C();
	WriteI2C(0x9a); IdleI2C();
	WriteI2C(0x01); IdleI2C();
	RestartI2C(); IdleI2C();
	WriteI2C(0x9a | 0x01); IdleI2C();
	value = ReadI2C(); IdleI2C();
	NotAckI2C(); IdleI2C();
	StopI2C();
} while (!(value & 0x40));
	IdleI2C();
	StartI2C(); IdleI2C();
	WriteI2C(0x9a | 0x00); IdleI2C();
	WriteI2C(0x00); IdleI2C();
	RestartI2C(); IdleI2C();
	WriteI2C(0x9a | 0x01); IdleI2C();
	value = ReadI2C(); IdleI2C();
	NotAckI2C(); IdleI2C();
	StopI2C();

	return value;
}

void writeEEEPROM(unsigned char Addrhi, unsigned char Addrlo, unsigned char Data){

    IdleI2C();
	StartI2C(); IdleI2C();
	WriteI2C(0xA0); IdleI2C();
	WriteI2C(Addrhi); IdleI2C();
    WriteI2C(Addrlo); IdleI2C();
    WriteI2C(Data); IdleI2C();
	StopI2C();
}

unsigned char readEEEPROM(unsigned char Addrhi, unsigned char Addrlo){

    unsigned char value;

    IdleI2C();
	StartI2C(); IdleI2C();
	WriteI2C(0xA0); IdleI2C();
	WriteI2C(Addrhi); IdleI2C();
    WriteI2C(Addrlo); IdleI2C();
	RestartI2C(); IdleI2C();
	WriteI2C(0xA1); IdleI2C();
    value = ReadI2C(); IdleI2C();
    StopI2C();

    return value;
}


void writeIEEPROM(unsigned char Addr, unsigned char Data){

    unsigned char saveGIE;
    
    EEADR = Addr;
    EEDATA = Data;

  	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	EECON1bits.WREN = 1;
	saveGIE = INTCONbits.GIE;
	INTCONbits.GIE = 0;
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1;
	while(EECON1bits.WR);			
	INTCONbits.GIE = saveGIE;
    /// EECON1bits.EEPGD=1;
	EECON1bits.WREN = 0;   
}

unsigned char readIEEPROM(unsigned char Addr){

    unsigned char value;
    
    EEADR = Addr;
    
    // EECON1bits.CFGS = 0;
    EECON1bits.EEPGD=0;
    EECON1bits.WREN=1;
    EECON1bits.RD=1;
    value = EEDATA;
    // EECON1bits.EEPGD=1;

    return value;
}

/**** Assyncronous Serial Communication Interrupt Call ****/

void assync_serial_commun(void){

    char *reply_message;
    int reply_data[] = {-1,-1,-1,-1,-1};

    int i;
    char c;
    
    /*** Disable Global Interrupts ***/
    INTCONbits.GIE = 0;
    
    reply_data[0] = hours;
    reply_data[1] = minutes;
    reply_data[2] = seconds;
    reply_data[3] = temperature;
    reply_data[4] = luminosity;

    /*** Write Reply Message ***/
    reply_message=reply_message_serial_com(NALA,reply_data);
    for(i=0;i<strlen(reply_message)+1;i++){
        c=reply_message[i]; 
        while(BusyUSART());
        putcUSART(reply_message[i]);
    }
    while(BusyUSART());

    /*** Enable Global Interrupts ***/
    INTCONbits.GIE = 1;
    
    /*** Reset Interrupt Flag ***/
    PIR1bits.RCIF = 0;
}


/******************** INTERRUPTION CALLS ********************/

/**** Timer 1 Interrupt Call ****/
void timer1_isr (void) 
{
    int k;
    
    /* Reset Timer1 */  
    TMR1H=0x80; // Counter
    TMR1L=0; // Counter
    PIR1bits.TMR1IF = 0; // Interrupt Reset
    
    /* Reset Update External EEPROM */
    for(k=0;k<4;k++){updateExternalEEPROM[k]=0;}  
    
    /*** Update Counters ***/
    countClock ++;
    countMonitor ++;
    countTINA ++;
    countSaveTime++;
    if(countAlarmTemperature>-1){countAlarmTemperature ++;}
    if(countAlarmLuminosity>-1){countAlarmLuminosity ++;}
    
    /*** Update Clock ***/
    seconds = countClock;
    if(seconds>=60){seconds=countClock=0;minutes++;};
    if(minutes>=60){minutes=0;hours++;}
    if(hours>=24){hours=0;}
    
    /*** Update Monitor ***/
    if(countMonitor==PMON){
        /* Get Temperature */
        temperature = (int) getTemperature();
      /* Get Luminosity */
        luminosity = getLuminosity();
        countMonitor=0;
        /* Check New Extreme Temperature && Alarm Luminosity */        
        if(temperature>maxTemperature){
            updateExternalEEPROM[0]=1;
            maxTemperature=temperature;}
        if(temperature<minTemperature){
            updateExternalEEPROM[1]=1;
            minTemperature=temperature;}
        if(luminosity>maxLuminosity){
            updateExternalEEPROM[2]=1;
            maxLuminosity=luminosity;}
        if(luminosity<minLuminosity){
            updateExternalEEPROM[3]=1;
            minLuminosity=luminosity;
        }

        for(k=0;k<5;k++){  
            if(updateExternalEEPROM[k]){
                dataInEEEPROM[k][0] = (unsigned char) hours;
                dataInEEEPROM[k][1] = (unsigned char) minutes;
                dataInEEEPROM[k][2] = (unsigned char) seconds;
                dataInEEEPROM[k][3] = (unsigned char) temperature;
                dataInEEEPROM[k][4] = (unsigned char) luminosity;                
            }
        }
    }
    
    /*** Check If New Alarm ***/
    if(ALAF && mode==0){    
        /* Check Alarm Temperature && Alarm Luminosity */        
        if(temperature>=ALAT){
            /** Wake From Sleep **/
            goToSleep=0;
            countTINA=0;
            iALTON=1;wALTON=1;
            /* Send Message to Server */
            assync_serial_commun();
            /* Start Alarm + Start Buzzer */ 
            OnBuffer(); countAlarmTemperature=0;}
        if(luminosity>=ALAL){
            /** Wake From Sleep **/
            goToSleep=0;
            countTINA=0;
            iALLON=1;wALLON=1;
            /* Send Message to Server */
            assync_serial_commun();
            /* Start Alarm + Start Buzzer */ 
            OnBuffer(); countAlarmLuminosity=0;}
    }
            
    /*** Update Alarm Temperature + Luminosity ***/
    if(countAlarmLuminosity==TSOM){
        /* Reset Alarm + Stop Buzzer */ 
        if(!iALTON){OffBuffer();};
        countAlarmLuminosity = -1;
        iALLON=0;
    }
    if(countAlarmTemperature==TSOM){
        /* Reset Alarm + Stop Buzzer */ 
        if(!iALLON){OffBuffer();};
        countAlarmTemperature = -1;
        iALTON=0;
    }
    
    /** Check If Have Time Stamp **/
    if(countSaveTime==15){
        updateIEEPROM=1;
        countSaveTime=0;
    }
    
    /*** Check If GO TO SLEEP ***/
    if(countTINA==TINA){
        goToSleep=1;
    }
    
    /*** Enable Display ***/
    updateDisplay = 1;
        
}

/**** RBO Interrupt Call ****/
void rb0_isr(void){

    /* Reset Timer1 */  
    INTCONbits.INT0IF = 0;
    
    /** Reset Sleep Counter **/
    countTINA=0;
    
    if(goToSleep==1){
        /** Wake From Sleep **/
        goToSleep=0;
    }
    else if(mode==0){
        /** Change Operation Mode - Normal TO Modified **/
        mode=1;
        status=0;
        wALTON = wALLON = 0;
        timer0_init(); // Enable Timer 0
    }
    else{
        /** Change Operation Mode - Modified TO Normal **/
        if(status==7){
            mode=0;
            status=-1;
            timer0_reset();
        }
        /** Update Modified Mode **/
        else
        {status++;pressedMode=0;}
    }
    
    /*** Enable Display ***/
    updateDisplay = 1;
}

/**** RA4 Interrupt Call ****/
void ra4_isr(void){
    
    /* Reset Timer0 */  
    TMR0 = 0XFF;
    INTCONbits.TMR0IF = 0;
    
    /** Reset Sleep Counter **/
    countTINA=0;
    
    /** Change Operation Mode - Modified to Modified Pressed **/
    pressedMode=1;
    
    switch(status){
        case 0:
            displayHours++;
            displayHours %= 24;
            hours = displayHours;
            break;
        case 1:
            displayMinutes++;
            displayMinutes %= 60;
            minutes = displayMinutes;
            break;
        case 2:
            displaySeconds++;
            displaySeconds %= 60;
            countClock = seconds = displaySeconds;
            break;
        case 3:
            displayTemperature = minTemperature;
            displayLuminosity = minLuminosity;
            break;
        case 4:
            displayTemperature = maxTemperature;
            displayLuminosity = maxLuminosity;
            break;
        case 5:
            ALAT++;
            ALAT%=51;
            break;
        case 6:
            ALAL++;
            ALAL%=6;
            break;
        case 7:
            ALAF++;
            ALAF%=2;
            break;
        default:
            break;            
    }   
    
    /*** Enable Display ***/
    updateDisplay = 1;
}

/**** Syncronous Serial Communication Interrupt Call ****/
void syn_serial_commun_isr(){

    char c;
    char request_message[16], *reply_message;
    int request_data[] = {-1,-1,-1,-1,-1};
    int reply_data[20] = {-1};
    int i=0,j,count=0;
    
    int msg=-1, oper;
    int start=1, start_msg=0, end=0;
    
    /*** Disable Global Interrupts ***/
    INTCONbits.GIE = 0;
    
    /*** Read Request Message ***/
    while(1){
     	
        // BUID STRING //
        /*** Clean Buffer ***/
        j=0;memset(&request_message[0],0, sizeof(request_message));
        while(1){
            while(!DataRdyUSART());
            c = getcUSART();
            if(c==' '||c=='\0'){break;}
            else{request_message[j++]=c;}
        }
        
        // PROCESS STRING //
        oper=atoi(request_message);       
        if(start==1){
	 		/// CHECK <SOM> ///
	 		assert(oper==SOM);
	 		start=0;}
	 	else if(start==0 && end==0){
	 		 /// CHECK <MSG> + <DATA> ///
	 		if(msg==-1){msg=oper;}
	 		assert(oper!=EOM);
	 		switch(msg){
                case(RCLK):
                case(RRTL):
                case(RPAR):
                case(RALA):
                case(TRTL):
                case(TRMM):
                    end=1; break;
                case(MPMN):
                case(MTIN):
                case(AALA):
                    if(start_msg==0){
	 					start_msg=1;}
	 				else{
	 					request_data[i++]=oper;}
	 				if(i==1){end=1;};
	 				break;
                case(DATL):
                    if(start_msg==0){
	 					start_msg=1;}
	 				else{
	 					request_data[i++]=oper;}
	 				if(i==2){end=1;};
	 				break;   
                case(SCLK):
                    if(start_msg==0){
	 					start_msg=1;}
	 				else{
	 					request_data[i++]=oper;}
	 				if(i==3){end=1;};
                    break;
				default: 
					assert(1==0);
					break;}}
	 	else if(start==0 && end==1){
	 		/// CHECK <EOM> ///
	 		assert(oper==EOM);
            break;}       
    }

    /*** Process Request ***/
    switch(msg){
        case(RCLK):
            reply_data[0] = hours;
            reply_data[1] = minutes;
            reply_data[2] = countClock;
            break;
        case(SCLK):
            hours = request_data[0];
            minutes = request_data[1];
            seconds = countClock = request_data[2];
            break;
        case(RRTL):
            reply_data[0] = temperature;
            reply_data[1] = luminosity;
            break;
        case(RPAR):
            reply_data[0] = PMON;
            reply_data[1] = TSOM;
            reply_data[2] = TINA;
            break;
        case(MPMN):
            PMON = request_data[0];
            break;
        case(MTIN):
            TINA = request_data[0]*60;
            break;
        case(RALA):
            reply_data[0] = ALAT;
            reply_data[1] = ALAL;
            reply_data[2] = ALAF;
            break;
        case(DATL):
            ALAT = request_data[0];
            ALAL = request_data[1];
            break;
        case(AALA):
            ALAF = request_data[0];
            break;
        case(TRMM):
            for(i=0;i<4;i++){
                for(j=0;j<5;j++){
                    Addrlo = (unsigned char) 5*i+j; 
                    reply_data[count++] = (int) readEEEPROM(Addrhi,Addrlo);
                    DelayExternalEEPROM();}}
            break;
        case(TRTL):
            reply_data[0] = hours;
            reply_data[1] = minutes;
            reply_data[2] = countClock;
            reply_data[3] = temperature;
            reply_data[4] = luminosity;
            break;
        default:
            assert(1==0);
            break;
    }
    
    /*** Write Reply Message ***/
    reply_message=reply_message_serial_com(msg,reply_data);
    for(i=0;i<strlen(reply_message)+1;i++){
        c=reply_message[i]; 
        while(BusyUSART());
        putcUSART(reply_message[i]);
    }
    while(BusyUSART());
    
    /*** Enable Global Interrupts ***/
    INTCONbits.GIE = 1;
    
    /*** Reset Interrupt Flag ***/
    PIR1bits.RCIF = 0;
}

/******************** INTERRUPTION VECTOR ********************/
void interrupt ISR (void) 
{
  if(PIR1bits.TMR1IF){timer1_isr();}
  else if(INTCONbits.INT0IF){rb0_isr();}
  else if(INTCONbits.TMR0IF){ra4_isr();}
  else if(PIR1bits.RCIF){syn_serial_commun_isr();}
}

/******************** INITIALIZATION ********************/
void global_init(void)
{
  RCONbits.IPEN = 1;    /* enable interrupt priority levels */
  INTCONbits.GIE = 1;  /* enable all high priority interrupts */
  INTCONbits.GIEH = 1;  /* enable all high priority interrupts */

}


void LCD_init(){
    
  ADCON1 = 0x0E;                    
  LATDbits.LATD7 = 1;
  TRISDbits.TRISD7 = 0;
  OpenXLCD( FOUR_BIT & LINES_5X7);
  
}

void RBO_init(){
  INTCONbits.INT0IF=0;
  INTCON2bits.INTEDG0=0;
  INTCON2bits.RBPU=0;
  TRISBbits.TRISB0=1;
  INTCONbits.INT0IE=1;
}

void RA4_init(void){
    ////// CLEAN ANALOG SHARED ENTRY WITH RA4 ///////
    TRISAbits.TRISA4=1;
}

void buzzer_init(void){
    T2CON = 0x05;		
    TRISCbits.TRISC2 = 0;  
    PR2 = 0x80;		
    CCPR1L = 0x80;		
    
}

void timer0_init(){
    
    /* Initialize Counter */
    TMR0 = 0XFF;
    
    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS = 1;
    T0CONbits.T0SE = 1;
    T0CONbits.PSA = 1;

    /* Enable Interrupt Timer0 */
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    
    /* Start Timer0 */
    T0CONbits.TMR0ON = 1;    
}

void timer1_init(){

  T1CON = 0x0E;

  /* Initialize Counter */
  TMR1H=0x80;
  TMR1L=0;
  
  /* Enable Interrupt Timer1 */
  PIR1bits.TMR1IF=0;
  PIE1bits.TMR1IE=1;
  
  /* Start Timer1 */
  T1CONbits.TMR1ON = 1; 

}

void TC74_init(){
	OpenI2C(MASTER, SLEW_ON);
	SSPADD = 9;
}

void USART_init(){
    OpenUSART( USART_TX_INT_OFF  &
             USART_RX_INT_ON  &
             USART_ASYNCH_MODE &
             USART_EIGHT_BIT   &
             USART_CONT_RX     &
             USART_BRGH_HIGH,
             25 );
}

void meteo_init_IEEPROM(){

    int i;
    unsigned char Addr;
    unsigned char dataOut;

    for(i=0;i<8;i++){
        Addr = (unsigned char) i; 
        dataOut = readIEEPROM(Addr);
        if(dataOut!=0xff){
            switch(i){
                case 0:
                   hours = (int) dataOut;
                   break;
                case 1:
                   minutes = (int) dataOut; 
                   break;
                case 2:
                   PMON = (int) dataOut; 
                    break;
                case 3:
                    TSOM = (int) dataOut;
                    break;
                case 4:
                    TINA = (int) dataOut;
                    break;
                case 5:
                    ALAT = (int) dataOut; 
                    break;
                case 6:
                    ALAL = (int) dataOut;
                    break;
                case 7:
                    ALAF = (int) dataOut;
                    break; 
                default:
                    break;
            }
        }
    }
}

void meteo_init_EEEPROM(){
    
    int i, j;
    unsigned char Addrhi=0,Addrlo; 
    unsigned char dataOut;
    
    for(i=0;i<4;i++){
        for(j=0;j<5;j++){
            Addrlo = (unsigned char) 5*i+j; 
            dataOut = readEEEPROM(Addrhi,Addrlo);
            if(dataOut!=0xff){
                if(i==0 && j==3)
                    maxTemperature = (int) dataOut;
                else if(i==1 && j==3){
                    minTemperature = (int) dataOut;
                }
                else if(i==2 && j==4){
                    maxLuminosity = (int) dataOut;
                }
                else if(i==3 && j==4){
                    minLuminosity = (int) dataOut;
                }
            }           
        }
    }  
}

/******************** RESET FUNCTIONS ********************/
void timer0_reset(){
    
    /* Disenable Interrupt Timer0 */
    INTCONbits.TMR0IE = 1;

    /* Stop Timer0 */
    T0CONbits.TMR0ON = 0;  
}

/******************** Main ********************/
void main (void)
{	 
    
    int i, j;
    unsigned char dataIn=5, dataOut1 = 9, dataOut2 = 0;
    
    ///// CALL INITIALIZATION /////
    global_init();
    RA4_init();
    LCD_init();
    timer1_init();
    buzzer_init();
    RBO_init();
    TC74_init();
    USART_init();
    
    ///// CHECK PREVIOURS METEO INFO /////
    //meteo_init_IEEPROM();
    meteo_init_EEEPROM();
    
    ///// FOREGROUND PROGRAM /////
    displayInterface(mode, status);
    while(1){   
        
        if(goToSleep){
            ///// GO TO SLEEP /////
            WriteCmdXLCD(DOFF); 
            while(BusyXLCD());
            //Sleep();
        }      
        else{
            ///// UPDATE DISPLAY /////
            if(updateDisplay){displayInterface(mode,status);updateDisplay=0;}
        }
        
        ///// UPDATE EEEPROM CONTENT /////
        for(i=0;i<4;i++){
            if(updateExternalEEPROM[i]){
                for(j=0;j<5;j++){
                    Addrlo = (unsigned char) 5*i+j; 
                    writeEEEPROM(Addrhi,Addrlo,dataInEEEPROM[i][j]);
                    DelayExternalEEPROM();
                    dataOut1=readEEEPROM(Addrhi,Addrlo);// 
                }
            }
        }
        ///// UPDATE IEEPROM CONTENT /////
        if(updateIEEPROM){
            writeIEEPROM(0,hours);
            writeIEEPROM(1,minutes);
            writeIEEPROM(2,PMON);
            writeIEEPROM(3,TSOM);
            writeIEEPROM(4,TINA);
            writeIEEPROM(5,ALAT);
            writeIEEPROM(6,ALAL);
            writeIEEPROM(7,ALAF);
            updateIEEPROM=0;
        }
              
    } 
    
  return;
}