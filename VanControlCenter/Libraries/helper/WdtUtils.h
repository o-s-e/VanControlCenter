// WdtUtils.h

#ifndef _WDTUTILS_h
#define _WDTUTILS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

extern void disableWDT();
extern void enableWDT(uint32_t timeout);
extern void resetWDT();

#endif
