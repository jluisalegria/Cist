/********************************************************************
* FileName:		MiWi PRO UNISON FIXED NWK.c
* Dependencies: none   
* Processor:	PIC18,
*               tested with 18F47j13, dsPIC33FJ256GP710
* Hardware:     FI Wireless Board
* Complier:     Microchip C18 v3.04 or higher
*				
*               Microchip C32 v1.11 or higher	
* Company       Futuro Inteligente
*
* Copyright and Disclaimer 
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
* KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
* WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
* PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
* LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
* CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
* DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
* ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
* LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
* TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
* NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
*********************************************************************
* File Description:
*
*  este archivo contiene la interfaz miwi pro
*  se agrega soporte para wiz107sr
*  
*
* Change History:
*  Rev   Date         Author    Description
*  4.1   06/01/2011   yfy       Initial release
********************************************************************/

/************************ HEADERS **********************************/

#include <p18cxxx.h>
#include "types.h"
#include "ConfigApp.h"
#include "gpios/gpios.h"
#include "spi/spi.h"
#include "st7032/st7032.h"
#include "uart1/uart1.h"
#include "uart1/uart2.h"
#include "printf/printf.h"
#include "delays/delays.h"
#include "system/system.h"
#include <pps.h>
#include <usart.h>
#include "WirelessProtocols/NVM.h"
#include "MAC_EEProm.h"
#include "SST_SerialFlash.h"
#include "ConfigApp.h"
#include "WirelessProtocols/MCHP_API.h"
#include "WirelessProtocols/Console.h"
#include "DemoOutput.h"
#include "hardware_profile.h"


/************************ VARIABLES ********************************/

/*******************************************************************/
// AdditionalConnectionPayload variable array defines the additional 
// information to identify a device on a P2P connection. This array
// will be transmitted with the P2P_CONNECTION_REQUEST command to 
// initiate the connection between the two devices. Along with the 
// long address of this device, this  variable array will be stored 
// in the P2P Connection Entry structure of the partner device. The 
// size of this array is ADDITIONAL_CONNECTION_PAYLOAD, defined in 
// P2PDefs.h.
// In this demo, this variable array is set to be empty.
/******************************************************************/
#if ADDITIONAL_NODE_ID_SIZE > 0
    BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {0x00};
#endif

/*******************************************************************/
// The variable myChannel defines the channel that the P2P connection
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available with
// least amount of energy (or noise).
/*******************************************************************/
BYTE myChannel = 25;

BYTE TxNum = 0;
BYTE RxNum = 0;
BOOL EthernetRxHasData = 0;
BOOL EthernetRxHasMessage = 0;

BOOL AckEthernet = 0;

#define NUM_NODOS 24
#define NUM_CISTERNAS  16
BYTE CisValue[NUM_CISTERNAS];

//BYTE UartTxBuff[16];
BYTE UartRxBuff[32];    //
BYTE RS485Buff[24];
BYTE RSSIVals[12];      // valor de 0 a 10 en nible bajo y de 0 a 10 en nible alto
BYTE RLYStats[4];       // 1 bit por cada relay
//BYTE BattValues[24];
//BYTE NodesShortAdress[48];


extern BYTE myLongAddress[];

#if defined(PROTOCOL_MIWI_PRO)
ROM char *const menu =
    "\r\n     1: Enable/Disable Join"
    "\r\n     2: Show Family Tree"
    "\r\n     3: Show Routing Table"
    "\r\n     4: Send Message"
    "\r\n     5: Set Family Tree"
    "\r\n     6: Set Routing Table"
    "\r\n     7: Set Neighbor Routing Table"
    "\r\n     8: Start Frequency Agility"
    "\r\n     9: Socket"
    "\r\n     z: Dump Connection"
    ;

extern BYTE RoutingTable[NUM_COORDINATOR/8];
extern BYTE FamilyTree[NUM_COORDINATOR];
extern BYTE NeighborRoutingTable[NUM_COORDINATOR][NUM_COORDINATOR/8];

