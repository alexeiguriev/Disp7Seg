/*
 * Disp7SegTypes.h
 *
 * Created: 7/13/2022 2:58:16 PM
 *  Author: alexei.guriev
 */ 


#ifndef DISP7SEGTYPES_H_
#define DISP7SEGTYPES_H_
#include "TimerSw.h"

typedef struct  
{
	TimerSwHandle* timer;
	Disp7SegHandleConfig * cfg;
}Disp7SegHandle;





#endif /* DISP7SEGTYPES_H_ */