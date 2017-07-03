#ifndef __ADC_H
#define __ADC_H
/******************************************************************************
 *                  ADC PERIPHERAL LIBRARY HEADER FILE
 ******************************************************************************
 * FileName:        		adc.h
 * Dependencies:    	    See include below
 * Processor:       		PIC18
 * Compiler:        		MCC18
 * Company:         		Microchip Technology, Inc.
 *
 * Software License Agreement
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *****************************************************************************/

#include <xc.h>
#define ADC_V5


/***********************************************************************************
Macro       : ADC_INT_ENABLE()
Overview : Macro enables the ADC interrupt
Parameters   : None
Remarks     : Interrupt is configured as low priority interrupt
***********************************************************************************/
#define ADC_INT_ENABLE()     (PIR1bits.ADIF=0,INTCONbits.PEIE=1,PIE1bits.ADIE=1) 

/***********************************************************************************
Macro       : ADC_INT_DISABLE()
Overview : Macro disables the ADC interrupt
Parameters   : None
Remarks     : None
***********************************************************************************/
#define ADC_INT_DISABLE()    (PIE1bits.ADIE=0)

#ifndef USE_OR_MASKS
//*************** A/D Conversion Clock Selection *****************************
#define ADC_FOSC_2       0b10001111 			//A/D conversion clock source is Fosc/2
#define ADC_FOSC_4       0b11001111 			//A/D conversion clock source is Fosc/4
#define ADC_FOSC_8       0b10011111 			//A/D conversion clock source is Fosc/8
#define ADC_FOSC_16      0b11011111 			//A/D conversion clock source is Fosc/16
#define ADC_FOSC_32      0b10101111 			//A/D conversion clock source is Fosc/32
#define ADC_FOSC_64      0b11101111 			//A/D conversion clock source is Fosc/64
#define ADC_FOSC_RC      0b11111111 			//A/D conversion clock source is Internal RC OSC

//************** A/D Acquisition Time Selection *******************************
#define ADC_0_TAD        0b11110001				//A/D Acquisition Time is 0 TAD
#define ADC_2_TAD        0b11110011				//A/D Acquisition Time is 2 TAD
#define ADC_4_TAD        0b11110101				//A/D Acquisition Time is 4 TAD
#define ADC_6_TAD        0b11110111				//A/D Acquisition Time is 6 TAD
#define ADC_8_TAD        0b11111001				//A/D Acquisition Time is 8 TAD
#define ADC_12_TAD       0b11111011				//A/D Acquisition Time is 12 TAD
#define ADC_16_TAD       0b11111101				//A/D Acquisition Time is 16 TAD
#define ADC_20_TAD       0b11111111				//A/D Acquisition Time is 20 TAD

//*************** ADC Interrupt Enable/Disable *******************************
#define ADC_INT_ON       0b11111111				//A/D Interrupt Enable
#define ADC_INT_OFF      0b01111111				//A/D Interrupt Disable

#else // USE_OR_MASKS
//*************** A/D Conversion Clock Select *****************************
#define ADC_FOSC_2       0b00000000 			//A/D conversion clock source is Fosc/2
#define ADC_FOSC_4       0b01000000 			//A/D conversion clock source is Fosc/4
#define ADC_FOSC_8       0b00010000 			//A/D conversion clock source is Fosc/8
#define ADC_FOSC_16      0b01010000 			//A/D conversion clock source is Fosc/16
#define ADC_FOSC_32      0b00100000 			//A/D conversion clock source is Fosc/32
#define ADC_FOSC_64      0b01100000 			//A/D conversion clock source is Fosc/64
#define ADC_FOSC_RC      0b01110000 			//A/D conversion clock source is Internal RC OSC
#define ADC_FOSC_MASK	 (~ADC_FOSC_RC)			//Mask A/D conversion clock source bits

//************** A/D Acquisition Time Selection *******************************
#define ADC_0_TAD        0b00000000				//A/D Acquisition Time is 0 TAD
#define ADC_2_TAD        0b00000010				//A/D Acquisition Time is 2 TAD
#define ADC_4_TAD        0b00000100				//A/D Acquisition Time is 4 TAD
#define ADC_6_TAD        0b00000110				//A/D Acquisition Time is 6 TAD
#define ADC_8_TAD        0b00001000				//A/D Acquisition Time is 8 TAD
#define ADC_12_TAD       0b00001010				//A/D Acquisition Time is 12 TAD
#define ADC_16_TAD       0b00001100				//A/D Acquisition Time is 16 TAD
#define ADC_20_TAD       0b00001110				//A/D Acquisition Time is 20 TAD
#define ADC_TAD_MASK	(~ADC_20_TAD)			//Mask A/D Acquisition Time bits

