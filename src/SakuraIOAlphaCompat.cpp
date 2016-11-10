#include <stdint.h>
#include "SakuraIO.h"
#include "SakuraIOAlphaCompat.h"

SakuraIOAlphaCompatSPI::SakuraIOAlphaCompatSPI(int _cs){
  sakuraio = new SakuraIO_SPI(_cs);
}

SakuraIOAlphaCompatI2C::SakuraIOAlphaCompatI2C(){
  sakuraio = new SakuraIO_I2C();
}

uint8_t SakuraIOAlphaCompat::getNetworkStatus() {
  if((sakuraio->getConnectionStatus() & 0x80) == 0x80){
    return 1;
  }
  return 0;
}

void SakuraIOAlphaCompat::transmit(uint8_t mode){
  if(mode != TRANSMIT_ONCE){
    return;
  }
  sakuraio->send();
}

uint8_t SakuraIOAlphaCompat::getTxChannelStatus(uint8_t ch){
  uint8_t available = 0;
  uint8_t queued = 0;
  sakuraio->getTxQueueLength(&available, &queued);
  if(queued > 0){
    return 1;
  }
  return 0;
}

void SakuraIOAlphaCompat::writeChannel(uint8_t ch, int32_t data){
  sakuraio->enqueueTx(ch, data);
}
void SakuraIOAlphaCompat::writeChannel(uint8_t ch, uint32_t data){
  sakuraio->enqueueTx(ch, data);
}
void SakuraIOAlphaCompat::writeChannel(uint8_t ch, int64_t data){
  sakuraio->enqueueTx(ch, data);
}
void SakuraIOAlphaCompat::writeChannel(uint8_t ch, uint64_t data){
  sakuraio->enqueueTx(ch, data);
}
void SakuraIOAlphaCompat::writeChannel(uint8_t ch, float data){
  sakuraio->enqueueTx(ch, data);
}
void SakuraIOAlphaCompat::writeChannel(uint8_t ch, double data){
  sakuraio->enqueueTx(ch, data);
}
void SakuraIOAlphaCompat::writeChannel(uint8_t ch, uint8_t *data){
  sakuraio->enqueueTx(ch, data);
}
