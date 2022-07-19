/*
 * Disp7Seg.c
 *
 * Created: 7/13/2022 2:43:06 PM
 *  Author: alexei.guriev
 */ 

#include "Disp7Seg.h"
#include "Disp7SegCfg.h"
#include "TimerCfg/TimerCfg.h"
#include <math.h>

#define NEGATIVE_SIGN	127
#define COMA_SIGN		255
#define COMA_INDEX		10
#define MINUS_INDEX		11


static uint8_t digitCount = 0;
static Disp7SegHandleConfig* disp7SegHandleConfig;
TimerSwHandle timerSwHandle;
uint8_t digitsValue[DISPLAY_7_SEGMENT_DIGITS_COUNT];
uint8_t segCode[12]= {
	//  a  b  c  d  e  f  g  .
	0b00000011,  // 0
	0b01100000,  // 1
	0b11011010,  // 2
	0b11110010,  // 3
	0b01100110,  // 4
	0b10110110,  // 5
	0b10111110,  // 6
	0b11100000,  // 7
	0b11111110,  // 8
	0b11110110,  // 9
	0b00000001,  // .
	0b00000010   // -
};

/* local functions */
static void FloatToBuff(float value,uint8_t * data);


/* Global functions */
void Disp7SegInit(void)
{
	StatusError err;
	for(uint8_t index = 0; index < DISPLAY_7_SEGMENT_DIGITS_COUNT; index++)
	{
		digitsValue[index] = 0;
	}
	disp7SegHandleConfig = Disp7SegCfgInitAndGet();	
	
	TimerSwInitParam *pTimerSwInitParam = TimerGetIntervalPointerCfg();
	
	err = TimerSwInit(pTimerSwInitParam,&timerSwHandle);
	
	if (err == StatusErrNone)
	{
		TimerSwStartup(&timerSwHandle,DISPLAY_7_SEGMENT_TIMER_MS);
	}
	digitCount = 0;
}

void Disp7SegRutine(void)
{
	StatusError err;
	err = TimerSwIsExpired(&timerSwHandle);
	if (err == StatusErrTime)
	{
		if (digitCount >= DISPLAY_7_SEGMENT_DIGITS_COUNT)
		{
			digitCount = 0;
		}
		Disp7SegCfgAllDigitsOff();
		
		Disp7SegCfgSetDigitValue(digitsValue[digitCount]);
		
		Disp7SegCfgDigitOn(digitCount);
		
		TimerSwStartup(&timerSwHandle,DISPLAY_7_SEGMENT_TIMER_MS);
		
		digitCount++;
	}
}

static uint8_t tempDataBuff[DISPLAY_7_SEGMENT_DIGITS_COUNT + 1];

StatusError Disp7SegSetVal(float value)
{
	uint8_t mainIndex = 0, tempIndex = 0;
	FloatToBuff(value,tempDataBuff);
	
	while(mainIndex < DISPLAY_7_SEGMENT_DIGITS_COUNT)
	{
		if (tempDataBuff[tempIndex] == NEGATIVE_SIGN)
		{
			digitsValue[mainIndex] = segCode[MINUS_INDEX];
			mainIndex++;
			tempIndex++;
		}
		else if (tempDataBuff[tempIndex] == COMA_SIGN)
		{
			tempIndex++;
			digitsValue[mainIndex - 1] |= segCode[COMA_INDEX];
		}
		else
		{
			digitsValue[mainIndex] = segCode[tempDataBuff[tempIndex]];
			mainIndex++;
			tempIndex++;
		}
	}
	
	return StatusErrNone;
}

static void FloatToBuff(float value,uint8_t * data)
{
	float decVal;
	uint8_t localdigitCount = 0;
	uint8_t tmp;
	float localVlaue, localdecValue, decimalMultipler;
	
	decimalMultipler = (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT - 1));
	
	if (value < 0)
	{
		value *= -1;
		data[localdigitCount] = (uint8_t) NEGATIVE_SIGN;
		localdigitCount++;
		decVal = (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT - 2));
		
		if (value >= (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT - 1)))
		{
			localVlaue = (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT - 1)) - 1;
		}
		else
		{
			localVlaue = value;
		}
	}
	else
	{
		decVal = (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT - 1));
		
		if (value >= (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT)))
		{
			localVlaue = (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT)) - 1;
		}
		else
		{
			localVlaue = value;
		}
	}
	
	localdecValue = (localVlaue * (float)decimalMultipler) - (float)((int)localVlaue * (int)decimalMultipler);
	
	while(localVlaue >= 1)
	{
		if (localVlaue >= decVal)
		{
			tmp = (uint8_t)(localVlaue / decVal);
			data[localdigitCount] =  tmp;
			localdigitCount++;
			localVlaue -= (float)tmp * decVal;
		}
		decVal /= 10;
	}
	
	if (localdigitCount >= DISPLAY_7_SEGMENT_DIGITS_COUNT)
	{
		return;
	}
	
	if (localdigitCount == 0)
	{
		data[localdigitCount] = 0;
		localdigitCount++;
	}
	data[localdigitCount] = (uint8_t) COMA_SIGN;
	localdigitCount++;
	
	decVal = decimalMultipler / 10;
	
	while(localdigitCount <= DISPLAY_7_SEGMENT_DIGITS_COUNT)
	{
		tmp = (uint8_t)localdecValue / decVal;
		data[localdigitCount] =  tmp;
		localdigitCount++;
		localdecValue -= (float)tmp * decVal;
		decVal /= 10;
	}
	
	return;
}