//*************** ADC Interrupt Enable/Disable *******************************
#define ADC_INT_ON       0b10000000				//A/D Interrupt Enable
#define ADC_INT_OFF      0b00000000				//A/D Interrupt Disable
#define ADC_INT_MASK     (~ADC_INT_ON)			//Mask A/D Interrupt
#endif	// USE_OR_MASKS


#if defined (ADC_V5)

#ifndef USE_OR_MASKS
//************** Voltage Reference Configuration ****************************** 
#define ADC_REF_VDD_VREFMINUS   	0b11111110 	// ADC voltage source VREF+ = VDD and VREF- = ext.source at VREF-
#define ADC_REF_VREFPLUS_VREFMINUS  0b11111111 	// ADC voltage source VREF+ = ext.source at VREF+ and VREF- = ext.source at VREF-
#define ADC_REF_VREFPLUS_VSS		0b11111101 	// ADC voltage source VREF+ = ext.source at VREF+ and VREF- = VSS
#define	ADC_REF_VDD_VSS  			0b11111100 	// ADC voltage source VREF+ = VDD and VREF- = VSS
#else // USE_OR_MASKS	
//************** Voltage Reference Configuration ****************************** 
#define ADC_REF_VDD_VREFMINUS   	0b00000010 	// ADC voltage source VREF+ = VDD and VREF- = ext.source at VREF-
#define ADC_REF_VREFPLUS_VREFMINUS  0b00000011 	// ADC voltage source VREF+ = ext.source at VREF+ and VREF- = ext.source at VREF-
#define ADC_REF_VREFPLUS_VSS		0b00000001 	// ADC voltage source VREF+ = ext.source at VREF+ and VREF- = VSS
#define	ADC_REF_VDD_VSS  		    0b00000000 	// ADC voltage source VREF+ = VDD and VREF- = VSS
#define ADC_REF_MASK	            (~ADC_REF_VREFPLUS_VREFMINUS)	//Mask ADC Voltage Source
#endif	// USE_OR_MASKS

#endif	// End of versions

#ifndef	USE_OR_MASKS
//*************** A/D Result Format Select ***********************************
#define ADC_RIGHT_JUST   0b11111111 			// Right justify A/D result
#define ADC_LEFT_JUST    0b01111111 			// Left justify A/D result
#else // USE_OR_MASKS
//*************** A/D Result Format Select ***********************************
#define ADC_RIGHT_JUST   0b10000000 			// Right justify A/D result
#define ADC_LEFT_JUST    0b00000000 			// Left justify A/D result
#define ADC_RESULT_MASK	 (~ADC_RIGHT_JUST)		//Mask ADC Result adjust bit
#endif // USE_OR_MASKS


#if defined (ADC_V1)

