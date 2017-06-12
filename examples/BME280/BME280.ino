// This example requires SparkFun's BME280 library.
// https://github.com/sparkfun/SparkFun_BME280_Arduino_Library
#include "SparkFunBME280.h"

#include "Wire.h"
#include <SakuraIO.h>

SakuraIO_I2C sakuraio;
BME280 bme280;

uint32_t cnt = 0;

void setup() {
  Serial.begin(9600);

  // Setup BME280
  bme280.settings.commInterface = I2C_MODE; // Use I2C Interface
  bme280.settings.I2CAddress = 0x76;        // I2C Address
  bme280.settings.runMode = 3;              // Normal mode
  bme280.settings.tempOverSample = 1;
  bme280.settings.pressOverSample = 1;
  bme280.settings.humidOverSample = 1;

  delay(10);
  if (bme280.begin() != 0x60)
  {
    Serial.println("BME280 communication error!");
    while (1);
  }

  Serial.print("Waiting to come online");
  for (;;) {
    if ( (sakuraio.getConnectionStatus() & 0x80) == 0x80 ) break;
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

void loop() {
  cnt++;
  float temp = bme280.readTempC();
  float humi = bme280.readFloatHumidity();
  float pressure = bme280.readFloatPressure() / 100; // hPa
  float altitude = bme280.readFloatAltitudeMeters();

  Serial.println();
  Serial.println(cnt);

  Serial.print("Temperature: ");
  Serial.print(temp, 2);
  Serial.println(" deg C");

  Serial.print("Humidity: ");
  Serial.print(humi, 2);
  Serial.println(" %");

  Serial.print("Pressure: ");
  Serial.print(pressure, 2);
  Serial.println(" hPa");

  Serial.print("Altitude: ");
  Serial.print(altitude, 2);
  Serial.println(" m");



  if (sakuraio.enqueueTx(0, cnt) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
  }
  if (sakuraio.enqueueTx(1, temp) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
  }
  if (sakuraio.enqueueTx(2, humi) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
  }
  if (sakuraio.enqueueTx(3, pressure) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
  }
  if (sakuraio.enqueueTx(4, altitude) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
  }

  sakuraio.send();
  delay(5000);

  uint8_t available;
  uint8_t queued;
  if (sakuraio.getTxQueueLength(&available, &queued) != CMD_ERROR_NONE) {
    Serial.println("[ERR] get tx queue length error");
  }

  Serial.print("Available :");
  Serial.print(available);
  Serial.print(" Queued :");
  Serial.println(queued);

}

