#include <Wire.h>
#include <SPI.h>
#include "SakuraIO.h"
#include "SakuraIO/commands.h"
#include "SakuraIO/debug.h"

uint8_t SakuraIO::executeCommand(uint8_t cmd,uint8_t requestLength, uint8_t *request, uint8_t *responseLength, uint8_t *response)
{
  uint8_t parity = 0x00;
  uint8_t result = 0x00;
  uint8_t tmpResponseLength, tmpResponse;

  dbgln("executeCommand");

  this->begin();

  // request
  this->sendByte(cmd);
  this->sendByte(requestLength);
  parity = cmd ^ requestLength;
  for(int16_t i=0; i<requestLength; i++){
    parity ^= request[i];
    this->sendByte(request[i]);
  }
  this->sendByte(parity);
  //this->finishSending();

  tmpResponseLength = 0;
  if(responseLength != NULL){
    tmpResponseLength = *responseLength;
  }

  delay(10);

  // response
  this->startReceive(tmpResponseLength+3);
  result = this->receiveByte();
  if(result != CMD_ERROR_NONE){
    dbgln("Invalid status");
    this->end();
    return result;
  }
  tmpResponseLength = this->receiveByte();
  parity = result ^ tmpResponseLength;
  if(responseLength != NULL){
    if(*responseLength < tmpResponseLength){
      tmpResponseLength = *responseLength;
    }
    *responseLength = tmpResponseLength;
  }
  for(int16_t i=0; i<tmpResponseLength; i++){
    tmpResponse = this->receiveByte();
    parity ^= tmpResponse;
    if(response != NULL){
      response[i] = tmpResponse;
    }
  }
  dbgln("Parity");
  uint8_t p = this->receiveByte(true);
  parity ^= p;
  dbg("Parity=");
  dbgln(p);
  if(parity != 0x00){
    result = CMD_ERROR_PARITY;
    dbgln("Invalid parity");
  }else{
    dbgln("Success");
  }
  //this->finishReceiving();

  this->end();
  return result;
}

/* Common Commands */

uint8_t SakuraIO::getConnectionStatus(){
  uint8_t responseLength = 1;
  uint8_t response[1] = {0x00};
  if(executeCommand(CMD_GET_CONNECTION_STATUS, 0, NULL, &responseLength, response) != CMD_ERROR_NONE){
    return 0xFF;
  }
  return response[0];
}

uint8_t SakuraIO::getSignalQuarity(){
  uint8_t responseLength = 1;
  uint8_t response[1] = {0x00};

  if(executeCommand(CMD_GET_SIGNAL_QUALITY, 0, NULL, &responseLength, response) != CMD_ERROR_NONE){
    return 0x00;
  }
  return response[0];
}

uint64_t SakuraIO::getUnixtime(){
  uint8_t responseLength = 8;
  uint8_t response[8] = {0x00};
  if(executeCommand(CMD_GET_DATETIME, 0, NULL, &responseLength, response) != CMD_ERROR_NONE){
    return 0x00;
  }
  return *((uint64_t *)response);
}

uint8_t SakuraIO::echoback(uint8_t length, uint8_t *data, uint8_t *response){
  uint8_t responseLength = length;
  if(executeCommand(CMD_ECHO_BACK, length, data, &responseLength, response) != CMD_ERROR_NONE){
    return 0x00;
  }
  return responseLength;
}

/* IO Commands */

uint16_t SakuraIO::getADC(uint8_t channel){
  uint8_t request[1] = {channel};
  uint8_t response[2] = {0x00};
  uint8_t responseLength = sizeof(response);
  if(executeCommand(CMD_READ_ADC, 1, request, &responseLength, response) != CMD_ERROR_NONE){
    return 0xffff;
  }
  return *((uint16_t *)response);
}

/* TX Commands */
uint8_t SakuraIO::enqueueTxRaw(uint8_t ch, uint8_t type, uint8_t length, uint8_t *data, uint64_t offset){
  uint8_t request[18] = {0x00};
  uint8_t requestLength = 10;
  request[0] = ch;
  request[1] = type;
  for(uint8_t i=0;i<length;i++){
    request[2+i] = data[i];
  }
  if(offset != 0){
    requestLength = 18;
    for(uint8_t i=0;i<8;i++){
      request[10+i] = ((uint8_t *)&offset)[i];
    }
  }
  return executeCommand(CMD_TX_ENQUEUE, requestLength, request, NULL, NULL);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, int32_t value, uint64_t offset){
  return enqueueTxRaw(ch, 'i', 4, (uint8_t *)&value, offset);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, uint32_t value, uint64_t offset){
  return enqueueTxRaw(ch, 'I', 4, (uint8_t *)&value, offset);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, int64_t value, uint64_t offset){
  return enqueueTxRaw(ch, 'l', 8, (uint8_t *)&value, offset);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, uint64_t value, uint64_t offset){
  return enqueueTxRaw(ch, 'L', 8, (uint8_t *)&value, offset);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, float value, uint64_t offset){
  return enqueueTxRaw(ch, 'f', 4, (uint8_t *)&value, offset);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, double value, uint64_t offset){
  return enqueueTxRaw(ch, 'd', 8, (uint8_t *)&value, offset);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, uint8_t value[8], uint64_t offset){
  return enqueueTxRaw(ch, 'b', 8, (uint8_t *)value, offset);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, int32_t value){
  return enqueueTx(ch, value, (uint32_t)0);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, uint32_t value){
  return enqueueTx(ch, value, (uint32_t)0);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, int64_t value){
  return enqueueTx(ch, value, (uint32_t)0);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, uint64_t value){
  return enqueueTx(ch, value, (uint32_t)0);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, float value){
  return enqueueTx(ch, value, (uint32_t)0);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, double value){
  return enqueueTx(ch, value, (uint32_t)0);
}

