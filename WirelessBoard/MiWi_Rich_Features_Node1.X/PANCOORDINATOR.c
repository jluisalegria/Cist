/* 
 * File:   main.c
 * Author: Lap
 *
 * Created on 7 de abril de 2015, 03:29 PM
 */


/*

 * dispositivos soportados:  128
 * rango:                    0 - 127

 * dispositivos Coordinador 1:   0  - 31  >> 32
 *
 * dispositivos PAN COORDINATOR: 32 - 95  >> 16
 *
 * dispositivos Coordinador 2:   96 - 127
 *
 */


/////////////////////////////////////////////////////////////////////////////////////
/*  Direccionamiento de memoria externa                                             */
/*                                                                                  */
/* 0 - 11000     espacio reservado para almacenar hasta 10000 eventos de reservacion*/
/* paginas 0 - 429                                                                  */
/* fin de pagina 429 = 110079*/
/*                                                                                  */
/* espacio reservado para calibraciones 256 bytes: pagina 508                       */
/*    inicio calibraciones =  130,048                                               */
/*    Fin calibraciones    =  130,303                                               */
/*                                                                                  */
/* espacio reservado para almacenar dispositivos controlados manualmente DCM        */
/*  inicio: 130,304                                                                 */
/*  fin:    130,320                                                                 */
/*                                                                                  */
/* espacio reservado para almacenar estado de disp contr manualmente EDCM           */
/*   inicio: 130,321                                                                */
/*   fin:    130,337                                                                */
/*                                                                                  */
/* espacio reservado para almacenar valores de tolerancia al encendido y al apagado */
/*   Inicio: 130338                                                                 */
/*   Fin:    130339                                                                 */
/*                                                                                  */
/* espacio reservado para almacenar dispositivos que tienen reservacion confirmada  */
/* Se almacena un bit en alto por cada dispositivo con reserv confirmada y al final */
/* se almacena fecha y hora de borrado de reservaciones                             */
/*   Inicio: 130340                                                                 */
/*   FIN:    130362                                                                 */
/**/
/**/
////////////////////////////////////////////////////////////////////////////////

// j2 pins  

// 1.- VDD
// 2.-  
// 3.- RX
// 4.- TX
// 5.- GND



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





// time vars

                     //         Datos en BCD
typedef struct      //Definicion de Tipo Hora y Fecha
{
BYTE segundos;       //Dato de Segundo   00-59
BYTE minutos;        //Dato de Minuto    00-59
BYTE horas;          //Dato de Hora      00-23
BYTE dia_sem;        //Dato del Dia de la Semana 01-07
BYTE dia_mes;        //Dato del Día del Mes      01-31
BYTE mes;            //Dato del Numero de Mes    01-12
BYTE anio;           //Dato del Numero de Año    00-99
}HoraFecha;


HoraFecha Time;

_U08 Rs485Buff[24];
_U08 Index;




extern BYTE myLongAddress[];

#define C_CHANNEL            16

#define PanCoordToCoordChan  16
#define Coord1ToEndDevChan   17
#define Coord2ToEndDevChan   18
#define PanCoordToEndDevChan 19

//#define DevicesInstalled     48

#define PCDevices            48
#define AdrFinReserv         129999
#define AdrDCM               130304        
#define AdrEDCM              130321

#define CAL_LEN              46      // Bytes per Device
#define HRS_LEN              732     // Bytes per Device



const _U08 Resync          = 0x0A;
const _U08 OutputStat      = 0x0B;
const _U08 ListenDevices   = 0x0C;
const _U08 CalibrationMode = 0x0D;

const _U32 CalibrationAddress    = 130048;
const _U32 ToleranceValueAddress = 130338;

_U08 myNodeNumber = 0;

_U08 EnabledDevices[PCDevices/8];   // dispositivos a los que habra que revisar horario

_U08 myDevicesRequiredStatus[16];
_U08 myDevicesOutputStatus[16];
_U08 ConnectedDevices[16];
//_U08 C1DevicesOutputStatus[4];
//_U08 C2DevicesOutputStatus[4];

_U08 MyDevicesTemp[PCDevices];
_U08 OffsetMyDevicesTemp[PCDevices];
_U08 OffsetAppliedMyDevicesTemp[PCDevices];


//_U08 DCM[16];     // Dispositivos controlados manualmente
//_U08 EDCM[16];    // Estado de los dispositivos controlados manualmente

_U08 myvar = 0;

//_U08 Segundos,Minutos,Horas,DiaSemana,DiaMes,Anio,ClockTick;

BOOL TemperatureCalibration = 0;
_U08 TemperatureCalibrationValue = 0;

_U16 _U16Var;
_U32 U32EEADR;

_U32 U32EEIndex;


#if ADDITIONAL_NODE_ID_SIZE > 0
    BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {0x08};
#endif
    
    //const BYTE OFFSET[50] = {0, 3, 3, 1, 4, 3, 3, 2, 4, 2, 3, 1, 2, 2, 3, 0, 4, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 4, 3, 3, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                       //   01-02-03-04-05-06-07-08-09-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40-41-42-43-44-45-46-47-48-49-50     


BYTE myChannel = C_CHANNEL;
_U08 Tick,TickScaler;
_U08 SwTimer0,SwTimer1,SwTimer2,SwTimer3,SwTimer4;
_U08 BaseTimeToTx = 4;
_U08 MyTrackingNumber;
_U08 ConnectedCoordinators;


_U08 PanCoordStateMachine = 0;

BYTE i, j;


typedef struct
{
    _U08 Byte;
    _U08 Bit;
}_ByteBit;

_ByteBit ByteBitVal;

//ROM _U08 DiasMes[12] = {31,29,31,30,31,30,31,31,30,31,30,31}; //

void Return_ByteBit(_U08 dia, _U08 mes);
void QueryCalendars(void);
void QueryHrs(void);
void MuxUart(_U08 Uart);
void Calibration(void);
void SearchEepromIndex(void);

