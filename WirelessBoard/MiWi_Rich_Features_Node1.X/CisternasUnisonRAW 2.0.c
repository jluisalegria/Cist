/********************************************************************
* FileName:		Cisternas Unison 2.0.c
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
* Se usa topologia Cluster tree en aplication layer
* se emplean dos repetidores: Nodo 13 y Nodo 7
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
#include <timers.h>
#include "WirelessProtocols/NVM.h"
#include "MAC_EEProm.h"
#include "SST_SerialFlash.h"
#include "ConfigApp.h"
#include "WirelessProtocols/MCHP_API.h"
#include "WirelessProtocols/Console.h"
#include "DemoOutput.h"


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
//#if ADDITIONAL_NODE_ID_SIZE > 0
   // BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {0x00};
//#endif

/*******************************************************************/
// The variable myChannel defines the channel that the P2P connection
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available with
// least amount of energy (or noise).
/*******************************************************************/
//#define REPEATER

#if defined (REPEATER)

//#define HOP_1
//#define HOP_2
//_U16 NodesToRepeat;  // nodos a los que hay que escuchar y reenviar su mensaje hacia siguiente HOP

#else

//#define HOP_1_CHILD
//#define HOP_2_CHILD
//#define PAN_COORD_CHILD

#endif


#define SENSOR_BOARD_NUMBER         1
#define NETWORK_DEBUG


#define NUM_NODOS 16
#define NUM_CISTERNAS  16

#define RSSI_MAX_VAL   0x45

//#define FORCE_COORDINATOR

#define PAN_COORD           0   // rectoria
#define HOP_1               1   // Derecho
#define HOP_2               2   // E.Beisbol
#define HOP_1_CHILD         3   // NODE 8 to 12
#define HOP_2_CHILD         4   // NODE 4 to 6
#define PAN_COORD_CHILD     5   // NODE 1 to 3

#define HOP_1_MAC           0xC0A5A7B5
#define HOP_2_MAC           0x05060708

#define C_CHANNEL            26


_U08 myNodeNumber =1;
_U08 myNodeType   = PAN_COORD_CHILD;
_U08 MyTempValue  = 0;
_U08 MyBattValue  = 0;

#if ADDITIONAL_NODE_ID_SIZE > 0
    BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {0x01};
#endif


//......RAW protocol vars

    extern BYTE RXBuff[32];
    extern BOOL PacketReceived;
    extern volatile BYTE        MACTxBuffer[TX_PACKET_SIZE];


    

//.......................

// PAN COORDINATOR VARIABLES
BOOL EthernetRxHasData = 0;
BOOL EthernetRxHasMessage = 0;
BOOL AckEthernet = 0;
_U08 SoftwareRequest = 0;
BYTE RequestedOPeration = 0;
BOOL MiWiRequest = 0;
BYTE CisValue[NUM_CISTERNAS];
BYTE BattValue[NUM_NODOS];
BYTE TempValue[NUM_NODOS];
BYTE UartRxBuff[32];    //
BYTE RSSIVals[12];      // valor de 0 a 10 en nible bajo y de 0 a 10 en nible alto
_U32 RLYStats = 0;       // 1 bit por cada relay
_U32 NewRLYStats = 0;       // 1 bit por cada relay
_U16 FloatValues;
// END OF PAN COORDINATOR VARIABLES

BYTE myChannel = C_CHANNEL;

 //_U16 mnl = 0;
 BYTE i, j;
 //BYTE TxSynCount = 0;
 BOOL bReceivedMessage = FALSE;
 BOOL SensorBoardRead = FALSE;
 _U16  m;
 _U16 SoftwareTimer = 0;
 _U08 MyNewRelayValue =0;
 _BOOL myPkt = 0;
 _BOOL SensorAck = 0;

 _U08 Rs485Buff[24];
 _U08 Rs485BuffIndex;

 _U08 MyLevelValue = 0;
 _U08 MyRelaysValue = 0;
 _U08 MyFloatValue = 0;

 _U08 MyTrackingNumber;
 _U08 BaseTimeToTx = 0;
 _U08 TxMode = 0;
 //_U08 Seconds0,Seconds1,Seconds2;
 _U16 Nodes_Ready = 0;
 _U16 ConnectedNodes = 0;
 _U08 TxTrack;
 _U08 Index;
 _BOOL TXSync = 0;

 _U16 ListenNodes=0;

 _U08 Tick,TickScaler;
 _U08 SwTimer0,SwTimer1,SwTimer2,SwTimer3,SwTimer4;

 _U32 Shift32;

 _U08 SecNum = 0;
 _U08 MACSecNum = 0;

extern BYTE myLongAddress[];

MIWI_TICK _t1,_t0;


_U16 NodesToRepeat;  // nodos a los que hay que escuchar y reenviar su mensaje hacia siguiente HOP




//extern _U08 UserTimer1;
//extern _U08 UserTimer2;

#if defined(PROTOCOL_MIWI_PRO)
ROM char * const menu =
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

extern BOOL TxPacket(INPUT BYTE TxPacketLen, INPUT BOOL CCA);


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
            	        //TxNum++;
            	        break;

            	    case '2':
            	        Printf("\r\nConnection Index: ");
            	        while( !ConsoleIsGetReady());
            	        c = GetHexDigit();
                        MiApp_UnicastConnection(c, FALSE);
                        //TxNum++;
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
                            	//TxNum++;
                            	break;

                        	case '2':
                        	    Printf("\r\nDestination Short Address: ");
                        	    tempLongAddress[1] = GetMACByte();
                        	    tempLongAddress[0] = GetMACByte();
                        	    MiApp_UnicastAddress(tempLongAddress, FALSE, FALSE);
                        	    //TxNum++;
                        	    break;

                        	default:
                        	    break;
                        }
                        break;

                    default:
                        break;
            	}
            }
            //LCDTRXCount(TxNum, RxNum);
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

////////////////////////////////////////////////////////////////////////////////

void Rs485Put(_U08 c)
{
    while(!TXSTA1bits.TRMT);
    TXREG1 = c;
}


