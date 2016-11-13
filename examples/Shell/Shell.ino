#include "ntshell.h"

static int func_read(char *buf, int cnt, void *extobj);
static int func_write(const char *buf, int cnt, void *extobj);
static int func_callback(const char *text, void *extobj);
static void ntshell_poll(ntshell_t *p);

ntshell_t ntshell;


void setup() {
  // put your setup code here, to run once:
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
  Serial.println(text);
}

static void ntshell_poll(ntshell_t *p)
{
  if (p->initcode != INITCODE) {
    return;
  }
  unsigned char ch;
  SERIAL_READ(p, (char *)&ch, sizeof(ch));
  vtrecv_execute(&(p->vtrecv), &ch, sizeof(ch));
}
