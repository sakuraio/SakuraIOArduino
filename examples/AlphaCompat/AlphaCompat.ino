#include <SakuraIOAlphaCompat.h>

/*
 * WARNING : SakuraIOAlphaCompat is deprecated. DO NOT USE in new development work.
 */

//SakuraIOAlphaCompatSPI sakuraio(10);
SakuraIOAlphaCompatI2C sakuraio;

uint32_t cnt = 0;

void setup() {
  Serial.begin(9600);  // start serial for output

  Serial.println("[WARNING] SakuraIOAlphaCompat is deprecated. DO NOT USE in new development work.");
  Serial.println("Waiting to come online");
  for(;;){
    if( sakuraio.getNetworkStatus() == 1 ) break;
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
}

void loop() {
  cnt++;
  Serial.println(cnt);
  sakuraio.writeChannel(0,cnt);
  sakuraio.writeChannel(1,cnt);
  sakuraio.writeChannel(2,cnt);
  delay(250);

  Serial.print("Tx status: ");
  Serial.print( sakuraio.getTxChannelStatus(0) );
  Serial.println("");

  sakuraio.transmit(TRANSMIT_ONCE);
  delay(5000);
}
