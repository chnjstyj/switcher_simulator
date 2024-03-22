#include <iostream>
#include <stddef.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "com.h"

#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args);

static int cmd_run(char *args)
{
    //run one step
    return 0;
}

static int cmd_q(char *args)
{
    //exit
    return -1;
}

static struct {
    const char *name;
    const char *description;
    int (*handler) (char *);
} cmd_table [] = {
        { "help", "Display information about all supported commands", cmd_help },
        { "run", "Run one step ", cmd_run },
        { "q", "Exit", cmd_q },
        /*
        { "si", "Run one stop", cmd_si },
        { "info", "Print program status,r regfiles w watchpoints", cmd_info },
        { "x", "Print memory", cmd_x },
        /

        /* TODO: Add more commands */

};

static int cmd_help(char *args) {
    /* extract the first argument */
    char *arg = strtok(NULL, " ");
    int i;

    if (arg == NULL) {
        /* no argument given */
        for (i = 0; i < NR_CMD; i ++) {
            printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        }
    }
    else {
        for (i = 0; i < NR_CMD; i ++) {
            if (strcmp(arg, cmd_table[i].name) == 0) {
                printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
                return 0;
            }
        }
        printf("Unknown command '%s'\n", arg);
    }
    return 0;
}

static char* rl_gets() {
    static char *line_read = NULL;

    if (line_read) {
        free(line_read);
        line_read = NULL;
    }

    line_read = readline("(simulator) ");

    if (line_read && *line_read) {
        add_history(line_read);
    }
    return line_read;
}

void sdb_mainloop() {
    for (char *str; (str = rl_gets()) != NULL; ) {
        char *str_end = str + strlen(str);

        /* extract the first token as the command */
        char *cmd = strtok(str, " ");
        if (cmd == NULL) { continue; }

        /* treat the remaining string as the arguments,
         * which may need further parsing
         */
        char *args = cmd + strlen(cmd) + 1;
        if (args >= str_end) {
            args = NULL;
        }

        int i;
        for (i = 0; i < NR_CMD; i ++) {
            if (strcmp(cmd, cmd_table[i].name) == 0) {
                if (cmd_table[i].handler(args) < 0) { return; }
                break;
            }
        }

        if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
    }
}