#ifndef	USE_OR_MASKS
//***************A/D Port Configuration Control *******************************
#define ADC_8ANA_0REF    0b11110000				// VREF+=VDD,VREF-=VSS: all analog channels   (8 analog channels/0 Voltage Reference)
#define ADC_7ANA_1REF    0b11110001				// AN3=VREF+:  all analog channels except AN3 (7 analog channels/1 Voltage Reference)
#define ADC_5ANA_0REF    0b11110010 			// VREF+=VDD,VREF-=VSS: DIG-AN7,6,5 : ANG-AN4,3,2,1,0  (5 analog channels/0 Voltage Reference)
#define ADC_4ANA_1REF    0b11110011 			// AN3=VREF+:  DIG- AN7,6,5 : ANG- AN4,2,1,0  (4 analog channels/1 Voltage Reference)
#define ADC_3ANA_0REF    0b11110100 			// VREF+=VDD,VREF-=VSS: DIG- AN7,6,5,4,2 : ANG- AN3,1,0   (3 analog channels/0 Voltage Reference)
#define ADC_2ANA_1REF    0b11110101 			// AN3=VREF+:  DIG- AN7,6,5,4,2 : ANG- AN1,0   (2 analog channels/1 Voltage Reference)
#define ADC_0ANA_0REF    0b11110111 			// ALL DIGITAL Channels  (0 analog channels/0 Voltage Reference)
#define ADC_6ANA_2REF    0b11111000 			// AN3=VREF+,AN2=VREF-:  ANG- AN7,6,5,4,1,0  (6 analog channels/2 Voltage Reference)
#define ADC_6ANA_0REF    0b11111001 			// VREF+=VDD,VREF-=VSS:  DIG- AN7,6 : ANG- AN5,4,3,2,1,0  (6 analog channels/0 Voltage Reference)
#define ADC_5ANA_1REF    0b11111010 			// AN3=VREF+,VREF-=VSS:  DIG- AN7,6 : ANG- AN5,4,2,1,0  (5 analog channels/1 Voltage Reference)
#define ADC_4ANA_2REF    0b11111011 			// AN3=VREF+ AN2=VREF-:   DIG- AN7,6 : ANG- AN5,4,1,0 (4 analog channels/2 Voltage Reference)
#define ADC_3ANA_2REF    0b11111100 			// AN3=VREF+ AN2=VREF-:  DIG- AN7,6,5 : ANG- AN4,1,0  (3 analog channels/2 Voltage Reference)
#define ADC_2ANA_2REF    0b11111101 			// AN3=VREF+ AN2=VREF-:   DIG- AN7,6,5,4 : ANG- AN1,0  (2 analog channels/2 Voltage Reference)
#define ADC_1ANA_0REF    0b11111110 			// AN0 is the only analog input  (1 analog channels/0 Voltage Reference)
#define ADC_1ANA_2REF    0b11111111 			// AN3=VREF+ AN2=VREF-:   DIG- AN7,6,5,4,1 : ANG- AN0   (1 analog channels/2 Voltage Reference)
#else // USE_OR_MASKS
//***************A/D Port Configuration Control *******************************
#define ADC_8ANA_0REF    0b00000000				// VREF+=VDD,VREF-=VSS: all analog channels   (8 analog channels/0 Voltage Reference)
#define ADC_7ANA_1REF    0b00000001				// AN3=VREF+:  all analog channels except AN3 (7 analog channels/1 Voltage Reference)
#define ADC_5ANA_0REF    0b00000010 			// VREF+=VDD,VREF-=VSS: DIG-AN7,6,5 : ANG-AN4,3,2,1,0  (5 analog channels/0 Voltage Reference)
#define ADC_4ANA_1REF    0b00000011 			// AN3=VREF+:  DIG- AN7,6,5 : ANG- AN4,2,1,0  (4 analog channels/1 Voltage Reference)
#define ADC_3ANA_0REF    0b00000100 			// VREF+=VDD,VREF-=VSS: DIG- AN7,6,5,4,2 : ANG- AN3,1,0   (3 analog channels/0 Voltage Reference)
#define ADC_2ANA_1REF    0b00000101 			// AN3=VREF+:  DIG- AN7,6,5,4,2 : ANG- AN1,0   (2 analog channels/1 Voltage Reference)
#define ADC_0ANA_0REF    0b00000111 			// ALL DIGITAL Channels  (0 analog channels/0 Voltage Reference)
#define ADC_6ANA_2REF    0b00001000 			// AN3=VREF+,AN2=VREF-:  ANG- AN7,6,5,4,1,0  (6 analog channels/2 Voltage Reference)
#define ADC_6ANA_0REF    0b00001001 			// VREF+=VDD,VREF-=VSS:  DIG- AN7,6 : ANG- AN5,4,3,2,1,0  (6 analog channels/0 Voltage Reference)
#define ADC_5ANA_1REF    0b00001010 			// AN3=VREF+,VREF-=VSS:  DIG- AN7,6 : ANG- AN5,4,2,1,0  (5 analog channels/1 Voltage Reference)
#define ADC_4ANA_2REF    0b00001011 			// AN3=VREF+ AN2=VREF-:   DIG- AN7,6 : ANG- AN5,4,1,0 (4 analog channels/2 Voltage Reference)
#define ADC_3ANA_2REF    0b00001100 			// AN3=VREF+ AN2=VREF-:  DIG- AN7,6,5 : ANG- AN4,1,0  (3 analog channels/2 Voltage Reference)
#define ADC_2ANA_2REF    0b00001101 			// AN3=VREF+ AN2=VREF-:   DIG- AN7,6,5,4 : ANG- AN1,0  (2 analog channels/2 Voltage Reference)
#define ADC_1ANA_0REF    0b00001110 			// AN0 is the only analog input  (1 analog channels/0 Voltage Reference)
#define ADC_1ANA_2REF    0b00001111 			// AN3=VREF+ AN2=VREF-:   DIG- AN7,6,5,4,1 : ANG- AN0   (1 analog channels/2 Voltage Reference)
#define ADC_CONFIG_MASK	 (~ADC_1ANA_2REF)		//Mask ADC port configuration bits
#endif // USE_OR_MASKS

