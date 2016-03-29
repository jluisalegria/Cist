/* 
Define en este archivo las diferentes opciones de configuracion existentes en tus drivers de bajo
nivel dentro de la carpeta "bsp"
 */
#ifndef __HARDWARE_PROFILE_H__
#define	__HARDWARE_PROFILE_H__

#define BSP_CLOCK   12000000

#define EEPROM25XX_N_MEMORIES          1 /*su valor por defaul es cero*/
#define EEPROM25XX_SPI0                SPI_PORT2  /*puerto spi*/

#define EEPROM25XX_SS_P0               GPIOS_PORTB /*puerto*/
#define EEPROM25XX_SS_B0               0            /*pin*/




#define ST7032_N_LCDS 1
#define ST7032_SPI0                SPI_PORT2    /*puerto spi*/
#define ST7032_SS_P0               GPIOS_PORTA  /*puerto de chip select*/
#define ST7032_SS_B0               2            /*pin de chip select*/
#define ST7032_RST_P0              GPIOS_PORTA  /*puerto de reset*/
#define ST7032_RST_B0              5            /*pin de reset*/
#define ST7032_RS_P0               GPIOS_PORTA  /*puerto de RS*/
#define ST7032_RS_B0               3            /*pin de RS*/

#define BRIDGE_BOARD


// definir lo siguiente en un nuevo archivo "PROTOCOL_PROFILE"

#define StartByte0   0x23
#define StartByte1   0x5E
#define EndByte0     0x3C
#define EndByte1     0x3F



#endif	
