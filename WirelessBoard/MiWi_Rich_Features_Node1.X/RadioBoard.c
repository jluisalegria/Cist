/* 
 * File:   main.c
 * Author: Lap
 *
 * Created on 7 de abril de 2015, 03:29 PM
 */


/*

 * dispositivos soportados:  128
 * rango:                    0 - 127
 *
 */

////////////////////////////////////////////////////////////////////////////////

/* ----j2 pins */  

// 1.- VDD
// 2.-  
// 3.- RX
// 4.- TX
// 5.- GND


//#define RADIO_BOARD


#include <p18cxxx.h>
#include "types.h"
#include "ConfigApp.h"
#include "gpios/gpios.h"
#include "spi/spi.h"
#include "st7032/st7032.h"
#include "eeprom25xx/eeprom25xx.h"
#include "uart1/uart1.h"
#include "uart1/uart2.h"
#include "printf/printf.h"
#include "delays/delays.h"
#include "system/system.h"
#include <pps.h>
#include <usart.h>
#include <timers.h>
#include "WirelessProtocols/NVM.h"
#include "MAC_EEProm.h"
#include "ConfigApp.h"
#include "WirelessProtocols/MCHP_API.h"
#include "WirelessProtocols/Console.h"
//#include "sw_i2c.h"

//_U08 Rs485Buff[24];
_U08 Index;

extern BYTE myLongAddress[];



#define C_CHANNEL            16
#define PCDevices            64

const _U08 StartCmd        = 0x23; 
const _U08 EndCmd          = 0x3F;

const _U08 Resync          = 0x0A;
const _U08 OutputStat      = 0x0B;
const _U08 ListenDevices   = 0x0C;
const _U08 CalibrationMode = 0x0D;

const _U32 CalibrationAddress    = 130048;
const _U32 ToleranceValueAddress = 130338;

_U08 myNodeNumber = 0;

_U08 myDevicesRequiredStatus[16];
_U08 ConnectedDevices[16];
//_U08 C1DevicesOutputStatus[4];
//_U08 C2DevicesOutputStatus[4];

_U08 MyDevicesTemp[PCDevices];

BOOL MessageToSend = 0;
_U08 MessageType   = 0;
BYTE BuffIndex = 0;

_U16 _U16Var;
_U32 U32EEADR;


#if ADDITIONAL_NODE_ID_SIZE > 0
    BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {0x08};
#endif
  

BYTE myChannel = C_CHANNEL;
_U08 Tick,TickScaler;
_U08 SwTimer0,SwTimer1,SwTimer2,SwTimer3,SwTimer4;
_U08 BaseTimeToTx = 4;
_U08 MyTrackingNumber;
_U08 ConnectedCoordinators;

_U08 PanCoordStateMachine = 0;

_U08 i, j;

_U08 UartBuff[32];
_U08 Uart2RXState;

typedef struct
{
    _U08 Byte;
    _U08 Bit;
}_ByteBit;

_ByteBit ByteBitVal;

//********************************************************************//
void StartWirelessConnection(void)
 {
    Read_MAC_Address();

    LED_1 = 0;
    LED_2 = 0;


 if(myNodeNumber == 0) // if the node are PAN coordinator
 {
  MiApp_ProtocolInit(FALSE);

  if( MiApp_SetChannel(myChannel) == FALSE )
  {
      return;
  }

  MiApp_ConnectionMode(ENABLE_ALL_CONN);

  MiApp_ProtocolInit(FALSE);
  MiApp_StartConnection(START_CONN_DIRECT, 0, 0);

 }
 else
 {

  MiApp_ProtocolInit(FALSE);

    // Force PANID & SHORT ADDRESS

  if( MiApp_SetChannel(myChannel) == FALSE )
  {
   return;
  }

  MiApp_ConnectionMode(ENABLE_ALL_CONN);

  myPANID.v[0] = 0x34;
  myPANID.v[1] = 0x12;

  myShortAddress.v[0] = 0x00;
  myShortAddress.v[1] = myNodeNumber;

  myChannel = C_CHANNEL;
  //role = ROLE_COORDINATOR;

  if( MiApp_SetChannel(myChannel) == FALSE )
  {
   return;
  }

 }
}
//********************************************************************//

 void Timer1Tick(void)
 {
   _U16 Scaler;

  if(PIR1bits.TMR1IF)
  {
    PIR1bits.TMR1IF = 0;
    //WriteTimer1(9286);           // overflow every 150 ms
    
    //WriteTimer1(28036);          // overflow every 100 ms
    //WriteTimer1(28036);          // overflow every 50 ms
    WriteTimer1(56161);          // overflow every 25 ms
    
    ++Tick;

    if(myNodeNumber == 0)
    {
      
    }
    else
    {

    }

    TickScaler = BaseTimeToTx;
    switch(TickScaler)
    {
        case 0:  Scaler = 10;    break;
        case 1:  Scaler = 1;     break;     // 25 ms
        case 2:  Scaler = 2;     break;     // 50 ms
        case 3:  Scaler = 4;     break;     // 100 ms
        case 4:  Scaler = 8;     break;     // 200 ms
        case 5:  Scaler = 16;    break;     // 400 ms
        case 6:  Scaler = 32;    break;     // 800 ms
        case 7:  Scaler = 64;    break;     // 1.6 s
        case 8:  Scaler = 128;   break;
        case 9:  Scaler = 256;   break;
        case 10: Scaler = 512;   break;
        case 11: Scaler = 1024;  break;
        case 12: Scaler = 2048;  break;
        case 13: Scaler = 4096;  break;
        case 14: Scaler = 8192;  break;
        case 15: Scaler = 16384; break;
        case 16: Scaler = 32768; break;
        default: Scaler = 8;     break;
    }



    if(Tick >= Scaler)
    {
     Tick =0;
     ++SwTimer0;
     ++SwTimer1;
     ++SwTimer2;
     ++SwTimer3;     // MIWIPRO STATUS TIMER
     ++SwTimer4;
    }
   }
 }

