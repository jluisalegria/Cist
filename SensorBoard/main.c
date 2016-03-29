/*Modificaciones:
   *Se cambio el parpadeo del led status,
      Un parpadeo cada segundo = Flotador no activado
      Un Parpadeo cada 200 ms  = Flotador activado
      
   * Se hizo un castaje a las variables de la formula del calculo del porcentaje,
      Dado que en cisternas con mayor presion, superabamos valores de variables y
      obteniamos resultados erroneos.
*/
#include <18F14K22.h>
#device ADC=10
//*******************************Fuses***************************************\\
//***************************************************************************\\
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES PUT                      //Power Up Timer
#FUSES NOPLLEN                  //
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES NOIESO 
#FUSES MCLR
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES HS                       //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES PUT                      //No Power Up Timer
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOWRT                    //Program memory not write protected
#FUSES RESERVED                 //Used to set the reserved FUSE bits

#use delay(clock=12000000)
#use rs232(baud=38400,parity=N,xmit=PIN_B7,rcv=PIN_B5,bits=8,stream=RS485) //Comunicacion con Wireless Board
#use rs232(baud=38400,parity=N,xmit=PIN_A0,rcv=PIN_A1,bits=8,stream=pickit) //Comunicacion con Pickit para configuracion

#priority rda,ext1,timer1

#define Led_Status     PIN_C1
#define DRV_485        PIN_B6
#define Pin_Flotador   PIN_C2
#define Relay_Izq      PIN_C5
#define Relay_Der      PIN_C4
//*********************************Variables*********************************\\
//***************************************************************************\\
int8 Blink=0;
int8 Tiempo=0;
int8 Blink_Flotador=0;
int8 Tiempo_Blink=0;
int8 Tiempo_Envio_CMD;  //Tiempo Para el envio del comando status
int8 Num_Serie1;        //Numero de serie de la tarjeta 1er Byte
/*int8 Num_Serie2;      //Numero de serie de la tarjeta 2do Byte
int8 Num_Serie3;        //Numero de serie de la tarjeta 3er Byte
int8 Num_Serie4;        //Numero de serie de la tarjeta 4to Byte
int8 Num_Serie5;        //Numero de serie de la tarjeta 5to Byte
int8 Num_Serie6;        //Numero de serie de la tarjeta 6to Byte
*/
//*********************Variables para calculos de cisternas******************\\
// Las variables de 8 bits se almacenan en EEprom del micro,
// Las variables de 16 bits se utilizan para realizar los calculos

int16 Valor_ADC=0;      // Para el ADC de 10 bits, es el valor leido
int8  Valor_ADC_High=0; //Byte alto de la lectura del ADC
int8  Valor_ADC_Low=0;  //Byte bajo de la lectura del ADC

int16 Valor_ADC_Vacio;
int8 Valor_ADC_Vacio_Low;
int8 Valor_ADC_Vacio_High;

int16 Valor_ADC_Lleno;
int8 Valor_ADC_Lleno_Low;
int8 Valor_ADC_Lleno_High;

int16 Valor_ADC_1m;
int8 Valor_ADC_1m_Low;
int8 Valor_ADC_1m_High;

int8 Metros_Cubicos_Cisterna;  
int8 Numero_de_Cisterna;
int8 Altura_Cisterna;

//int16 Metros_Cubicos_Medidos2 ;     //pruebas
//int16 Metros_Cubicos_Medidos;     //pruebas

int8 Contador_ADC_Minimo_Valor=0;   
int8 Contador_ADC_Maximo_Valor=0;
//***************************************************************************\\
int8 Flotador=0;        //Estado del flotador
int8 Respuesta;         //Bytes de Respuesta de algun CMD recibido, 00= Rechazado, 01= Aceptado
int8 CMD_Ejecutado;     //indica el comando del cual se esta dando respuesta
int8 Segundos=0;        //Temporizador
int8 Estado_Relays=0;   //Byte Para manipular el estado de los relays on board

