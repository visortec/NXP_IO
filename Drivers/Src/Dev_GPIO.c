/*
 * Dev_GPIO.c
 *
 *  Created on: 05-Jan-2022
 *      Author: Durga prasad
 */

#include "Dev_GPIO.h"
#include "MPC5744P.h"
#include "project.h"


unsigned char BinToHexFormate(char *pBuf , char Len);


const uint32_t GPIO_INPUT_PINOUT[] = {
		PE0,PE2,PE4,PE5,PE6,PE7,PE9,PE10,PE11,PE13,PE14,PF3,PF4,
		PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF14,PF15,PG2,PG5,PG6
 };

const uint32_t GPIO_OUTPUT_PINPUT[] = {
		PA0, PA1, PA2, PA3,PA4,PA9,PA10,PA11,PA12,
		PA13,PA14,PA15,PD5,PD6,PD7,PD8, PD10
};

#define NUMOF_INPUTS    25
#define NUMOF_OUTPUTS   17

/*@GPIO Initialization@
 *@param : None
 */
void Dev_GPIOInitialization(void)
{
	int GPIO_PInIndex = 0;
	/* Input pin s configurations */
	for(GPIO_PInIndex = 0; GPIO_PInIndex < NUMOF_INPUTS ; GPIO_PInIndex++ )
	{
		SIUL2.MSCR[GPIO_INPUT_PINOUT[GPIO_PInIndex]].B.IBE = 0x01;
	}

	/* Output Pins Configurations */
	for(GPIO_PInIndex = 0; GPIO_PInIndex < NUMOF_OUTPUTS ; GPIO_PInIndex++ )
	{
		SIUL2.MSCR[GPIO_OUTPUT_PINPUT[GPIO_PInIndex]].B.OBE = 0x01;
	}

}

void Dev_GPIOReadOutDinToBuffer(unsigned char *pBuff)
{
	char BufIndex = 0 , index = 0;
	/* read All Digital Inputs */
	char Input =0 ;
	for( ; Input < NUMOF_INPUTS ; Input++ )
	{
		pBuff[BufIndex] |= GPIO_InputReadOut(GPIO_INPUT_PINOUT[Input]) >> index;
		if (++index % 8 )
		{
			BufIndex++;
			index = 0;
		}
	}
}

void Dev_GPIOReadOutDoutToBuffer(unsigned char *pBuff)
{
	char BufIndex = 0 , index = 0;
	/* read All Digital Inputs */
	char Output =0 ;
	for( ; Output < NUMOF_INPUTS ; Output++ )
	{
		pBuff[BufIndex] |= (SIUL2.GPDO[GPIO_OUTPUT_PINPUT[Output]].B.PDO ) >> index;
		if (++index % 8 )
		{
			BufIndex++;
			index = 0;
		}
	}
}

#if 0
/*@Brief : CAN PACKET Formation
 *@param : char pointer pBuf
 *@retval: None
 */
void Dev_GPIOReadOutToBuffer(char *pBuf)
{
	char BufIndex = 0 , index = 0;
	char lBuff[9] = {0};
	/* read All Digital Inputs */
	char Input =0 ;
	for( ; Input < NUMOF_INPUTS ; Input++ )
	{
		lBuff[index++] = GPIO_InputReadOut(GPIO_INPUT_PINOUT[Input]);
		if (index % 8 )
		{
			pBuf[BufIndex++] = BinToHexFormate(lBuff, 8);
			memset(lBuff, 0x00, 8);
			index = 0;
		}
	}
	pBuf[BufIndex++] = BinToHexFormate(lBuff, 8);

	/* Read Digital Output pins data */
	memset(lBuff, 0x00, 8);   index = 0;
	char Output = 0 ;
	for( ; Output < NUMOF_OUTPUTS ; Output++ )
	{
		lBuff[index++] = SIUL2.GPDO[GPIO_OUTPUT_PINPUT[Output]].B.PDO ;  //GPIO_InputReadOut(GPIO_INPUT_PINOUT[Input]);
		if (index % 8 )
		{
			pBuf[BufIndex++] = BinToHexFormate(lBuff, 8);
			memset(lBuff, 0x00, 8);
			index = 0;
		}
	}
	pBuf[BufIndex++] = BinToHexFormate(lBuff, 8);
	pBuf[BufIndex]   = 0x00;
}
#endif

/*@Brief : Set/Clear the output pin
 *@param : input pin number
 *@param : state
 *@return: None
 */
void Dev_GPIOSetClearPin(int PinNumber , _Bool State )
{
	if (State){
		SIUL2.GPDO[PinNumber].B.PDO = 1;
	}else{
		SIUL2.GPDO[PinNumber].B.PDO = 0;
	}
}


_Bool GPIO_InputReadOut(int PinNumber)
{
	return SIUL2.GPDI[PinNumber].B.PDI;
}



/*@Brief  : Binary To Hex Format Conversion
 *@param  : Binary String
 *@retval : Hex value In Hex Format
 */
unsigned char BinToHexFormate(char *pBuf , char Len)
{
	char index =0; unsigned char hexadecimalval = 0;
	int i = 0 ;
    while (Len)
    {
        hexadecimalval += pBuf[Len-1] * i;
        i = i * 2;
        index ++; Len--;
    }
    return hexadecimalval;
}