_U08 Rs485Get(void)
{
    _U16 timeout =0;

    while((!PIR1bits.RC1IF)&&(++timeout)<500);
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


 void RS485GetLevels(void)
 {
     DRV_485 = 1;

     Rs485Put(0x23);
     Rs485Put(0x5E);
     Rs485Put(0x01); //DEST
     Rs485Put(0x01); //ADR
     Rs485Put(0x00); //CMD
     Rs485Put(0x3C);
     Rs485Put(0x3F);

     while(!TXSTA1bits.TRMT);
     DRV_485 = 0;

     //ConsolePutROMString( (ROM char * const) "\r\n Attempt to read Level " );

     for(j=0;j<17;j++)
     {
         Rs485Buff[j] = 0;
     }

     Rs485GetString( &Rs485Buff, 18);

     //ConsolePut(0x55);
    // for(j=0;j<17;j++)
    // {
         //ConsolePut(Rs485Buff[j]);
     //}
     //ConsolePut(0x55);


     for(j=0;j<5;j++)
     {
         if(Rs485Buff[j]==0x23)
         {
             Index = j;
             break;
         }
     }




     if((Rs485Buff[Index]== 0x23)&&(Rs485Buff[Index+1]== 0x5E)&&(Rs485Buff[Index+2]== 0x01)&&(Rs485Buff[Index+15]== 0x3C)&&(Rs485Buff[Index+16]== 0x3F))
     {
          //ConsolePut(0xBB);

          if((Rs485Buff[Index+4] == 0x00)||(Rs485Buff[Index+4] == 0x01)||(Rs485Buff[Index+4] == 0x10)||(Rs485Buff[Index+4] == 0x11))
          {
              MyRelaysValue = Rs485Buff[Index+4];
          }

          if((Rs485Buff[Index+5] == 0x00)||(Rs485Buff[Index+5] == 0x01))
          {
              MyFloatValue  = Rs485Buff[Index+5];
          }

          if(Rs485Buff[Index+12] <= 100)
          {
              MyLevelValue  = Rs485Buff[Index+12];
          }
 
     }
     else
     {

     }

     if(SENSOR_BOARD_NUMBER == 2)
     {

         Rs485Buff[0] = 0;
         Rs485Buff[1] = 0;

         DRV_485 = 1;

         Rs485Put(0x23);
         Rs485Put(0x5E);
         Rs485Put(0x01); //DEST
         Rs485Put(0x02); //ADR
         Rs485Put(0x00); //CMD
         Rs485Put(0x3C);
         Rs485Put(0x3F);

        while(!TXSTA1bits.TRMT);
        DRV_485 = 0;

        Rs485GetString( &Rs485Buff, 24);


        if((Rs485Buff[0]== 0x23)&&(Rs485Buff[1]== 0x5E)&&(Rs485Buff[2]== 0x02)&&(Rs485Buff[3]== 0x00)&&(Rs485Buff[4]== 0x00)&&(Rs485Buff[15]== 0x3C)&&(Rs485Buff[16]== 0x3F))
        {
          //ConsolePut(0x55);
          //ConsolePut(0x02);
            ConsolePutROMString( (ROM char * const) "\r\nLevel Readed!!! " );
        }
     }


 }

////////////////////////////////////////////////////////////////////////////////
 void StartWirelessConnection(void)
 {
    Read_MAC_Address();

    LED_1 = 0;
    LED_2 = 0;

                    //ConsolePutROMString((ROM char *)"\r\n<MAC Addr:");
                    //PrintChar(myLongAddress[3]);
                    //PrintChar(myLongAddress[2]);
                    //PrintChar(myLongAddress[1]);
                    //PrintChar(myLongAddress[0]);
                    //ConsolePutROMString((ROM char *)"\r>");

                    //ConsolePutROMString((ROM char *)"\r\n Board role: ");



 if(myNodeNumber == 0) // if the node are PAN coordinator
 {
  //ConsolePutROMString((ROM char *)"PAN COORDINATOR\r\n");

  MiApp_ProtocolInit(FALSE);

  /*******************************************************************/
   // Set Device Communication Channel
  /*******************************************************************/

	if( MiApp_SetChannel(myChannel) == FALSE )
	{
         //Printf("\r\nERROR: Unable toSet Channel..");
         //LCDDisplay((char *)"ERROR: Unable toSet Channel..", 0, TRUE);
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

            //Printf("\r\nCreating Network");
            //LCDDisplay((char *)"Creating Network", 0, TRUE);
            MiApp_ProtocolInit(FALSE);
	    MiApp_StartConnection(START_CONN_DIRECT, 0, 0);
             //MiApp_StartConnection(START_CONN_INDIRECT, 0, 0);

              //Printf("\r\nCreated Network Successfully");
             //Printf("\r\n");
              //LCDDisplay((char *)"Created Network Successfully", 0, TRUE);


	      //ConsolePutROMString((ROM char *)"\r\nPANID: ");
              //PrintChar(myPANID.v[1]);
              //PrintChar(myPANID.v[0]);
              //ConsolePutROMString((ROM char *)"   Channel: ");
              //PrintDec(myChannel);

              //ConsolePutROMString((ROM char *)"\r\nAddress: ");
              //PrintChar(myShortAddress.v[1]);
              //PrintChar(myShortAddress.v[0]);
              //Printf("\r\n");

              //sprintf((char *)&(LCDText), (far rom char*)"PANID:%02x%02x Ch:%02d",myPANID.v[1],myPANID.v[0],myChannel);
              //sprintf((char *)&(LCDText[16]), (far rom char*)"Address: %02x%02x", myShortAddress.v[1], myShortAddress.v[0]);
              //LCDUpdate();


 }
 else
 {

  // if the node are Coordinator
  ConsolePutROMString((ROM char *)"COORDINATOR >> ");
  PrintChar(myNodeNumber);

  ConsolePutROMString((ROM char *)"\r\n<MAC Addr:");
  PrintChar(myLongAddress[3]);
  PrintChar(myLongAddress[2]);
  PrintChar(myLongAddress[1]);
  PrintChar(myLongAddress[0]);
  ConsolePutROMString((ROM char *)"\r>");



     /*
  MiApp_ProtocolInit(FALSE);

  TryToJoin:

   myChannel = 25;
   //ConsolePutROMString((ROM char *)"\r\nStarting Active Scan...");

   i = MiApp_SearchConnection(10, 0x02000000);

            if( i > 0 )
            {
                // now print out the scan result.
                Printf("\r\nActive Scan Results: \r\n");
                for(j = 0; j < i; j++)
                {
                    Printf("Channel: ");
                    PrintDec(ActiveScanResults[j].Channel );
                    Printf("   RSSI: ");
                    PrintChar(ActiveScanResults[j].RSSIValue);
                    Printf("\r\n");
                    myChannel = ActiveScanResults[j].Channel;
                    Printf("PeerInfo: ");
                    PrintChar( ActiveScanResults[j].PeerInfo[0]);
                }
            }
   

    MiApp_ConnectionMode(ENABLE_ALL_CONN);

    if( i > 0 )
    {
     if( MiApp_EstablishConnection(0xff, CONN_MODE_DIRECT) == 0xFF )
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


      // Force PANID & SHORT ADDRESS

    myPANID.v[0] = 0x34;
    myPANID.v[1] = 0x12;

    myShortAddress.v[0] = 0x00;
    myShortAddress.v[1] = myNodeNumber;

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

    */


    MiApp_ProtocolInit(FALSE);
    
    // Force PANID & SHORT ADDRESS

     if( MiApp_SetChannel(myChannel) == FALSE )
	{
         //Printf("\r\nERROR: Unable toSet Channel..");
         //LCDDisplay((char *)"ERROR: Unable toSet Channel..", 0, TRUE);
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

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
 void TxWirelessTask(void)
 {
    if(((myNodeNumber == 0)&&(SwTimer0 >= (_U08)NUM_NODOS))||(SoftwareRequest==1))
    {
        if(SoftwareRequest==1)
        {
            SoftwareRequest = 2;
            MiWiRequest = 1;
            BaseTimeToTx = 2;           // Base time lower for get answer as fast as posible
            Nodes_Ready = 0;
        }
            SwTimer0 = 0;
            MyTrackingNumber++;
            TXSync = 1;                 // sincroniza actualizacion al software

j = myNodeNumber + (NewRLYStats>>24) + (NewRLYStats>>16) + (NewRLYStats>>8) + (NewRLYStats) + BaseTimeToTx + TxMode + MyTrackingNumber;


            MiApp_FlushTx();

    	    //MiApp_WriteData(' ');                 // 0
            //MiApp_WriteData(myNodeNumber);        // 1
            //MiApp_WriteData(NewRLYStats>>24);     // 2
            //MiApp_WriteData(NewRLYStats>>16);     // 3
            //MiApp_WriteData(NewRLYStats>>8);      // 4
            //MiApp_WriteData(NewRLYStats);         // 5
            //MiApp_WriteData(BaseTimeToTx);        // 6
            //MiApp_WriteData(TxMode);              // 7
            //MiApp_WriteData(MyTrackingNumber);    // 8
            //MiApp_WriteData(j);                   // 9
            //MiApp_WriteData(ConnectedNodes>>8);   // 10
            //MiApp_WriteData(ConnectedNodes);      // 11


            MiApp_WriteData('0');                 // 0
            MiApp_WriteData('1');        // 1
            MiApp_WriteData('2');     // 2
            MiApp_WriteData('3');     // 3
            MiApp_WriteData('4');      // 4
            MiApp_WriteData('5');         // 5
            MiApp_WriteData('6');        // 6
            MiApp_WriteData('7');              // 7
            MiApp_WriteData('8');    // 8
            MiApp_WriteData('9');                   // 9
            MiApp_WriteData('A');   // 10
            MiApp_WriteData('B');      // 11


            MiApp_BroadcastPacket(FALSE);

            ConnectedNodes = 0;

            LED_1 ^= 1;


            if(SoftwareRequest==0)
            {
                for(j=0;j<12;j++)
                {
                  RSSIVals[j] = 0x11;
                }
            }

           }
           else
           {
               if((myNodeNumber != 0)&&((SwTimer0 >= (_U08)myNodeNumber))&&(bReceivedMessage))
               {
                   SwTimer0 = 0;
                   bReceivedMessage = 0;     // from pan coordinator
                   SensorBoardRead = TRUE;   // request to read level over RS485
                   MyTrackingNumber++;

                   //ConsolePut(0xAA);
                   //ConsolePut(myNodeNumber);
                   //ConsolePut(MyRelaysValue);
                   //ConsolePut(MyFloatValue);
                   //ConsolePut(MyLevelValue);
                   //ConsolePut(MyTrackingNumber);
                   //ConsolePut(0xAA);


                   ++MyLevelValue;

                   if(MyLevelValue > 100)
                   {
                       MyLevelValue = 0;
                   }
                   
j = myNodeNumber + myLongAddress[3] + myLongAddress[2] + myLongAddress[1] + myLongAddress[0] + MyRelaysValue + MyFloatValue + MyLevelValue + MyBattValue + MyTempValue + MyTrackingNumber;


                    MiApp_FlushTx();

                    MiApp_WriteData(' ');                //0
                    MiApp_WriteData(myNodeNumber);       //1
                    MiApp_WriteData(myLongAddress[3]);   //2
                    MiApp_WriteData(myLongAddress[2]);   //3
                    MiApp_WriteData(myLongAddress[1]);   //4
                    MiApp_WriteData(myLongAddress[0]);   //5
                    MiApp_WriteData(MyRelaysValue);      //6
                    MiApp_WriteData(MyFloatValue);       //7
                    MiApp_WriteData(MyLevelValue);       //8
                    MiApp_WriteData(MyBattValue);        //9
                    MiApp_WriteData(MyTempValue);        //10
                    MiApp_WriteData(MyTrackingNumber);   //11
                    MiApp_WriteData(j);




                   //MiApp_BroadcastPacket(FALSE);

                    switch(myNodeType)
                    {
                        case HOP_1:  // send msg to PAN COORDINATOR
                        {
                            tempLongAddress[3] = 0xC0;
                            tempLongAddress[2] = 0xA5;
                            tempLongAddress[1] = 0x8C;
                            tempLongAddress[0] = 0x15;
                            MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);
                        }
                        break;

                        case HOP_2:  // send msg to HOP 1
                        {
                            tempLongAddress[3] = 0xC0;
                            tempLongAddress[2] = 0xA5;
                            //tempLongAddress[1] = 0xA7  //DERECHO
                            //tempLongAddress[0] = 0xB5;

                            tempLongAddress[1] = 0x91;   // ECA
                            tempLongAddress[0] = 0x82;
                            MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);
                        }
                        break;

                        case HOP_1_CHILD:   // send msg to HOP 1 <2>
                        {
                           tempLongAddress[3] = 0xC0;
                           tempLongAddress[2] = 0xA5;
                           tempLongAddress[1] = 0xA7;
                           tempLongAddress[0] = 0xB5;
                           MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);
                        }
                        break;

                        case HOP_2_CHILD:  // send msg to HOP 2
                        {
                            tempLongAddress[3] = 0xC0;
                            tempLongAddress[2] = 0xA5;
                            tempLongAddress[1] = 0x91;
                            tempLongAddress[0] = 0x29;

                            MiApp_BroadcastPacket(FALSE);
                            //MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);

                        }
                        break;

                        case PAN_COORD_CHILD:  // send msg to PAN COORDINATOR
                        {
                            tempLongAddress[3] = 0xC0;
                            tempLongAddress[2] = 0xA5;
                            tempLongAddress[1] = 0x8C;
                            tempLongAddress[0] = 0x15;

                            
                            MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);
                        }
                        break;

                        default:
                        {
                            MiApp_BroadcastPacket(FALSE);
                        }
                        break;
                    }


                    //MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);
                            
                     LED_1 ^= 1;


                     //////////////////
               }
           }

 }
////////////////////////////////////////////////////////////////////////////////

 void GetNodeData(void)
 {
     _U08 NODE = 0;

      if((rxMessage.Payload[0]) == 0x20)     // si nodo CISTERNA
      {
       if((rxMessage.Payload[1]) <= 16)      // si nodo pertenece a este sistema de monitoreo
       {
         if(((rxMessage.Payload[2]) == 0xC0)&&((rxMessage.Payload[3]) == 0xA5)) //si MACADDRESS reconocida
         {
             j = rxMessage.Payload[1] + rxMessage.Payload[2] + rxMessage.Payload[3] + rxMessage.Payload[4] + rxMessage.Payload[5] + rxMessage.Payload[6] + rxMessage.Payload[7] + rxMessage.Payload[8] + rxMessage.Payload[9] + rxMessage.Payload[10] + rxMessage.Payload[11];

             if(j == rxMessage.Payload[12])   // si checksumm ok... Procces data
             {
                 ///////>>>>>
                 NODE = rxMessage.Payload[1]&0x3F;    // Get clear Node Number


     if(SoftwareRequest==2)  //if waiting for response to ack the software...
     {
         SET_16BIT(Nodes_Ready,NODE-1);

         if(Nodes_Ready  >= 0x1FFF)     // si ha respondido nodo 1 a 13
         {
             SoftwareRequest =3;
         }
     }

     SET_16BIT(ConnectedNodes,NODE-1);


     j = rxMessage.PacketRSSI;   // RSSI_MAX_VAL


     //RSSIVals[NODE+5] = j;

     //j = (j*(BYTE)10)/(BYTE)RSSI_MAX_VAL;

     if(j > RSSI_MAX_VAL)
     {
         j = 10;
     }
     else
     {
         i =0;
         while(j > RSSI_MAX_VAL/10)
         {
             j = j-(RSSI_MAX_VAL/10);
             ++i;
         }

         j =i;
     }

     if(j > 10)
     {
         j = 10;
     }


     if((NODE&0x01)== 0)  // si nodo par => Nibble alto
     {

         j= j<<4;
         j &= 0xF0;
         RSSIVals[(NODE/2)] = RSSIVals[(NODE/2)]&0x0F;
         RSSIVals[(NODE/2)] |= j;
     }
     else
     {
         j &= 0x0F;
         RSSIVals[(NODE-1)/2] = RSSIVals[(NODE-1)/2]&0xF0;
         RSSIVals[(NODE-1)/2] |= j;
     }


     //CisValue[13]  = rxMessage.Payload[6]; //RLY
     //CisValue[14]  = rxMessage.Payload[7]; //FLOAT
     //CisValue[15]  = rxMessage.Payload[8]; //LEVEL
     //CisValue[15]  = rxMessage.Payload[11];//TRACK

                   // toma valores de relevador

                  //RSSIVals[8] = rxMessage.Payload[6];



                  switch(rxMessage.Payload[6])    // estado de relay recibido
                  {

                      case 0x00:
                      {
                          //CLEAR_32BIT(RLYStats,((rxMessage.Payload[6]*2)-2));
                          //CLEAR_32BIT(RLYStats,((rxMessage.Payload[6]*2)-1));

                          CLEAR_32BIT(RLYStats,((NODE-1)*2));
                          CLEAR_32BIT(RLYStats,((NODE-1)*2)+1);
                      }
                      break;

                      case 0x01:
                      {
                          //SET_32BIT(RLYStats,((rxMessage.Payload[6]*2)-2));
                          //CLEAR_32BIT(RLYStats,((rxMessage.Payload[6]*2)-1));

                          SET_32BIT(RLYStats,((NODE-1)*2));
                          CLEAR_32BIT(RLYStats,((NODE-1)*2)+1);

                      }
                      break;

                      case 0x10:
                      {

                          //CLEAR_32BIT(RLYStats,((rxMessage.Payload[6]*2)-2));
                          //SET_32BIT(RLYStats,((rxMessage.Payload[6]*2)-1));

                          CLEAR_32BIT(RLYStats,((NODE-1)*2));
                          SET_32BIT(RLYStats,((NODE-1)*2)+1);
                      }
                      break;

                      case 0x11:
                      {

                          //SET_32BIT(RLYStats,((rxMessage.Payload[6]*2)-2));
                          //SET_32BIT(RLYStats,((rxMessage.Payload[6]*2)-1));

                          SET_32BIT(RLYStats,((NODE-1)*2));
                          SET_32BIT(RLYStats,((NODE-1)*2)+1);
                      }
                      break;

                      default:
                      break;
                  }

                  // toma valor de flotador

                  if(rxMessage.Payload[7] == 1)
                  {
                     SET_16BIT(FloatValues,NODE-1);
                  }
                  else
                  {
                      CLEAR_16BIT(FloatValues,NODE-1);
                  }

                  // toma valor de NIVEL

                  CisValue[NODE-1] = rxMessage.Payload[8];

                  // toma valor de Bateria

                  BattValue[NODE-1] = rxMessage.Payload[9];


                  // toma valor de Temperatura

                  TempValue[NODE-1] = rxMessage.Payload[10];
                 //////<<<<<<
             }
             else
             {

             }
         }
         else
         {

         }
       }
       else
       {

       }
      }
      else
      {

      }

 }


////////////////////////////////////////////////////////////////////////////////

            //MiApp_WriteData(' ');                 // 0
            //MiApp_WriteData(myNodeNumber);        // 1
            //MiApp_WriteData(NewRLYStats>>24);     // 2
            //MiApp_WriteData(NewRLYStats>>16);     // 3
            //MiApp_WriteData(NewRLYStats>>8);      // 4
            //MiApp_WriteData(NewRLYStats);         // 5
            //MiApp_WriteData(BaseTimeToTx);        // 6
            //MiApp_WriteData(TxMode);              // 7
            //MiApp_WriteData(MyTrackingNumber);    // 8
            //MiApp_WriteData(j);                   // 9
            //MiApp_WriteData(ConnectedNodes>>8);   // 10
            //MiApp_WriteData(ConnectedNodes);      // 11



 void GetMasterData(void)
 {
    if((rxMessage.Payload[1]) == 0)   // si PAN COORDINATOR
    {

     ListenNodes = (rxMessage.Payload[10]<<8)+(rxMessage.Payload[11]);

     if(QUERY_16BIT(ListenNodes,myNodeNumber-1));
     {
          //ConsolePutROMString((ROM char *)">> ");
          //PrintChar(rxMessage.Payload[10]);
          //PrintChar(rxMessage.Payload[11]);
     }



      
      SensorAck = 0;

     myPkt = 0;
     bReceivedMessage = 1;
     SwTimer0 = 0;
     Tick = 0;

     if((rxMessage.Payload[6])<= 16)               // if expected value of time to tx
     {
          TickScaler = rxMessage.Payload[6];      //
     }
     else
     {
          TickScaler = 4;                          // Standard time base
     }

     if(myNodeNumber >12)   // nodo 13 -16
     {
         j = (myNodeNumber-13)*2;
         MyNewRelayValue = rxMessage.Payload[2];
     }
     else if(myNodeNumber >8) // nodo 9 -12
     {
         j = (myNodeNumber-9)*2;
         MyNewRelayValue = rxMessage.Payload[3];
     }
     else if(myNodeNumber >4) // nodo 5 -8
     {
         j = (myNodeNumber-5)*2;
         MyNewRelayValue = rxMessage.Payload[4];
     }
     else                     // nodo 1 - 4
     {
         j = (myNodeNumber-1)*2;
         MyNewRelayValue = rxMessage.Payload[5];
     }

     MyNewRelayValue = MyNewRelayValue>>(j);

     MyNewRelayValue &= 0x03;


     switch(MyNewRelayValue)
     {
         case 1:
         {
             //LED_1 = 1;
             //LED_2 = 0;
         }
         break;

         case 2:
         {
          MyNewRelayValue = 0x10;
          
          //LED_1 = 0;
          //LED_2 = 1;
         }
         break;

         case 3:
         {
          MyNewRelayValue = 0x11;

          //LED_1 = 1;
          //LED_2 = 1;
         }
         break;

         default: break;
     }

     //ConsolePut(MyNewRelayValue);
     //ConsolePut(MyRelaysValue);

     if(MyNewRelayValue != MyRelaysValue)
     {

         //MyRelaysValue = MyNewRelayValue;
/*
         switch(MyNewRelayValue)
         {   case 0:    LED_1 = 0; LED_2 = 0;break;
             case 0x01: LED_2 = 0; LED_1 = 1; break;
             case 0x10: LED_1 = 0; LED_2 = 1; break;
             case 0x11: LED_1 = 1; LED_2 = 1; break;
             default:break;
         }

*/



         // cmd to sensor board
         //ConsolePutROMString( (ROM char * const) "\r\n New Relay Values >> " );


         ////??????????????????????????

     for(j=0;j<17;j++)
     {
         Rs485Buff[j] = 0xFF;
     }

     DRV_485 = 1;

     Rs485Put(0x23);
     Rs485Put(0x5E);
     Rs485Put(0x01); //DEST
     Rs485Put(0x01); //ADR
     Rs485Put(0x02); //CMD RELAY
     Rs485Put(MyNewRelayValue);
     Rs485Put(0x3C);
     Rs485Put(0x3F);

     while(!TXSTA1bits.TRMT);
     DRV_485 = 0;
    

     Rs485GetString( &Rs485Buff, 10);

     SensorAck = 1;


     //for(j=0;j<10;j++)
     //{
         //ConsolePut(Rs485Buff[j]);
     //}

     //////////////////////?????????????????????

     }

    
  }
 }

////////////////////////////////////////////////////////////////////////////////

 void RxWirelessTask(void)
 {
   if( MiApp_MessageAvailable() )
   {
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
       if(myNodeNumber != 0)
       {
           //Printf("\r\n  ");
/*
           Printf("\r\n  ");
            if( rxMessage.flags.bits.secEn )
            {
                ConsolePutROMString((ROM char *)"Secured ");
            }

            if( rxMessage.flags.bits.broadcast )
            {
             ConsolePutROMString((ROM char *)"Broadcast Packet with RSSI ");
            }
            else
            {
             ConsolePutROMString((ROM char *)"Unicast Packet with RSSI ");
            }
            PrintChar(rxMessage.PacketRSSI);


           if( rxMessage.flags.bits.srcPrsnt )
            {
                ConsolePutROMString((ROM char *)" from ");
                if( rxMessage.flags.bits.altSrcAddr )
                {
                    PrintChar(rxMessage.SourceAddress[1]);
                    PrintChar(rxMessage.SourceAddress[0]);
                }
                else
                {
                    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                    {
                        PrintChar(rxMessage.SourceAddress[MY_ADDRESS_LENGTH-1-i]);

                    }
                }
            }

            ConsolePutROMString((ROM char *)": ");


            for(i = 0; i < rxMessage.PayloadSize; i++)
            {
                ConsolePut(rxMessage.Payload[i]);
            }
*/


           //ConsolePut(rxMessage.SourceAddress[1]);
           //ConsolePut(rxMessage.SourceAddress[0]);

           for(i = 0; i < rxMessage.PayloadSize; i++)
            {
                //ConsolePut(rxMessage.Payload[i]);
            }

   }
            
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

       SwTimer3 = 0; // CLEAR MIWIPRO STATUS TIMER

       if((myNodeNumber == 0))
       {
           GetNodeData();
       }
       else // myNodeNumber != 0
       {
        if((rxMessage.SourceAddress[1]==0)&&(rxMessage.SourceAddress[0]==0))    // if msg from PAN Coordinator
        {

         GetMasterData();
         //ConsolePutROMString((ROM char *)"<<");


        }
        else
        {
          //if(rxMessage.flags.bits.broadcast )
          //{

          //}
            // Unicast packet
          {
              // retransmit packet to next HOP


               MiApp_FlushTx();

               for(i = 0; i < rxMessage.PayloadSize; i++)
               {
                MiApp_WriteData(rxMessage.Payload[i]);
               }



                   switch(myNodeType)
                    {
                        case HOP_1:  // send msg to PAN COORDINATOR <RECTORIA>
                        {
                            tempLongAddress[3] = 0xC0;
                            tempLongAddress[2] = 0xA5;
                            tempLongAddress[1] = 0x8C;
                            tempLongAddress[0] = 0x15;
                            MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);
                            LED_1 ^= 1;

                        }
                        break;

                        case HOP_2:  // send msg to HOP 1 <DERECHO>
                        {
                            tempLongAddress[3] = 0xC0;
                            tempLongAddress[2] = 0xA5;
                            //tempLongAddress[1] = 0xA7; //DERECHO
                            //tempLongAddress[0] = 0xB5;

                            tempLongAddress[1] = 0x91;   // ECA
                            tempLongAddress[0] = 0x82;


                            MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);
                            LED_1 ^= 1;
                        }
                        break;

                        case HOP_1_CHILD:   // send msg to HOP 1 <DERECHO>
                        {
                           tempLongAddress[3] = 0xC0;
                           tempLongAddress[2] = 0xA5;
                           tempLongAddress[1] = 0xA7;
                           tempLongAddress[0] = 0xB5;

                           MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);
                           LED_1 ^= 1;


                        }
                        break;

                        case HOP_2_CHILD:  // send msg to HOP 2 <E.BEISBOL>
                        {
                            tempLongAddress[3] = 0xC0;
                            tempLongAddress[2] = 0xA5;
                            tempLongAddress[1] = 0x91;
                            tempLongAddress[0] = 0x29;

                            MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);
                            LED_1 ^= 1;
                        }
                        break;

                        case PAN_COORD_CHILD:  // send msg to PAN COORDINATOR <RECTORIA>
                        {
                            tempLongAddress[3] = 0xC0;
                            tempLongAddress[2] = 0xA5;
                            tempLongAddress[1] = 0x8C;
                            tempLongAddress[0] = 0x15;

                            MiApp_UnicastAddress(tempLongAddress, TRUE, FALSE);
                            LED_1 ^= 1;
                        }
                        break;

                        default: break;
                    }


          }
        }
       }

       LED_2 ^= 1;
       MiApp_DiscardMessage();
   }
   else
   {
     TxWirelessTask();    // Try to send my values packet
   }
 }