int32 Resta;  
int32 Resta2;
int8 Porcentaje=0;
//****************************Variables com serial***************************\\
//int8 Timeout=0;
int8 Char_Recibido_Pickit=0;
int8 Char_Recibido_RS485=0;
int8 Indice_Pickit=0;
int8 Indice_RS485=0;
char Pickitbuff[30];
char RS485buff[30];
int1 F_CMD_Completo_Pickit=0;
int1 F_CMD_Completo_RS485=0;

//*****************************Interrupcion Timer 1**************************\\
// Cada 100 ms
#INT_TIMER1
void  TIMER1_isr(void) 
{
   set_timer1(28036);
   Blink++;
   Blink_Flotador++;
   Tiempo++; 
   if(Tiempo>=10)
   {
      Tiempo=0;
      Segundos++;
   }
}

//***************Interrupción por serial del pickit***************************\\
#INT_EXT1   
void  EXT1_isr(void) 
{
   Char_Recibido_Pickit=0x00;
  if(kbhit(pickit))
   {
     Char_Recibido_Pickit=fgetc(pickit);                
     Pickitbuff[Indice_Pickit++]=Char_Recibido_Pickit; 
     
     if(Indice_Pickit>29)
     {
         Indice_Pickit=0;
     }
       if (Char_Recibido_Pickit==0X3F)  
         {
            F_CMD_Completo_Pickit=1;
         }                                                                                                               
   }
}
//************************Interrupcion serial 485***************************\\
#INT_RDA
void  RDA_isr(void) 
{
   Char_Recibido_RS485=0x00;
  if(kbhit(RS485))
   {
     Char_Recibido_RS485=fgetc(RS485);                // lo descargo
     RS485buff[Indice_RS485++]=Char_Recibido_RS485;                 // lo añado al buffer
     
     if(Indice_RS485>29)
     {
         Indice_RS485=0;
     }
      if (Char_Recibido_RS485==0X3F)  
         {
            F_CMD_Completo_RS485=1;
         }                                                                                                          
   }
   
}

void Leer_Bytes_de_Config(void)
{
   delay_ms(1);     
   Tiempo_Envio_CMD = read_eeprom (0);
   Num_Serie1 = read_eeprom (1);    //En la localidad 1 se encuentra el 1er byte del numero de serie
   Estado_Relays = read_eeprom(2);
/* 
   Num_Serie2 = read_eeprom (2);    //En la localidad 2 se encuentra el 2do byte del numero de serie
   Num_Serie3 = read_eeprom (3);    //En la localidad 3 se encuentra el 3er byte del numero de serie
   Num_Serie4 = read_eeprom (4);    //En la localidad 4 se encuentra el 4to byte del numero de serie
   Num_Serie5 = read_eeprom (5);
   Num_Serie6 = read_eeprom (6);
*/  
   Valor_ADC_Vacio_Low = read_eeprom (17);
   Valor_ADC_Vacio_High = read_eeprom(16);
   Valor_ADC_Lleno_Low = read_eeprom (19);
   Valor_ADC_Lleno_High = read_eeprom (18);
   Valor_ADC_1m_Low = read_eeprom (21);
   Valor_ADC_1m_High = read_eeprom (20);
   Metros_Cubicos_Cisterna = read_eeprom (23);
   Altura_Cisterna = read_eeprom(22);
   Numero_de_Cisterna= read_eeprom(24);

// Convierto variables int8 a int16
   Valor_ADC_Vacio = (Valor_ADC_Vacio_High<<8);
   Valor_ADC_Vacio += Valor_ADC_Vacio_Low;    
   Valor_ADC_Lleno = (Valor_ADC_Lleno_High<<8);
   Valor_ADC_Lleno += Valor_ADC_Lleno_Low;  
   Valor_ADC_1m = (Valor_ADC_1m_High<<8);
   Valor_ADC_1m += Valor_ADC_1m_Low;  
}

