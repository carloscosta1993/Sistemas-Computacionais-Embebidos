#include <xc.h>
#include "adc.h"

#if defined (ADC_V1)
void OpenADC( unsigned char config, 
              unsigned char config2)
{
  ADCON0 = 0;

  ADCON1 = config;
  ADCON0 = ((config<<2) & 0xC0)|(config2 & 0b00111000);
  if( config2 & 0b10000000 )
  {
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
  }
  ADCON0bits.ADON = 1;
}


#elif defined (ADC_V5)
void OpenADC( unsigned char config,
              unsigned char config2,
              unsigned char portconfig)
{
    ADCON0 = 0;
    ADCON2 = 0;

    ADCON0 = (config2 >> 1) & 0b00111100;	// GET CHANNELS 
    ADCON1 = (portconfig & 0b00001111) |                   // GET CONFIG
	         ((config2 << 4) & 0b00110000); //GET  Vref
    ADCON2 = (config & 0b10000000)	|	// RESULT
             ((config >> 4) & 0b00000111) |	// CONV CLK
             ((config << 2) & 0b00111000);	// ACQ TIME

    if( config2 & 0b10000000 )			// ADC INT.
    {
      PIR1bits.ADIF = 0;
      PIE1bits.ADIE = 1;
      INTCONbits.PEIE = 1;
    }
    ADCON0bits.ADON = 1;
}

#endif

/*************************************************************************************
Function:       char BusyADC(void)

Overview:    This function returns the ADC conversion status

Parameters:  None

Returns:        If the value of GO is 1, then 1 is returned,indicating that the
	           ADC is busy in conversion. If the value of GO is 0, then 0 is
                      returned, indicating that the ADC has completed conversion.

Remarks:     This function returns the complement of the ADCON0 <GO/~DONE> bit
                     status which indicates whether the ADC is busy in conversion.
            
***************************************************************************************/
char BusyADC(void)
{
  return(ADCON0bits.GO);
}

/*************************************************************************************
Function :       void CloseADC(void)

Overview :    This function turns off the ADC module and disables the ADC interrupts

Parameters :  None

Returns :        None

Remarks :     This function first disables the ADC interrupt and then turns off the ADC
		module.The Interrupt Flag bit (ADIF) is also cleared.
            
***************************************************************************************/
void CloseADC(void)
{
  ADCON0bits.ADON = 0;
  PIE1bits.ADIE = 0;
}

/************************************************************************************
Function       : void ConvertADC(void)
 
Overview     : This function starts the A/D conversion.
 
Parameters   : None

Returns         : None

Remarks        : This function sets the ADCON0<GO> bit and thus
		    starts conversion.
**************************************************************************************/ 
void ConvertADC(void)
{
  ADCON0bits.GO = 1;
}

/*************************************************************************************
Function:       int ReadADC(void)

Overview:    This function reads the ADC Buffer register which contains
                      the conversion value.

Parameters:  None

Returns:        Returns the ADC Buffer value

Remarks:     None
            
***************************************************************************************/	
int ReadADC(void)
{
  return (((unsigned int)ADRESH)<<8)|(ADRESL);
}