//********************************************************************//
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
  // if the node are Coordinator
  //ConsolePutROMString((ROM char *)"COORDINATOR >> ");
  //PrintChar(myNodeNumber);

  //ConsolePutROMString((ROM char *)"\r\n<MAC Addr:");
  //PrintChar(myLongAddress[3]);
  //PrintChar(myLongAddress[2]);
  //PrintChar(myLongAddress[1]);
  //PrintChar(myLongAddress[0]);
  //ConsolePutROMString((ROM char *)"\r>");

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
//****************************************************************************//
 void ApplyOffsetValues(void)
 {
     
     _U32 k = CalibrationAddress;
     
     
    
     Eeprom25xx_Read(EEPROM25XX_MEMORY0, OffsetMyDevicesTemp,k, PCDevices);      // lee offsets de temperatura
     
     
     
     
     for(j=0;j<PCDevices;j++)
     {

         
         
         /*
         if(QUERY_8BIT(i,7))
         {
             OffsetMyDevicesTemp[j] = MyDevicesTemp[j] - i;
         }
         else
         {
             OffsetMyDevicesTemp[j] = MyDevicesTemp[j] + i;
         }
         */
         
          OffsetAppliedMyDevicesTemp[j] = MyDevicesTemp[j] + OffsetMyDevicesTemp[j];
         
     }
     
     
     
     //for(j=0;j<PCDevices;j++)
     //{
         //OffsetAppliedMyDevicesTemp[j] = MyDevicesTemp[j] + OffsetMyDevicesTemp[j];
       
         
     //}
     
     
     
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

 void GetDateTime(void)
 {
     Rs485Put(0x23);
     Rs485Put(0x5E);
     Rs485Put(0x01); 
     Rs485Put(0x3C);
     Rs485Put(0x3F);

     while(!TXSTA1bits.TRMT);
     
     for(j=0;j<17;j++)
     {
         Rs485Buff[j] = 0;
     }
     
     Rs485GetString( &Rs485Buff, 11);
     
     for(j=0;j<10;j++)
     {
         if(Rs485Buff[j]==0x23)
         {
             Index = j;
             LED_1 ^= 1;
             break;
         }
     }
     
     //for(j=0;j<11;j++)
     //{
         //ConsolePut(Rs485Buff[j]);
     //}
     
     //ConsolePut(0x55);
     //ConsolePut(0xAA);
     //ConsolePut(0x3F);
     
     
     
     if((Rs485Buff[Index]== 0x23)&&(Rs485Buff[Index+1]== 0x5E)&&(Rs485Buff[Index+9]== 0x3C)&&(Rs485Buff[Index+10]== 0x3F))
     {
         
         // comprobacion de formato correcto
       if((((Rs485Buff[Index+2])&(0x7f))<= 0x60)&&(((Rs485Buff[Index+3])&(0x7f))<= 0x60)&&(((Rs485Buff[Index+4])&(0x3f))<= 0x24)&&(((Rs485Buff[Index+5])&(0x07))<= 0x07)&&(((Rs485Buff[Index+6])&(0x3f))<= 0x31)&&(((Rs485Buff[Index+7])&(0x1f))<= 0x12)&&(((Rs485Buff[Index+8])&(0xff))<= 0x99))
       {
         
         Time.segundos = Rs485Buff[Index+2];
         Time.minutos  = Rs485Buff[Index+3];
         Time.horas    = Rs485Buff[Index+4];
         Time.dia_sem  = Rs485Buff[Index+5];
         Time.dia_mes  = Rs485Buff[Index+6];
         Time.mes      = Rs485Buff[Index+7];
         Time.anio     = Rs485Buff[Index+8];
         
         LED_2 ^= 1;
       }
     }
     
     
     
     
 }
 
 ///////////////////////////////////////////////////////////////////////////////
 //devuelve el indice y compara si coinciden ID de dispositivos 
 
 _U16 SearchStoredID(_U08 Device,_U08 ID1,_U08 ID0)
 {
     U32EEADR = 0;
     i=0;
     
     for(_U16Var = 0; _U16Var < 10000; _U16Var++)
     {
         U32EEADR = (_U32)_U16Var * (_U32)13;
         U32EEADR += 10;
         
         Eeprom25xx_Read(EEPROM25XX_MEMORY0, Rs485Buff,U32EEADR,3);
         
         U32EEADR -= 10;
         
         if((Rs485Buff[0] == Device)&&(Rs485Buff[1] == ID1)&&((Rs485Buff[2] == ID0)))
         {
             i=1;
             break;
         }
     }

     
     if(i == 1)
     {
         return _U16Var;
     }
     else
     {
         return 0xFFFF;
     }
 }
 ///////////////////////////////////////////////////////////////////////////////
//****************************************************************************//
 _U08 EthernetGetByte(void)
{
    _U08 timeout =0;
    _U08 RCVChar;

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

        RCVChar = RCREG2;

        //ConsolePut(RCVChar);

        return RCVChar;

    }
    else
    {

         return 0xFF;
    }
}
//****************************************************************************//

 void EthernetTask(void)
 {
     _U08 i,l,Cmd,Param,ByteCount;
     _U32 volatile EEAdr;
     _U08 volatile  UartRxBuff[64];
     _U08 volatile AuxBuff[16];

  if(EthernetGetByte() == 0x23)
  {
    if(EthernetGetByte() == 0x5E)
    {
      switch(EthernetGetByte())     // if command
      {
       case 0x00:  {Cmd = 0; Param =0;}   break;             // ?
       case 0x01:  {Cmd = 1; Param =48;}  break;             // almacenar  calendario
       case 0x02:  {Cmd = 2; Param =1;}   break;             // Solicitud de estado de dispositivos (estado y temperatura)
       case 0x03:  {Cmd = 3; Param =1;}   break;             // almacenar horario
       case 0x04:  {Cmd = 4; Param =9;}   break;             // Actualizar fecha y hora
       case 0x05:  {Cmd = 5; Param =0;}   break;             // Descargar  fecha y hora
       case 0x06:  {Cmd = 6; Param =1;}   break;             // Descargar horario
       case 0x07:  {Cmd = 7; Param =1;}   break;             // Descargar Calendario
       case 0x08:  {Cmd = 8; Param =0;}   break;             // borrar calendarios
       case 0x09:  {Cmd = 9; Param =0;}   break;             // borrar horarios
       case 0x0A:  {Cmd = 10; Param =2;}  break;             // control manual de salidas
       case 0x0B:  {Cmd = 11; Param =13;} break;             // almacena evento de reservacion
       
       case 0x0C:  {Cmd = 12; Param = 0;} break;  
       case 0x0D:  {Cmd = 13; Param = 1;} break;             // Descargar eventos de reservacion
       case 0x0E:  {Cmd = 14; Param = 13;}break;             // Editar reservacion
       case 0x0F:  {Cmd = 15; Param = 3;} break;             // Borrar reservacion
       case 0x10:  {Cmd = 16; Param = 3;} break;             // confirmar reservacion
       case 0x11:  {Cmd = 17; Param = 2;} break;             // minutos de tolerancia
       
       
       case 0x20:  {Cmd = 32; Param =1;}  break;             // armar calendario
       case 0x21:  {Cmd = 33; Param =2;}  break;
       case 0x22:  {Cmd = 34; Param =1;}  break;             // control de todas las salidas para proposito de pruebas
       case 0x23:  {Cmd = 35; Param =1;}  break;             // base time to tx
       case 0x24:  {Cmd = 36; Param =2;}  break;             // valor de calibracion de temperatura individual
       case 0x25:  {Cmd = 37; Param =0;}  break;             // lee valores de calibracion de temperatura
       case 0x26:  {Cmd = 38; Param =0;}  break;             // reinicia valores de calibracion de temperatura
       case 0x27:  {Cmd = 39; Param =0;}  break;             // lee valores de tolerancia de encendido y apagado
       
       default:    {Cmd=0xff;Param =0;}   break;
      }





      if(Cmd == 3)
      {
          goto __skip;
      }

      for(i=0;i<Param;i++)
      {
       UartRxBuff[i] = EthernetGetByte();
       ++ByteCount;
      }

      if(EthernetGetByte() == 0x3C)
      {
        if(EthernetGetByte() == 0x3F)
        {

   __skip:
         switch(Cmd)
         {
           case 0:
           {
            LED_1 ^= 1;
            LED_2 ^= 1;

           }
           break;
//------------------------------------------------------------------------------
 //235E010001020304050607080910111213141516171819202122232425262728293031323334353637383940414243444546473C3F

           case 1:  //   Almacenar Calendario   ok
           {
               if(UartRxBuff[0]<128)   // si indice de calendario dentro del rango de dispositivos
               {
                 EEAdr = (_U32)UartRxBuff[0]*CAL_LEN;
            
                 
                 for(j=0;j<46;j++)                    // shift one byte to right the entire array
                 {
                     UartRxBuff[j] = UartRxBuff[j+1];
                 }

                 Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr,46);    // almacena calendario

                 ConsolePut(0x23); 
                 ConsolePut(0x5E);
                 ConsolePut(0x01);
                 ConsolePut(0x01);
                 ConsolePut(0x3C);
                 ConsolePut(0x3F);

               } 
           }
           break;
//------------------------------------------------------------------------------
           case 2:  //Estado de dispositivos  235E02063c3f
           {
               if(UartRxBuff[0] == 6)
               {
                ConsolePut(0x23);
                ConsolePut(0x5E);
                ConsolePut(0x02);

                /*  
                for(j=0;j<16;j++)
                {
                 ConsolePut(myDevicesOutputStatus[j]);
                }
                */
                
               // para facilitar las cosas al software el estado de cada dispositivo se envia en un byte cada uno
                
                
                for(j=0;j<16;j++)    // 
                {
                    /*
                    for(i=0;i<8;i++)
                    {
                        if(QUERY_8BIT(myDevicesOutputStatus[j],i))
                        {
                            ConsolePut(0x01);
                        }
                        else
                        {
                            ConsolePut(0x00);
                        }
                    }
                    
                    */
                 ConsolePut(myDevicesOutputStatus[j]);
                }
                
                
                

                for(j=0;j<PCDevices;j++)
                {
                 //ConsolePut(MyDevicesTemp[j]);
                 ConsolePut(OffsetAppliedMyDevicesTemp[j]);
                }

                for(j=PCDevices;j<128;j++)
                {
                 ConsolePut(0);
                }

                ConsolePut(151);
                
                ConsolePut(0x3C);
                ConsolePut(0x3F);
               }
           }
           break;
//------------------------------------------------------------------------------
           case 3:    // recibe horario      ok
           {
                // Return_ByteBit(10, 0);

             UartRxBuff[0] = EthernetGetByte();

             if(UartRxBuff[0] < 128)
             {
               // calcula direccion de almacenamiento
              EEAdr = ((_U32)UartRxBuff[0]*(_U32)HRS_LEN)+((_U32)PCDevices*(_U32)CAL_LEN);

              for(i=0;i<11;i++)
              {

                for(j=0;j<64;j++)
                {
                 UartRxBuff[j] = EthernetGetByte();
                }


                Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr, 64);

                EEAdr += 64;
              }


              for(j=0;j<28;j++)
              {
               UartRxBuff[j] = EthernetGetByte();
              }

              Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr, 28);

             }


              ConsolePut(0x23);
              ConsolePut(0x5E);
              ConsolePut(0x03);
              ConsolePut(0x01);
              ConsolePut(0x3C);
              ConsolePut(0x3F);


           }
           break;
//------------------------------------------------------------------------------
           case 4:   // Actualizar fecha y hora   ok
           {
              
              Rs485Put(0x23);
              Rs485Put(0x5E);
              Rs485Put(0x02); 
              
              
              // conversion HEX to BCD
              
              
              for(l=0;l<7;l++)
              {
                  i = UartRxBuff[l];           //respalda valor
                  UartRxBuff[l] = ((i/10)<<4) | (i%10);
              }
              
              
              
              Rs485Put(UartRxBuff[0]); 
              Rs485Put(UartRxBuff[1]);
              Rs485Put(UartRxBuff[2]);
              Rs485Put(UartRxBuff[3]);
              Rs485Put(UartRxBuff[4]);
              Rs485Put(UartRxBuff[5]);
              Rs485Put(UartRxBuff[6]);
              
              Rs485Put(0x3C);
              Rs485Put(0x3F);
             
              ConsolePut(0x23);
              ConsolePut(0x5E);
              ConsolePut(0x04);
              ConsolePut(0x01);
              ConsolePut(0x3C);
              ConsolePut(0x3F);
           }
           break;
//------------------------------------------------------------------------------
           case 5:      //  descargar fecha y hora ok  235e053c3f
           {
               //EEAdr = 131064;
               //Eeprom25xx_Read(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr,7);
               
               GetDateTime();
               
               UartRxBuff[0] = (((Time.segundos&0xF0)>>4)*10) + (Time.segundos&0x0F);
               UartRxBuff[1] = (((Time.minutos&0xF0) >>4)*10) + (Time.minutos&0x0F);
               UartRxBuff[2] = (((Time.horas&0xF0)   >>4)*10) + (Time.horas&0x0F);
               UartRxBuff[3] = (((Time.dia_sem&0xF0) >>4)*10) + (Time.dia_sem&0x0F);
               UartRxBuff[4] = (((Time.dia_mes&0xF0) >>4)*10) + (Time.dia_mes&0x0F);
               UartRxBuff[5] = (((Time.mes&0xF0)     >>4)*10) + (Time.mes&0x0F);
               UartRxBuff[6] = (((Time.anio&0xF0)    >>4)*10) + (Time.anio&0x0F);
               

               ConsolePut(0x23);
               ConsolePut(0x5E);
               ConsolePut(0x05);
               
               for(i=0;i<7;i++)
               {
                ConsolePut(UartRxBuff[i]);
               }

               ConsolePut(0x3C);
               ConsolePut(0x3F);


           }
           break;
//------------------------------------------------------------------------------
           case 6:   // descargar horario    235E06XX3C3F    ok
           {
            if(UartRxBuff[0] <128)
            {
               // calcula direccion de lectura
              EEAdr = ((_U32)UartRxBuff[0]*(_U32)HRS_LEN)+((_U32)PCDevices*(_U32)CAL_LEN);

              ConsolePut(0x23);
              ConsolePut(0x5E);
              ConsolePut(0x06);

              for(i=0;i<11;i++)
              {
                Eeprom25xx_Read(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr,64);
                EEAdr += 64;

                for(l=0;l<64;l++)
                {
                  ConsolePut(UartRxBuff[l]);
                }
              }

               Eeprom25xx_Read(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr,28);

              for(j=0;j<28;j++)
              {
                  ConsolePut(UartRxBuff[j]);
              }

               ConsolePut(0x02);
               ConsolePut(0xE3);

               ConsolePut(0x3C);
               ConsolePut(0x3F);

            }
           }
           break;
//------------------------------------------------------------------------------

           case 7:   // Descargar calendario   235E07XX  3C3F   ok
           {

             if(UartRxBuff[0] <128)
             {
                  // calcula direccion de lectura
              EEAdr = ((_U32)UartRxBuff[0]*(_U32)CAL_LEN);

              ConsolePut(0x23);
              ConsolePut(0x5E);
              ConsolePut(0x07);

              Eeprom25xx_Read(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr,46);

              for(j=0;j<46;j++)
              {
                  ConsolePut(UartRxBuff[j]);
              }

              ConsolePut(0x35);
              ConsolePut(0x3C);
              ConsolePut(0x3F);

             }

           }
           break;

//------------------------------------------------------------------------------

           case 8:  // Borrar todos los calendarios     235E083C3F   ok
           {

               EEAdr = 0;                 // 5888 bytes to erase

            for(i=0;i<64;i++)
            {
                UartRxBuff[i] = 0x00;
            }

            for(i=0;i<92;i++)
            {
                Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr, 64);
                EEAdr += 64;
            }


            ConsolePut(0x23);
            ConsolePut(0x5E);
            ConsolePut(0x08);
            ConsolePut(0x01);
            ConsolePut(0x3C);
            ConsolePut(0x3F);

           }
           break;

           //-------------------------------------------------------------------
           //           borrar horarios
           case 9:    
           {
             
           }
           break;
//------------------------------------------------------------------------------

             case 10:  // control manual de salidas
             {         // 235E0A XX YY 3C3F
              
              //_U08 DCM[16];     Dispositivos controlados manualmente  "condicion forzada"
              //_U08 EDCM[16];    Estado de los dispositivos controlados manualmente  "Estado solicitado"
                                 
              if((UartRxBuff[0] <= 127)&&(UartRxBuff[1] <= 1))  
              {
                //_U16Var = ((_U16)UartRxBuff[0]<<8) + (_U16)UartRxBuff[1];
                  
                  l = UartRxBuff[1];
                
                j = (UartRxBuff[0]-1)/8;    // calcula byte
                i = (UartRxBuff[0]-1)%8;    // calcula bit
                
                U32EEADR = AdrDCM;  
                Eeprom25xx_Read(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR,16);   // lee estados en eeprom
                //SET_8BIT(DCM[j],i);
                SET_8BIT(UartRxBuff[j],i);                                     // actualiza estados en RAM
                Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR,16);  // actualiza estados en eeprom
                
                U32EEADR = AdrEDCM;  
                Eeprom25xx_Read(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR,16);   // lee estados en eeprom
                
                  
                  if(l == 1)
                  {
                      SET_8BIT(myDevicesRequiredStatus[j],i);   
                      SET_8BIT(UartRxBuff[j],i);
                      //SET_8BIT(EDCM[j],i);
                      
                  }
                  
                  if(l == 0)
                  {
                      CLEAR_8BIT(myDevicesRequiredStatus[j],i);
                      CLEAR_8BIT(UartRxBuff[j],i);
                      //CLEAR_8BIT(EDCM[j],i);
                  }
                
                Eeprom25xx_Write(EEPROM25XX_MEMORY0, myDevicesRequiredStatus,U32EEADR,16);  // actualiza estados en eeprom
                  
                  ConsolePut(0x23);
                  ConsolePut(0x5E);
                  ConsolePut(0x0A);
                  ConsolePut(0x01);
                  ConsolePut(0x3C);
                  ConsolePut(0x3F);  
              }
              
              
              
              
             }
             break;

           //-------------------------------------------------------------------
             case 11:       // almacenar evento de reservacion
             {
                 if((UartRxBuff[10] > 0)&&(UartRxBuff[10] <= 128))     // si el ID esta comprendido entre 1 y 128
                 {
                     // si los elementos de fecha y hora estan dentro de los rangos permitidos...
                     
                     _U16Var = ((_U16)UartRxBuff[11]<<8) + UartRxBuff[12];
                     
                     if((UartRxBuff[0] <= 99)&&(UartRxBuff[1] <= 12)&&(UartRxBuff[2] <= 31)&&(UartRxBuff[3] <= 23)&&(UartRxBuff[4] <= 59)
                      &&(UartRxBuff[5] <= 99)&&(UartRxBuff[6] <= 12)&&(UartRxBuff[7] <= 31)&&(UartRxBuff[8] <= 23)&&(UartRxBuff[9] <= 59)
                      &&(UartRxBuff[10] <= 128)&&(_U16Var <10000))
                     {
                         
                       if(U32EEIndex < AdrFinReserv)       // si hay espacio para almacenar reservacion...
                       {
                           Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,U32EEIndex, 13);
                           
                           U32EEIndex+=13;                 // incrementa indice a localidad para nueva reservacion... 
                           
                           UartRxBuff[0] = 0x2A;
                           UartRxBuff[1] = 0x46;
                           UartRxBuff[2] = 0x49;
                           UartRxBuff[3] = 0x4E;
                           
                           UartRxBuff[4] = _U16Var >> 8;    // parte alta de ultimo ID reservacion
                           UartRxBuff[5] = _U16Var;         // parte baja de ultimo ID reservacion
                           
                           UartRxBuff[6] = U32EEIndex >> 24;  // Direccion actual de escritura U32EEADR 
                           UartRxBuff[7] = U32EEIndex >> 16;
                           UartRxBuff[8] = U32EEIndex >> 8;
                           UartRxBuff[9] = U32EEIndex;
                           
                           UartRxBuff[10] = 0x55;  
                           UartRxBuff[11] = 0xAA;
                           UartRxBuff[12] = 0x55; 
                           
                           Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,U32EEIndex, 13);
                           
                           
                       }
                       else                              // se ha agotado espacio => se reinicia indice de escritura
                       {
                           U32EEIndex = 0;
                       }

       
                      ConsolePut(0x23);
                      ConsolePut(0x5E);
                      ConsolePut(0x0B);
                      ConsolePut(0x01);
                      ConsolePut(0x3C);
                      ConsolePut(0x3F);
                         
                     }
                     else          // algun elemento del comando se encuentra fuera de rango
                     {
                      ConsolePut(0x23);
                      ConsolePut(0x5E);
                      ConsolePut(0x0B);
                      ConsolePut(0x00);
                      ConsolePut(0x3C);
                      ConsolePut(0x3F);
                     }
                 }
             }
             break;
            
           //-------------------------------------------------------------------
             case 12:              // borrar todas las reservaciones  235E0C3C3F
             {
                 
                 /*Inicio Sector 0:  00000h   FIN:  7FFFh          0 -   32,767 */     
                 /*Inicio Sector 1:   8000h   FIN: 0FFFFh     32,768 -   65,535 */
                 /*Inicio Sector 2:  10000h   FIN: 17FFFh     65,536 -   98,303 */
                 /*Inicio Sector 3:  18000h   FIN: 1FFFFh     98,304 -  131,071 */
                 
                 /* envia comando borrar sector 0*/
                 EEAdr = 0;
                 Eeprom25xx_SectorErase(EEPROM25XX_MEMORY0, EEAdr);
                 
                 
                 /* envia comando borrar sector 1*/
                 EEAdr = 0x00008000;
                 Eeprom25xx_SectorErase(EEPROM25XX_MEMORY0, EEAdr);
                 
                 /* envia comando borrar sector 2*/
                 EEAdr = 0x00010000;
                 Eeprom25xx_SectorErase(EEPROM25XX_MEMORY0, EEAdr);
                 
                 /* borra el resto de reservaciones escribiendo por paginas :)*/
                 
                 
                 
                 
               EEAdr = 0x00018000;            

               for(i=0;i<64;i++)
               {
                 UartRxBuff[i] = 0xFF;
               }
               
               LED_1 =1;
               LED_2 =1;
               
               

            for(_U16Var =0;_U16Var<512;_U16Var++)                             // borra 512 paginas de 64 bytes C/U
            {
                Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr, 64);
                EEAdr += 64;
                
                LED_1 ^=1;
                LED_2 ^=1;
            }
               
               
               
               U32EEADR = 0;
               U32EEIndex = 0;
               
               UartRxBuff[0] = 0x2A;
               UartRxBuff[1] = 0x46;
               UartRxBuff[2] = 0x49;
               UartRxBuff[3] = 0x4E;
                           
               UartRxBuff[4] = 0;    // parte alta de ultimo ID reservacion
               UartRxBuff[5] = 0;         // parte baja de ultimo ID reservacion
                           
               UartRxBuff[6] = U32EEADR >> 24;  // Direccion actual de escritura U32EEADR 
               UartRxBuff[7] = U32EEADR >> 16;
               UartRxBuff[8] = U32EEADR >> 8;
               UartRxBuff[9] = U32EEADR;
                           
               UartRxBuff[10] = 0x55;  
               UartRxBuff[11] = 0xAA;
               UartRxBuff[12] = 0x55; 
                           
               Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR, 13);
                 

                 
                      ConsolePut(0x23);
                      ConsolePut(0x5E);
                      ConsolePut(0x0C);
                      ConsolePut(0x01);
                      ConsolePut(0x3C);
                      ConsolePut(0x3F);
             }
             break;
           //-------------------------------------------------------------------
             
             case 13:    // descargar reservaciones ok  235e0D003c3f
             {
                 if(UartRxBuff[0] < 100)       // si bloque de reservaciones existente
                 {
                     EEAdr = (_U16)UartRxBuff[0]*(_U16)1300;
                     
                     
                      ConsolePut(0x23);
                      ConsolePut(0x5E);
                      ConsolePut(0x0D);
                      
                      
                      for(j=0;j<20;j++)
                      {
                       Eeprom25xx_Read(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr,64);
                       
                       for(i=0;i<64;i++)
                       {
                           ConsolePut(UartRxBuff[i]);
                       }
                       EEAdr += 64;
                      }
                      
                      Eeprom25xx_Read(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr,20);
                      
                      for(i=0;i<20;i++)
                      {
                        ConsolePut(UartRxBuff[i]);
                      }
                      
                      ConsolePut(0x3C);
                      ConsolePut(0x3F);
                 }
                 else
                 {
                      ConsolePut(0x23);
                      ConsolePut(0x5E);
                      ConsolePut(0x0D);
                      ConsolePut(0x00);
                      ConsolePut(0x3C);
                      ConsolePut(0x3F);
                 }
             }
             break;
           //-------------------------------------------------------------------
             case 14:    // Editar reservacion
             {
                 
                 _U16Var = SearchStoredID(UartRxBuff[10],UartRxBuff[11],UartRxBuff[12]);
                 
                 if( _U16Var != 0XFFFF)
                 {
                     
                    U32EEADR = (_U32)_U16Var * (_U32)13;
        
                    Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR,10);
                     
                    
                     ConsolePut(0x23);
                     ConsolePut(0x5E);
                     ConsolePut(0x0E);
                     ConsolePut(0x01);
                     ConsolePut(0x3C);
                     ConsolePut(0x3F);
                 }
                 else
                 {
                     ConsolePut(0x23);
                     ConsolePut(0x5E);
                     ConsolePut(0x0E);
                     ConsolePut(0x00);
                     ConsolePut(0x3C);
                     ConsolePut(0x3F);
                 }
             }
             break;
           //-------------------------------------------------------------------
                     
             case 15:    // Eliminar reservacion
             {
                 _U16Var = SearchStoredID(UartRxBuff[0],UartRxBuff[1],UartRxBuff[2]);
                 
                 if( _U16Var != 0XFFFF)
                 {
                     
                     
                     for(j=0;j<13;j++)
                     {
                         UartRxBuff[j] = 0xff;
                     }
                     
                     U32EEADR = (_U32)_U16Var * (_U32)13;
        
                    Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR,13);
                     
                     
                     ConsolePut(0x23);
                     ConsolePut(0x5E);
                     ConsolePut(0x0F);
                     ConsolePut(0x01);
                     ConsolePut(0x3C);
                     ConsolePut(0x3F);
                 }
                 else
                 {
                     ConsolePut(0x23);
                     ConsolePut(0x5E);
                     ConsolePut(0x0F);
                     ConsolePut(0x00);
                     ConsolePut(0x3C);
                     ConsolePut(0x3F);
                 }
                 
             }
             break;
           //-------------------------------------------------------------------
             case 16:    // Confirmar reservacion
             {
                  _U16Var = SearchStoredID(UartRxBuff[0],UartRxBuff[1],UartRxBuff[2]);
                  
                 if( _U16Var != 0XFFFF)
                 {
                      U32EEADR = ((_U32)_U16Var * (_U32)13);
                      
                      
                      Eeprom25xx_Read(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR,1);
                      SET_8BIT(UartRxBuff[0],7);
                      
                      Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR,1);
                      
                      
                     ConsolePut(0x23);
                     ConsolePut(0x5E);
                     ConsolePut(0x10);
                     ConsolePut(0x01);
                     ConsolePut(0x3C);
                     ConsolePut(0x3F);
                      
                 }
                 else
                 {
                     ConsolePut(0x23);
                     ConsolePut(0x5E);
                     ConsolePut(0x10);
                     ConsolePut(0x00);
                     ConsolePut(0x3C);
                     ConsolePut(0x3F);
                 }
             }
             break;
           //-------------------------------------------------------------------
             case 17:    // configurar minutos de tolerancia al encendido y al apagado
             {
                 if((UartRxBuff[0]<=60)&&(UartRxBuff[1]<=60))    // valores limitados a una hora 
                 {
                     
                     U32EEADR = ToleranceValueAddress;
                     Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR,2);
                     
                     ConsolePut(0x23);
                     ConsolePut(0x5E);
                     ConsolePut(0x11);
                     ConsolePut(0x01);
                     ConsolePut(0x3C);
                     ConsolePut(0x3F);
                     
                 }
                 else
                 {
                     ConsolePut(0x23);
                     ConsolePut(0x5E);
                     ConsolePut(0x11);
                     ConsolePut(0x00);
                     ConsolePut(0x3C);
                     ConsolePut(0x3F);
                 }
             }
             break;
           //-------------------------------------------------------------------
             
             
             
           //-------------------------------------------------------------------  
             
             
             //235E01BB00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000363C3F
             //235E01BBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF363C3F
             //235E01BB55555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555363C3F
             //235E01BBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA363C3F
             //235E010001020304050607080910111213141516171819202122232425262728293031323334353637383940414243444546473C3F
             
             
             
           case 0x20:     // arma calendario
           {
               if((UartRxBuff[0] == 0)||(UartRxBuff[0] == 0xff)||(UartRxBuff[0] == 0x55)||(UartRxBuff[0] == 0xAA))
               {
                   ConsolePut(0x23);
                   ConsolePut(0x5E);
                   ConsolePut(0x01);
                   ConsolePut(0xBB);
                   
                   for(j=0;j<46;j++)
                   {
                     ConsolePut(UartRxBuff[0]);  
                   }
                   
                   ConsolePut(0x00);
                   ConsolePut(0x36);
                   ConsolePut(0x3C);
                   ConsolePut(0x3F);
               }

           }
           break;
           
           //-------------------------------------------------------------------
           case 0x21:
           {
                 if((UartRxBuff[0] <= 31)&&(UartRxBuff[1] <= 12))
                 {
                     Time.dia_mes = UartRxBuff[0];
                     Time.mes = UartRxBuff[1];
                     
                     MuxUart(1);
                     QueryHrs();
                     MuxUart(0);

                 }
           }
           break;
           //-------------------------------------------------------------------
           
             case 0x22:     // encendido/apagado de todos los dispositivos
             { 
                if(UartRxBuff[0] <= 1)
                {
                 if(UartRxBuff[0] == 1)         // si encender todas las salidas
                 {
                  for(j=0;j<16;j++)
                  {
                    myDevicesRequiredStatus[j] = 0xff;
                    UartRxBuff[j] = 0xff;
                  }
                  
                  U32EEADR = AdrDCM;
                  Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR, 16);  // todos los dispositivos controlados manualmente
                  
                  U32EEADR = AdrEDCM; 
                  Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR, 16);  // todos los dispositivos habilitados
                 }
                    
                    
                 if(UartRxBuff[0] == 0)    // si apagar todas las salidas
                 {
                  for(j=0;j<16;j++)
                  {
                   myDevicesRequiredStatus[j] = 0x00;
                   UartRxBuff[j] = 0xff;
                  }
                  
                  U32EEADR = AdrDCM;
                  Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR, 16);  // todos los dispositivos controlados manualmente
                  
                  U32EEADR = AdrEDCM; 
                  Eeprom25xx_Write(EEPROM25XX_MEMORY0, myDevicesRequiredStatus,U32EEADR, 16);  // todos los dispositivos deshabilitados
                  
                 }
                    
                 ConsolePut(0x23);
                 ConsolePut(0x5E);
                 ConsolePut(0x22);
                 ConsolePut(0x01);
                 ConsolePut(0x3C);
                 ConsolePut(0x3F);
                   
                }
                         
             }
             break;
           
           //-------------------------------------------------------------------
             case 0x23:     
             {
                 if(UartRxBuff[0] <= 16)
                 {
                   BaseTimeToTx = UartRxBuff[0];
                 
                   ConsolePut(0x23);
                   ConsolePut(0x5E);
                   ConsolePut(0x23);
                   ConsolePut(0x01);
                   ConsolePut(0x3C);
                   ConsolePut(0x3F);
                 }
             }
             break;
             
           //-------------------------------------------------------------------
             
             case 0x24:   // valor de calibracion de temperatura
             {
                 if((UartRxBuff[0] <= PCDevices) && UartRxBuff[0] != 0)
                 {
                     
                     if(UartRxBuff[1] <= 15)
                     {                        
                      --UartRxBuff[0];
                      EEAdr = ((_U32)UartRxBuff[0]) + (_U32)CalibrationAddress;
                      i = UartRxBuff[0];
                      l = UartRxBuff[1];
                      Eeprom25xx_Write(EEPROM25XX_MEMORY0,&l ,EEAdr, 1);  // Guarda valor de calibracion
                         
                      DelayMs(10);
                      
                      EEAdr = (_U32)CalibrationAddress;
                    
                      Eeprom25xx_Read(EEPROM25XX_MEMORY0,UartRxBuff,EEAdr,16);
                      
                      
                      if(UartRxBuff[i] == l)
                      {
                         ConsolePut(0x23);
                         ConsolePut(0x5E);
                         ConsolePut(0x24);
                         ConsolePut(0x01); 
                         ConsolePut(0x3C);
                         ConsolePut(0x3F);
                      }
                            
                     }  
                     
                 }
             }
             break;
             
             
             //-----------------------------------------------------------------
             case 0x25:   // lee valores de calibracion de temperatura
             {
                 
                 EEAdr = (_U32)CalibrationAddress;
                    
                         
                 Eeprom25xx_Read(EEPROM25XX_MEMORY0,UartRxBuff,EEAdr,PCDevices);
                 
                 ConsolePut(0x23);
                 ConsolePut(0x5E);
                 ConsolePut(0x25);
                         
                 for(j=0;j<PCDevices;j++)
                 {
                  ConsolePut(UartRxBuff[j]); 
                 }
                         
                         
                 ConsolePut(0x3C);
                 ConsolePut(0x3F);
             }
             break;
             
             //-----------------------------------------------------------------
             
             case 0x26:   // reinicia valores de calibracion de temperatura
             {
                 
                 EEAdr = (_U32)CalibrationAddress;
                 
                 for(j=0;j<PCDevices;j++)
                 {
                   UartRxBuff[j] = 0;
                 }
                 
                 Eeprom25xx_Write(EEPROM25XX_MEMORY0,UartRxBuff,EEAdr,PCDevices);
                 
                 DelayMs(20);
                         
                 Eeprom25xx_Read(EEPROM25XX_MEMORY0,UartRxBuff,EEAdr,PCDevices);
                 
                 ConsolePut(0x23);
                 ConsolePut(0x5E);
                 ConsolePut(0x26);
                         
                 for(j=0;j<PCDevices;j++)
                 {
                  ConsolePut(UartRxBuff[j]); 
                 }
                         
                         
                 ConsolePut(0x3C);
                 ConsolePut(0x3F);
             }
             break;
             
             //-----------------------------------------------------------------
           
             case 0x27:
             {
                     U32EEADR = ToleranceValueAddress;
                     Eeprom25xx_Read(EEPROM25XX_MEMORY0, UartRxBuff,U32EEADR,2);
                     
                     ConsolePut(0x23);
                     ConsolePut(0x5E);
                     ConsolePut(0x27);
                     ConsolePut(UartRxBuff[0]);
                     ConsolePut(UartRxBuff[1]);
                     ConsolePut(0x3C);
                     ConsolePut(0x3F);
             }
             break;
             //-----------------------------------------------------------------
             
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
//----------------------------------------------------------------------------//
 void Calibration(void)
 {
     /*
     for(j=0;j<PCDevices;j++)
     {
         MyDevicesTemp[j] = 0;
     }
     
     MiApp_FlushTx();
     MiApp_WriteData(' ');                            // 0
     MiApp_WriteData(myNodeNumber);                   // 1
     MiApp_WriteData(MyTrackingNumber);               // 2
     MiApp_WriteData(CalibrationMode);                // 3
     MiApp_WriteData(BaseTimeToTx);                   // 4
     MiApp_WriteData(TemperatureCalibrationValue);    // 5
     
     MiApp_BroadcastPacket(FALSE);
     */
     
/* espacio reservado para calibraciones 256 bytes: pagina 430                       */
/*    inicio calibraciones =  110080                                                */
/*    Fin calibraciones    =  110335*/
     
  //Eeprom25xx_Write(EEPROM25XX_MEMORY0, UartRxBuff,EEAdr, 64);
 _U32 k = CalibrationAddress;    
 
 for(j=0;j<PCDevices;j++)
 {
     if(MyDevicesTemp[j] == TemperatureCalibrationValue)  // temperatura de calibracion = temperatura leida
     {
         i = 0;
     }
     else
     {
      if(TemperatureCalibrationValue > MyDevicesTemp[j])              // la temp de referencia es mayor a la leida
        {
          i = (TemperatureCalibrationValue - MyDevicesTemp[j]) & 0x0F;
                                    
        }
        else                                           // la temperatura leida es mayor a la temp de referencia
        {
         i = (MyDevicesTemp[j] - TemperatureCalibrationValue) & 0x0F;
         SET_8BIT(i,7);
        }
        Eeprom25xx_Write(EEPROM25XX_MEMORY0, i,k, 1);
        ConsolePut(i);
     }
     k++;
 }
     
     
     
     
 }
