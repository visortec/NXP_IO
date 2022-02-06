/*
 * Dev_Can.c
 *
 *  Created on: 05-Jan-2022
 *      Author: Durga prasad
 */



#include "Dev_Can.h"
#include "MPC5744P.h"
#include "project.h"
#include<string.h>


/*@Brief: CAN2 Initialization
 *@CAN Bus Speed 500k bps
 *@
 */
void Dev_CANInitialization(void)
{
	  uint8_t   i;

	  CAN_0.MCR.B.MDIS = 1;       /* Disable module before selecting clock source*/
	  CAN_0.CTRL1.B.CLKSRC=0;     /* Clock Source = oscillator clock (40 MHz) */
	  CAN_0.MCR.B.MDIS = 0;       /* Enable module for config. (Sets FRZ, HALT)*/
	  while (!CAN_0.MCR.B.FRZACK) {} /* Wait for freeze acknowledge to set */
	  /* Bus Configuration */
	  CAN_0.CTRL1.R = 0x09FB0002; /* CAN bus: 40 MHz clksrc, 500K bps with 16 tq */
	                               /* PRESDIV+1 = Fclksrc/Ftq = 40 MHz/8MHz = 5 */
	                               /*    so PRESDIV = 4 */
	                               /* PSEG2 = Phase_Seg2 - 1 = 4 - 1 = 3 */
	                               /* PSEG1 = PSEG2 = 3 */
	                               /* PROPSEG= Prop_Seg - 1 = 7 - 1 = 6 */
	                               /* RJW = Resync Jump Width - 1 = 4 = 1 */
	                               /* SMP = 1: use 3 bits per CAN sample */
	                               /* CLKSRC=0 (unchanged): Fcanclk= Fxtal= 40 MHz*/
	  for(i=0; i<64; i++){ 			//MPC574xP has 64 buffers
	    CAN_0.MB[i].CS.B.CODE = 0;   /* Inactivate all message buffers */
	  }
	  CAN_0.MB[0].CS.B.CODE = 8;     /* Message Buffer 0 set to TX INACTIVE */

	  /* Configure the CAN0_TX pin to transmit. */

	  SIUL2.MSCR[PB0].B.SSS = 1; //PTB0 is for CAN0_TX. Select signal source select to CAN0_TX
	  SIUL2.MSCR[PB0].B.OBE = 1; //Set pin to output. Enable output buffer
	  SIUL2.MSCR[PB0].B.SRC = 3; //Maximum slew rate

	  /* Configure the CAN0_RX pin. */

	  SIUL2.MSCR[PB1].B.IBE = 1; //PB1 is CAN0_RX pin. Enable input buffer
	  SIUL2.IMCR[32].B.SSS = 0b0010; //Set PB1 as CAN0_RX.



	  CAN_0.MCR.R = 0x0000003F;       /* Negate FlexCAN 0 halt state for 64 MB */
	  while (CAN_0.MCR.B.FRZACK & CAN_0.MCR.B.NOTRDY) {} /* Wait to clear */
	                 /* Good practice: wait for FRZACK on freeze mode entry/exit */

#if 0

	  CAN_1.MCR.B.MDIS = 1;       /* Disable module before selecting clock source*/
	  CAN_1.CTRL1.B.CLKSRC=0;     /* Clock Source = oscillator clock (40 MHz) */
	  CAN_1.MCR.B.MDIS = 0;       /* Enable module for config. (Sets FRZ, HALT)*/
	  while (!CAN_1.MCR.B.FRZACK) {} /* Wait for freeze acknowledge to set */
	  /* Bus Configuration */
	  CAN_1.CTRL1.R = 0x04DB0086; /* CAN bus: 40 MHz clksrc, 500K bps with 16 tq */
	                               /* PRESDIV+1 = Fclksrc/Ftq = 40 MHz/8MHz = 5 */
	                               /*    so PRESDIV = 4 */
	                               /* PSEG2 = Phase_Seg2 - 1 = 4 - 1 = 3 */
	                               /* PSEG1 = PSEG2 = 3 */
	                               /* PROPSEG= Prop_Seg - 1 = 7 - 1 = 6 */
	                               /* RJW = Resync Jump Width - 1 = 4 = 1 */
	                               /* SMP = 1: use 3 bits per CAN sample */
	                               /* CLKSRC=0 (unchanged): Fcanclk= Fxtal= 40 MHz*/
	   //for (i=0; i<96; i++) {      /* MPC574xG has 96 buffers after MPC5748G rev 0*/


	  for(i=0; i<64; i++){ 			//MPC574xP has 64 buffers
	    CAN_1.MB[i].CS.B.CODE = 0;   /* Inactivate all message buffers */
	  }
	  CAN_1.MB[0].CS.B.CODE = 8;     /* Message Buffer 0 set to TX INACTIVE */

	  /* Configure the CAN0_TX pin to transmit. */

	  SIUL2.MSCR[PF14].B.SSS = 2; //PTF14 is for CAN2_TX. Select signal source select to CAN2_TX
	  SIUL2.MSCR[PF14].B.OBE = 1; //Set pin to output. Enable output buffer
	  SIUL2.MSCR[PF14].B.SRC = 3; //Maximum slew rate

	  /* Configure the CAN0_RX pin. */

	  SIUL2.MSCR[PF15].B.IBE = 1; //PF15 is CAN2_RX pin. Enable input buffer
	  SIUL2.IMCR[34].B.SSS = 0b0001; //Set PF15 as CAN2_RX.


	  CAN_1.MCR.R = 0x0000003F;       /* Negate FlexCAN 2 halt state for 64 MB */
	  while (CAN_1.MCR.B.FRZACK & CAN_1.MCR.B.NOTRDY) {} /* Wait to clear */
	                 /* Good practice: wait for FRZACK on freeze mode entry/exit */
#endif
}

/*@Brief: CAN Tx Frame
 *
 */
void Dev_CANTransmitData(uint32_t ExID, char *pBuf, char MsgLen)
{
	int i = 0;
	CAN_0.MB[0].CS.B.IDE = 1;       /* Use standard ID length */
	CAN_0.MB[0].ID.R = ExID;		  /* Transmit ID is 0x555 */
	CAN_0.MB[0].CS.B.RTR = 0;       /* Data frame, not remote Tx request frame */
	CAN_0.MB[0].CS.B.DLC = MsgLen ;    /*#bytes to transmit w/o null*/
	for ( i=0; i < MsgLen; i++) {
		CAN_0.MB[0].DATA.B[i] = pBuf[i];      /* Data to be transmitted */
	}
	CAN_0.MB[0].CS.B.SRR = 1;     /* Tx frame (not req'd for standard frame)*/
	CAN_0.MB[0].CS.B.CODE =0xC;   /* Activate msg. buf. to transmit data frame */
}

/*@Brief: RX CAN Frame
 *
 */
uint8_t Dev_CANFrameReceive(char *pBuf)
{

}
