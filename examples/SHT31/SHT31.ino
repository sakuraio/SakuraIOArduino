// This example requires Adafruit's SHT31 library.
// https://github.com/adafruit/Adafruit_SHT31
#include "Adafruit_SHT31.h"

#include <SakuraIO.h>`

Adafruit_SHT31 sht31 = Adafruit_SHT31();
SakuraIO_I2C sakuraio;

uint32_t cnt = 0;

void setup() {
  if (! sht31.begin(0x45)) { // AE-SHT3X default addr = 0x45
  	Serial.println("SHT31 initialize error!");
	while(1);
  }
  Serial.begin(9600);
  Serial.print("Waiting to come online");
  for(;;){
    if( (sakuraio.getConnectionStatus() & 0x80) == 0x80 ) break;
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

void loop() {
  cnt++;
  Serial.println(cnt);

  float temp = sht31.readTemperature();

  if (isnan(temp)) {
    Serial.println("Failed to read temperature!");
  } else {
	Serial.print("Temperature: ");
	Serial.println(temp);
  }

  float humi = sht31.readHumidity();

  if (isnan(humi)) {
    Serial.println("Failed to read humidity!");
  } else {
	Serial.print("Humidity: ");
	Serial.println(humi);
  }

  if(sakuraio.enqueueTx(0,temp) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
  if(sakuraio.enqueueTx(1,humi) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
  if(sakuraio.enqueueTx(2,cnt) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
  sakuraio.send();
  delay(5000);

  uint8_t available;
  uint8_t queued;
  if(sakuraio.getTxQueueLength(&available, &queued) != CMD_ERROR_NONE){
    Serial.println("[ERR] get tx queue length error");
  }

  Serial.print("Available :");
  Serial.print(available);
  Serial.print(" Queued :");
  Serial.println(queued);
}