//****************Funcion que envia el estado por el Pickit******************\\
void Envia_Estado_Pickit(void)
{
       //output_toggle(Led_Status);
       //Segundos = (Tiempo_Envio_CMD-1);
      // delay_ms(5);
       fputc(0X23,Pickit);
       fputc(0X5E,Pickit);
       fputc(Num_Serie1,Pickit);
       /*fputc(Num_Serie2,Pickit);
       fputc(Num_Serie3,Pickit);
       fputc(Num_Serie4,Pickit);
       fputc(Num_Serie5,Pickit);
       fputc(Num_Serie6,Pickit);*/
       fputc(0X00,pickit);
       fputc(Estado_Relays,Pickit);
       fputc(Flotador,Pickit);
       fputc(Valor_ADC_High,Pickit);
       fputc(Valor_ADC_Low,Pickit);
       fputc(Valor_ADC_Vacio_High,Pickit);
       fputc(Valor_ADC_Vacio_Low,Pickit);
       fputc(Valor_ADC_Lleno_High,Pickit);
       fputc(Valor_ADC_Lleno_Low,Pickit);
       fputc(Porcentaje,Pickit);
     //fputc(Metros_Cubicos_Medidos2,Pickit);
     //fputc(Metros_Cubicos_Medidos,Pickit);
       fputc(Metros_Cubicos_Cisterna,Pickit);
       fputc(Numero_de_Cisterna,Pickit);
       fputc(0X3C,Pickit);
       fputc(0X3F,Pickit);
}

//*****************Funcion que envia el estado por el 485*******************\\
void Envia_Estado_RS485(void)
{
       //output_toggle(Led_Status);
       //Segundos = (Tiempo_Envio_CMD-1);
       output_high(DRV_485);                                     //Envia   rs-485
       delay_ms(1);
       fputc(0X23,RS485);
       fputc(0X5E,RS485);
       fputc(Num_Serie1,RS485);
       /*fputc(Num_Serie2,RS485);
       fputc(Num_Serie3,RS485);
       fputc(Num_Serie4,RS485);
       fputc(Num_Serie5,RS485);
       fputc(Num_Serie6,RS485);*/
       fputc(0X00,RS485);
       fputc(Estado_Relays,RS485);
       fputc(Flotador,RS485);
       fputc(Valor_ADC_High,RS485);
       fputc(Valor_ADC_Low,RS485);
       fputc(Valor_ADC_Vacio_High,RS485);
       fputc(Valor_ADC_Vacio_Low,RS485);
       fputc(Valor_ADC_Lleno_High,RS485);
       fputc(Valor_ADC_Lleno_Low,RS485);
       fputc(Porcentaje,RS485);
     //fputc(Metros_Cubicos_Medidos2,RS485);
     //fputc(Metros_Cubicos_Medidos,RS485);
       fputc(Metros_Cubicos_Cisterna,RS485);
       fputc(Numero_de_Cisterna,RS485);
       fputc(0X3C,RS485);
       fputc(0X3F,RS485);
       delay_ms(3);
       output_low(DRV_485);
}

//**************Funcion para cambiar el estado de los relays*****************\\
void Actualiza_Estado_Relays(void)
{
   switch(Estado_Relays)
   {
      case 0X00:
            output_low(Relay_Izq);
            output_low(Relay_Der);
            break;
      case 0x01:
            output_low(Relay_Izq);
            output_high(Relay_Der);
            break;
      case 0x10:
            output_high(Relay_Izq);
            output_low(Relay_Der);
            break;
      case 0x11:
            output_high(Relay_Izq);
            output_high(Relay_Der);
            break;
      default:
            output_low(Relay_Izq);
            output_low(Relay_Der);
   }        
}
//*******************Verifica el estado del flotador*************************\\
void Sensores(void)
{
      if(input(Pin_Flotador))
      {
         Flotador=0x01;          
         //Estado_Relays=0X00;
               //output_high(Led_Status);
               Tiempo_Blink=2;
               
      }
      else 
      {
         Flotador=0x00;

               Tiempo_Blink=10;

      } 
}
//****************************Lee el estado del ADC**************************\\
void Lee_ADC(void)
{
   set_adc_channel(4);       
   delay_us(100);
   Valor_ADC=read_adc();
   int16 Valor_ADC_2;
   Valor_ADC_2 = Valor_ADC;
   Valor_ADC_Low = Valor_ADC_2;
   Valor_ADC_High = Valor_ADC_2>>8;
}

