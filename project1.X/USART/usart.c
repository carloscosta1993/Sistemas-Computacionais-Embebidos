#include <xc.h>
#include "usart.h"

union USART USART_Status;

/********************************************************************
*    Function Name:  OpenUSART                                      *
*    Return Value:   void                                           *
*    Parameters:     config: bit definitions to configure USART     *
*                    spbrg: baudrate value for register SPBRG1      *
*                    or for SPBRGH:SPBRG for 16-bit baud rate       *
*                    generation for applicable parts                *
*    Description:    This routine first resets the USART regs       *
*                    to the POR state. It then configures the       *
*                    USART for interrupts, synch/async, 8/9-bit,    *
*                    sync slave/master and single/cont. rx.         *
*    Notes:          The bit definitions for config can be found    *
*                    in the usart.h file.                           *
********************************************************************/
#if defined (AUSART_V1)

void OpenUSART( unsigned char config, unsigned int spbrg)
{
  TXSTA = 0;           // Reset USART registers to POR state
  RCSTA = 0;
 
  if(config&0x01)      // Sync or async operation
    TXSTAbits.SYNC = 1;

  if(config&0x02)      // 8- or 9-bit mode
  {
    TXSTAbits.TX9 = 1;
    RCSTAbits.RX9 = 1;
  }

  if(config&0x04)      // Master or Slave (sync only)
    TXSTAbits.CSRC = 1;

  if(config&0x08)      // Continuous or single reception
    RCSTAbits.CREN = 1;
  else
    RCSTAbits.SREN = 1;

  if(config&0x10)      // Baud rate select (asychronous mode only)
    TXSTAbits.BRGH = 1;
  else
    TXSTAbits.BRGH = 0;

  PIR1bits.TXIF = 0;

  if(config&0x40)      // Interrupt on receipt
    PIE1bits.RCIE = 1;
  else
    PIE1bits.RCIE = 0;

  PIR1bits.RCIF = 0;

  if(config&0x80)      // Interrupt on transmission
    PIE1bits.TXIE = 1;
  else
    PIE1bits.TXIE = 0;

  SPBRG = spbrg;       // Write baudrate to SPBRG1
  TXSTAbits.TXEN = 1;  // Enable transmitter
  RCSTAbits.SPEN = 1;  // Enable receiver
 
	TRISCbits.TRISC6 = 0;TRISCbits.TRISC7 = 1; 
	if(TXSTAbits.SYNC && !TXSTAbits.CSRC)	//synchronous  slave mode
		TRISCbits.TRISC6 = 1;
}

#else

void OpenUSART( unsigned char config, unsigned int spbrg)
{
  TXSTA = 0;           // Reset USART registers to POR state
  RCSTA = 0;
 
  if(config&0x01)      // Sync or async operation
    TXSTAbits.SYNC = 1;

  if(config&0x02)      // 8- or 9-bit mode
  {
    TXSTAbits.TX9 = 1;
    RCSTAbits.RX9 = 1;
  }

  if(config&0x04)      // Master or Slave (sync only)
    TXSTAbits.CSRC = 1;

  if(config&0x08)      // Continuous or single reception
    RCSTAbits.CREN = 1;
  else
    RCSTAbits.SREN = 1;

  if(config&0x10)      // Baud rate select (asychronous mode only)
    TXSTAbits.BRGH = 1;
  
    PIR1bits.TXIF = 0;
	
  if(config&0x20)  // Address Detect Enable
	 RCSTAbits.ADEN = 1;
	 
  if(config&0x40)      // Interrupt on receipt
    PIE1bits.RCIE = 1;
  else
    PIE1bits.RCIE = 0;

  PIR1bits.RCIF = 0;

  if(config&0x80)      // Interrupt on transmission
    PIE1bits.TXIE = 1;
  else
    PIE1bits.TXIE = 0;

  SPBRG = spbrg;       // Write baudrate to SPBRG1
  SPBRGH = spbrg >> 8; // For 16-bit baud rate generation

  TXSTAbits.TXEN = 1;  // Enable transmitter
  RCSTAbits.SPEN = 1;  // Enable receiver

  TRISCbits.TRISC6 = 0;TRISCbits.TRISC7 = 1; 
  if(TXSTAbits.SYNC && !TXSTAbits.CSRC)	//synchronous  slave mode
    TRISCbits.TRISC6 = 1;

}

#endif

/********************************************************************
*    Function Name:  ReadUSART                                      *
*    Return Value:   char: received data                            *
*    Parameters:     void                                           *
*    Description:    This routine reads the data from the USART     *
*                    and records the status flags for that byte     *
*                    in USART_Status (Framing and Overrun).         *
********************************************************************/
char ReadUSART(void)		//this function can be removed by macro #define ReadUSART RCREG
{
  char data;   // Holds received data

  USART_Status.val &= 0xf2;          // Clear previous status flags

  if(RCSTAbits.RX9)                  // If 9-bit mode
  {
    USART_Status.RX_NINE = 0;        // Clear the receive bit 9 for USART
    if(RCSTAbits.RX9D)               // according to the RX9D bit
      USART_Status.RX_NINE = 1;
  }

  if(RCSTAbits.FERR)                 // If a framing error occured
    USART_Status.FRAME_ERROR = 1;    // Set the status bit

  if(RCSTAbits.OERR)                 // If an overrun error occured
    USART_Status.OVERRUN_ERROR = 1;  // Set the status bit

  data = RCREG;                      // Read data

  return (data);                     // Return the received data
}

/********************************************************************
*    Function Name:  WriteUSART                                     *
*    Return Value:   none                                           *
*    Parameters:     data: data to transmit                         *
*    Description:    This routine transmits a byte out the USART.   *
********************************************************************/
void WriteUSART(char data)
{
  if(TXSTAbits.TX9)  // 9-bit mode?
  {
    TXSTAbits.TX9D = 0;       // Set the TX9D bit according to the
    if(USART_Status.TX_NINE)  // USART Tx 9th bit in status reg
      TXSTAbits.TX9D = 1;
  }

  TXREG = data;      // Write the data byte to the USART
}

/********************************************************************
*    Function Name:  getsUSART                                      *
*    Return Value:   void                                           *
*    Parameters:     buffer: pointer to string                      *
*                    len: length of characters to receive           *
*    Description:    This routine receives a string of characters   *
*                    from the USART of length specified by len.     *
********************************************************************/
void getsUSART(char *buffer, unsigned char len)
{
  char i;    // Length counter
  unsigned char data;

  for(i=0;i<len;i++)  // Only retrieve len characters
  {
    while(!DataRdyUSART());// Wait for data to be received

    data = getcUSART();    // Get a character from the USART
                           // and save in the string
    *buffer = data;
    buffer++;              // Increment the string pointer
  }
}

/**********************************************************************
*    Function Name:  putsUSART                                        *
*    Return Value:   void                                             *
*    Parameters:     data: pointer to string of data                  *
*    Description:    This routine transmits a string of characters    *
*                    to the USART including the null.                 *
**********************************************************************/
void putsUSART( char *data)
{
  do
  {  // Transmit a byte
    while(BusyUSART());
    putcUSART(*data);
  } while( *data++ );
}

/**********************************************************************
*    Function Name:  putrsUSART                                       *
*    Return Value:   void                                             *
*    Parameters:     data: pointer to string of data                  *
*    Description:    This routine transmits a string of characters    *
*                    in ROM to the USART including the null.          *
**********************************************************************/
void putrsUSART(const char *data)
{
  do
  {  // Transmit a byte
    while(BusyUSART());
    putcUSART(*data);
  } while( *data++ );
}