#elif defined (ADC_V5)

#ifndef USE_OR_MASKS
//***************A/D Port Configuration Control *******************************
#define ADC_0ANA   0b11111111 					// All channels are digital
#define ADC_1ANA   0b11111110 					// analog: AN0 and remaining channels are digital
#define ADC_2ANA   0b11111101 					// analog: AN0->1 and remaining channels are digital
#define ADC_3ANA   0b11111100 					// analog: AN0->2 and remaining channels are digital
#define ADC_4ANA   0b11111011 					// analog: AN0->3 and remaining channels are digital
#define ADC_5ANA   0b11111010 					// analog: AN0->4 and remaining channels are digital
#define ADC_6ANA   0b11111001 					// analog: AN0->5 and remaining channels are digital
#define ADC_7ANA   0b11111000 					// analog: AN0->6 and remaining channels are digital
#define ADC_8ANA   0b11110111 					// analog: AN0->7 and remaining channels are digital
#define ADC_9ANA   0b11110110 					// analog: AN0->8 and remaining channels are digital
#define ADC_10ANA  0b11110101 					// analog: AN0->9 and remaining channels are digital
#define ADC_11ANA  0b11110100 					// analog: AN0->10 and remaining channels are digital
#ifndef ADC_V12
#define ADC_12ANA  0b11110011 					// analog: AN0->11 and remaining channels are digital
#define ADC_13ANA  0b11110010 					// analog: AN0->12 and remaining channels are digital
#define ADC_14ANA  0b11110001 					// analog: AN0->13 and remaining channels are digital
#define ADC_15ANA  0b11110000 					// All channels are analog
#endif
#else // USE_OR_MASKS
//***************A/D Port Configuration Control *******************************
#define ADC_0ANA   0b00001111 					// All channels are digital
#define ADC_1ANA   0b00001110 					// analog: AN0  and remaining channels are digital
#define ADC_2ANA   0b00001101 					// analog: AN0->1 and remaining channels are digital
#define ADC_3ANA   0b00001100 					// analog: AN0->2 and remaining channels are digital
#define ADC_4ANA   0b00001011 					// analog: AN0->3 and remaining channels are digital
#define ADC_5ANA   0b00001010 					// analog: AN0->4 and remaining channels are digital
#define ADC_6ANA   0b00001001 					// analog: AN0->5 and remaining channels are digital
#define ADC_7ANA   0b00001000 					// analog: AN0->6 and remaining channels are digital
#define ADC_8ANA   0b00000111 					// analog: AN0->7 and remaining channels are digital
#define ADC_9ANA   0b00000110 					// analog: AN0->8 and remaining channels are digital
#define ADC_10ANA  0b00000101 					// analog: AN0->9 and remaining channels are digital
#define ADC_11ANA  0b00000100 					// analog: AN0->10 and remaining channels are digital
#ifndef ADC_V12
#define ADC_12ANA  0b00000011 					// analog: AN0->11 and remaining channels are digital
#define ADC_13ANA  0b00000010 					// analog: AN0->12 and remaining channels are digital
#define ADC_14ANA  0b00000001 					// analog: AN0->13 and remaining channels are digital
#define ADC_15ANA  0b00000000 					// All channels are analog
#endif
#define ADC_CONFIG_MASK  (~ADC_0ANA)			//Mask ADC port configuration bits
#endif // USE_OR_MASKS


#endif		// End of versions



