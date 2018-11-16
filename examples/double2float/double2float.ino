#include <SakuraIO.h>
#include <SakuraIOUtils.h>

//SakuraIO_SPI sakuraio(10);
SakuraIO_I2C sakuraio;

void setup(){

}

void loop(){
  uint8_t ret;

  uint8_t channel;
  uint8_t type;
  uint8_t values[8];
  int64_t offset;
  ret = sakuraio.dequeueRx(&channel, &type, values, &offset);

  if (ret != 0x01) {
    return;
  }

  if (type == 'd') {
    float f = SakuraIOUtils::double2float(*((uint64_t *)values));

    // On the Uno and other ATMEGA based boards, this occupies 4 bytes.
    // That is, the double implementation is exactly the same as the float.
    // DO NOT USE C type casting.
    // double d = *((double *)values); // bad
    // float f = (float)*((double *)values); // bad
  }
}
