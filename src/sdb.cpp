#include <iostream>
#include <stddef.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "include/sdb.h"

void pmem_read(uint32_t vaddr, uint8_t* buffer, uint32_t length);

void init_regex();

#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args);

static int cmd_c(char *args) {
//    npc_exec(-1);
    return 0;
}

static int cmd_q(char *args) {
//    npc_state.state = NPC_QUIT;
    return -1;
}

//static int cmd_si(char *args) {
//    if (args == NULL) {
//        npc_exec(1);
//    }
//    else {
//        int step_num = atoi(args);
//        if (step_num >= 0 && step_num <= 127)
//            npc_exec(step_num);
//        else
//            printf("Invalid parameter, the number of single-step execution should be between 0 and 127. \n");
//    }
//    return 0;
//}

static int cmd_x(char *args) {
    // extract the args
    char *arg = args;
    int scan_num = atoi(strtok(arg, " ")); // 切分参数
    char *addr_test = strtok(NULL, " ");

    // paddr_t addr = atoi(addr_test);
    unsigned long addr_long = strtoul(addr_test, NULL, 0);
    uint32_t addr = (uint32_t)addr_long;
    // t: printf("%d %ld %s %x \n",scan_num, addr_long, addr_test, addr);

    for (int i = 0; i < scan_num; i++) {
        printf("0x%x: 0x%08x\n",addr, pmem_read(addr, 4));
        addr += 4;
    }

    return 0;
}

static struct {
    const char *name;
    const char *description;
    int (*handler) (char *);
} cmd_table [] = {
        { "help", "Display information about all supported commands", cmd_help },
        { "run", "Run one step ", cmd_run },
        { "q", "Exit", cmd_q },

        { "si", "Run one stop", cmd_si },
        { "info", "Print program status,r regfiles w watchpoints", cmd_info },
        { "x", "Print memory", cmd_x },

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

void init_sdb() {
    /* Compile the regular expressions. */
    init_regex();
}