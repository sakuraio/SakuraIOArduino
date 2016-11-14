
typedef void (*cmd_func_t)(int, char **);

typedef struct {
    char *command;
    cmd_func_t func;
} cmd_list_t;


void cmd_version(int argc, char **argv);
void cmd_serial(int argc, char **argv);
void cmd_status(int argc, char **argv);
void cmd_sqi(int argc, char **argv);
void cmd_unixtime(int argc, char **argv);
void cmd_reset(int argc, char **argv);
void cmd_update(int argc, char **argv);
void cmd_enqueue(int argc, char **argv);
void cmd_send(int argc, char **argv);
void cmd_size(int argc, char **argv);
void cmd_cleartx(int argc, char **argv);
void cmd_clearrx(int argc, char **argv);
void cmd_dequeue(int argc, char **argv);
void cmd_peek(int argc, char **argv);