//----------------------------------------------------------------------------//

void  WirelessTxRx(void)
 {
     //Timer1Tick();
    
     switch(PanCoordStateMachine)
     {

         //  envia estados a todos los dispositivos (broadcast)

         case 0:
         {
             if( MiApp_MessageAvailable())
             {
                 
                 //for(i=0;i<rxMessage.PayloadSize;i++)
                 //{
                   //ConsolePut(rxMessage.Payload[i]);
                 //}
                 
                 //ConsolePut(0x3F);
                 
                 
                 
                 MiApp_DiscardMessage();
             }

             ++MyTrackingNumber;


             MiApp_FlushTx();
             MiApp_WriteData(' ');                 // 0
             MiApp_WriteData(myNodeNumber);        // 1
             MiApp_WriteData(MyTrackingNumber);    // 2
             MiApp_WriteData(OutputStat);          // 3
             MiApp_WriteData(BaseTimeToTx);        // 4

             for(j=0;j<16;j++)
             {
              MiApp_WriteData(myDevicesRequiredStatus[j]);
             }
             
             MiApp_BroadcastPacket(FALSE);
             
             
             LED_1 ^= 1;
             SwTimer0 = 0;
             PanCoordStateMachine = 1;


             



         }
         break;
//------------------------------------------------------------------------------
         // espera respuesta de cada dispositivo

         case 1:
         {
             if( MiApp_MessageAvailable())
             {
              ClrWdt();
              if((rxMessage.Payload[1] <= PCDevices)&&((rxMessage.Payload[3] == OutputStat))&&((rxMessage.Payload[4] <= 1))&&((rxMessage.Payload[5] <= 150)))
              {
                  MyDevicesTemp[rxMessage.Payload[1]-1] = rxMessage.Payload[5];   // get temperature value
                  
                  j = (rxMessage.Payload[1]-1)/8;    // calcula byte
                  i = (rxMessage.Payload[1]-1)%8;    // calcula bit
                  
                  
                  
                  //ConsolePut(rxMessage.Payload[1]);
                  //ConsolePut(rxMessage.Payload[4]);
                  //ConsolePut(rxMessage.Payload[5]);
                 

                  SET_8BIT(ConnectedDevices[j],i);
                  
                  
                  
                  if(rxMessage.Payload[4] == 1)
                  {
                   SET_8BIT(myDevicesOutputStatus[j],i);
                  }
                  else
                  {
                   if(rxMessage.Payload[4] == 0)
                   {
                    CLEAR_8BIT(myDevicesOutputStatus[j],i);
                   }
                  }
                  
              }

              MiApp_DiscardMessage();
             }

             // if((ConnectedCoordinators == 3)||(SwTimer4 >= 10))
          if(SwTimer0 >= PCDevices+5)
          {
           
            ApplyOffsetValues();
          
            PanCoordStateMachine = 2;
          }

         }
         break;
//------------------------------------------------------------------------------
         case 2:
         {
             MiApp_FlushTx();
             MiApp_WriteData(' ');                 // 0
             MiApp_WriteData(myNodeNumber);        // 1
             MiApp_WriteData(MyTrackingNumber);    // 2
             MiApp_WriteData(ListenDevices);       // 3
             //MiApp_WriteData(BaseTimeToTx);      // 4

             for(j=0;j<16;j++)
             {
              MiApp_WriteData(ConnectedDevices[j]);
             }
             
             MiApp_BroadcastPacket(FALSE);
          
           PanCoordStateMachine = 3;
         }
         break;
//------------------------------------------------------------------------------
         case 3:                          // envia cadena con dispositivos que estan siendo escuchados
         {
             
           //GetDateTime();
           //QueryCalendars();
           //QueryHrs();
           
             
             for(j=0;j<16;j++)
             {
              ConnectedDevices[j]= 0;
             }
             
             PanCoordStateMachine = 4;
             SwTimer0 = 0;
         }
         break;
//------------------------------------------------------------------------------
         case 4:
         {
               if(SwTimer0 >= 2)
               {
                PanCoordStateMachine = 0;
               }
          
         }
         break;
//------------------------------------------------------------------------------
         case 5:
         {
           
         }
         break;
         
//------------------------------------------------------------------------------
         
         
         default:
         {
          PanCoordStateMachine = 0;
         }
         break;
     }
 }

