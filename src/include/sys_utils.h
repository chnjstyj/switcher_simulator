#ifndef __SYS_UTILS_H__
#define __SYS_UTILS_H__

#include <iostream>
#include <string.h>

// ============ 终端颜色规定

// 输出错误时，要附带错误输出程序的所在文件与行号
// printf(ANSI_FG_RED_L "[%s:%d] no match at position %d\n%s\n%*.s^\n" ANSI_NONE, \
//                             filename(__FILE__), __LINE__, position, e, position, ""); printf("\n");

#define ANSI_TMP_DI ANSI_FG_MAGENTA // 临时调试信息：洋红色、常规体
#define ANSI_DI ANSI_FG_WHITE_L // 调试的大量内容（如打印每一条执行的指令）：白色、常规体
#define ANSI_ST ANSI_FG_BLACK // SDB与Trace信息：黑色、粗体
#define ANSI_ST_DI ANSI_FG_BLACK_L // SDB与Trace的调试信息（如打印函数池）：黑色、常规体
#define ANSI_MI_E ANSI_FG_RED_L // 交互式错误信息：红色、常规体
#define ANSI_MI_C ANSI_FG_GREEN_L // 交互式正确信息：绿色、常规体
#define ANSI_MI_WN ANSI_FG_YELLOW_L // 交互式Warning和Notification信息：黄色、常规体
#define ANSI_PR_E ANSI_FG_RED // 程序/结果类错误信息：红色、粗体
#define ANSI_PR_C ANSI_FG_GREEN // 程序/结果类正确信息：绿色、粗体
#define ANSI_PR_WN ANSI_FG_YELLOW // 程序/结果类Warning和Notification信息：黄色、粗体

#define ANSI_FG_BLACK   "\33[1;30m"
#define ANSI_FG_RED     "\33[1;31m"
#define ANSI_FG_GREEN   "\33[1;32m"
#define ANSI_FG_YELLOW  "\33[1;33m"
#define ANSI_FG_BLUE    "\33[1;34m"
#define ANSI_FG_MAGENTA "\33[1;35m"
#define ANSI_FG_CYAN    "\33[1;36m"
#define ANSI_FG_WHITE   "\33[1;37m"

#define ANSI_FG_BLACK_L   "\33[0;30m"
#define ANSI_FG_RED_L     "\33[0;31m"
#define ANSI_FG_GREEN_L   "\33[0;32m"
#define ANSI_FG_YELLOW_L  "\33[0;33m"
#define ANSI_FG_BLUE_L    "\33[0;34m"
#define ANSI_FG_MAGENTA_L "\33[0;35m"
#define ANSI_FG_CYAN_L    "\33[0;36m"
#define ANSI_FG_WHITE_L   "\33[0;37m"

#define ANSI_BG_BLACK   "\33[1;40m"
#define ANSI_BG_RED     "\33[1;41m"
#define ANSI_BG_GREEN   "\33[1;42m"
#define ANSI_BG_YELLOW  "\33[1;43m"
#define ANSI_BG_BLUE    "\33[1;44m"
#define ANSI_BG_MAGENTA "\33[1;35m"
#define ANSI_BG_CYAN    "\33[1;46m"
#define ANSI_BG_WHITE   "\33[1;47m"
#define ANSI_NONE       "\33[0m"

uint64_t get_time();

#define ANSI_FMT(str, fmt) fmt str ANSI_NONE

#define log_write(...) IFDEF(CONFIG_TARGET_NATIVE_ELF, \
  do { \
    extern FILE* log_fp; \
    extern bool log_enable(); \
    if (log_enable()) { \
      fprintf(log_fp, __VA_ARGS__); \
      fflush(log_fp); \
    } \
  } while (0) \
)

#define _Log(...) \
  do { \
    printf(__VA_ARGS__); \
    log_write(__VA_ARGS__); \
  } while (0)

//#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x

inline const char* filename(const char* path) {
    const char* lastSlash = strrchr(path, '/');
    return lastSlash ? lastSlash + 1 : path;
}

#endif