//****************************************************************************//

 void Rs485Put(_U08 c)
{
    while(!TXSTA1bits.TRMT);
    TXREG1 = c;
}


_U08 Rs485Get(void)
{
    _U16 timeout =0;

    while((!PIR1bits.RC1IF)&&(++timeout)<5000);
    //while((!PIR1bits.RC1IF));
    if(PIR1bits.RC1IF)
    {
        if(RCSTA1bits.OERR)
        {
            RCSTA1bits.CREN = 0;   // Disable UART receiver
            RCSTA1bits.CREN = 1;   // Enable UART receiver
        }

        return RCREG1;
    }
    else
    {
         return 0xFF;
    }
}



void Rs485GetString(_U08 *dest,  _U16 count)
{
 _U08 oldGIEH = INTCONbits.GIEH;
 INTCONbits.GIEH = 0;

 while( count > 0 )
 {
  *dest++ = Rs485Get();
  count--;
 }

  INTCONbits.GIEH = oldGIEH;

}
//****************************************************************************// 
void RadioTask(void)
{
    
    _U08 DevStatusTemp;
    
    if( MiApp_MessageAvailable())   
    {
        if((rxMessage.Payload[1] <= PCDevices)&&((rxMessage.Payload[3] == OutputStat))&&((rxMessage.Payload[4] <= 1))&&((rxMessage.Payload[5] <= 200)))
        {
            DevStatusTemp = rxMessage.Payload[5];
            if(rxMessage.Payload[4]==1)
            {
                SET_8BIT(DevStatusTemp,7);
            }
            else
            {
                CLEAR_8BIT(DevStatusTemp,7);
            }
            
            ConsolePut(StartCmd);
            ConsolePut(OutputStat);
            ConsolePut(rxMessage.Payload[1]);
            ConsolePut(DevStatusTemp);
            ConsolePut(EndCmd);
            
            ClrWdt();
            LED_1 ^=1;
        }    
        
        MiApp_DiscardMessage();
    }
    
 
    
    if(MessageToSend)
    {
        MessageToSend = 0;
        
        ClrWdt();
        
        switch(MessageType)
        {
            case 1:          // envia nuevos estados para dispositivos
            {
             ++MyTrackingNumber;
             
             if(UartBuff[0] <= 16)
             {
                 BaseTimeToTx = UartBuff[0];
             }
             

             MiApp_FlushTx();
             MiApp_WriteData(' ');                 // 0
             MiApp_WriteData(myNodeNumber);        // 1
             MiApp_WriteData(MyTrackingNumber);    // 2
             MiApp_WriteData(OutputStat);          // 3
             MiApp_WriteData(BaseTimeToTx);        // 4

             for(j=1;j<17;j++)
             {
              MiApp_WriteData(UartBuff[j]);
              
             }
             
             MiApp_BroadcastPacket(FALSE);
             
             LED_2 ^=1;
            }
            break;
            
            case 2:          // envia dispositivos que se estan escuchando
            {
             MiApp_FlushTx();
             MiApp_WriteData(' ');                 // 0
             MiApp_WriteData(myNodeNumber);        // 1
             MiApp_WriteData(MyTrackingNumber);    // 2
             MiApp_WriteData(ListenDevices);       // 3

             for(j=0;j<16;j++)
             {
              MiApp_WriteData(UartBuff[j]);
             }
             
             MiApp_BroadcastPacket(FALSE);   
             
             LED_2 ^=1;
            }
            break;
            
            default: break;
        }
    }
    
    
    
}
/*----------------------------------------------------------------------------*/