////////////////////////////////////////////////////////////////////////////////
  void RxWirelessTask3(void)
  {
   if( MiApp_MessageAvailable() )
   {
            if( rxMessage.flags.bits.secEn )  
            {
                ConsolePutROMString((ROM char *)"Secured ");
            }

            if( rxMessage.flags.bits.broadcast )
            {
             ConsolePutROMString((ROM char *)"Broadcast Packet with RSSI ");
            }
            else
            {
             ConsolePutROMString((ROM char *)"Unicast Packet with RSSI ");
            }
            PrintChar(rxMessage.PacketRSSI);
            if( rxMessage.flags.bits.srcPrsnt )
            {
                ConsolePutROMString((ROM char *)" from ");
                if( rxMessage.flags.bits.altSrcAddr )
                {
                    PrintChar(rxMessage.SourceAddress[1]);
                    PrintChar(rxMessage.SourceAddress[0]);

                    if((rxMessage.SourceAddress[1]==0)&&(rxMessage.SourceAddress[0]==0))    // if msg from PAN Coordinator
                    {
                        m=0;
                        myPkt =0;
                        bReceivedMessage = TRUE;
                    }
                }
                else
                {
                    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                    {
                        PrintChar(rxMessage.SourceAddress[MY_ADDRESS_LENGTH-1-i]);

                    }
                }
            }

            ConsolePutROMString((ROM char *)": ");


            for(i = 0; i < rxMessage.PayloadSize; i++)
            {
                ConsolePut(rxMessage.Payload[i]);
            }

            // Toggle LED2 to indicate receiving a packet.
            //LED_2 ^= 1;

            /*******************************************************************/
            // Function MiApp_DiscardMessage is used to release the current
            // received message. After calling this function, the stack can
            // start to process the next received message.
            /*******************************************************************/
            MiApp_DiscardMessage();

           

            /*******************************************************************/
            // Following block update the total received and transmitted messages
            // on the LCD of the demo board.
            /*******************************************************************/
            //LCDTRXCount(TxNum, ++RxNum);
   }
   else
   {
           m++;
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
               if(bReceivedMessage&&(myNodeNumber != 0))
               {
                   bReceivedMessage = 0;

                   MiApp_FlushTx();

                    MiApp_WriteData(myNodeNumber);
                    MiApp_WriteData(myLongAddress[3]);
                    MiApp_WriteData(myLongAddress[2]);
                    MiApp_WriteData(myLongAddress[1]);
                    MiApp_WriteData(myLongAddress[0]);
                    MiApp_WriteData(MyRelaysValue);
                    MiApp_WriteData(MyFloatValue);
                    MiApp_WriteData(MyLevelValue);
                    MiApp_WriteData(MyBattValue);
                    MiApp_WriteData(MyTempValue);
                    MiApp_WriteData(MyTrackingNumber);

            MiApp_BroadcastPacket(FALSE);
               }
           }
       }
  }
 ///////////////////////////////////////////////////////////////////////////////
 void RxWirelessTask1(void)
 {
     _U08 _i,_j;

     if( MiApp_MessageAvailable() )
        {

          if(myNodeNumber == 0)
          {
              _i = rxMessage.Payload[0]&0x3F;    // Get clear Node Number

              if((rxMessage.Payload[0])&0x40 == 0x40)     // si nodo CISTERNA
              {
                   // toma valores de relevador

                  switch(rxMessage.Payload[5])    // estado de relay recibido
                  {
                      case 0:
                      {
                          CLEAR_32BIT(RLYStats,((rxMessage.Payload[5]*2)-2));
                          CLEAR_32BIT(RLYStats,((rxMessage.Payload[5]*2)-1));
                      }
                      break;

                      case 1:
                      {
                          SET_32BIT(RLYStats,((rxMessage.Payload[5]*2)-2));
                          CLEAR_32BIT(RLYStats,((rxMessage.Payload[5]*2)-1));

                      }
                      break;

                      case 2:
                      {

                          CLEAR_32BIT(RLYStats,((rxMessage.Payload[5]*2)-2));
                          SET_32BIT(RLYStats,((rxMessage.Payload[5]*2)-1));
                      }
                      break;

                      case 3:
                      {

                          SET_32BIT(RLYStats,((rxMessage.Payload[5]*2)-2));
                          SET_32BIT(RLYStats,((rxMessage.Payload[5]*2)-1));
                      }
                      break;

                      default:
                      break;
                  }

                  // toma valor de flotador
                  
                  if(rxMessage.Payload[0] == 1)
                  {
                     SET_16BIT(FloatValues,rxMessage.Payload[0]-1);
                  }
                  else
                  {
                      CLEAR_16BIT(FloatValues,rxMessage.Payload[0]-1);
                  }

                  // toma valor de NIVEL

                  CisValue[rxMessage.Payload[0]-1] = rxMessage.Payload[7];

                  // toma valor de Bateria

                  BattValue[rxMessage.Payload[0]-1] = rxMessage.Payload[8];


                  // toma valor de Temperatura

                  TempValue[rxMessage.Payload[0]-1] = rxMessage.Payload[9];


              }  // NODO no es cisterna
          }           // end of if(myNodeNumber == 0)
          else
          {

          }



         
            if( rxMessage.flags.bits.secEn )
            {
                if(myNodeNumber == 0)
                {
                  ConsolePutROMString((ROM char *)"Secured ");
                }
            }

            if( rxMessage.flags.bits.broadcast )
            {
                if(myNodeNumber == 0)
                {
                    ConsolePutROMString((ROM char *)"Broadcast Packet with RSSI ");
                }
             
            }
            else
            {
                if(myNodeNumber == 0)
                {
                    ConsolePutROMString((ROM char *)"Unicast Packet with RSSI ");
                }
             
            }
            if(myNodeNumber == 0)
            {
                PrintChar(rxMessage.PacketRSSI);
            }
            
            if( rxMessage.flags.bits.srcPrsnt )
            {
                if(myNodeNumber == 0)
                {
                 ConsolePutROMString((ROM char *)" from ");
                }
                
                if( rxMessage.flags.bits.altSrcAddr )
                {
                    if(myNodeNumber == 0)
                    {
                     PrintChar(rxMessage.SourceAddress[1]);
                     PrintChar(rxMessage.SourceAddress[0]);
                    }
                    

                    if((rxMessage.SourceAddress[1]==0)&&(rxMessage.SourceAddress[0]==0))    // if msg from PAN Coordinator
                    {
                        SoftwareTimer=0;
                        myPkt =0;
                        //_t0 = MiWi_TickGet();
                        //_t1 =  _t0;
                    }
                }
                else
                {
                    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                    {
                        if(myNodeNumber == 0)
                        {
                            PrintChar(rxMessage.SourceAddress[MY_ADDRESS_LENGTH-1-i]);
                        }
                        

                    }
                }
            }

            ConsolePutROMString((ROM char *)": ");


            for(i = 0; i < rxMessage.PayloadSize; i++)
            {
                if(myNodeNumber == 0)
                {
                    PrintChar(rxMessage.Payload[i]);
                }
                
            }

            if(myNodeNumber == 0)
            {
             ConsolePutROMString( (ROM char * const) "  \r\n" );
            }


            // Toggle LED2 to indicate receiving a packet.
            //LED_2 ^= 1;

            MiApp_DiscardMessage();

            bReceivedMessage = TRUE;

        }
 }

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

 void TxWirelessTask1(void)
 {
  #if defined (REPEATER)

    
  #else

     //_t1 = MiWi_TickGet();

     if(myNodeNumber == 0)
     {
      //if(MiWi_TickGetDiff(_t1,_t0) >= (ONE_SECOND
      if(SoftwareTimer > 5000)
      {
            SoftwareTimer = 0;
            ++MyTrackingNumber;

            MiApp_FlushTx();

    	    MiApp_WriteData(myNodeNumber);
    	   // MiApp_WriteData((_U08)NewRLYStats);
    	   // MiApp_WriteData((_U08)NewRLYStats>>8);
    	    //MiApp_WriteData((_U08)NewRLYStats)>>16;
            //MiApp_WriteData((_U08)NewRLYStats>>24);
            MiApp_WriteData(BaseTimeToTx);
            MiApp_WriteData(TxMode);
            MiApp_WriteData(MyTrackingNumber);

            MiApp_BroadcastPacket(TRUE);

            LED_1 ^= 1;

      }
     }
     else
     {
         //if((MiWi_TickGetDiff(_t1,_t0) >= (ONE_SECOND*myNodeNumber))&&(myPkt == 0))
         if((SoftwareTimer > 10000)&&(myPkt == 0))
         {
            myPkt = 1;
            ++MyTrackingNumber;
            MiApp_FlushTx();

    	    MiApp_WriteData(myNodeNumber);
    	    MiApp_WriteData(myLongAddress[3]);
    	    MiApp_WriteData(myLongAddress[2]);
    	    MiApp_WriteData(myLongAddress[1]);
            MiApp_WriteData(myLongAddress[0]);
            MiApp_WriteData(MyRelaysValue);
            MiApp_WriteData(MyFloatValue);
    	    MiApp_WriteData(MyLevelValue);
            MiApp_WriteData(MyBattValue);
            MiApp_WriteData(MyTempValue);
            MiApp_WriteData(MyTrackingNumber);

            MiApp_BroadcastPacket(TRUE);

            LED_1 ^= 1;

         }
         else
         {

         }
     }
     


  #endif
 }

