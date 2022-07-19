/*
 * Disp7Seg.h
 *
 * Created: 7/13/2022 2:43:21 PM
 *  Author: alexei.guriev
 */ 


#ifndef DISP7SEG_H_
#define DISP7SEG_H_

#include "general/if/StatuError.h"
#include "Disp7SegCfg.h"
#include "TimerSw.h"

void Disp7SegInit(void);

void Disp7SegRutine(void);

StatusError Disp7SegSetVal(float value);


#endif /* DISP7SEG_H_ */