//********************************************************************//

 void  TaskScheduler(void)
 {
     Timer1Tick();

     switch(PanCoordStateMachine)
     {

         // Resincronizacion de coordinadores < Broadcast Resync packet>
         case 0:         
         {
             if( MiApp_MessageAvailable())
             {
                 MiApp_DiscardMessage();
             }


             ++MyTrackingNumber;

             MiApp_FlushTx();
             MiApp_WriteData(' ');
             MiApp_WriteData(myNodeNumber);
             MiApp_WriteData(MyTrackingNumber);
             MiApp_WriteData(Resync);
             MiApp_WriteData(BaseTimeToTx);

             MiApp_BroadcastPacket(FALSE);


             LED_1 ^= 1;

             PanCoordStateMachine = 1;
             ConnectedCoordinators = 0;
             SwTimer4 = 0;

         }
         break;

//------------------------------------------------------------------------------

         // Resincronizacion de coordinadores < espera respuesta de coordinadores>
         case 1:
         {
           if( MiApp_MessageAvailable())
           {
             if((rxMessage.Payload[3] == Resync)&&((rxMessage.Payload[2] == MyTrackingNumber))&&((rxMessage.Payload[1] <= 8)))
             {
                 SET_8BIT(ConnectedCoordinators,rxMessage.Payload[1]-1);
                 LED_2 ^= 1;
             }

             MiApp_DiscardMessage();
           }

         // if((ConnectedCoordinators == 3)||(SwTimer4 >= 10))
           if(SwTimer4 >= 15)
          {
            PanCoordStateMachine = 2;
          }
         }
         break;
//------------------------------------------------------------------------------
         // envia pkt de actualizacion a end devices pertenecientes al PANCOORD

         case 2:
         {
          
              MiApp_SetChannel(PanCoordToEndDevChan);    // switch channel to my End Devices

             MiApp_FlushTx();
             MiApp_WriteData(' ');
             MiApp_WriteData(myNodeNumber);
             MiApp_WriteData(MyTrackingNumber);
             MiApp_WriteData(OutputStat);
             MiApp_WriteData(BaseTimeToTx);

             for(j=0;j<8;j++)
             {
              MiApp_WriteData(myDevicesOutputStatus[j]);
             }

             MiApp_BroadcastPacket(FALSE);

             LED_1 ^= 1;
             SwTimer4 = 0;
             PanCoordStateMachine = 3;
         }
         break;

//------------------------------------------------------------------------------
         case 3:
         {
          if( MiApp_MessageAvailable())
          {
              //MiApp_WriteData(' ');
              //MiApp_WriteData(myNodeNumber);
              //MiApp_WriteData(MyTrackingNumber);
              //MiApp_WriteData(OutputStat);
              //MiApp_WriteData(MyStat);
              //MiApp_WriteData(MyTemp);

              if((rxMessage.Payload[1] <= PCDevices)&&((rxMessage.Payload[3] == OutputStat))&&((rxMessage.Payload[4] <= 255))&&((rxMessage.Payload[5] <= 150)))
              {
                  ConsolePutROMString( (ROM char * const) "  \r\n NODE" );
                  PrintDec(rxMessage.Payload[1]);
                  ConsolePutROMString( (ROM char * const) ": " );
                  PrintDec(rxMessage.Payload[5]); 
                  ConsolePutROMString( (ROM char * const) "C" );

              }
             
              MiApp_DiscardMessage();
          }

           if(SwTimer4 >= 8)
           {
             SwTimer4 = 0;
             PanCoordStateMachine = 4;
           }


         }
         break;
//------------------------------------------------------------------------------
         case 4:
         {
          if(SwTimer4 >= 2)
          {
            MiApp_SetChannel(PanCoordToCoordChan);    // switch channel to Coordinators Channel
            PanCoordStateMachine = 0;
          }
         }
         break;
//------------------------------------------------------------------------------

         default:
         {

         }
         break;
     }


 }

