#ifndef __SAKURA_IO_DEBUG_H__
#define __SAKURA_IO_DEBUG_H__


#if defined(SAKURA_DEBUG)
#include <Arduino.h>
#define dbg(...) Serial.print(__VA_ARGS__)
#define dbgln(...) Serial.println(__VA_ARGS__)
#else
#define dbg(...)
#define dbgln(...)
#endif

#endif //__SAKURA_IO_DEBUG_H__
