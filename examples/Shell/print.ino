void printChannel(uint8_t channel, uint8_t type, uint8_t value[8], int64_t offset){
  Serial.print("Offset: ");
  Serial.println((int32_t)(offset/1000));

  Serial.print("Channel: ");
  Serial.println(channel);

  Serial.print("Type: ");
  Serial.println((char)type);

  Serial.print("Value: ");
  switch(type){
    case 'i':
      Serial.println(*(int32_t *)value);
      break;
    case 'I':
      Serial.println(*(uint32_t *)value);
      break;
    case 'l':
      Serial.println((int32_t)*(int64_t *)value);
      break;
    case 'L':
      Serial.println((uint32_t)*(uint64_t *)value);
      break;
    case 'b':
      for(uint8_t i=0; i<8; i++){
        if(value[i] < 0x10){
          Serial.print("0");
        }
        Serial.print(value[i], HEX);
      }
      Serial.println("");
      break;
    default:
      Serial.println("unsupported");
  }
}

void printError(uint8_t status){
  if(status == CMD_ERROR_NONE){
    return;
  }

  Serial.print("[Execute error] ");
  switch(status){
    case CMD_ERROR_NONE:
      break;
    case CMD_ERROR_PARITY:
      Serial.println("Parity missmached");
      break;
    case CMD_ERROR_MISSING:
      Serial.println("Missing command");
      break;
    case CMD_ERROR_INVALID_SYNTAX:
      Serial.println("Invalid syntax");
      break;
    case CMD_ERROR_RUNTIME:
      Serial.println("Runtime error");
      break;
    case CMD_ERROR_LOCKED:
      Serial.println("Locked");
      break;
    case CMD_ERROR_BUSY:
      Serial.println("Busy");
      break;
    default:
      Serial.print("Error code ");
      Serial.println(status);
      break;
  }
}
