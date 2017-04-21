#include <SakuraIO.h>
#include "ntshell.h"
#include "commands.h"

static int func_read(char *buf, int cnt, void *extobj);
static int func_write(const char *buf, int cnt, void *extobj);
static int func_callback(const char *text, void *extobj);
static int ntopt_callback(int argc, char **argv, void *extobj);
static void ntshell_poll(ntshell_t *p);

ntshell_t ntshell;

cmd_list_t cmd_list[] = {
  {(char*)"version", cmd_version},
  {(char*)"serial", cmd_serial},
  {(char*)"status", cmd_status},
  {(char*)"sqi", cmd_sqi},
  {(char*)"unixtime", cmd_unixtime},
  {(char*)"update", cmd_update},
  {(char*)"reset", cmd_reset},
  {(char*)"enqueue", cmd_enqueue},
  {(char*)"send", cmd_send},
  {(char*)"size", cmd_size},
  {(char*)"cleartx", cmd_cleartx},
  {(char*)"dequeue", cmd_dequeue},
  {(char*)"peek", cmd_peek},
  {(char*)"clearrx", cmd_clearrx},
  {(char*)"file", cmd_file},
  {NULL, NULL}
};

//SakuraIO_SPI sakuraio(10);
SakuraIO_I2C sakuraio;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello");
  Serial.print(">");
  ntshell_init(&ntshell, func_read, func_write, func_callback, (void *)&ntshell);
  ntshell_set_prompt(&ntshell, ">");
}

void loop() {
  ntshell_poll(&ntshell);
}


/* NT-Shell */
static int func_read(char *buf, int cnt, void *extobj) {
  if (Serial.available())
    return Serial.readBytes(buf, cnt);
  else
    return 0;
}

static int func_write(const char *buf, int cnt, void *extobj) {
  return Serial.write(buf, cnt);
}

static int func_callback(const char *text, void *extobj) {
  return ntopt_parse((const char *)text, ntopt_callback, extobj);
}

static int ntopt_callback(int argc, char **argv, void *extobj){
     if (argc == 0) {
         return 0;
     }

     int execnt = 0;
     const cmd_list_t *p = cmd_list;
     while (p->command != NULL) {
         if (ntlibc_strcmp((const char *)argv[0], p->command) == 0) {
             p->func(argc, argv);
             execnt++;
         }
         p++;
     }
     if (execnt == 0) {
            Serial.println("unknown command");
     }

     return 0;
}


static void ntshell_poll(ntshell_t *p)
{
  if (p->initcode != INITCODE) {
    return;
  }
  unsigned char ch;
  if(SERIAL_READ(p, (char *)&ch, sizeof(ch)) > 0)
    vtrecv_execute(&(p->vtrecv), &ch, sizeof(ch));
}
