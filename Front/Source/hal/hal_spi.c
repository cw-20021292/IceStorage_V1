#include "hw.h"
#include "hal_spi.h"



static void delay_us(U16 x);

void HAL_SPI_Init(void)
{
    TURN_OFF_SPI_RESET();
    delay_us( 400U );
    TURN_ON_SPI_RESET();

    TURN_ON_SPI_CLK();
    TURN_OFF_SPI_CSB();
    TURN_OFF_SPI_SEL();

    delay_us( 400U );
}

static void delay_us(U16 x)
{
    int i;

    for (i = 0; i < x; i++)
    {
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
    }
}


#define MK_COMMAND 0x8000

#if CONFIG_VOICE_NUVOTON    
#define SPI_DELAY_1     50U
#define SPI_DELAY_2     50U
#else   // CONFIG_VOICE_NEXTLAB
#define SPI_DELAY_1     10U
#define SPI_DELAY_2     20U
#endif
void HAL_SPI_Write(U16 mu16Data)
{
    int i = 0;

    TURN_OFF_SPI_CSB(); //start SPI
    delay_us( SPI_DELAY_1 );        /*CSB Low 후 5ns~20ns delay*/
    TURN_OFF_SPI_CLK(); //stand-by status,

    /*16bit 처리*/
    while (i < 16)
    {
        TURN_OFF_SPI_CLK(); 
        delay_us( SPI_DELAY_1 ); /*mimium 500ns*/

        if( (mu16Data & MK_COMMAND) != 0 )
        {
            WRITE_SPI_SDI( 1 );
        }
        else
        {
            WRITE_SPI_SDI( 0 );
        }

        mu16Data <<= 1; //shift left

        delay_us( SPI_DELAY_1 );   /*mimium 500ns*/
        TURN_ON_SPI_CLK(); 
        i++;
        delay_us(SPI_DELAY_2);
    }

    WRITE_SPI_SDI( 0 );
    delay_us( SPI_DELAY_1 );
    TURN_ON_SPI_CSB();  //stand-by status
    delay_us(SPI_DELAY_2);       /*KMH next mu16Data를 위해 5us delay*/
}
