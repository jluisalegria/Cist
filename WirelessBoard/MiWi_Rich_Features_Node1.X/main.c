/* 
 * File:   main.c
 * Author: Pedro
 *
 * Created on 25 de julio de 2014, 01:09 PM
 */


#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "gpios/gpios.h"
#include "spi/spi.h"
//#include "st7032/st7032.h"
#include "uart1/uart1.h"
#include "uart1/uart2.h"
#include "printf/printf.h"
#include "delays/delays.h"
#include "system/system.h"
#include <pps.h>
#include <usart.h>

#include "ConfigApp.h"
#include "WirelessProtocols/MCHP_API.h"
#include "WirelessProtocols/Console.h"
#include "DemoOutput.h"
//#include "Transceivers/MRF89XA/MRF89XA.h"
//#include "HardwareProfile.h"


void Lcd_Putc(_U08 Char)  /*funcion para usar printf con lcd*/
{
    //ST7032_WriteData(ST7032_LCD0, Char);
}

void LcdPutROMString(ROM char* str)
{
   // BYTE c;

    //while(c = *str++)
       // Lcd_Putc(c);
}

#define LcdPrintf(x) LcdPutROMString((ROM char*)x)

/************************** VARIABLES ************************************/
#define LIGHT   0x01
#define SWITCH  0x02

/*************************************************************************/
// AdditionalNodeID variable array defines the additional
// information to identify a device on a PAN. This array
// will be transmitted when initiate the connection between
// the two devices. This  variable array will be stored in
// the Connection Entry structure of the partner device. The
// size of this array is ADDITIONAL_NODE_ID_SIZE, defined in
// ConfigApp.h.
// In this demo, this variable array is set to be empty.
/*************************************************************************/
#if ADDITIONAL_NODE_ID_SIZE > 0
     BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {LIGHT};
#endif

/*************************************************************************/
// The variable myChannel defines the channel that the device
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available with
// least amount of energy (or noise).
/*************************************************************************/
BYTE myChannel = 24;

BYTE tempAddr[8] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
extern DWORD_VAL OutgoingFrameCounter;

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
* Overview:		    This is the main function that runs the simple
*                   example demo. The purpose of this example is to
*                   demonstrate the simple application programming
*                   interface for the MiWi(TM) Development
*                   Environment. By virtually total of less than 30
*                   lines of code, we can develop a complete
*                   application using MiApp interface. The
*                   application will first try to establish a
*                   link with another device and then process the
*                   received information as well as transmit its own
*                   information.
*                   MiWi(TM) DE also support a set of rich
*                   features. Example code FeatureExample will
*                   demonstrate how to implement the rich features
*                   through MiApp programming interfaces.
*
* Note:
**********************************************************************/