//**************Y ajusta los setting maximo y minimo del ADC*****************\\
void Calcula_Valores_Limite_ADC(void)
{
   if(Valor_ADC_Vacio_High != 0XFF && Valor_ADC_LLeno_High != 0XFF) //si estan configurados los valores del adc vacio y lleno
    {    
      if(Valor_ADC < Valor_ADC_Vacio)     //si el valor leido es menor al valor minimo configurado, 
         {
            Contador_ADC_Minimo_Valor++;  //inicia un contador, si despues de 25 segundos el valor se conserva ajusta el valor de adc minimo
            Porcentaje=0X00;              //Envia el porcentaje minimo posible,
            
            if(Contador_ADC_Minimo_Valor>40)  //despues de 10 segundos ajusta el valor Que indica la lectura minima del ADC.
            {
               Contador_ADC_Minimo_Valor=0;
               Valor_ADC_Vacio=Valor_ADC;
               Valor_ADC_Vacio_Low = Valor_ADC_Vacio;
               Valor_ADC_Vacio_High = Valor_ADC_Vacio>>8;
               Valor_ADC_Vacio=Valor_ADC;
               write_eeprom(16,Valor_ADC_Vacio_High);   //Valor_ADC_Vacio_High
               write_eeprom(17,Valor_ADC_Vacio_Low);   //Valor_ADC_Vacio_Low   
            }
         }
         
         if(Valor_ADC > Valor_ADC_Lleno )        //si el valor leido es mayor al valor minimo configurado, y aun no se ha activado el flotador.
         {
            Porcentaje=0X64;
            if (Flotador == 0)               // solamente seteara el valor maximo hasta llegar al flotador,
            {
               Contador_ADC_Maximo_Valor++;        //inicia un contador, si despues de 10 segundos el valor se conserva ajusta el valor de adc maximo.
                                  //Envia el porcentaje maximo posible
                  if(Contador_ADC_Maximo_Valor>40)     //despues de 25 segundos. Debera ser mayor a 20 seg, para evitar settings erroneos
                  {
                     Contador_ADC_Maximo_Valor=0;
                     Valor_ADC_Lleno=Valor_ADC;
                     Valor_ADC_Lleno_Low = Valor_ADC_Lleno;
                     Valor_ADC_Lleno_High = Valor_ADC_Lleno>>8;
                     Valor_ADC_Lleno=Valor_ADC;
                     write_eeprom(18,Valor_ADC_Lleno_High);   //Valor_ADC_Vacio_High
                     write_eeprom(19,Valor_ADC_Lleno_Low);   //Valor_ADC_Vacio_Low   
                  }
            }
         }
    }
    
}

void Calcula_Nivel(void)
{  
//************Calcula el Valor del adc maximo con cisterna llena*************\\

    if(Valor_ADC_1m_High!=0XFF && Valor_ADC_Vacio_High != 0XFF && Altura_Cisterna!=0XFF && Valor_ADC_Lleno_High == 0XFF )
    {
         int16 Valor_ADC_Lleno_2 = ((Valor_ADC_1m - Valor_ADC_Vacio)*(Altura_Cisterna)); //Valor_ADC_Lleno_2 se usa para guardarlo en dos bytes.
         Valor_ADC_LLeno = Valor_ADC_Lleno_2;
         Valor_ADC_Lleno_Low = Valor_ADC_Lleno_2;
         Valor_ADC_Lleno_High = Valor_ADC_Lleno_2>>8;
         write_eeprom(18,Valor_ADC_Lleno_High);   //Valor_ADC_Vacio_High
         write_eeprom(19,Valor_ADC_Lleno_Low);    //Valor_ADC_Vacio_Low 
         write_eeprom(20,0XFF);                   //seteo el valor del ADC a 1 m a ff para no volver a realizar el calculo
         write_eeprom(21,0XFF);                   //seteo el valor de ADC 1m a ff
         Leer_Bytes_de_Config();
    }
//***************************************************************************\\    
//****************Calcula el porcentaje de agua en cisterna******************\\
//Si se han configurado el valor del adc con cisterna llena y vacia, se puede realizar el
//calculo del porcentaje del valor leido.

    if(Valor_ADC_Vacio_High != 0XFF && Valor_ADC_LLeno_High != 0XFF) //si estan configurados los varores del adc vacio y lleno
    {    
         if(Valor_ADC >= Valor_ADC_Vacio && Valor_ADC <= Valor_ADC_Lleno)
         {
            Contador_ADC_Minimo_Valor=0;
            Contador_ADC_Maximo_Valor=0;
            Resta = ((int32)(Valor_ADC - Valor_ADC_Vacio)*(0x64));
            Resta2 = ((int32)(Valor_ADC_Lleno - Valor_ADC_Vacio));
            Porcentaje = (Resta/Resta2);
            //Metros_Cubicos_Medidos2 = (Porcentaje * Metros_Cubicos_Cisterna);
            //Metros_Cubicos_Medidos = (Metros_Cubicos_Medidos2/0X0064);
         }
      /*if(Porcentaje<=0X5A)      //90 % expresado en hexadecimal
      {
            //Estado_Relays=0x11;
      }
      else
      {
            Estado_Relays=0x00;
      }*/
    }
}

