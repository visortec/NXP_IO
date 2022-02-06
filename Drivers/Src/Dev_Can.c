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


	  /* Message Buffer 4 set to RX IN-ACTIVE */

	  CAN_0.MB[4].CS.B.IDE = 1;      /* MB 4 will look for a Ex_ID */
	  CAN_0.MB[4].ID.R = 0x06FE0800; /* MB 4 will look for ID */
	  CAN_0.MB[4].CS.B.CODE = 4;     /* MB 4 set to RX EMPTY */
	  CAN_0.RXMGMASK.R = 0x1FFFFFFF; /* Global acceptance mask */

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

uint32_t  RxCODE;              /* Received message buffer code */
uint32_t  RxID;                /* Received message ID */
uint32_t  RxLENGTH;            /* Recieved message number of data bytes */
uint8_t   RxDATA[8];           /* Received message data string*/
uint32_t  RxTIMESTAMP;         /* Received message time */

/*@Brief: RX CAN Frame
 *
 */
uint8_t Dev_CANFrameReceive(char *pBuf)
{
	uint8_t j; 	uint32_t dummy;

	if(CAN_0.IFLAG1.B.BUF4TO1I != 8)
	{
		RxCODE   = CAN_0.MB[4].CS.B.CODE; /* Read CODE, ID, LENGTH, DATA, TIMESTAMP*/
		RxID     = CAN_0.MB[4].ID.B.ID_STD;
		RxLENGTH = CAN_0.MB[4].CS.B.DLC;
		for (j=0; j< RxLENGTH; j++) {
			pBuf[j] = CAN_0.MB[4].DATA.B[j];
		}
		RxTIMESTAMP = CAN_0.MB[4].CS.B.TIMESTAMP;
		dummy = CAN_0.TIMER.R;             /* Read TIMER to unlock message buffers */
		if(dummy){}
		CAN_0.IFLAG1.R = 0x00000010;       /* Clear CAN 1 MB 4 flag */
	}  /* Wait for CAN 1 MB 4 flag */
	else {
		return 0;
	}
	return 1;
}
