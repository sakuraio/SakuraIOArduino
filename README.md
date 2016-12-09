# SAKURA Internet IoT Communication Module Library for Arduino

**Under development**

[Sakura Communication Module](https://iot.sakura.ad.jp/product/) (with [Sakura IoT Platform β](https://iot.sakura.ad.jp/)) library for Arduino.

# Support

This library supports following products.

* [SCM-LTE-Beta (Sakura Communication Module (LTE) β)](http://iot-store.sakura.ad.jp/items/4326125)

with

* [SCO-ARD-01 (Arduino Shield for SCM)](http://iot-store.sakura.ad.jp/items/4350904)

# Install

* Download ZIP from [here](https://github.com/sakura-internet/SakuraIOArduino/archive/master.zip).
* Place the ZIP file in Arduino `libraries` directory.
  * Mac `~/Documents/Arduino/libraries`
  * Linux The `libraries` folder in your sketchbook
  * Windows `My Documents\Arduino\libraries`


# Reference

Please see the [datasheet](https://iot.sakura.ad.jp/developer/pdf/sakura_module_datasheet_latest.pdf).


## Example

Please see more [examples](#examples).

```cpp
#include <SakuraIO.h>

//SakuraIO_SPI sakuraio(10);
SakuraIO_I2C sakuraio;
uint32_t cnt;

void setup(){
  cnt = 0;
  while((sakuraio.getConnectionStatus() & 0x80) != 0x80){
    delay(1000);
  }
}

void loop() {
  cnt++;
  sakuraio.enqueueTx(0,cnt);
  sakuraio.enqueueTx(1,cnt);
  sakuraio.enqueueTx(2,cnt);
  sakuraio.send();
  delay(1000);
}
```


## Constant

```cpp
// Response
#define CMD_ERROR_NONE             0x01
#define CMD_ERROR_PARITY           0x02
#define CMD_ERROR_MISSING          0x03
#define CMD_ERROR_INVALID_SYNTAX   0x04
#define CMD_ERROR_RUNTIME          0x05
#define CMD_ERROR_LOCKED           0x06
#define CMD_ERROR_BUSY             0x07
```

## Methods

```cpp
// Common
uint8_t getConnectionStatus();
uint8_t getSignalQuarity();
uint64_t getUnixtime();
uint8_t echoback(uint8_t length, uint8_t *data, uint8_t *response);

// I/O
uint16_t getADC(uint8_t channel);

// Send
uint8_t enqueueTx(uint8_t ch, int32_t value, uint64_t offset);
uint8_t enqueueTx(uint8_t ch, uint32_t value, uint64_t offset);
uint8_t enqueueTx(uint8_t ch, int64_t value, uint64_t offset);
uint8_t enqueueTx(uint8_t ch, uint64_t value, uint64_t offset);
uint8_t enqueueTx(uint8_t ch, float value, uint64_t offset);
uint8_t enqueueTx(uint8_t ch, double value, uint64_t offset);
uint8_t enqueueTx(uint8_t ch, uint8_t value[8], uint64_t offset);
uint8_t enqueueTx(uint8_t ch, int32_t value);
uint8_t enqueueTx(uint8_t ch, uint32_t value);
uint8_t enqueueTx(uint8_t ch, int64_t value);
uint8_t enqueueTx(uint8_t ch, uint64_t value);
uint8_t enqueueTx(uint8_t ch, float value);
uint8_t enqueueTx(uint8_t ch, double value);
uint8_t enqueueTx(uint8_t ch, uint8_t value[8]);
uint8_t getTxQueueLength(uint8_t *available, uint8_t *queued);
uint8_t clearTx();
uint8_t getTxStatus(uint8_t *queue, uint8_t *immediate);
uint8_t send();

// Reeceive
uint8_t dequeueRx(uint8_t *ch, uint8_t *type, uint8_t *value, int64_t *offset);
uint8_t peekRx(uint8_t *ch, uint8_t *type, uint8_t *value, int64_t *offset);
uint8_t getRxQueueLength(uint8_t *available, uint8_t *queued);
uint8_t clearRx();

// Operation
uint16_t getProductID();
uint8_t getUniqueID(char *data);
uint8_t getFirmwareVersion(char *data);
uint8_t unlock();
uint8_t updateFirmware();
uint8_t getFirmwareUpdateStatus();
uint8_t reset();
```


# Examples

Please see example code.

* [Standard](./examples/Standard/Standard.ino)
* [Firmware update](./examples/FirmwareUpdate/FirmwareUpdate.ino)
* [Send temperature and humidity](./examples/HDC1000/HDC1000.ino) (depends on [hotchpotch/Arduino-HDC1000](https://github.com/hotchpotch/Arduino-HDC1000))
* [Send human presence](./examples/SB412A/SB412A.ino)
* [Shell](./examples/Shell/Shell.ino)
* [Compatiblity of Sakura IoT Alpha](./examples/AlphaCompat/AlphaCompat.ino) (deprecated and send only)



# License
The MIT License (MIT)

Copyright (c) SAKURA Internet Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