//*****************Funcion que atiende las temporizaciones*******************\\
void Temporizaciones(void)
{
   if(Segundos>=Tiempo_Envio_CMD && Tiempo_Envio_CMD != 0 ) //
      {
         Sensores();
         Lee_ADC();
         Calcula_Nivel();
         Envia_Estado_Pickit();
        //Envia_Estado_RS485();
         Tiempo=0;
         Segundos=0;
      }
   if(Blink>=10)
      {
         if(Tiempo_Envio_CMD==0)
         {
               //output_toggle(Led_Status);    //cuando el envio por el pickit se hace por peticiones, es decir t=0.
         }
         Blink=0;
         Indice_Pickit=0;
         Indice_RS485=0;
         //Sensores();
         Lee_ADC();
         Calcula_Valores_Limite_ADC();
      }
    if(Blink_Flotador>=Tiempo_Blink)
      {
         Blink_Flotador=0;
         output_toggle(Led_Status);
      }
      
}

//****************************************************************************\\
void Verifica_CMD_Pickit(void)
{
   if(F_CMD_Completo_Pickit==1)
   {
      F_CMD_Completo_Pickit=0;   
      
       if(Pickitbuff[0]==0X23)      //Tiene llave de inicio 1er byte
       {
         if(Pickitbuff[1]==0X5E)    //Tiene llave de inicio 2do byte
         {
             if(Pickitbuff[2]==0X02) //Es CMD para la wireless Board, solo reeenvia por el aurt RS485
            {
                        output_high(DRV_485);                                     //Envia   rs-485
                        delay_ms(1);
                       for(int i=0;i<Indice_Pickit;i++)
                       {
                           fputc(Pickitbuff[i],RS485);   
                       }
                       delay_ms(3);
                        output_low(DRV_485); 
                       //Indice_Pickit=0;
                       break;
            }
            if(Pickitbuff[2]==0X01) //Es CMD para sensor Board, procede a verificar el comando.
            {
                     if(Pickitbuff[3] == Num_Serie1)     
            
                       switch(Pickitbuff[4])  // verifica el byte de comandos [3]
                       {
                       
                           case 0:
                                 {
                                    if(Pickitbuff[5]==0X3C && Pickitbuff[6]==0X3F)
                                    {
                                       Sensores();
                                       Lee_ADC();
                                       Calcula_Nivel();
                                       Envia_Estado_Pickit();
                                       goto Salida;
                                    }
                                 }
                                 break;
                           case 1:  //Comando para modificar el tiempo entre comandos de status
                                 {
                                   if(Pickitbuff[6]==0X3C && Pickitbuff[7]==0X3F)
                                   {
                                       if(Pickitbuff[5]<0xF0) //0x019= 25 en decimal, multiplicado po 10=250 
                                       {
                                          write_eeprom(0,(Pickitbuff[5]));// el dato recibido esta expresado en segundos 
                                          delay_ms(1);
                                          Tiempo_Envio_CMD = read_eeprom (0);
                                          Tiempo=0;
                                          Segundos=0;
                                          Respuesta=1;
                                          CMD_Ejecutado=1;
                                       //goto Respuesta_CMD;
                                       }
                                    else
                                       {   
                                        CMD_Ejecutado=1;
                                          Respuesta=0;
                                       //goto Respuesta_CMD;
                                       }
                                   }
                                 }
                                 break;
                            case 2:  //Comando para activar relays onboard
                                 {
                                 if(Pickitbuff[6]==0X3C && Pickitbuff[7]==0X3F)
                                   {
                                       CMD_Ejecutado=2;
                                       Respuesta=0;
                                       if(Pickitbuff[5]==0x00)
                                       {
                                          Estado_Relays=0X00;
                                          Respuesta=1;
                                           write_eeprom(2,0X00);
                                       }
                                       if(Pickitbuff[5]==0X10)
                                       {
                                          Estado_Relays=0X10;
                                          Respuesta=1;
                                           write_eeprom(2,0X10);
                                       }
                                       if(Pickitbuff[5]==0X01)
                                       {
                                          Estado_Relays=0X01;
                                          Respuesta=1;
                                           write_eeprom(2,0X01);
                                       }
                                       if(Pickitbuff[5]==0X11)
                                       {
                                          Estado_Relays=0X11;
                                          Respuesta=1;
                                           write_eeprom(2,0X11);
                                       }
                                    }
                                 }
                                 break;
                                 
                             case 3:  //Comando para establecer el numero de serie
                                {
                                    if(Pickitbuff[6]==0X3C && Pickitbuff[7]==0X3F)
                                   {
                                          CMD_Ejecutado=3;
                                          write_eeprom(1,(Pickitbuff[5]));
                                       /* write_eeprom(2,(Pickitbuff[6]));
                                          write_eeprom(3,(Pickitbuff[7]));
                                          write_eeprom(4,(Pickitbuff[8]));
                                          write_eeprom(5,(Pickitbuff[9]));
                                          write_eeprom(6,(Pickitbuff[10]));*/
                                          Leer_Bytes_de_Config();
                                          Respuesta=1;
                                    }
                                }
                                 break;
                                 
                              case 4:  //Comando para enviar el valor del offset
                                 {
                                    if(Pickitbuff[9]==0X3C && Pickitbuff[10]==0X3F)
                                   {
                                       CMD_Ejecutado=4;
                                       Respuesta=1;
                                       write_eeprom(16,(Pickitbuff[5]));   //Valor_ADC_Vacio_High
                                       write_eeprom(17,(Pickitbuff[6]));   //Valor_ADC_Vacio_Low
                                       write_eeprom(18,(Pickitbuff[7]));   //Valor_ADC_LLeno_High
                                       write_eeprom(19,(Pickitbuff[8]));   //Valor_ADC_Lleno_Low
                                       Leer_Bytes_de_Config();
                                    }
                                 }
                                 break;
                                 
                              case 5 :
                                 {
                                    if(Pickitbuff[10]==0X3C && Pickitbuff[11]==0X3F)
                                   {
                                          write_eeprom(20,(Pickitbuff[5]));   //Valor_ADC_1m_High
                                          write_eeprom(21,(Pickitbuff[6]));  //Valor_ADC_1m_Low
                                          write_eeprom(22,(Pickitbuff[7]));  //Altura cisterna
                                          write_eeprom(23,(Pickitbuff[8]));  //Metros cubicos cisterna
                                          write_eeprom(24,(Pickitbuff[9]));  //Numero de cisterna
                                          Leer_Bytes_de_Config();
                                          CMD_Ejecutado=5;
                                          Respuesta=1;
                                    }
                                 }
                                 break;
                             default:
                                    Respuesta=0;
                                    CMD_Ejecutado=0;
                       }
            }
         }
       }
    //F_CMD_Completo_Pickit=0;  
      
       fputc(0X23,Pickit);
       fputc(0X5E,Pickit);
       fputc(0X01,Pickit);             //indica que es una sensor board
       fputc(Num_Serie1,Pickit);
       /*fputc(Num_Serie2,Pickit);
       fputc(Num_Serie3,Pickit);
       fputc(Num_Serie4,Pickit);
       fputc(Num_Serie5,Pickit);
       fputc(Num_Serie6,Pickit);*/
       fputc(CMD_Ejecutado,Pickit);
       fputc(Respuesta,Pickit);
       fputc(0X3C,Pickit);
       fputc(0X3F,Pickit);
 Salida:
       Respuesta=0;
       CMD_Ejecutado=0;
       Indice_Pickit=0;

   }
}
//***************************************************************************\\
void Verifica_CMD_RS485(void)
{
   if(F_CMD_Completo_RS485==1)
   {
      F_CMD_Completo_RS485=0;
      
       if(RS485buff[0]==0X23)
       {
         if(RS485buff[1]==0X5E)
         {
             if(RS485buff[2]==0X02) //hay que reenviar todo el buffer 485 por el pickit
            {
                       for(int i=0;i<Indice_RS485;i++)
                       {
                           fputc(RS485buff[i],Pickit);   
                       }
                       //Indice_RS485=0;
                       break;
            }
            if(RS485buff[2]==0X01) 
            {
               //if(RS485buff[3] == Num_Serie1 && RS485buff[4]== Num_Serie2 && RS485buff[5]== Num_Serie3 && RS485buff[6]== Num_Serie4 && RS485buff[7]== Num_Serie5 && RS485buff[8]== Num_Serie6)
                  if(RS485buff[3] == Num_Serie1) 
                  
                       switch(RS485buff[4])
                       {
                       
                           case 0:   //Enviar estado por el 485
                               {
                                  if(RS485buff[5]==0X3C && RS485buff[6]==0X3F)
                                    {
                                       Sensores();       //Toma el valor del flotador
                                       Lee_ADC();        //toma el valor del sensor de nivel
                                       Calcula_Nivel();  //Calcula el porcentaje del valor leido
                                       Envia_Estado_RS485();   //Envia el estado por el bus 485
                                       goto Salida2;
                                    }
                               }
                                break;
                           
                           case 1:  //Comando para modificar el tiempo entre comandos de status
                                 {
                                 
                                   if(RS485buff[6]==0X3C && RS485buff[7]==0X3F)
                                   {
                                       if(RS485buff[5]<0xF0) //Debera ser menor a 240 segundos
                                       {
                                             write_eeprom(0,(RS485buff[5]));// el dato recibido esta expresado en segundos 
                                             delay_ms(1);
                                             Tiempo_Envio_CMD = read_eeprom (0);
                                             Tiempo=0;
                                             Segundos=0;
                                             Respuesta=1;
                                             CMD_Ejecutado=1;
                                             //goto Respuesta_CMD;
                                       }
                                       else
                                       {
                                             Respuesta=0;
                                             CMD_Ejecutado=1;
                                           //goto Respuesta_CMD;
                                       }
                                    }
                                 }
                                 break; 
                            case 2:  //Comando para activar relays onboard
                                 {
                                  if(RS485buff[6]==0X3C && RS485buff[7]==0X3F)
                                  {
                                       CMD_Ejecutado=2;
                                       Respuesta=0;
                                    if(RS485buff[5]==0x00)
                                    {
                                       Estado_Relays=0X00;
                                       Respuesta=1;
                                       write_eeprom(2,0X00);
                                    }
                                    if(RS485buff[5]==0X10)//Enciendelo siempre y cuando no este activo el flotador
                                    {
                                       Estado_Relays=0X10;
                                       Respuesta=1;
                                       write_eeprom(2,0X10);
                                    }
                                    if(RS485buff[5]==0X01)
                                    {
                                       Estado_Relays=0X01;
                                       Respuesta=1;
                                       write_eeprom(2,0X01);
                                    }
                                    if(RS485buff[5]==0X11)
                                    {
                                       Estado_Relays=0X11;
                                       Respuesta=1;
                                       write_eeprom(2,0X11);
                                    }
                                  }
                                 }
                                 break;
                            
                             case 3:  //Comando para establecer numero de serie
                                {
                                    if(RS485buff[6]==0X3C && RS485buff[7]==0X3F)
                                    {
                                       CMD_Ejecutado=3;
                                       write_eeprom(1,(RS485buff[5]));
                                       /*write_eeprom(2,(RS485buff[6]));
                                       write_eeprom(3,(RS485buff[7]));
                                       write_eeprom(4,(RS485buff[8]));
                                       write_eeprom(5,(RS485buff[9]));
                                       write_eeprom(6,(RS485buff[10]));*/
                                       Leer_Bytes_de_Config();
                                       Respuesta=1;
                                    }
                                 }
                                 break;
                             case 4:  //Comando para enviar los settings de los valores del adc de la cisterna
                                 {
                                    if(RS485buff[9]==0X3C && RS485buff[10]==0X3F)
                                    {
                                       CMD_Ejecutado=4;
                                       Respuesta=1;
                                       write_eeprom(16,(RS485buff[5]));   
                                       write_eeprom(17,(RS485buff[6]));   
                                       write_eeprom(18,(RS485buff[7]));   
                                       write_eeprom(19,(RS485buff[8]));   
                                       Leer_Bytes_de_Config();
                                    }
                                 }
                                 break;
                              case 5 :
                                 {
                                    if(RS485buff[10]==0X3C && RS485buff[11]==0X3F)
                                    {
                                       write_eeprom(20,(RS485buff[5]));   
                                       write_eeprom(21,(RS485buff[6]));  
                                       write_eeprom(22,(RS485buff[7]));  
                                       write_eeprom(23,(RS485buff[8]));  
                                       write_eeprom(24,(RS485buff[9]));  
                                       Leer_Bytes_de_Config();
                                       CMD_Ejecutado=5;
                                       Respuesta=1;
                                    }
                                 }
                                 break;
                             default:
                                    Respuesta=0;
                                    CMD_Ejecutado=0;
                       }
            }
         }
       }
       output_high(DRV_485);                                     
       delay_ms(1);
       fputc(0X23,RS485);
       fputc(0X5E,RS485);
       fputc(0X01,RS485);  
       fputc(Num_Serie1,RS485);
       /*fputc(Num_Serie2,RS485);
       fputc(Num_Serie3,RS485);
       fputc(Num_Serie4,RS485);
       fputc(Num_Serie5,RS485);
       fputc(Num_Serie6,RS485);*/
       fputc(CMD_Ejecutado,RS485);
       fputc(Respuesta,RS485);
       fputc(0X3C,RS485);
       fputc(0X3F,RS485);
       delay_ms(3);
       output_low(DRV_485);  
 Salida2:
       Respuesta=0;
       CMD_Ejecutado=0;
       Indice_RS485=0;
   }
}