extern void PHYSetShortRAMAddr(INPUT BYTE address, INPUT BYTE value);
extern void InitMRF24J40(void);

void Rs485Put(_U08 c)
{
    while(!TXSTA1bits.TRMT);
    TXREG1 = c;
}





_U08 Rs485Get(void)
{
    _U16 timeout =0;

    //while((!PIR1bits.RC1IF)&&(++timeout)<65000);
    while((!PIR1bits.RC1IF));
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


void Rs485PutROMString(ROM char* str)
{
   BYTE c;

   DRV_485 = 1;

    while( c = *str++ )
    Rs485Put(c);

   while(!TXSTA1bits.TRMT);
   
   DRV_485 = 0;
}


void Rs485GetString(_U08 Count)
{
    _U08 i;

    for(i=0;i<Count;i++)
    {
        RS485Buff[i] = Rs485Get();

        if(RS485Buff[0] == 0xFF)
        {
            break;
        }
    }
}


BYTE GetHexDigit( void )
{
    BYTE    c;

    while (!ConsoleIsGetReady());
    c = ConsoleGet();
    ConsolePut(c);

    if (('0' <= c) && (c <= '9'))
        c -= '0';
    else if (('a' <= c) && (c <= 'f'))
        c = c - 'a' + 10;
    else if (('A' <= c) && (c <= 'F'))
        c = c - 'A' + 10;
    else
        c = 0;

    return c;
}

BYTE GetMACByte( void )
{
    BYTE    oneByte;

    oneByte = GetHexDigit() << 4;
    oneByte += GetHexDigit();

    return oneByte;
}

BYTE GetTwoDigitDec(void)
{
    BYTE oneByte;
    
    oneByte = GetHexDigit() * 10;
    oneByte += GetHexDigit();
    
    return oneByte;
}


void PrintMenu( void )
{
    ConsolePutROMString(menu);
    ConsolePutROMString( (ROM char * const) "\r\nEnter a menu choice: " );
}





void ProcessMenu( void )
{

    BYTE        c;
    BYTE        i;

    // Get the key value.
    c = ConsoleGet();
    ConsolePut( c );
    switch (c)
    {
        case '1':
            ConsolePutROMString((ROM char * const)"\r\n1=ENABLE_ALL 2=ENABLE PREV 3=ENABLE SCAN 4=DISABLE: ");
            while( !ConsoleIsGetReady());
        	c = ConsoleGet();
        	ConsolePut(c);
        	switch(c)
        	{
        		case '1':
        		    MiApp_ConnectionMode(ENABLE_ALL_CONN);
        		    break;
        		
        		case '2':
        		    MiApp_ConnectionMode(ENABLE_PREV_CONN);
        		    break;
        		    
        		case '3':
        		    MiApp_ConnectionMode(ENABLE_ACTIVE_SCAN_RSP);
        		    break;
        		    
        		case '4':
        		    MiApp_ConnectionMode(DISABLE_ALL_CONN);
        		    break;
        		
        	    default:
        	        break;	
            }
            break;
            
        case '2':
            Printf("\r\nFamily Tree: ");
            for(i = 0; i < NUM_COORDINATOR; i++)
            {
                PrintChar(FamilyTree[i]);
                Printf(" ");
            }
            break;
            
        case '3':
            Printf("\r\nMy Routing Table: ");
            for(i = 0; i < NUM_COORDINATOR/8; i++)
            {
                PrintChar(RoutingTable[i]);
            }
            Printf("\r\nNeighbor Routing Table: ");
            for(i = 0; i < NUM_COORDINATOR; i++)
            {
                BYTE j;
                for(j = 0; j < NUM_COORDINATOR/8; j++)
                {
                    PrintChar(NeighborRoutingTable[i][j]);
                }
                Printf(" ");
            }
            break;
            
        case '4':
            {   
                WORD_VAL tempShortAddr;
                Printf("\r\n1=Broadcast 2=Unicast Connection 3=Unicast Addr: ");
                while( !ConsoleIsGetReady());
            	c = ConsoleGet();
            	ConsolePut(c);
            	
    	        MiApp_FlushTx();
    	        MiApp_WriteData('T');
    	        MiApp_WriteData('e');
    	        MiApp_WriteData('s');
    	        MiApp_WriteData('t');
    	        MiApp_WriteData(0x0D);
    	        MiApp_WriteData(0x0A);
        	    switch(c)
        	    {
            	    case '1':
            	        MiApp_BroadcastPacket(FALSE);
            	        TxNum++;
            	        break;
            	        
            	    case '2':
            	        Printf("\r\nConnection Index: ");
            	        while( !ConsoleIsGetReady());
            	        c = GetHexDigit();
                        MiApp_UnicastConnection(c, FALSE);
                        TxNum++;
                        break;
                        
                    case '3':
                        Printf("\r\n1=Long Address 2=Short Address: ");
                        while( !ConsoleIsGetReady());
                    	c = ConsoleGet();
                    	ConsolePut(c);
                    	switch(c)
                    	{
                        	case '1':
                        	    Printf("\r\nDestination Long Address: ");
                        	    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                        	    {
                            	    tempLongAddress[MY_ADDRESS_LENGTH-1-i] = GetMACByte();
                            	}
                            	MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);
                            	TxNum++;
                            	break;
                        	
                        	case '2':
                        	    Printf("\r\nDestination Short Address: ");
                        	    tempLongAddress[1] = GetMACByte();
                        	    tempLongAddress[0] = GetMACByte();
                        	    MiApp_UnicastAddress(tempLongAddress, FALSE, FALSE);
                        	    TxNum++;
                        	    break;
                        	
                        	default:
                        	    break;
                        }
                        break;
                        
                    default:
                        break;
            	}
            }
            LCDTRXCount(TxNum, RxNum);
            break;
            
        case '5':
            {
                Printf("\r\nMSB of the Coordinator: ");
                i = GetMACByte();
                Printf("\r\nSet MSB of this Node's Parent: ");
                FamilyTree[i] = GetMACByte();
            }
            break;
            
        case '6':
            {
                Printf("\r\nSet my Routing Table: ");
                for(i = 0; i < NUM_COORDINATOR/8; i++)
                {
                    RoutingTable[i] = GetMACByte();
                    Printf(" ");
                }
            }
            break;
            
        case '7':
            {
                BYTE j;
                
                Printf("\r\nNode Number: ");
                i = GetMACByte();
                Printf("\r\nContent of Neighbor Routing Table: ");
                for(j = 0; j < NUM_COORDINATOR/8; j++)
                {
                    NeighborRoutingTable[i][j] = GetMACByte();
                    Printf(" ");
                }
            }
            break;
        
        case '8':
            {
                MiApp_InitChannelHopping(0xFFFFFFFF);
            }
            break;    
        
        
        case '9':
            {
                Printf("\r\nSocket: ");
                PrintChar(MiApp_EstablishConnection(0xFF, CONN_MODE_INDIRECT));
            }
            break;
        
        
        case 'z':
        case 'Z':
            {
                DumpConnection(0xFF);
            }    
        
        default:
            break;
    }
    PrintMenu();
}

