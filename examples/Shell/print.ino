const char errorMessage_error_code[] PROGMEM  = {"Error code "};

const char errorMessage_parity[] PROGMEM  = {"Parity missmached"};
const char errorMessage_missing[] PROGMEM  = {"Missing command"};
const char errorMessage_invalid_syntax[] PROGMEM  = {"Invalid syntax"};
const char errorMessage_runtime[] PROGMEM  = {"Runtime error"};
const char errorMessage_locked[] PROGMEM  = {"Locked"};
const char errorMessage_busy[] PROGMEM  = {"Busy"};

const char fileErrorMessage_msg[] PROGMEM  = {"[file error] "};
const char fileErrorMessage_error[] PROGMEM  = {"Download error"};
const char fileErrorMessage_invalid_request[] PROGMEM  = {"Invalid request"};
const char fileErrorMessage_notfound[] PROGMEM  = {"Not found"};
const char fileErrorMessage_server_error[] PROGMEM  = {"Server error"};
const char fileErrorMessage_invalid_data[] PROGMEM  = {"Invalid data from server"};


static void printConstString(const char msg[]){
  int len = strlen_P(msg);
  for (uint16_t k = 0; k < len; k++)
  {
    char c =  pgm_read_byte_near(msg + k);
    Serial.print(c);
  }
}

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
      printConstString(errorMessage_parity);Serial.println();
      break;
    case CMD_ERROR_MISSING:
      printConstString(errorMessage_missing);Serial.println();
      break;
    case CMD_ERROR_INVALID_SYNTAX:
      printConstString(errorMessage_invalid_syntax);Serial.println();
      break;
    case CMD_ERROR_RUNTIME:
      printConstString(errorMessage_runtime);Serial.println();
      break;
    case CMD_ERROR_LOCKED:
      printConstString(errorMessage_locked);Serial.println();
      break;
    case CMD_ERROR_BUSY:
      printConstString(errorMessage_busy);Serial.println();
      break;
    default:
      printConstString(errorMessage_error_code);
      Serial.println(status);
      break;
  }
}

void printFileError(uint8_t status){
  if (status == 0x00) {
    return;
  }
  printConstString(fileErrorMessage_msg);
  switch(status){
    case FILE_STATUS_ERROR:
      printConstString(fileErrorMessage_error); Serial.println();
      break;
    case FILE_STATUS_INVALID_REQUEST:
      printConstString(fileErrorMessage_invalid_request); Serial.println();
      break;
    case FILE_STATUS_NOTFOUND:
      printConstString(fileErrorMessage_notfound); Serial.println();
      break;
    case FILE_STATUS_SERVER_ERROR:
      printConstString(fileErrorMessage_server_error); Serial.println();
      break;
    case FILE_STATUS_INVALID_DATA:
      printConstString(fileErrorMessage_invalid_data); Serial.println();
      break;
    default:
      printConstString(errorMessage_error_code);
      Serial.println(status);
      break;
  }
}