#pragma code
void main(void)
{

    BYTE i;
    BYTE TxSynCount = 0;
    BYTE TxSynCount2 = 0;
    BYTE TxNum = 0;
    BYTE RxNum = 0;

     _U32 baudrate;
     #define BAUDRG 77
    /*******************************************************************/
    // Initialize the system
    /*******************************************************************/
    
    ANCON0 = 0XFF;     /*desactiva entradas analogicas*/
    ANCON1 = 0XFF;     /*desactiva entradas analogicas*/


    PPSUnLock();
    PPSOutput(PPS_RP10, PPS_TX2CK2);    // TX2 RP17/RC6
    PPSInput(PPS_RX2DT2, PPS_RP9);      // RX2 RP18/RC7
    //PPSInput(PPS_INT3,PPS_RP19);        // EXT INT3/ D3
    PPSLock();

    System_PeripheralPinSelect( ExternalInterrupt3, 19);  /*external interrupt 3 B3*/

     BoardInit();
     ConsoleInit();

     Open2USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_EIGHT_BIT & USART_ASYNCH_MODE & USART_ADDEN_OFF, BAUDRG);
     baud2USART(BAUD_IDLE_TX_PIN_STATE_HIGH & BAUD_IDLE_RX_PIN_STATE_HIGH & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE & USART_RX_INT_OFF);


     DemoOutput_Greeting();


    LED_1 = 0;   // apaga led
    LED_2 = 0;   // apaga led
    
    /*******************************************************************/
    // Initialize Microchip proprietary protocol. Which protocol to use
    // depends on the configuration in ConfigApp.h
    /*******************************************************************/
    /*******************************************************************/
    // Function MiApp_ProtocolInit initialize the protocol stack. The
    // only input parameter indicates if previous network configuration
    // should be restored. In this simple example, we assume that the
    // network starts from scratch.
    /*******************************************************************/
    MiApp_ProtocolInit(FALSE);
   

    // Set default channel
    if( MiApp_SetChannel(myChannel) == FALSE )
    {
        DemoOutput_ChannelError(myChannel);
    }


    /*******************************************************************/
    // Function MiApp_ConnectionMode defines the connection mode. The
    // possible connection modes are:
    //  ENABLE_ALL_CONN:    Enable all kinds of connection
    //  ENABLE_PREV_CONN:   Only allow connection already exists in
    //                      connection table
    //  ENABL_ACTIVE_SCAN_RSP:  Allow response to Active scan
    //  DISABLE_ALL_CONN:   Disable all connections.
    /*******************************************************************/
    MiApp_ConnectionMode(ENABLE_ALL_CONN);
    DemoOutput_Channel(myChannel, 0);


     /*******************************************************************/
    // Function MiApp_EstablishConnection try to establish a new
    // connection with peer device.
    // The first parameter is the index to the active scan result,
    //      which is acquired by discovery process (active scan). If
    //      the value of the index is 0xFF, try to establish a
    //      connection with any peer.
    // The second parameter is the mode to establish connection,
    //      either direct or indirect. Direct mode means connection
    //      within the radio range; indirect mode means connection
    //      may or may not in the radio range.
    /*******************************************************************/
    //LcdPrintf((char *)"connecting...");
    i = MiApp_EstablishConnection(0xFF, CONN_MODE_DIRECT);

    //LcdPrintf((char *)"connect...");



       /*******************************************************************/
    // Display current opertion on LCD of demo board, if applicable
    /*******************************************************************/
    if( i != 0xFF )
    {
        DemoOutput_Channel(myChannel, 1);
    }
    else
    {
        /*******************************************************************/
        // If no network can be found and join, we need to start a new
        // network by calling function MiApp_StartConnection
        //
        // The first parameter is the mode of start connection. There are
        // two valid connection modes:
        //   - START_CONN_DIRECT        start the connection on current
        //                              channel
        //   - START_CONN_ENERGY_SCN    perform an energy scan first,
        //                              before starting the connection on
        //                              the channel with least noise
        //   - START_CONN_CS_SCN        perform a carrier sense scan
        //                              first, before starting the
        //                              connection on the channel with
        //                              least carrier sense noise. Not
        //                              supported for current radios
        //
        // The second parameter is the scan duration, which has the same
        //     definition in Energy Scan. 10 is roughly 1 second. 9 is a
        //     half second and 11 is 2 seconds. Maximum scan duration is
        //     14, or roughly 16 seconds.
        //
        // The third parameter is the channel map. Bit 0 of the
        //     double word parameter represents channel 0. For the 2.4GHz
        //     frequency band, all possible channels are channel 11 to
        //     channel 26. As the result, the bit map is 0x07FFF800. Stack
        //     will filter out all invalid channels, so the application
        //     only needs to pay attention to the channels that are not
        //     preferred.
        /*******************************************************************/
        MiApp_StartConnection(START_CONN_DIRECT, 10, 0);
    }

     /*******************************************************************/
    // Function DumpConnection is used to print out the content of the
    //  Connection Entry on the hyperterminal. It may be useful in
    //  the debugging phase.
    // The only parameter of this function is the index of the
    //  Connection Entry. The value of 0xFF means to print out all
    //  valid Connection Entry; otherwise, the Connection Entry
    //  of the input index will be printed out.
    /*******************************************************************/
    DumpConnection(0xFF);

    // Turn on LED 1 to indicate connection established
    LED_1 = 1;
    DemoOutput_Instruction();

     while(1)
    {
        /*******************************************************************/
        // Function MiApp_MessageAvailable returns a boolean to indicate if 
        // a packet has been received by the transceiver. If a packet has 
        // been received, all information will be stored in the rxFrame, 
        // structure of RECEIVED_MESSAGE.
        /*******************************************************************/
        if( MiApp_MessageAvailable() )
        {
            /*******************************************************************/
            // If a packet has been received, handle the information available 
            // in rxMessage.
            /*******************************************************************/
            DemoOutput_HandleMessage();
            DemoOutput_UpdateTxRx(TxNum, ++RxNum);
            
            // Toggle LED2 to indicate receiving a packet.
            LED_2 ^= 1;
            
            /*******************************************************************/
            // Function MiApp_DiscardMessage is used to release the current 
            //  received packet.
            // After calling this function, the stack can start to process the
            //  next received frame 
            /*******************************************************************/        
            MiApp_DiscardMessage();
        }
        else
        {
            /*******************************************************************/
            // If no packet received, now we can check if we want to send out
            // any information.
            // Function ButtonPressed will return if any of the two buttons
            // has been pushed.
            /*******************************************************************/
            BYTE PressedButton = ButtonPressed();
            switch( PressedButton )
            {
                case 1:                 
                    /*******************************************************************/ 
                    // Button 1 pressed. We need to send out the bitmap of word "MiWi".
                    // First call MiApp_FlushTx to reset the Transmit buffer. Then fill 
                    // the buffer one byte by one byte by calling function 
                    // MiApp_WriteData
                    /*******************************************************************/
                    
                    MiApp_FlushTx();
                    for(i = 0; i < 21; i++)
                    {
                        MiApp_WriteData(MiWi[(TxSynCount%6)][i]);
                    }
                    TxSynCount++;
                    
                    /*******************************************************************/
                    // Function MiApp_BroadcastPacket is used to broadcast a message
                    //    The only parameter is the boolean to indicate if we need to
                    //       secure the frame
                    /*******************************************************************/
                    MiApp_BroadcastPacket(FALSE);

                    DemoOutput_UpdateTxRx(++TxNum, RxNum);

                    break;
                    
                case 2:
                    /*******************************************************************/                
                    // Button 2 pressed. We need to send out the bitmap of word "DE" 
                    // encrypted.
                    // First call function MiApp_FlushTx to reset the Transmit buffer. 
                    //  Then fill the buffer one byte by one byte by calling function 
                    //  MiApp_WriteData
                    /*******************************************************************/
                    MiApp_FlushTx();   
                    for(i = 0; i < 11; i++)
                    {
                        MiApp_WriteData(DE[(TxSynCount2%6)][i]);
                    }
                    TxSynCount2++;
                    
                    /*******************************************************************/
                    // Function MiApp_UnicastConnection is one of the functions to 
                    //  unicast a message.
                    //    The first parameter is the index of Connection Entry for 
                    //       the peer device. In this demo, since there are only two
                    //       devices involved, the peer device must be stored in the 
                    //       first Connection Entry
                    //    The second parameter is the boolean to indicate if we need to
                    //       secure the frame. If encryption is applied, the security
                    //       level and security key are defined in ConfigApp.h
                    //
                    // Another way to unicast a message is by calling function
                    //  MiApp_UnicastAddress. Instead of supplying the index of the 
                    //  Connection Entry of the peer device, this function requires the 
                    //  input parameter of destination address.
                    /*******************************************************************/
                    if( MiApp_UnicastConnection(0, TRUE) == FALSE )
                    {
                        DemoOutput_UnicastFail();
                    }
                    else
                    {
                        TxNum++;
                    }                   
                    DemoOutput_UpdateTxRx(TxNum, RxNum);
                    break;
                    
                default:
                    break;
            } 
        }
    }
}


#pragma interruptlow YourLowPriorityISRCode
void YourLowPriorityISRCode(void)
{
    /*coloca aquí el código que llevará tu interrupción de baja prioridad en caso de usarla*/

}