#else
    #define PrintMenu()
    #define ProcessMenu()
#endif


#if defined(BRIDGE_BOARD)
void EthernetRxProcess(void)
{

}

_U08 EthernetGetByte(void)
{
    _U08 timeout =0;

    CTS = 0;

    while((!PIR3bits.RC2IF)&&(++timeout)<250);
    CTS = 1;
    if(PIR3bits.RC2IF)
    {
        if(RCSTA2bits.OERR)
        {
            RCSTA2bits.CREN = 0;   // Disable UART receiver
            RCSTA2bits.CREN = 1;   // Enable UART receiver
        }

        return RCREG2;

    }
    else
    {

         return 0xFF;
    }
}


BOOL EthernetHasData(void)
{
    return 1;
}


void EthernetTask(void)
{
    _U08 i,Cmd,Param;

    if(EthernetHasData)
    {
        if(EthernetGetByte() == StartByte0)
        {
            if(EthernetGetByte() == StartByte1)
            {
                switch(EthernetGetByte())     // if command
                {
                    case 0x00:                // ?
                    {
                     Cmd = 0;
                     Param =0;
                    }
                    break;

                    case 0x01:                // nuevo estado para relevadores
                    {
                     Cmd = 1;
                     Param =4;
                    }
                    break;

                    case 0x02:                // capacidad en m^3 de cisternas
                    {
                     Cmd = 2;
                     Param =16;
                    }
                    break;

                    case 0x03:               // solicitud de Short address
                    {
                     Cmd = 3;
                     Param =0;
                    }
                    break;

                    case 0x04:               // solicitud de MAC address
                    {
                     Cmd = 4;
                     Param =0;
                    }
                    break;

                    case 0x05:               // solicitud de nivel de baterias
                    {
                     Cmd = 5;
                     Param =0;
                    }
                    break;

                    default:
                    {
                     Cmd = 0xff;
                     Param =0;
                    }
                    break;
                }

                 for(i=0;i<Param;i++)
                 {
                     UartRxBuff[i] = EthernetGetByte();
                 }

                 if(EthernetGetByte() == EndByte0)
                 {
                     if(EthernetGetByte() == EndByte1)
                     {
                         switch(Cmd)
                         {
                             case 0:
                             {

                             }
                             break;

                             case 1:        // nuevo estado para relevadores
                             {
                                 for(i=0;i<Param;i++)
                                 {
                                   RLYStats[i] = UartRxBuff[i];
                                 }

                                 AckEthernet = 1;

                             }
                             break;

                             case 2:
                             {

                             }
                             break;

                             case 3:
                             {

                             }
                             break;

                             case 4:
                             {

                             }
                             break;

                             default:
                             {

                             }
                             break;
                         }
                     }
                 }

            }
        }
    }
}