////////////////////////////////////////////////////////////////////////////////

 void GetLevelTask(void)
 {
   if((SensorBoardRead == TRUE)||(SensorAck ==1))
       //if((SensorBoardRead == TRUE)||(SwTimer2 >= 20))
   {
       SensorBoardRead = FALSE;
       SwTimer2 =0;

        RS485GetLevels();

        if(SensorAck == 1)
        {
            SensorAck = 0;
        }

       //MyLevelValue = myNodeNumber*10;
       //MyFloatValue = 1;
   }

 }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
 BOOL EthernetHasData(void)
{
    return 1;
}
////////////////////////////////////////////////////////////////////////////////
 void TXETHERNET(void)
 {
  if(AckEthernet)
  {
   SwTimer1 = 1;
   AckEthernet = 0;
  }

  //if((SwTimer1 >= 5)||(SoftwareRequest == 3))  // TXSync
  if((TXSync)||(SoftwareRequest == 3))
  {
   SwTimer1 = 0;
   TXSync = 0;

   if(SoftwareRequest==3)
   {
       SoftwareRequest=0;
       BaseTimeToTx = 4;    // Normal base time
   }

   ConsolePut(0x23);
   ConsolePut(0x5E);
   ConsolePut(0x81); //CMD

   for(j=0;j<NUM_CISTERNAS;j++)   // envia nivel de cisternas (16)
   {
    ConsolePut(CisValue[j]);
   }

       // envia estado de relevadores (4 bytes)
    
   Shift32 =  RLYStats;

    ConsolePut(Shift32);
    ConsolePut(Shift32>>8);
    ConsolePut(Shift32>>16);
    ConsolePut(Shift32>>24);

    

   for(j=0;j<12;j++)     // envia RSSI de cada nodo (24 empauetados de a dos en cada byte)
   {
    ConsolePut(RSSIVals[j]);
    //RSSIVals[j]+= 0x11;

  }

  ConsolePut(0x3C);
  ConsolePut(0x3F);


  }



  ++TxTrack;

  if(TxTrack >= 3)
  {
      TxTrack = 0;

      

/*
         if(QUERY_16BIT(ConnectedNodes,0)== 0)
         {
             RSSIVals[0] = RSSIVals[0]&0xF0;
         }

         if(QUERY_16BIT(ConnectedNodes,1)== 0)
         {
             RSSIVals[0] = RSSIVals[0]&0x0F;
         }
         ///////////////

         if(QUERY_16BIT(ConnectedNodes,2)== 0)
         {
             RSSIVals[1] = RSSIVals[1]&0xF0;
         }

         if(QUERY_16BIT(ConnectedNodes,3)== 0)
         {
             RSSIVals[1] = RSSIVals[1]&0x0F;
         }
         ///////////////

         if(QUERY_16BIT(ConnectedNodes,4)== 0)
         {
             RSSIVals[2] = RSSIVals[2]&0x0F;
         }

         if(QUERY_16BIT(ConnectedNodes,5)== 0)
         {
             RSSIVals[2] = RSSIVals[2]&0xF0;
         }
         ///////////////

         if(QUERY_16BIT(ConnectedNodes,6)== 0)
         {
             RSSIVals[3] = RSSIVals[3]&0x0F;
         }

         if(QUERY_16BIT(ConnectedNodes,7)== 0)
         {
             RSSIVals[3] = RSSIVals[3]&0xF0;
         }
         ///////////////

         if(QUERY_16BIT(ConnectedNodes,8)== 0)
         {
             RSSIVals[4] = RSSIVals[4]&0x0F;
         }

         if(QUERY_16BIT(ConnectedNodes,9)== 0)
         {
             RSSIVals[4] = RSSIVals[4]&0xF0;
         }
         ///////////////

         if(QUERY_16BIT(ConnectedNodes,10)== 0)
         {
             RSSIVals[5] = RSSIVals[5]&0x0F;
         }

         if(QUERY_16BIT(ConnectedNodes,11)== 0)
         {
             RSSIVals[5] = RSSIVals[5]&0xF0;
         }
         ///////////////

         if(QUERY_16BIT(ConnectedNodes,12)== 0)
         {
             RSSIVals[6] = RSSIVals[6]&0x0F;
         }

         if(QUERY_16BIT(ConnectedNodes,13)== 0)
         {
             RSSIVals[6] = RSSIVals[6]&0xF0;
         }
         ///////////////
*/

  }

  
 }
 ////////////////////////////////////////////////////////////////////////////////