#if	defined (ADC_V1) || defined (ADC_V2) || defined (ADC_V3) || defined (ADC_V4) ||\
    defined (ADC_V5) || defined (ADC_V6) || defined (ADC_V8) || defined (ADC_V9) || defined (ADC_V9_1) ||\
	defined (ADC_V10) || defined (ADC_V11) || defined (ADC_V12) || defined (ADC_V11_1) 
#ifndef USE_OR_MASKS
//**************** channel selection ******************************************
#define ADC_CH0          0b10000111  			//Select Channel 0
#define ADC_CH1          0b10001111  			//Select Channel 1
#define ADC_CH2          0b10010111  			//Select Channel 2
#define ADC_CH3          0b10011111  			//Select Channel 3
#define ADC_CH4          0b10100111  			//Select Channel 4
#define ADC_CH5          0b10101111  			//Select Channel 5
#define ADC_CH6          0b10110111  			//Select Channel 6
#define ADC_CH7          0b10111111  			//Select Channel 7
#define ADC_CH8          0b11000111  			//Select Channel 8
#define ADC_CH9          0b11001111  			//Select Channel 9
#define ADC_CH10         0b11010111  			//Select Channel 10
#define ADC_CH11         0b11011111  			//Select Channel 11
#define ADC_CH12         0b11100111  			//Select Channel 12
#if defined (ADC_V11) || defined (ADC_V11_1) 
#define ADC_CH_CTMU      0b11101111  			// All analog inputs are off
#define ADC_CH_VDDCORE   0b11110111  			// VDDCORESelect Channel
#define ADC_CH_VBG       0b11111111  			// Voltage Band gapSelect Channel
#else
#define ADC_CH13         0b11101111  			//Select Channel 13
#define ADC_CH14         0b11110111  			//Select Channel 14
#define ADC_CH15         0b11111111  			//Select Channel 15
#endif

#else // USE_OR_MASKS

//**************** channel selection ******************************************
#define ADC_CH0          0b00000000  			//Select Channel 0
#define ADC_CH1          0b00001000  			//Select Channel 1
#define ADC_CH2          0b00010000  			//Select Channel 2
#define ADC_CH3          0b00011000  			//Select Channel 3
#define ADC_CH4          0b00100000  			//Select Channel 4
#define ADC_CH5          0b00101000  			//Select Channel 5
#define ADC_CH6          0b00110000  			//Select Channel 6
#define ADC_CH7          0b00111000  			//Select Channel 7
#define ADC_CH8          0b01000000  			//Select Channel 8
#define ADC_CH9          0b01001000  			//Select Channel 9
#define ADC_CH10         0b01010000  			//Select Channel 10
#define ADC_CH11         0b01011000  			//Select Channel  11
#define ADC_CH12         0b01100000  			//Select Channel 12
#if defined (ADC_V11) || defined (ADC_V11_1) 
#define ADC_CH_CTMU      0b01101000  			// All analog inputs are off - CTMU
#define ADC_CH_VDDCORE   0b01110000  			// VDDCORESelect Channel
#define ADC_CH_VBG       0b01111000  			// Voltage Band gapSelect Channel
#define ADC_CH_MASK		 (~ADC_CH_VBG)			//Mask ADC channel selection bits
#elif defined (ADC_V10)
#define ADC_CH_MASK		 (~FVR1)			//Mask ADC channel selection bits
#else
#define ADC_CH13         0b01101000  			//Select Channel 13
#define ADC_CH14         0b01110000  			//Select Channel 14
#define ADC_CH15         0b01111000  			//Select Channel 15
#define ADC_CH_MASK		 (~ADC_CH15)			//Mask ADC channel selection bits
#endif

#endif // USE_OR_MASKS
#endif


/* Stores the result of an A/D conversion. */
union ADCResult
{
	int lr;			//holds the 10-bit ADC Conversion value as integer
 	char br[2];		//holds the 10-bit ADC Conversion value as two byte values
};

char BusyADC (void);

void ConvertADC (void);

void CloseADC(void);

int ReadADC(void);

#if defined (ADC_V1)

void OpenADC ( unsigned char ,
               unsigned char );

#elif defined (ADC_V5)

void OpenADC ( unsigned char ,
               unsigned char ,
               unsigned char );
		   
#endif

 
void SetChanADC(unsigned char );



#endif