BOOL Ethernet_MessageAvailable(void)
{
    EthernetTask();
    return EthernetRxHasMessage;
}

#endif




void GetLevels(void)
{

 RS485Buff[0] = 0x00;
 RS485Buff[1] = 0x00;
 RS485Buff[2] = 0x00;
 RS485Buff[3] = 0x00;
 RS485Buff[4] = 0x00;
 RS485Buff[5] = 0x00;
 RS485Buff[6] = 0x00;
 RS485Buff[7] = 0x00;

 RS485Buff[8] = 0x00;
 RS485Buff[9] = 0x00;
 RS485Buff[10] = 0x00;
 RS485Buff[11] = 0x00;
 RS485Buff[12] = 0x00;
 RS485Buff[13] = 0x00;
 RS485Buff[14] = 0x00;
 RS485Buff[15] = 0x00;
 RS485Buff[16] = 0x00;
 RS485Buff[17] = 0x00;


 //DRV_485 = 1;
 //Rs485Put(0x23);
 //Rs485Put(0x5E);
 //Rs485Put(0x01);
 //Rs485Put(0x01);
 //Rs485Put(0x00);
 //Rs485Put(0x3C);
 //Rs485Put(0x3F);

 //while(!TXSTA1bits.TRMT);

 //DRV_485 = 0;



 Rs485GetString(17);




 //DRV_485 = 1;

 //Rs485Put(0x55);
 //Rs485Put(0xAA);

 //Rs485Put(RS485Buff[0]);
 //Rs485Put(RS485Buff[1]);
 //Rs485Put(RS485Buff[2]);
 //Rs485Put(RS485Buff[3]);
 //Rs485Put(RS485Buff[4]);
 //Rs485Put(RS485Buff[5]);
 //Rs485Put(RS485Buff[6]);
 //Rs485Put(RS485Buff[7]);


 //while(!TXSTA1bits.TRMT);

 //DRV_485 = 0;

 // 23 5E 01 00 00 01 00 64 00 60 02 22 00 22 12 3C 3F
 // 0   1  2  3 4   5  6  7  8  9 10 11 12 13 14 15 16



 //if( (RS485Buff[0]==0x23)&&(RS485Buff[1]==0x5E)&&(RS485Buff[2]==0x01)&&(RS485Buff[3]==0x00)&&(RS485Buff[15]==0x3C)&&(RS485Buff[16]==0x3F))
 if( (RS485Buff[0]==0x23)&&(RS485Buff[1]==0x5E))
 {
     LED_1 ^= 1;
     LED_2 ^= 1;
 }

 


}