void main()
{
   setup_adc_ports(sAN4);                    
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_vref(FALSE);
   setup_spi(SPI_SS_DISABLED);
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_EXT1);
   enable_interrupts(INT_RDA);
   clear_interrupt(INT_TIMER1);
   clear_interrupt(INT_EXT1);
   clear_interrupt(INT_RDA);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);      //100 ms overflow
   set_timer1(28036);
   output_low(DRV_485);
   //ext_int_edge( 2, H_TO_L);
   ext_int_edge(1, H_TO_L);
 
//*******************Verificación de bytes de configuracion******************\\
   delay_ms(10);     //tiempo requerido para poder trabajar con le eeprom
   Leer_Bytes_de_Config();
   
   if(Tiempo_Envio_CMD==0XFF) //si no se ha configurado el tiempo pone por default el valor de 5 segundos
   {
      write_eeprom(0,5);
      Tiempo_Envio_CMD=5;
   }
   if(Num_Serie1==0XFF) //si no se ha configurado numero de serie, se pone por default 1
   {
      write_eeprom(1,1);
      Num_Serie1=1;
   }
    if(Valor_ADC_Vacio_High==0XFF && Valor_ADC_Lleno_High==0Xff) //si no se ha configurado los niveles de adc máximo y mínimo, porcentaje toma valor de 0XFE
   {
      Porcentaje = 0xFE;
   }
   if(Estado_Relays==0XFF)
   {
      Estado_Relays = 0X00;
      write_eeprom(2,0);
   }
   
   while(true)
   {
      Temporizaciones();   
      Actualiza_Estado_Relays();
      Verifica_CMD_Pickit();
      Verifica_CMD_RS485();
     // Calcula_Nivel();
      //Sensores();
      /*output_low(LED);
      delay_ms(DELAY);
      output_high(LED);
      delay_ms(DELAY);*/
   }
}
