/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.  */
/******************************************************************************	
* File Name    : main.c
* Version      : 1.00
* Device(s)    : 
* Tool-Chain   : 
* H/W Platform : 
* Description  : 
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.07.2015 1.00    First Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_macro.h"  /* System macro and standard type definition */
#include "r_spi_if.h" /* SPI driver interface */
#include "lcd.h"      /* LCD driver interface */
#include "intp.h"     /* External Interrupt interface */
#include "timer.h"    /* Timer driver interface */
#include "stdio.h"    /* Standard IO library: sprintf */

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define LED15_PM       PM4_bit.no1
#define LED15          P4_bit.no1
#define LED_ON         (0)
#define LED_OFF        (1)
int time=0;
char * string_shown_on_lcd[10];
int flag=0;
int sw=0;
int i=0;


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
void r_main_userinit(void);

/******************************************************************************
* Function Name: main
* Description  : Main program
* Arguments    : none
* Return Value : none
******************************************************************************/
void main(void)
{
	/* Local time structure to temporary store the global time */
	time_t p_time;

	/* Initialize user system */
	r_main_userinit();
	
	/* Clear LCD display */
	ClearLCD();
	DisplayLCD(LCD_LINE1, (uint8_t *)"Setting");
	sprintf(string_shown_on_lcd,"%d%d:%d%d ",g_time.minute/10, g_time.minute%10, g_time.second/10, g_time.second%10);
	DisplayLCD(LCD_LINE2,string_shown_on_lcd);
	/* Initialize external interrupt for switches */
	intp_init();
	
	/* Main loop - Infinite loop */
	g_time.minute=0;
	g_time.second=0;
	
	while (1) 
	{
		if(sw==0){
		if(flag==0){
			g_time.second=g_time.second+1;
			if(g_time.second>59){
				g_time.second=0;
				g_time.minute=g_time.minute+1;
			}
			DisplayLCD(LCD_LINE1, (uint8_t *)"Setting");
			sprintf(string_shown_on_lcd,"%d%d:%d%d ",g_time.minute/10, g_time.minute%10, g_time.second/10, g_time.second%10);
			DisplayLCD(LCD_LINE2,string_shown_on_lcd);
			
		}
		}
		if(sw==1){
		if(flag==0){
			g_time.minute=g_time.minute+1;
			DisplayLCD(LCD_LINE1, (uint8_t *)"Setting");
			sprintf(string_shown_on_lcd,"%d%d:%d%d ",g_time.minute/10, g_time.minute%10, g_time.second/10, g_time.second%10);
			DisplayLCD(LCD_LINE2,string_shown_on_lcd);
			
		}
		if(flag==2){ 
			flag=0;
			g_time.minute=0;
			g_time.second=0;
			DisplayLCD(LCD_LINE1, (uint8_t *)"Paused");
			sprintf(string_shown_on_lcd,"%d%d:%d%d ",g_time.minute/10, g_time.minute%10, g_time.second/10, g_time.second%10);
			DisplayLCD(LCD_LINE2,string_shown_on_lcd);
			
		}
		}
		if(sw==2){
		if ( (flag==1) && ((g_time.minute!=0) || (g_time.second!=0))) {
			//start_flag = 0;
			DisplayLCD(LCD_LINE1, (uint8_t *)"Counting");
			g_time.second--;
			if(g_time.second==0){
				if(g_time.minute!=0) {
					g_time.second=59;
					g_time.minute=g_time.minute-1;
				}
				else {
					sw=0;
					flag=0;
					DisplayLCD(LCD_LINE1, (uint8_t *)"Setting");
					sprintf(string_shown_on_lcd,"%d%d:%d%d ",g_time.minute/10, g_time.minute%10, g_time.second/10, g_time.second%10);
					DisplayLCD(LCD_LINE2,string_shown_on_lcd);
				}
			}
			if(g_time.minute==255){
				g_time.minute=0;
			}
			if(g_time.second==255){
				g_time.second=0;
			}
			sprintf(string_shown_on_lcd,"%d%d:%d%d ",g_time.minute/10, g_time.minute%10, g_time.second/10, g_time.second%10);
			DisplayLCD(LCD_LINE2,string_shown_on_lcd);
			for(i=0; i<=100; i++){
				Wait1CentiSecond();
			}
			flag=1;
	}
		if(flag==2){
			flag=2;
			DisplayLCD(LCD_LINE1, (uint8_t *)"Paused");
			sprintf(string_shown_on_lcd,"%d%d:%d%d ",g_time.minute/10, g_time.minute%10, g_time.second/10, g_time.second%10);
			DisplayLCD(LCD_LINE2,string_shown_on_lcd);
			for(i=0; i<=100; i++){
				Wait1CentiSecond();
			}
			
		}
		
		}
		
}
}	

/******************************************************************************
* Function Name: r_main_userinit
* Description  : User initialization routine
* Arguments    : none
* Return Value : none
******************************************************************************/
void r_main_userinit(void)
{
	uint16_t i;

	/* Enable interrupt */
	EI();

	/* Output a logic LOW level to external reset pin*/
	P13_bit.no0 = 0;
	for (i = 0; i < 1000; i++)
	{
		NOP();
	}

	/* Generate a raising edge by ouput HIGH logic level to external reset pin */
	P13_bit.no0 = 1;
	for (i = 0; i < 1000; i++)
	{
		NOP();
	}

	/* Output a logic LOW level to external reset pin, the reset is completed */
	P13_bit.no0 = 0;
	
	/* Initialize SPI channel used for LCD */
	R_SPI_Init(SPI_LCD_CHANNEL);
	
	/* Initialize Chip-Select pin for LCD-SPI: P145 (Port 14, pin 5) */
	R_SPI_SslInit(
		SPI_SSL_LCD,             /* SPI_SSL_LCD is the index defined in lcd.h */
		(unsigned char *)&P14,   /* Select Port register */
		(unsigned char *)&PM14,  /* Select Port mode register */
		5,                       /* Select pin index in the port */
		0,                       /* Configure CS pin active state, 0 means active LOW level  */
		0                        /* Configure CS pin active mode, 0 means active per transfer */
	);
	
	/* Initialize LCD driver */
	InitialiseLCD();	
}

/******************************************************************************
End of file
******************************************************************************/