void EthernetTask(void)
{
    _U08 i,Cmd,Param;

    if(EthernetHasData)
    {
        if(EthernetGetByte() == 0x23)
        {
            if(EthernetGetByte() == 0x5E)
            {
                switch(EthernetGetByte())     // if command
                {
                    case 0x00:  {Cmd = 0; Param =0;}  break;            // ?
                    case 0x01:  {Cmd = 1; Param =4;}  break;            // nuevo estado para relevadores                                     
                    case 0x02:  {Cmd = 2; Param =16;} break;            // capacidad en m^3 de cisternas                                     
                    case 0x03:  {Cmd = 3; Param =0;}  break;            // solicitud de Short address                 
                    case 0x04:  {Cmd = 4; Param =0;}  break;            // solicitud de MAC address                   
                    case 0x05:  {Cmd = 5; Param =1;}  break;            // solicitud de nivel de baterias
                    case 0x06:  {Cmd = 6; Param =1;}  break;            // New BaseTimeToTx                   
                    case 0x07:  {Cmd = 7; Param =1;}  break;            // NETWORK INFORMATION-MODIFY REQUEST
                    default:    {Cmd=0xff;Param =0;}  break;
                }

                 for(i=0;i<Param;i++)
                 {
                     UartRxBuff[i] = EthernetGetByte();
                 }

                 if(EthernetGetByte() == 0x3C)
                 {
                     if(EthernetGetByte() == 0x3F)
                     {
                         switch(Cmd)
                         {
                             case 0:
                             {

                             }
                             break;

                             case 1:        // nuevo estado para relevadores
                             {

                              NewRLYStats = 0;

                              NewRLYStats = (((_U32)UartRxBuff[3]<<24)|((_U32)UartRxBuff[2]<<16)|((_U32)UartRxBuff[1]<<8)|((_U32)UartRxBuff[0]));

                              //RLYStats =  NewRLYStats;

                                 //SoftwareRequest =  1;
                                 //RequestedOPeration = Cmd;
                                 //AckEthernet = 1;

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

                             case 5:
                             {
                                 
                             }
                             break;

                             case 6:
                             {
                                 if(UartRxBuff[0] <= 16)
                                 {
                                     BaseTimeToTx = UartRxBuff[0];
                                     TickScaler = BaseTimeToTx;
                                 }

                                 //LED_1 = 1;
                                 //LED_2 = 1;

                                 DelayMs(500);
                             }
                             break;

                             case 7:            // NETWORK INFORMATION-MODIFY REQUEST
                             {
                                 switch(UartRxBuff[0])
                                 {

                                     case 1:  // Enable/Disable Join
                                     {

                                     }
                                     break;

                                     case 2: // Show Family Tree
                                     {
                                         ConsolePut(0x23);
                                         ConsolePut(0x5E);
                                         ConsolePut(0x82); //CMD

                                         Printf("\r\nFamily Tree: ");
                                         for(i = 0; i < NUM_COORDINATOR; i++)
                                         {
                                          PrintChar(FamilyTree[i]);
                                          Printf(" ");
                                         }

                                         ConsolePut(0x3C);
                                         ConsolePut(0x3F);
                                     }
                                     break;

                                     case 3: // Show Routing Table
                                     {
                                         ConsolePut(0x23);
                                         ConsolePut(0x5E);
                                         ConsolePut(0x82); //CMD

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

                                         ConsolePut(0x3C);
                                         ConsolePut(0x3F);
                                     }
                                     break;

                                     case 4: //Send Message
                                     {

                                     }
                                     break;

                                     case 5: // Set Family Tree
                                     {

                                     }
                                     break;

                                     case 6: // Set Routing Table
                                     {

                                     }
                                     break;

                                     case 7: // Set Neighbor Routing Table
                                     {

                                     }
                                     break;

                                     case 8: // Start Frequency Agility
                                     {

                                     }
                                     break;

                                     case 9: // Socket
                                     {
                                         ConsolePut(0x23);
                                         ConsolePut(0x5E);
                                         ConsolePut(0x82); //CMD

                                         PrintChar(MiApp_EstablishConnection(0xFF, CONN_MODE_INDIRECT));

                                         ConsolePut(0x3C);
                                         ConsolePut(0x3F);
                                     }
                                     break;

                                     case 10:                    // DUMP CONNECTION
                                     {
                                         ConsolePut(0x23);
                                         ConsolePut(0x5E);
                                         ConsolePut(0x82); //CMD

                                         DumpConnection(0xFF);

                                         ConsolePut(0x3C);
                                         ConsolePut(0x3F);
                                     }
                                     break;

                                     default:
                                     break;
                                 }
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
    
    
    TXETHERNET();
    
    
    
}
////////////////////////////////////////////////////////////////////////////////
 BOOL Ethernet_MessageAvailable(void)
{
    EthernetTask();
    return EthernetRxHasMessage;
}
////////////////////////////////////////////////////////////////////////////////
 void TrxEthernetTask(void)
 {
  if(Ethernet_MessageAvailable())
  {

  }
 }
////////////////////////////////////////////////////////////////////////////////

 void MIWIPRO_Connection_Status(void)
 {
     if( SwTimer3 >= 250)           // if lost connection ...
     {
      SwTimer3 = 0;

      //StartWirelessConnection();   //  Try to restart connection
     }
 }

////////////////////////////////////////////////////////////////////////////////
  void MACReBroadcast(_U08 PayLoadLen,BYTE *MACPayload)
  {
      for(i=0;i<PayLoadLen;i++)
      {
          MACTxBuffer[i] = MACPayload[i];
      }

      i = TxPacket(MACTxBuffer[0], TRUE);


  }
////////////////////////////////////////////////////////////////////////////////

 _U08 MACSendBroadcast(_U16 SourceAdress, _U08 PayLoadLen, BYTE *MACPayload)
 {
    _U08 TxIndex = 0;

     MACTxBuffer[1] = NUM_NODOS;        // number of HOPS

     MACTxBuffer[2] = 0x12;             // Source PANID
     MACTxBuffer[3] = 0x34;             // Source PANID



     MACTxBuffer[4] = 0;                // frame control 1
     MACTxBuffer[5] = 0;                // frame control 0

     MACTxBuffer[6] = 0xff;             // repeaters 1
     MACTxBuffer[7] = 0xff;             // repeaters 0

     MACTxBuffer[8] = 0xFF;             // DEST 1
     MACTxBuffer[9] = 0xFF;             // DEST 0

     MACTxBuffer[10] = SourceAdress >>8;
     MACTxBuffer[11] = SourceAdress;

     MACTxBuffer[12] = MACSecNum++;

     MACTxBuffer[13] = PayLoadLen;

     TxIndex = 14;      // PayLoad start

     for(i = 0; i < PayLoadLen; i++)
     {
      MACTxBuffer[TxIndex++] = MACPayload[i];
     }

     MACTxBuffer[0] = TxIndex;

      return TxPacket(TxIndex, FALSE);

 }

////////////////////////////////////////////////////////////////////////////////
  BOOL RAW_PROTOCOL_TASK(void)
 {
     BOOL Status = 0;

     if(PacketReceived)
     {
      PacketReceived = 0;
      //LED_2 ^=1;

      for(j=0;j<RXBuff[0];j++)
      {
       ConsolePut(RXBuff[j]);
      }

      // validate Packet

      //Status = 1;








      

      // end of validation



      

     }



     return Status;
 }
 ///////////////////////////////////////////////////////////////////////////////
 
 BOOL MsgReceived(void)
 {
     if(RAW_PROTOCOL_TASK())
     {
         return 1;
     }
     else
     {
         return 0;
     }
 }
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

 // TickScaler
 //
 // 0  == 10 tick +- 1 seg
 // 1     1          100 ms
 // 2     2          200 ms
 // 3     4          400 ms
 // 4     8          800 ms
 // 5     16         1.6 s
 // 6     32         3.2 s
 // 7     64         6.4 s
 // 8     128        12.8 s
 // 9     256        25.6 s
 // 10    512        51.2 s
 // 11    1024       102.4 s   1.7  min
 // 12    2048       204.8 s   3.4  min
 // 13    4096       409.6 s   6.8  min
 // 14    8192       819.2 s   13.6 min
 // 15    16384                27.3 min
 // 16    32768                54.6 min

 void Timer1Tick(void)
 {
   _U16 Scaler;

  if(PIR1bits.TMR1IF)
  {
    PIR1bits.TMR1IF = 0;
    WriteTimer1(9286);           // overflow every 150ms
    ++Tick;

    if(myNodeNumber == 0)
    {

    }
    else
    {

    }


    switch(TickScaler)
    {
        case 0:  Scaler = 10;    break;
        case 1:  Scaler = 1;     break;
        case 2:  Scaler = 2;     break;
        case 3:  Scaler = 4;     break;
        case 4:  Scaler = 8;     break;
        case 5:  Scaler = 16;    break;
        case 6:  Scaler = 32;    break;
        case 7:  Scaler = 64;    break;
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
////////////////////////////////////////////////////////////////////////////////


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
     #define BAUDRG 77

    BYTE SecNum = 0;
    /*******************************************************************/
    // Initialize the system
    /*******************************************************************/

    ANCON0 = 0XFF;     /*desactiva entradas analogicas*/
    ANCON1 = 0XFF;     /*desactiva entradas analogicas*/

    PPSUnLock();

    if(myNodeNumber == 0)
    {
       //PPSOutput(PPS_RP6, PPS_TX2CK2);    // TX2 RP17/RC6   ethernet
       //PPSInput(PPS_RX2DT2, PPS_RP5);     // RX2 RP18/RC7

     PPSOutput(PPS_RP10, PPS_TX2CK2);    // TX2 RP17/RC6     icsp
     PPSInput(PPS_RX2DT2, PPS_RP9);     // RX2 RP18/RC7
    }
    else
    {
     PPSOutput(PPS_RP10, PPS_TX2CK2);    // TX2 RP17/RC6     icsp
     PPSInput(PPS_RX2DT2, PPS_RP9);     // RX2 RP18/RC7
    }

    


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


     Open2USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_EIGHT_BIT & USART_ASYNCH_MODE & USART_ADDEN_OFF, BAUDRG);
     baud2USART(BAUD_IDLE_TX_PIN_STATE_HIGH & BAUD_IDLE_RX_PIN_STATE_HIGH & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE & USART_RX_INT_OFF);

    
     OpenTimer1( TIMER_INT_OFF &T1_16BIT_RW &T1_SOURCE_FOSC_4 & T1_PS_1_8 &T1_OSC1EN_OFF &T1_SYNC_EXT_OFF, TIMER_GATE_OFF  & TIMER_GATE_INT_OFF);



     Gpios_PinDirection(GPIOS_PORTD, 7, GPIOS_INPUT);  /*pin C0 como salida para SDI*/
     Gpios_PinDirection(GPIOS_PORTD, 6, GPIOS_OUTPUT); /*pin C1 como salida para SDO*/
     Gpios_PinDirection(GPIOS_PORTD, 5, GPIOS_OUTPUT); /*pin C2 como salida para SCK*/

     Spi_Init(SPI_PORT1, SPI_64DIV); /*Inicializamos SPI2*/
     Spi_Init(SPI_PORT2, SPI_64DIV); /*Inicializamos SPI2*/


   /////////////////////////////////////////////////////////////////////////////
   #if defined (REPEATER)

     switch(myNodeNumber)
     {
         case 0: { NodesToRepeat = 0xFFFF; }break;    // PAN COORD

         case 1: {NodesToRepeat = 0xFFFF; }break;    // POZO

         case 2: {NodesToRepeat = 0xFFFF; }break;    // Arquitectura

         case 3: {NodesToRepeat = 0xFFFF; }break;    // I.Quimica

         case 4: {NodesToRepeat = 0xFFFF; }break;    // Castro servin

         case 5: {NodesToRepeat = 0xFFFF; }break;    // Electronica

         case 6: {NodesToRepeat = 0xFFFF; }break;    // ECA

         case 7: {NodesToRepeat = 0xFFFF; }break;    // Trabajo Social

         case 8: {NodesToRepeat =  0xFFFF;}break;    // Derecho
                           
         case 9: {NodesToRepeat = 0xFFFF; }break;    // Historia

         case 10:{NodesToRepeat = 0xFFFF; }break;    // Gimnasio

         case 11:{NodesToRepeat = 0xFFFF; }break;    // E.Beisbol

         case 12:{NodesToRepeat = 0x2000;}break;     // Medicina Repeat Node 13
                              
         case 13:{ }break;    // Dictus
     }


   #endif


   /////////////////////////////////////////////////////////////////////////////



     for(j=0;j<10;j++)
     {
         DelayMs(500);
     }
     

    StartWirelessConnection();

    //MiMAC_SetPower(TX_POWER_N_10_DB);

    NewRLYStats = 0;

    RLYStats = 0;
    MyTrackingNumber = 0;
    


    SwTimer0 = 0;
    SwTimer1 = 0;
    SwTimer2 = 0;
    SwTimer3 = 0;
    SwTimer4 = 0;

    for(j=0;j<NUM_CISTERNAS;j++)   
   {
    (CisValue[j] = 0);
   }
    
    for(j=0;j<12;j++)
    {
     RSSIVals[j] = 0;
    }


    if(myNodeNumber == 0)
    {
      BaseTimeToTx = 4;
    }
    else
    {
        RS485GetLevels();    // Try to Get values from Sensor Board

    }
    
    if(MyLevelValue > 100)
    {
        MyLevelValue = 100;
    }

    TickScaler = 4;
    
    while(1)
    {
        Timer1Tick();
        //RxWirelessTask();

        if(MsgReceived())
        {
          


          if(myNodeNumber != 0)
          {
              if(myNodeNumber == 2)
              {
                  MiApp_FlushTx();


                for(j=0;j<16;j++)
                {
                    MiApp_WriteData(0xAA+j);
                }

                MACSendBroadcast(0x0200,TxData,TxBuffer);
                LED_1 ^= 1;
              }
          }

        }

        //RAW_PROTOCOL_TASK();
        

        if(myNodeNumber == 0)
        {
            //TrxEthernetTask();


            if(SwTimer4 >= 5)
            {
                SwTimer4 = 0;


                //TxData =0;
                //MACTxBuffer[0] = 31;
                //MACTxBuffer[1] = SecNum++;
                //MACTxBuffer[2] = 0xAB;

                //for(j=3;j<30;j++)
                //{
                    //MACTxBuffer[j] = j;
                //}
                //++j;

               // MACTxBuffer[j] = 0x55;
         
                //++j;
               
               //TxPacket(j, FALSE);


               // MACTxBuffer


                // _U08 MACSendBroadcast(_U16 SourceAdress, _U08 PayLoadLen, BYTE *MACPayload)

                MiApp_FlushTx();


                for(j=0;j<16;j++)
                {
                    MiApp_WriteData(0xAA+j);
                }
    	        
                MACSendBroadcast(0x0000,TxData,TxBuffer);
                LED_1 ^= 1;
            }

        }
        else
        {
            //GetLevelTask();

            
   
        }

        #if defined (NETWORK_DEBUG)

        if(myNodeNumber == 0)
        {
            
        }
        else
        {
            if ( ConsoleIsGetReady())
            {
                //ProcessMenu();
            }
        }

        #endif

    }
}



