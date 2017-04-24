void cmd_version(int argc, char **argv){
  char version[33];
  uint8_t ret;
  ret = sakuraio.getFirmwareVersion(version);
  if(ret != CMD_ERROR_NONE){
    printError(ret);
    return;
  }
  Serial.println(version);
}

void cmd_serial(int argc, char **argv){
  char serial[33];
  uint8_t ret;
  ret = sakuraio.getUniqueID(serial);
  if(ret != CMD_ERROR_NONE){
    printError(ret);
    return;
  }
  Serial.println(serial);
}


void cmd_status(int argc, char **argv){
  uint8_t ret = sakuraio.getConnectionStatus();

  if((ret & 0x80) == 0x80){
    Serial.println("Online");
  }else{
    Serial.println("Offline");
  }
  Serial.print("Last Error Code: ");
  Serial.println(ret&0x7f);
}

void cmd_sqi(int argc, char **argv){
  uint8_t ret = sakuraio.getSignalQuality();
  Serial.println(ret);
}

void cmd_unixtime(int argc, char **argv){
  uint64_t ret = sakuraio.getUnixtime();

  if(ret > 9999999999999999L){
    Serial.println("Invalid");
    return;
  }

  char buf[16] = {0};
  int8_t s;
  for(s=0; s<sizeof(buf); s++){
    buf[s] = (ret % 10) + '0';
    ret /= 10;
    if(ret == 0){
      break;
    }
  }

  for(; s>=0; s--){
    Serial.print(buf[s]);
  }
  Serial.print('\n');
}


void cmd_reset(int argc, char **argv){
  uint8_t ret;
  ret = sakuraio.unlock();
  if(ret != CMD_ERROR_NONE){
    printError(ret);
    return;
  }
  sakuraio.reset();
}


void cmd_update(int argc, char **argv){
  uint8_t ret;

  ret = sakuraio.getConnectionStatus();
  if((ret & 0x80) != 0x80){
    Serial.println("Offline");
    return;
  }

  char version[33];
  ret = sakuraio.getFirmwareVersion(version);
  if(ret != CMD_ERROR_NONE){
    printError(ret);
    return;
  }
  Serial.print("Current Version: ");
  Serial.println(version);

  delay(100);

  // Request unlock
  Serial.println("Unlock");
  if((ret = sakuraio.unlock()) != CMD_ERROR_NONE){
    printError(ret);
    return;
  }

  // Request firmware update
  Serial.println("Starting update");
  if((ret = sakuraio.updateFirmware()) != CMD_ERROR_NONE){
    printError(ret);
    return;
  }

  delay(1000);

  // Check update status
  uint8_t errCode = 0x00;
  Serial.println("Waiting for update");
  while(1){

    delay(1000);
    Serial.print('.');
    Serial.flush();

    uint8_t updateStatus = sakuraio.getFirmwareUpdateStatus();
    if(updateStatus == 0xff || (updateStatus & 0x80) != 0x00){
      continue;
    }else if(updateStatus == 0x00){
      // Success
      break;
    }else{
      // Error
      errCode = updateStatus & 0x3f;
      break;
    }
  }

  Serial.println("");
  switch(errCode){
    case 0x00:
      break;
    case 0x01:
      Serial.println("Already updated");
      break;
    default:
      Serial.print("Error code ");
      Serial.println(errCode);
      break;
  }

}

void cmd_enqueue(int argc, char **argv){
  // ch, type, value
  if(argc != 4){
    Serial.println("Invalid arguments");
    return;
  }

  uint8_t channel = (uint8_t)atoi(argv[1]);
  char type = argv[2][0];
  int64_t value = atoi(argv[3]);
  uint8_t ret;

  int32_t v_int32;
  uint32_t v_uint32;
  int64_t v_int64;
  uint64_t v_uint64;

  switch(type){
    case 'i':
      v_int32 = (int32_t)value;
      ret = sakuraio.enqueueTx(channel, v_int32);
      break;
    case 'I':
      v_uint32 = (int32_t)value;
      ret = sakuraio.enqueueTx(channel, v_uint32);
      break;
    case 'l':
      v_int64 = (int64_t)value;
      ret = sakuraio.enqueueTx(channel, v_int64);
      break;
    case 'L':
      v_uint64 = (uint64_t)value;
      ret = sakuraio.enqueueTx(channel, v_uint64);
      break;
    case 'b':
      Serial.println("Type 'b' is not implemented");
      return;
    default:
      Serial.print("Invalid type: ");
      Serial.println(argv[2]);
      return;
      break;
  }

  switch(ret){
    case CMD_ERROR_RUNTIME:
      Serial.println("full");
      break;
    default:
      printError(ret);
      break;
  }
}


void cmd_send(int argc, char **argv){
  uint8_t ret = sakuraio.send();
  if(ret != CMD_ERROR_NONE){
    printError(ret);
    return;
  }
}

void cmd_size(int argc, char **argv){
  uint8_t avail;
  uint8_t queued;
  uint8_t ret;

  ret = sakuraio.getTxQueueLength(&avail, &queued);
  if(ret != CMD_ERROR_NONE){
    printError(ret);
    return;
  }
  Serial.println("Tx Queue");
  Serial.print("   Avail: ");
  Serial.println(avail);
  Serial.print("  Queued: ");
  Serial.println(queued);

  ret = sakuraio.getRxQueueLength(&avail, &queued);
  if(ret != CMD_ERROR_NONE){
    printError(ret);
    return;
  }
  Serial.println("Rx Queue");
  Serial.print("   Avail: ");
  Serial.println(avail);
  Serial.print("  Queued: ");
  Serial.println(queued);
}

void cmd_cleartx(int argc, char **argv){
  uint8_t ret = sakuraio.clearTx();
  if(ret != CMD_ERROR_NONE){
    printError(ret);
    return;
  }
}

void cmd_clearrx(int argc, char **argv){
  uint8_t ret = sakuraio.clearRx();
  if(ret != CMD_ERROR_NONE){
    printError(ret);
    return;
  }
}

void cmd_dequeue(int argc, char **argv){
  uint8_t channel;
  uint8_t type;
  uint8_t value[8];
  int64_t offset;
  uint8_t ret;

  ret = sakuraio.dequeueRx(&channel, &type, value, &offset);
  if(ret != CMD_ERROR_NONE){
    switch(ret){
      case CMD_ERROR_RUNTIME:
        Serial.println("empty");
        return;
        break;
      default:
        printError(ret);
        return;
        break;
    }
  }

  printChannel(channel, type, value, offset);
}


void cmd_peek(int argc, char **argv){
  uint8_t channel;
  uint8_t type;
  uint8_t value[8];
  int64_t offset;
  uint8_t ret;

  ret = sakuraio.peekRx(&channel, &type, value, &offset);
  if(ret != CMD_ERROR_NONE){
    switch(ret){
      case CMD_ERROR_RUNTIME:
        Serial.println("empty");
        return;
        break;
      default:
        printError(ret);
        return;
        break;
    }
  }

  printChannel(channel, type, value, offset);
}
