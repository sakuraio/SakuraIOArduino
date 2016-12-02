#include <SPI.h>
#include "SakuraIO.h"
#include "SakuraIO/debug.h"


void SakuraIO_SPI::begin(){
  dbgln("CS=0");
  digitalWrite(cs, LOW);
}

void SakuraIO_SPI::end(){
  dbgln("CS=1");
  digitalWrite(cs, HIGH);
  delayMicroseconds(20);
}

void SakuraIO_SPI::sendByte(uint8_t data){
  delayMicroseconds(20);
  dbg("Send=");
  dbgln(data, HEX);
  SPI.transfer(data);
}


uint8_t SakuraIO_SPI::receiveByte(bool stop){
  return receiveByte();
}

uint8_t SakuraIO_SPI::receiveByte(){
  uint8_t ret;
  delayMicroseconds(20);
  ret = SPI.transfer(0x00);
  dbg("Recv=");
  dbgln(ret, HEX);
  return ret;
}

SakuraIO_SPI::SakuraIO_SPI(int _cs){
  cs = _cs;
  SPI.begin();
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
}