uint8_t SakuraIO::enqueueTx(uint8_t ch, uint8_t value[8]){
  return enqueueTx(ch, value, (uint32_t)0);
}

uint8_t SakuraIO::getTxQueueLength(uint8_t *available, uint8_t *queued){
  uint8_t response[2] = {0x00};
  uint8_t responseLength = 2;
  uint8_t ret = executeCommand(CMD_TX_LENGTH, 0, NULL, &responseLength, response);
  *available = response[0];
  *queued = response[1];
  return ret;
}

uint8_t SakuraIO::clearTx(){
  return executeCommand(CMD_TX_CLEAR, 0, NULL, NULL, NULL);
}

uint8_t SakuraIO::send(){
  return executeCommand(CMD_TX_SEND, 0, NULL, NULL, NULL);
}

uint8_t getTxStatus(uint8_t *queue, uint8_t *immediate){
  uint8_t response[2] = {0x00};
  uint8_t responseLength = 2;
  uint8_t ret = executeCommand(CMD_TX_STAT, 0, NULL, &responseLength, response);
  *queue = response[0];
  *immediate = response[1];
  return ret;
}

/* RX Commands */

uint8_t SakuraIO::dequeueRx(uint8_t *ch, uint8_t *type, uint8_t *value, int64_t *offset){
  uint8_t response[18] = {0x00};
  uint8_t responseLength = 18;
  uint8_t ret = executeCommand(CMD_RX_DEQUEUE, 0, NULL, &responseLength, response);
  if(ret != CMD_ERROR_NONE){
    return ret;
  }

  *ch = response[0];
  *type = response[1];
  for(uint8_t i=0; i<8; i++){
    value[i] = response[2+i];
  }
  for(uint8_t i=0; i<8; i++){
    ((uint8_t *)offset)[i] = response[10+i];
  }

  return ret;
}

uint8_t SakuraIO::peekRx(uint8_t *ch, uint8_t *type, uint8_t *value, int64_t *offset){
  uint8_t response[18] = {0x00};
  uint8_t responseLength = 18;
  uint8_t ret = executeCommand(CMD_RX_PEEK, 0, NULL, &responseLength, response);
  if(ret != CMD_ERROR_NONE){
    return ret;
  }

  *ch = response[0];
  *type = response[1];
  for(uint8_t i=0; i<8; i++){
    value[0] = response[2+i];
  }
  for(uint8_t i=0; i<8; i++){
    ((uint8_t *)offset)[i] = response[10+i];
  }

  return ret;
}

uint8_t SakuraIO::getRxQueueLength(uint8_t *available, uint8_t *queued){
  uint8_t response[2] = {0x00};
  uint8_t responseLength = 2;
  uint8_t ret = executeCommand(CMD_RX_LENGTH, 0, NULL, &responseLength, response);
  *available = response[0];
  *queued = response[1];
  return ret;
}

uint8_t SakuraIO::clearRx(){
  return executeCommand(CMD_RX_CLEAR, 0, NULL, NULL, NULL);
}

/* Operation command */

uint16_t SakuraIO::getProductID(){
  uint8_t response[2] = {0x00};
  uint8_t responseLength = 2;
  uint8_t ret = executeCommand(CMD_GET_PRODUCT_ID, 0, NULL, &responseLength, response);
  if(ret != CMD_ERROR_NONE){
    return 0x00;
  }
  return *((uint16_t *)response);
}

uint8_t SakuraIO::getUniqueID(char *data){
  uint8_t response[11] = {0x00};
  uint8_t responseLength = 10;
  uint8_t ret = executeCommand(CMD_GET_UNIQUE_ID, 0, NULL, &responseLength, response);
  if(ret != CMD_ERROR_NONE){
    return ret;
  }
  for(uint8_t i=0; i<responseLength; i++){
    data[i] = (char)response[i];
  }
  data[responseLength] = 0x00;
  return ret;
}

uint8_t SakuraIO::getFirmwareVersion(char *data){
  uint8_t response[33] = {0x00};
  uint8_t responseLength = 32;
  uint8_t ret = executeCommand(CMD_GET_FIRMWARE_VERSION, 0, NULL, &responseLength, response);
  if(ret != CMD_ERROR_NONE){
    return ret;
  }
  for(uint8_t i=0; i<responseLength; i++){
    data[i] = (char)response[i];
  }
  data[responseLength] = 0x00;
  return ret;
}

uint8_t SakuraIO::unlock(){
  uint8_t request[4] = {0x53, 0x6B, 0x72, 0x61};
  return executeCommand(CMD_UNLOCK, 4, request, NULL, NULL);
}

uint8_t SakuraIO::updateFirmware(){
  return executeCommand(CMD_UPDATE_FIRMWARE, 0, 0, NULL, NULL);
}

uint8_t SakuraIO::getFirmwareUpdateStatus(){
  uint8_t response[1] = {0x00};
  uint8_t responseLength = 1;
  if(executeCommand(CMD_GET_UPDATE_FIRMWARE_STATUS, 0, 0, &responseLength, response) != CMD_ERROR_NONE){
      return 0xff;
  }
  return response[0];
}

uint8_t SakuraIO::reset(){
  return executeCommand(CMD_SOFTWARE_RESET, 0, 0, NULL, NULL);
}