void main(void)
{
     #define BAUDRG 77

    //BYTE SecNum = 0;
    /*******************************************************************/
    // Initialize the system
    /*******************************************************************/

    ANCON0 = 0XFF;     /*desactiva entradas analogicas*/
    ANCON1 = 0XFF;     /*desactiva entradas analogicas*/

    PPSUnLock();

    //PPSOutput(PPS_RP6, PPS_TX2CK2);    // TX2 RP17/RC6   ethernet
    //PPSInput(PPS_RX2DT2, PPS_RP5);     // RX2 RP18/RC7

    PPSOutput(PPS_RP10, PPS_TX2CK2);  // TX2 RP17/RC6     icsp
    PPSInput(PPS_RX2DT2, PPS_RP9);    // RX2 RP18/RC7

    PPSOutput(PPS_RP23, PPS_SDO2);    // SDO2 RP23/RD6
    PPSInput(PPS_SDI2, PPS_RP24);     // SDI2 RP24/RD7
    PPSOutput(PPS_RP22, PPS_SCK2);    // SCK2 RP22/RD5

    PPSLock();

     System_PeripheralPinSelect( ExternalInterrupt3, 19);  /*external interrupt 3 B3*/

     BoardInit();
     ConsoleInit();

     Gpios_PinDirection(GPIOS_PORTC, 7, GPIOS_INPUT);  /*pin C0 como salida para SDI*/
     Gpios_PinDirection(GPIOS_PORTC, 6, GPIOS_OUTPUT); /*pin C1 como salida para SDO*/

     Open1USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_EIGHT_BIT & USART_ASYNCH_MODE & USART_ADDEN_OFF, BAUDRG);
     baud1USART(BAUD_IDLE_TX_PIN_STATE_HIGH & BAUD_IDLE_RX_PIN_STATE_HIGH & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE & USART_RX_INT_OFF);

     Open2USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_EIGHT_BIT & USART_ASYNCH_MODE & USART_ADDEN_OFF, BAUDRG);
     baud2USART(BAUD_IDLE_TX_PIN_STATE_HIGH & BAUD_IDLE_RX_PIN_STATE_HIGH & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE & USART_RX_INT_ON);

     OpenTimer1( TIMER_INT_OFF &T1_16BIT_RW &T1_SOURCE_FOSC_4 & T1_PS_1_8 &T1_OSC1EN_OFF &T1_SYNC_EXT_OFF, TIMER_GATE_OFF  & TIMER_GATE_INT_OFF);

     Gpios_PinDirection(GPIOS_PORTD, 7, GPIOS_INPUT);  /*pin C0 como salida para SDI*/
     Gpios_PinDirection(GPIOS_PORTD, 6, GPIOS_OUTPUT); /*pin C1 como salida para SDO*/
     Gpios_PinDirection(GPIOS_PORTD, 5, GPIOS_OUTPUT); /*pin C2 como salida para SCK*/

     Spi_Init(SPI_PORT1, SPI_64DIV); /*Inicializamos SPI2*/
     Spi_Init(SPI_PORT2, SPI_64DIV); /*Inicializamos SPI2*/

     LED_1 = 1;
     LED_2 = 0;

     for(j=0;j<10;j++)
     {
         DelayMs(50);

         LED_1 ^= 1;
         LED_2 ^= 1;
     }

     LED_1 = 0;
     LED_2 = 0;
  

     StartWirelessConnection();
     TickScaler = 2;

     for(j=0;j<16;j++)
     {
      ConnectedDevices[j]= 0;
      myDevicesRequiredStatus[j] = 0xff;
     }

     for(j=0;j<PCDevices;j++)
     {
         MyDevicesTemp[j] = 28;       /* inicializa temperaturas*/
     }
     
     Uart2RXState = 0;
 
     //ConsolePutROMString("\r\n Radio Board");
     RCONbits.IPEN   = 1;
     PIE3bits.RC2IE = 1;
     
     
     while(1)
     {
      Timer1Tick();
      RadioTask();
      //WirelessTxRx();
     }

}