/*********************************************************************
* Function:         void main(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    This is the main function that runs the demo.  
*                   The device will first search for an existing
*                   network.  If a network exists and the parameters 
*                   of the network are acceptable to the device (in 
*                   this example simple RSSI minimum) then the device
*                   will join the existing network.  If the device 
*                   does not find an acceptable network and is a 
*                   coordinator then the device will perform an 
*                   energy scan on all of the channels available and 
*                   determine which channel has the lowest noise.  
*                   It will form a new network on this channel as 
*                   the PAN coordinator.
*
* Note:			    
********************************************************************/
void main(void)

{   
    BYTE i, j;
    BYTE TxSynCount = 0;
    BOOL bReceivedMessage = FALSE;
    _U16  l,m,k;
    _U08 myNodeNumber = 0;
    _BOOL myPkt = 0;

    

    
     #define BAUDRG 77
    /*******************************************************************/
    // Initialize the system
    /*******************************************************************/

    ANCON0 = 0XFF;     /*desactiva entradas analogicas*/
    ANCON1 = 0XFF;     /*desactiva entradas analogicas*/

    PPSUnLock();
    //PPSOutput(PPS_RP6, PPS_TX2CK2);    // TX2 RP17/RB3   ethernet
    //PPSInput(PPS_RX2DT2, PPS_RP5);     // RX2 RP18/RB5

    PPSOutput(PPS_RP17, PPS_TX2CK2);    // TX2 RP17/RB3 icsp
    PPSInput(PPS_RX2DT2, PPS_RP18);     // RX2 RP18/RB5

    PPSOutput(PPS_RP23, PPS_SDO2);    // SDO2 RP23/RD6
    PPSInput(PPS_SDI2, PPS_RP24);     // SDI2 RP24/RD7
    PPSOutput(PPS_RP22, PPS_SCK2);    // SCK2 RP22/RD5

    PPSLock();

     System_PeripheralPinSelect( ExternalInterrupt3, 19);  /*external interrupt 3 B3*/

     BoardInit();
     ConsoleInit();

     //Open2USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_EIGHT_BIT & USART_ASYNCH_MODE & USART_ADDEN_OFF, BAUDRG);
     //baud2USART(BAUD_IDLE_TX_PIN_STATE_HIGH & BAUD_IDLE_RX_PIN_STATE_HIGH & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE & USART_RX_INT_OFF);

     //Open1USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_EIGHT_BIT & USART_ASYNCH_MODE & USART_ADDEN_OFF, BAUDRG);
     //baud1USART(BAUD_IDLE_TX_PIN_STATE_HIGH & BAUD_IDLE_RX_PIN_STATE_HIGH & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE & USART_RX_INT_OFF);


     Gpios_PinDirection(GPIOS_PORTD, 7, GPIOS_INPUT);  /*pin C0 como salida para SDI*/
     Gpios_PinDirection(GPIOS_PORTD, 6, GPIOS_OUTPUT); /*pin C1 como salida para SDO*/
     Gpios_PinDirection(GPIOS_PORTD, 5, GPIOS_OUTPUT); /*pin C2 como salida para SCK*/

     Spi_Init(SPI_PORT1, SPI_64DIV); /*Inicializamos SPI2*/
     Spi_Init(SPI_PORT2, SPI_64DIV); /*Inicializamos SPI2*/

    LED_1 = 1;
    LED_2 = 1;

    Printf("\r\nActive Scan Results: \r\n");

    DRV_485 = 0;

    while(1)
    {
        //GetLevels();

        ConsolePut(Rs485Get());
    }


    CisValue[0] = 90;
    CisValue[1] = 30;
    CisValue[2] = 10;
    CisValue[3] = 50;
    CisValue[4] = 70;
    CisValue[5] = 5;
    CisValue[6] = 80;
    CisValue[7] = 40;
    CisValue[8] = 20;
    CisValue[9] = 60;
    CisValue[10] = 0;
    CisValue[11] = 95;
    CisValue[12] = 85;
    CisValue[13] = 75;
    CisValue[14] = 75;
    CisValue[15] = 75;


    for(j=0;j<12;j++)
    {
     RSSIVals[j] = 0x55;
    }

    RLYStats[0] = 0x00;
    RLYStats[1] = 0x00;
    RLYStats[2] = 0x00;
    RLYStats[3] = 0x00;


    Read_MAC_Address();


 if(myNodeNumber == 0) // if the node are PAN coordinator
 {
  MiApp_ProtocolInit(FALSE);

  /*******************************************************************/
   // Set Device Communication Channel
  /*******************************************************************/

	if( MiApp_SetChannel(myChannel) == FALSE )
	{
	 return;
	}


  /*******************************************************************/
  //  Set the connection mode. The possible connection modes are:
  //      ENABLE_ALL_CONN:    Enable all kinds of connection
  //      ENABLE_PREV_CONN:   Only allow connection already exists in
  //                          connection table
  //      ENABL_ACTIVE_SCAN_RSP:  Allow response to Active scan
  //      DISABLE_ALL_CONN:   Disable all connections.
  /*******************************************************************/
   MiApp_ConnectionMode(ENABLE_ALL_CONN);
   MiApp_ProtocolInit(FALSE);
   MiApp_StartConnection(START_CONN_DIRECT, 0, 0);
 }
 else
 { // if the node are Coordinator
  MiApp_ProtocolInit(FALSE);

  TryToJoin:

   myChannel = 25;

   i = MiApp_SearchConnection(10, 0x02000000);

            if( i > 0 )
            {
                // now print out the scan result.
                Printf("\r\nActive Scan Results: \r\n");
                for(j = 0; j < i; j++)
                {
                   // Printf("Channel: ");
                   // PrintDec(ActiveScanResults[j].Channel );
                    //Printf("   RSSI: ");
                    //PrintChar(ActiveScanResults[j].RSSIValue);
                    //Printf("\r\n");
                    //myChannel = ActiveScanResults[j].Channel;
                    //Printf("PeerInfo: ");
                    //PrintChar( ActiveScanResults[j].PeerInfo[0]);
                }
            }

    MiApp_ConnectionMode(ENABLE_ALL_CONN);

    if( i > 0 )
    {
     if( MiApp_EstablishConnection(0, CONN_MODE_DIRECT) == 0xFF )
     {
      //Printf("\r\nJoin Fail");
      
      LED_1  = 1;
      LED_2  = 1;
      DelayMs(250);
      goto TryToJoin;

     }
     else
     {
      LED_1  = 1;  // joined!!!
      LED_2  = 0;

     }
    }
    else
    {
      //Printf("\r\nNO nwk found");
      LED_1  = 1;
      LED_2  = 1;
      DelayMs(250);
      goto TryToJoin;
    }


 }

 

 while(1)
 {
     if( MiApp_MessageAvailable() )
        {
            /*******************************************************************/
            // If a packet has been received, following code prints out some of
            // the information available in rxFrame.
            /*******************************************************************/
            if( rxMessage.flags.bits.secEn )
            {
                //ConsolePutROMString((ROM char *)"Secured ");
            }

            if( rxMessage.flags.bits.broadcast )
            {
             //ConsolePutROMString((ROM char *)"Broadcast Packet with RSSI ");
            }
            else
            {
             //ConsolePutROMString((ROM char *)"Unicast Packet with RSSI ");
            }
            //PrintChar(rxMessage.PacketRSSI);
            if( rxMessage.flags.bits.srcPrsnt )
            {
               // ConsolePutROMString((ROM char *)" from ");
                if( rxMessage.flags.bits.altSrcAddr )
                {
                    //PrintChar(rxMessage.SourceAddress[1]);
                    //PrintChar(rxMessage.SourceAddress[0]);

                    if((rxMessage.SourceAddress[1]==0)&&(rxMessage.SourceAddress[0]==0))    // if msg from PAN Coordinator
                    {
                        m=0;
                        myPkt =0;
                    }
                }
                else
                {
                    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                    {
                        //PrintChar(rxMessage.SourceAddress[MY_ADDRESS_LENGTH-1-i]);

                    }
                }
            }

            //ConsolePutROMString((ROM char *)": ");


            for(i = 0; i < rxMessage.PayloadSize; i++)
            {
                //ConsolePut(rxMessage.Payload[i]);
            }

            // Toggle LED2 to indicate receiving a packet.
            LED_2 ^= 1;

            /*******************************************************************/
            // Function MiApp_DiscardMessage is used to release the current
            // received message. After calling this function, the stack can
            // start to process the next received message.
            /*******************************************************************/
            MiApp_DiscardMessage();

            bReceivedMessage = TRUE;

            /*******************************************************************/
            // Following block update the total received and transmitted messages
            // on the LCD of the demo board.
            /*******************************************************************/
            //LCDTRXCount(TxNum, ++RxNum);
        }
        else
        {
         ++m;
         ++k;
         ++l;
         
         




         if(myNodeNumber == 0)
         {
             if(l >= 2000)
            {
             l=0;
             GetLevels();
            }


             if(AckEthernet)
             {
                 k=5000;
                 AckEthernet = 0;
             }

            if(k >= 5000)
             {
             k = 0;

             for(j=0;j<NUM_CISTERNAS;j++)   // niveles de cisterna (simula valores)
             {
                 if(CisValue[j] < 100)
                 {
                     ++CisValue[j];
                 }
                 else
                 {
                     CisValue[j] = 0;
                 }
             }


             ConsolePut(0x23);
             ConsolePut(0x5E);
             ConsolePut(0x81);

             for(j=0;j<NUM_CISTERNAS;j++)   // envia nivel de cisternas (16)
             {
                 ConsolePut(CisValue[j]);
             }
             
             //RLYStats[0] ^= 0xFF;
             //RLYStats[1] ^= 0xFF;
             //RLYStats[2] ^= 0xFF;
             //RLYStats[3] ^= 0xFF;

             for(j=0;j<4;j++)               // envia estado de relevadores (4 bytes)
             {
                 ConsolePut(RLYStats[j]);
             }



             for(j=0;j<NUM_NODOS/2;j++)     // envia RSSI de cada nodo (24 empauetados de a dos en cada byte)
             {
                 ConsolePut(RSSIVals[j]);
                 RSSIVals[j]+= 0x11;

                 //ConsolePut(RSSIVals[j]>>4);
             }

             ConsolePut(0x3C);
             ConsolePut(0x3F);

             //LED_1 ^= 1;

             //DRV_485 = 1;
             //Rs485Put('H');
             //Rs485Put('O');
             //Rs485Put('L');
             //Rs485Put('A');
             //Rs485Put(0x0D);
             //Rs485Put(0x0A);
             //while(!TXSTA1bits.TRMT);
             //DRV_485 = 0;

            }
         }
         /*
         else
         {
             if(l >= 2000)
             {
             l=0;
             //GetLevels();
              DRV_485 = 1;
              Rs485Put(0x23);
              Rs485Put(0x5E);
              Rs485Put(0x01);
              Rs485Put(0x01);
              Rs485Put(0x00);
              Rs485Put(0x3C);
              Rs485Put(0x3F);

              while(!TXSTA1bits.TRMT);

               DRV_485 = 0;

               LED_1 ^= 1;
               LED_2 ^= 1;
              }
         }
         */
           
               
              
           if((myNodeNumber == 0)&&(m >= 10000))
           {
            m=0;

            MiApp_FlushTx();

    	    MiApp_WriteData('B');
    	    MiApp_WriteData('E');
    	    MiApp_WriteData('A');
    	    MiApp_WriteData('C');
            MiApp_WriteData('O');
            MiApp_WriteData('N');
            MiApp_WriteData(0x0D);
    	    MiApp_WriteData(0x0A);

            MiApp_BroadcastPacket(FALSE);

            //LED_1 ^= 1;

            //Printf("\r\nBeacon >>\r\n");      
          
           }
           else
           {
            
             if((m >= ((_U16)myNodeNumber*(_U16)4000))&&(myPkt == 0))        // si dentro de mi slot y no he enviado mi paquete
            //if((m >= 400)&&(myPkt == 0))
               {
                //m=0;
                 myPkt = 1;
             
                 MiApp_FlushTx();
                 
                 MiApp_WriteData('N');
    	         MiApp_WriteData('O');
                 MiApp_WriteData('D');
                 MiApp_WriteData('E');
                 MiApp_WriteData(' ');
                 
                 switch(myNodeNumber)
                 {
                     case 1:  MiApp_WriteData('1');  break;
                     case 2:  MiApp_WriteData('2');  break;
                     case 3:  MiApp_WriteData('3');  break;
                     case 4:  MiApp_WriteData('4');  break;
                     case 5:  MiApp_WriteData('5');  break;
                     case 6:  MiApp_WriteData('6');  break;
                     case 7:  MiApp_WriteData('7');  break;
                     case 8:  MiApp_WriteData('8');  break;
                     case 9:  MiApp_WriteData('9');  break;
                     case 10: MiApp_WriteData('1');  MiApp_WriteData('0'); break;
                     case 11: MiApp_WriteData('1');  MiApp_WriteData('1'); break;
                     case 12: MiApp_WriteData('1');  MiApp_WriteData('2'); break;
                     case 13: MiApp_WriteData('1');  MiApp_WriteData('3'); break;
                     case 14: MiApp_WriteData('1');  MiApp_WriteData('4'); break;
                     case 15: MiApp_WriteData('1');  MiApp_WriteData('5'); break;
                     case 16: MiApp_WriteData('1');  MiApp_WriteData('6'); break;
                     case 17: MiApp_WriteData('1');  MiApp_WriteData('7'); break;
                     case 18: MiApp_WriteData('1');  MiApp_WriteData('8'); break;
                     case 19: MiApp_WriteData('1');  MiApp_WriteData('9'); break;
                     case 20: MiApp_WriteData('2');  MiApp_WriteData('0'); break;
                     case 21: MiApp_WriteData('2');  MiApp_WriteData('1'); break;
                     case 22: MiApp_WriteData('2');  MiApp_WriteData('2'); break;
                     case 23: MiApp_WriteData('2');  MiApp_WriteData('3'); break;
                     case 24: MiApp_WriteData('2');  MiApp_WriteData('4'); break;
                     
                     default:  break;
                 
                 }
                 
                 MiApp_WriteData(0x0D);
    	         MiApp_WriteData(0x0A);
                 
                 MiApp_BroadcastPacket(FALSE);

                 //ConsolePutROMString((ROM char *)"My Data >> ");
             }

           }
                       
        }

     if(Ethernet_MessageAvailable())
     {
         
     }



 }


////////////////////////////////////////////////////////////////////////////////
}



