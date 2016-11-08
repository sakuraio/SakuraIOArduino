#ifndef _SAKURAIOALPHACOMPAT_H_
#define _SAKURAIOALPHACOMPAT_H_

#include <stdint.h>
#include "SakuraIO.h"

#define TRANSMIT_NONE   0x00
//#define TRANSMIT_AUTO   0x01 // deprecated
#define TRANSMIT_ONCE   0x02

#define SakuraAlphaSPI SakuraIOAlphaCompatSPI
#define SakuraAlphaI2C SakuraIOAlphaCompatI2C

class SakuraIOAlphaCompat {
protected:
  SakuraIO sakuraio;
public:
  void writeChannel(uint8_t ch, int32_t data);
  void writeChannel(uint8_t ch, uint32_t data);
  void writeChannel(uint8_t ch, int64_t data);
  void writeChannel(uint8_t ch, uint64_t data);
  void writeChannel(uint8_t ch, float data);
  void writeChannel(uint8_t ch, double data);
  void writeChannel(uint8_t ch, uint8_t *data);
  void transmit(uint8_t mode);
  uint8_t getTxChannelStatus(uint8_t ch);
  uint8_t getNetworkStatus();
};

class SakuraIOAlphaCompatSPI : public SakuraIOAlphaCompat
{
public:
  SakuraIOAlphaCompatSPI(int _cs);
};

class SakuraIOAlphaCompatI2C : public SakuraIOAlphaCompat
{
public:
  SakuraIOAlphaCompatI2C();
};



#endif
