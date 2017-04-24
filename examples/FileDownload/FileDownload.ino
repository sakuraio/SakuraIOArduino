#include <SakuraIO.h>

#define FILE_ID     1
#define CHUNK_SIZE  16

//SakuraIO_SPI sakuraio(10);
SakuraIO_I2C sakuraio;

void setup()
{
  Serial.begin(9600);
  Serial.print("Waiting to come online");
  for (;;)
  {
    if ((sakuraio.getConnectionStatus() & 0x80) == 0x80)
      break;
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

void loop()
{
  uint8_t file_status, result = 0;
  uint32_t file_size, crc, received_size = 0;
  uint64_t timestamp;

  uint32_t unixtime = (uint32_t)(sakuraio.getUnixtime() / 1000UL);
  Serial.print("Download start time ");
  Serial.println(unixtime);

  // File download request
  while (result != CMD_ERROR_NONE)
  {
    result = sakuraio.startFileDownload(FILE_ID);
    Serial.print("File request result=");
    Serial.println(result);
    delay(100);
    if (result != CMD_ERROR_NONE)
    {
      // Cancel request
      Serial.println("Error or download already started.");
      sakuraio.cancelFileDownload();
      Serial.println("Cancel requested. retrying...");
      delay(1000);
      continue;
    }
    delay(100);
  }

  delay(500);

  // Get file metadata
  do
  {
    result = sakuraio.getFileMetaData(&file_status, &file_size, &timestamp, &crc);
    Serial.print("Get metadata result=");
    Serial.print(result);
    Serial.print(" status=");
    Serial.print(file_status);
    Serial.print(" total file size=");
    Serial.print(file_size);
    Serial.print(" timestamp=");
    Serial.print((uint32_t)(timestamp / 1000));
    Serial.print(" crc32=");
    Serial.println(crc);
    delay(500);
  } while (result != CMD_ERROR_NONE);

  // Handle server side error
  if (file_status & 0x80 == 0x80)
  {
    Serial.print("Server side error:");
    if (file_status == FILE_META_STATE_NOT_FOUND)
    {
      Serial.println("File not found.");
    } else if (file_status == FILE_META_STATE_SERVER_ERROR)
    {
      Serial.println("Server error.");
    } else if (file_status == FILE_META_STATE_INVALID_DATA)
    {
      Serial.println("Invalid data.");
    } else
    {
      Serial.println("Unknown error.");
    }
  }

  delay(500);

  // Get file body
  while (received_size < file_size)
  {
    uint8_t status = 0;
    uint32_t size = 0;
    uint8_t buf[CHUNK_SIZE] = {0};
    uint8_t len = CHUNK_SIZE;

    // Get request
    result = sakuraio.getFileData(&len, buf);
    Serial.print("File get result=");
    Serial.print(result);
    Serial.print(" chunksize=");
    Serial.println(len);
    if (result != CMD_ERROR_NONE)
    {
      Serial.print("getFileData error=");
      Serial.println(result);
      delay(500);
      continue;
    }

    // Chunk dump
    Serial.print("Chunk=[ ");
    for (int i = 0; i < len; i++)
    {
      char b[4];
      sprintf(b, "%02X ", buf[i]);
      Serial.print(b);
    }
    Serial.println("]");
    delay(5);

    // Get file download status
    result = sakuraio.getFileDownloadStatus(&status, &size);
    Serial.print("File download status result=");
    Serial.print(result);
    Serial.print(" status=");
    Serial.print(status);
    Serial.print(" transferred_size=");
    Serial.println(size);
    received_size += len;
    Serial.print("Downloaded file size=");
    Serial.println(received_size);
    delay(5);
  }

  Serial.println("Download complete");
  unixtime = (uint32_t)(sakuraio.getUnixtime() / 1000UL);
  Serial.print("Download end time ");
  Serial.println(unixtime);
  while (1)
    ;
}