//****************************************************************************//
 _U08 BCDTOHEX(_U08 c)
 {
    _U08 HEX;
    
    HEX = ((c>>4)*10)+(c&0x0f);
    return HEX;
 }
//------------------------------------------------------------------------------ 

 // Devuelve byte y bit correspondiente al dia y mes que se de como parametros

  void Return_ByteBit(_U08 dia, _U08 mes)
  {
      _U08 DiasMes[12] = {31,29,31,30,31,30,31,31,30,31,30,31}; //
      _U16 Dias = 0;


      --mes;
      
      
      for(i=0;i<mes;i++)
      {
          Dias += DiasMes[i];
      }

      Dias += dia;

      if(Dias > 0)
      {

      if(Dias < 9)
      {
          ByteBitVal.Byte = 0;
          ByteBitVal.Bit  = Dias-1;
      }
      else
      {
          ByteBitVal.Byte = (Dias-1)/8;

         if(Dias%8 == 0)
         {
          ByteBitVal.Bit = 7;
        
         }
         else
         {
          ByteBitVal.Bit  = (Dias%8)-1;
         }
      }
  }
   else
   {
     ByteBitVal.Bit = 0xFF;
     ByteBitVal.Byte = 0xFF;
   }

  }
//****************************************************************************//
  
  void QueryHrs(void)
  {
      _U08 DiasMes[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
      _U08 Hrio[2];
      _U08 HoraInicio;
      _U08 MinutoInicio;
      _U08 HoraFin;
      _U08 MinutoFin;
      _U08 Minutos;
      _U08 Horas;
      _U32 Index;
      _U16 Dias = 0;
      _U08 mes;
      _U08 _Bit  = 0;
      _U08 _Byte = 0;

      
      mes = BCDTOHEX(Time.mes);
      
      --mes;
      
      
      for(i=0;i<mes;i++)
      {
          Dias += DiasMes[i];
      }

      Dias += BCDTOHEX(Time.dia_mes);
      
      
     for(j=0;j<16;j++)
     {
        myDevicesRequiredStatus[j] = 0;
         //EnabledDevices[j] = 0xFF;
     }
      
      Minutos = BCDTOHEX(Time.minutos);
      Horas   = BCDTOHEX(Time.horas);
      
      /*
      MuxUart(1);
      
          ConsolePutROMString((ROM char *)"\r\n Fecha - Hora : ");
          PrintChar(Time.dia_mes);
          
          ConsolePutROMString((ROM char *)" / ");
          PrintChar(Time.mes);
          ConsolePutROMString((ROM char *)" / ");
          PrintChar(Time.anio);
          
          ConsolePutROMString((ROM char *)"  ");
          
          PrintChar(Time.horas);
          ConsolePutROMString((ROM char *)":");
          PrintChar(Time.minutos);
          ConsolePutROMString((ROM char *)":");
          PrintChar(Time.segundos);
                 
               */      
      
      
      for(j=0;j<PCDevices;j++)     // desde dispositivo 0 hasta PCDevices
      //for(j=0;j<10;j++)     // desde dispositivo 0 hasta PCDevices  
      {
          Index = ((_U32)j*(_U32)HRS_LEN)+((_U32)PCDevices*(_U32)CAL_LEN)+((Dias*2)-2);  // inicio del horario correspondiente al dispositivo indexado
          Eeprom25xx_Read(EEPROM25XX_MEMORY0, Hrio,Index,2);                             // extrae hora de inicio y de fin del dia de hoy del dispositivo indexado
         
          
          
          
          
          //ConsolePutROMString((ROM char *)"\r\n valor de horario Dispositivo");
          //PrintDec(j);
          //ConsolePutROMString((ROM char *)":");
          
          
          //ConsolePut(Index>>8); 
          //ConsolePut(Index);
          //ConsolePut(Hrio[0]);
          //ConsolePut(Hrio[1]);
          
          HoraInicio = Hrio[0]&0x1F;
          HoraFin =Hrio[1]&0x1F;
          
          MinutoInicio = ((Hrio[0] >> 5)&0x07)*10;
          MinutoFin    = ((Hrio[1] >> 5)&0x07)*10;
          
          
          
          //PrintDec(HoraInicio);
          //ConsolePutROMString((ROM char *)":");
          //PrintDec(MinutoInicio);
          //ConsolePutROMString((ROM char *)" - ");
          //PrintDec(HoraFin);
          //ConsolePutROMString((ROM char *)":");
          //PrintDec(MinutoFin);
          
          
          
          //SET_8BIT(EnabledDevices[_Byte],_Bit);
          
          // habilita a "x" hora y permanece encendido hasta siguiente dia (no hay hora de apagado)
          if((Hrio[0]<24)&&(Hrio[1]== 0xff)) 
          {
              if((Horas >= HoraInicio)&&(Minutos >= MinutoInicio))  // Si dispositivo debe estar habilitado
              {
                SET_8BIT(myDevicesRequiredStatus[_Byte],_Bit);      
                
                //ConsolePutROMString((ROM char *)" A");
              }
          }
          
          
          // se apaga a "x" hora y se habilita posteriormente a "y" hora  (X < Y)
          if((Hrio[0] > Hrio[1])&&(Hrio[0] != 0xff)&&(Hrio[1] != 0xff))
          {
              if((Horas < HoraFin)||((Horas == HoraFin)&&(Minutos < MinutoFin)))          
              {
                  SET_8BIT(myDevicesRequiredStatus[_Byte],_Bit);
                  
                  //ConsolePutROMString((ROM char *)" B");
              }
              else
              {
                  if((Horas >= HoraInicio)&&(Minutos >= MinutoInicio))   
                  {
                      SET_8BIT(myDevicesRequiredStatus[_Byte],_Bit);
                      
                     // ConsolePutROMString((ROM char *)" C");
                  }
              }
          }
          
          
          // se apaga a "X" hora (no hay hora de encendido)
          if((Hrio[0] == 0xff)&&(Hrio[1] < 24)) 
          {
              if((Horas <= HoraFin)&&(Minutos < MinutoFin))
              {
                  SET_8BIT(myDevicesRequiredStatus[_Byte],_Bit);
                  
                 // ConsolePutROMString((ROM char *)" D");
              }
          }
          
          
          // no hay eventos de apagado o habilitacion  ( el estado lo determina el calendario)
          if((Hrio[0] == 0xff)&&(Hrio[1] == 0xff))
          {
              //ConsolePutROMString((ROM char *)" E");
          }
          
          //ConsolePutROMString((ROM char *)"\r\n");
          
          
          
        if(_Bit < 7)
        {
            ++_Bit;
        }
        else
        {
            ++_Byte;
            _Bit = 0;
        }
          
          
      }
      
      //ConsolePut(0x3F);
     
      for(j=0;j<PCDevices;j++)
     {
         myDevicesRequiredStatus[j] &= EnabledDevices[j];    // aplica condiciones de calendario
     }
      
      //MuxUart(0);
      
  }
//----------------------------------------------------------------------------//


 // Toma de el calendario de cada dispositivo el bit correspondiente al dia de hoy y
 // lo pone en el arreglo myDevicesRequiredStatus
 void QueryCalendars(void) 
 {
     _U08 k;
     _U08 Today = 0;
     _U16 EEAdr;
     _U08 _Bit  = 0;
     _U08 _Byte = 0;
     

     for(j=0;j<16;j++)
     {
        //myDevicesRequiredStatus[j] = 0;
         EnabledDevices[j] = 0;
     }

     Return_ByteBit(Time.dia_mes,Time.mes);


     for(k=0;k<PCDevices;k++)   // loop trought calendars (devices)
     {
        EEAdr = ((_U16)k*CAL_LEN)+ ByteBitVal.Byte;
        Eeprom25xx_Read(EEPROM25XX_MEMORY0, &Today,EEAdr,1);   // lee byte 


        if(QUERY_8BIT(Today,ByteBitVal.Bit))
        {
            SET_8BIT(EnabledDevices[_Byte],_Bit);
        }
        else
        {
            //CLEAR_8BIT(EnabledDevices[_Byte],_Bit);
        }

        if(_Bit < 7)
        {
            ++_Bit;
        }
        else
        {
            ++_Byte;
            _Bit = 0;
        }
     }
 }

//------------------------------------------------------------------------------
 //Devuelve 1 si es tiempo de revisar los calendarios
 
 _U08 TimeToQueryCalendars(void)
 {
     if(SwTimer3 > 20)
     {
         SwTimer3 = 0;
         return 1;
     }
     else
     {
         
         return 0;
     }
     
 }
//****************************************************************************//

 void CalendarTask(void)
 {
     Timer1Tick();
     
     if(TimeToQueryCalendars()==1)
     {
      //GetDateTime();
      //QueryCalendars();
      //QueryHrs();
         
         
         //LED_1 ^= 1;
         //LED_2 ^= 1;
     }



 }

 //***************************************************************************//
 //---------------------------------------------------------------------------//
 /* Busca la direccion donde se ha almacenado ultimo evento de reservacion*/
 
 void SearchEepromIndex(void)
 {
     //if(U32EEADR < AdrFinReserv)
     
     
   U32EEIndex = 0;   
   j=0;
   
   ClrWdt();
   
   for(_U16Var = 0; _U16Var < 10000; _U16Var++)
   {
       Eeprom25xx_Read(EEPROM25XX_MEMORY0,Rs485Buff,U32EEIndex,13);               // lee localidad 
       
       if((Rs485Buff[0]==0x2A)&&(Rs485Buff[1]==0x46)&&(Rs485Buff[2]==0x49)&&(Rs485Buff[3]==0x4E))  // si se encuentra indice de lectura...
       {
           //ConsolePut(0x2A); 
           //ConsolePut(0x46); 
           //ConsolePut(0x3F); 
           
           break;
       }
       else
       {
           /*
           if(j==100)
           {
               j=0;
                      ConsolePut(U32EEADR>>24);
                      ConsolePut(U32EEADR>>16);
                      ConsolePut(U32EEADR>>8);
                      ConsolePut(U32EEADR);
                      ConsolePut(0x3F); 
           }
           
           j++;
                 */     
           
       }
       
       
       
      U32EEIndex += 13; 
      
   }
   
   
     
    
     
     
     
 }
 //-----------------------------------------------------------------------------
 /*Lee estado de dispositivos almacenado en eeprom */
 
 void LoadDevicesStatus(void)
 {
  U32EEADR = AdrEDCM;
  Eeprom25xx_Read(EEPROM25XX_MEMORY0,myDevicesRequiredStatus,U32EEADR,16);
  
  ConsolePut(0xAA); 
  ConsolePut(0xAA); 
  
  for(j=0;j<16;j++)
  {
      ConsolePut(myDevicesRequiredStatus[j]);
      
  }
  
  ConsolePut(0x3F); 
  
  
 }
 //-----------------------------------------------------------------------------
 //-----------------------------------------------------------------------------
 //-----------------------------------------------------------------------------
 void MuxUart(_U08 Uart)
 {
     if(Uart == 0)              // Uart out from Wiznet port
     {
         while(!TXSTA2bits.TRMT);            // espera finalice transmision en curso...
         
         PPSUnLock();
         
         PPSOutput(PPS_RP6, PPS_TX2CK2);    // TX2 RP17/RC6   ethernet
         PPSInput(PPS_RX2DT2, PPS_RP5);     // RX2 RP18/RC7
       
         PPSLock();
     }
     
     if(Uart == 1)              // Uart out from ICSP port
     {
         
         while(!TXSTA2bits.TRMT);            // espera finalice transmision en curso...
         
         PPSUnLock();
         
         PPSOutput(PPS_RP10, PPS_TX2CK2);    // TX2 RP17/RC6     icsp
         PPSInput(PPS_RX2DT2, PPS_RP9);      // RX2 RP18/RC7
         
         PPSLock();
     }
     
 }
 //***************************************************************************//
 

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

    if(myNodeNumber == 0)
    {
       PPSOutput(PPS_RP6, PPS_TX2CK2);    // TX2 RP17/RC6   ethernet
       PPSInput(PPS_RX2DT2, PPS_RP5);     // RX2 RP18/RC7

      //PPSOutput(PPS_RP10, PPS_TX2CK2);    // TX2 RP17/RC6     icsp
      //PPSInput(PPS_RX2DT2, PPS_RP9);     // RX2 RP18/RC7
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



     LED_1 = 1;
     LED_2 = 0;


     for(j=0;j<20;j++)
     {
         DelayMs(250);

         LED_1 ^= 1;
         LED_2 ^= 1;


     }

     LED_1 = 0;
     LED_2 = 0;
     
     
     Eeprom25xx_Init(EEPROM25XX_MEMORY0, EEPROM25XX_1M);          // inicializa memoria 0, 128 kbits
     
    U32EEADR = 0;
    
     
     SearchEepromIndex();
     
     LoadDevicesStatus();


     StartWirelessConnection();
     TickScaler = 2;

     for(j=0;j<16;j++)
     {
      ConnectedDevices[j]= 0;
      myDevicesOutputStatus[j] = 0;
     }

     for(j=0;j<PCDevices;j++)
     {
         MyDevicesTemp[j] = 0;
     }

     

     while(1)
     {
      //TaskScheduler();
      Timer1Tick();
      WirelessTxRx();
      EthernetTask();
      //CalendarTask();
     }

}