// Common
#define CMD_GET_CONNECTION_STATUS 0x01  // OK
#define CMD_GET_SIGNAL_QUALITY	  0x02  // OK
#define CMD_GET_DATETIME	        0x03  // OK
#define CMD_ECHO_BACK	            0x0f  // OK

// IO
#define CMD_READ_ADC              0x10  // OK

// Transmit
#define CMD_TX_ENQUEUE      	    0x20  // OK
#define CMD_TX_SENDIMMED          0x21  // OK
#define CMD_TX_LENGTH	            0x22  // OK
#define CMD_TX_CLEAR	            0x23  // OK
#define CMD_TX_SEND	              0x24  // OK
#define CMD_TX_STAT	              0x25  // OK

// Receive
#define CMD_RX_DEQUEUE	          0x30  // OK
#define CMD_RX_PEEK	              0x31  // OK
#define CMD_RX_LENGTH	            0x32  // OK
#define CMD_RX_CLEAR	            0x33  // OK

// File Download
#define CMD_START_FILE_DOWNLOAD       0x40  // OK
#define CMD_GET_FILE_METADATA         0x41  // OK
#define CMD_GET_FILE_DOWNLOAD_STATUS  0x42  // OK
#define CMD_CANCEL_FILE_DOWNLOAD      0x43  // OK
#define CMD_GET_FILE_DATA             0x44  // OK

// Operation
#define CMD_GET_PRODUCT_ID        0xA0  // OK
#define CMD_GET_UNIQUE_ID         0xA1  // OK
#define CMD_GET_FIRMWARE_VERSION  0xA2  // OK
#define CMD_UNLOCK                0xA8  // OK
#define CMD_UPDATE_FIRMWARE       0xA9  // OK
#define CMD_GET_UPDATE_FIRMWARE_STATUS       0xAA  // OK
#define CMD_SOFTWARE_RESET        0xAF  // OK


// Response
#define CMD_ERROR_NONE             0x01
#define CMD_ERROR_PARITY           0x02
#define CMD_ERROR_MISSING          0x03
#define CMD_ERROR_INVALID_SYNTAX   0x04
#define CMD_ERROR_RUNTIME          0x05
#define CMD_ERROR_LOCKED           0x06
#define CMD_ERROR_BUSY             0x07

// File Download status
#define FILE_DOWNLOAD_STATE_NONE 0x00
#define FILE_DOWNLOAD_STATE_REQUESTED 0x01
#define FILE_DOWNLOAD_STATE_RUNNING 0x02

// File metadata status
#define FILE_META_STATE_OK 0x00
#define FILE_META_STATE_ERROR 0x01
#define FILE_META_STATE_INVALID_REQUEST 0x02
#define FILE_META_STATE_NOT_FOUND 0x81
#define FILE_META_STATE_SERVER_ERROR 0x82
#define FILE_META_STATE_INVALID_DATA 0x83
