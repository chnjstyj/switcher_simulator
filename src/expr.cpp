/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POS IX regex functions.
 */
#include <regex.h>
#include "include/debug.h"
#include "include/common.h"
#include "include/marco.h"
#include "include/sys_utils.h"
#include "include/utils.h"


// =============== Token与规则定义 ===============
enum {
    TK_NOTYPE = 999,

    TK_NONDATA = 100, TK_DEC = 101, TK_HEX = 102, TK_REG = 109,

    // ------ Precedence level 1: 各种括号，如()、[]等、成员运算符
    TK_LPAREN = 11, TK_RPAREN = 12,
    // ------ Precedence level 2: 所有单目运算符，如++、--、!、~等
    TK_DEREF = 21,
    // ------ Precedence level 3: 乘法运算符*、除法运算符/、求余运算符%
    TK_TIMES = 31, TK_DIV = 32,
    // ------ Precedence level 4: 加法运算符+、减法运算符-
    TK_PLUS = 41, TK_MINUS = 42,

    // ------ Precedence level 7: 等、不等号
    TK_EQ = 71, TK_NEQ = 72,
    // ------ Precedence level 8。
    // TODO：先用第八级代替一下，感觉单用枚举来进行优先级的划分对于后续处理有些不太方便，好像也没有二维枚举这种东西
    // TODO：如果要改进可以用二维数组来保存token，用数组的下标来划分优先级
    TK_LAND = 81, // 逻辑与&&

    /* TODO: Add more token types */

};

static struct rule {
    const char *regex;
    int token_type;
} rules[] = {

        /* TODO: Add more rules.
         * Pay attention to the precedence level of different rules.
         */
        {"0x[0-9,a-f,A-F]+", TK_HEX}, //! 十六进制的TK要放在十进制的TK前。这很好地说明了前行注释中的优先级问题
        {"[0-9]+", TK_DEC}, //? 为什么用 \\d+ 就不行
        {"\\$(\\$0|ra|sp|gp|tp|t[0-6]|s[0-9]+|a[0-7]|pc)",TK_REG},
        {" +", TK_NOTYPE},
        {"\\+", TK_PLUS},
        {"\\-", TK_MINUS},
        {"\\*", TK_TIMES},
        {"\\/", TK_DIV},
        {"\\(", TK_LPAREN},
        {"\\)", TK_RPAREN},
        {"\\==", TK_EQ},
        {"\\!=", TK_NEQ},
        {"\\&&", TK_LAND},
};

// =============== 正则表达式相关 ===============
#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
    int i;
    char error_msg[128];
    int ret;

    for (i = 0; i < NR_REGEX; i ++) {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0) {
            regerror(ret, &re[i], error_msg, 128);
            panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

// =============== make token ===============
typedef struct token {
    int type;
    char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
    int position = 0;
    int i;
    regmatch_t pmatch;

    nr_token = 0;

    while (e[position] != '\0') {
        /* Try all rules one by one. */
        for (i = 0; i < NR_REGEX; i ++) {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;

                // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
                //     i, rules[i].regex, position, substr_len, substr_len, substr_start);
//Tips %.*s是一个格式化字符串，用于输出一个指定长度的字符串，因此需要两个参数，第一个是字符串长度，第二是个地址
                position += substr_len;

                /* TODO: Now a new token is recognized with rules[i]. Add codes
                 * to record the token in the array `tokens'. For certain types
                 * of tokens, some extra actions should be performed.
                 */
                int val;

                switch (rules[i].token_type) {
                    case TK_NOTYPE: break;
                    case TK_DEC:
                        tokens[nr_token].type = TK_DEC;
                        //! 在结构体中，str采用字符数组的形式定义，不能直接赋值
                        strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0';
                        nr_token++;
                        break;
                    case TK_HEX:
                        tokens[nr_token].type = TK_HEX;
                        // strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0';
                        // 先将十六进制转为十进制数参与计算
                        unsigned long dec; char hex_str[32];
                        strncpy(hex_str, substr_start, substr_len); hex_str[substr_len] = '\0';
                        // t: printf("测试用函数：\t位置：十六进制token值转换\n\t\t hex_str: %s\n", hex_str);
                        dec = charHex_To_uint32Dec(hex_str);
                        // t: printf("测试用函数：\t位置：十六进制token值转换\n\t\t dec: %ld\n", dec);
                        sprintf(tokens[nr_token].str, "%ld", dec);
                        nr_token++;
                        break;
//                    case TK_REG:
//                        tokens[nr_token].type = TK_REG;
//                        char reg_name[3];
//                        snprintf(reg_name, 3, "%.*s", substr_len - 1, substr_start + 1);
//                        // printf("%s的值为0x%08x\n",reg_name, isa_reg_str2val(reg_name, NULL));
//                        // printf("%s的值为%d\n",reg_name, isa_reg_str2val(reg_name, NULL));
//                        val = isa_reg_str2val(reg_name, NULL);
//                        sprintf(tokens[nr_token].str, "%d", val);
//                        nr_token++;
//                        break;
                    case TK_PLUS:
                        tokens[nr_token].type = TK_PLUS;
                        strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0';
                        nr_token++;
                        break;
                    case TK_MINUS:
                        tokens[nr_token].type = TK_MINUS;
                        strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0';
                        nr_token++;
                        break;
                    case TK_TIMES:
                        tokens[nr_token].type = TK_TIMES;
                        strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0';
                        nr_token++;
                        break;
                    case TK_DIV:
                        tokens[nr_token].type = TK_DIV;
                        strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0';
                        nr_token++;
                        break;
                    case TK_LPAREN:
                        tokens[nr_token].type = TK_LPAREN;
                        strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0';
                        nr_token++;
                        break;
                    case TK_RPAREN:
                        tokens[nr_token].type = TK_RPAREN;
                        strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0';
                        nr_token++;
                        break;
                    case TK_EQ:
                        tokens[nr_token].type = TK_EQ;
                        strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0';
                        nr_token++;
                        break;
                    case TK_NEQ:
                        tokens[nr_token].type = TK_NEQ;
                        strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0';
                        nr_token++;
                        break;
                    case TK_LAND:
                        tokens[nr_token].type = TK_LAND;
                        strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0';
                        nr_token++;
                        break;
                    default: TODO();
                }

                break;
            }
        }

        if (i == NR_REGEX) {
            printf(ANSI_FG_RED_L "[%s:%d] no match at position %d\n%s\n%*.s^\n" ANSI_NONE, \
                            filename(__FILE__), __LINE__, position, e, position, ""); printf("\n");
            return false;
        }
    }

    return true;
}

// =============== 辅助函数 ===============
// --------------- 括号检查 ---------------
static bool check_parentheses(int p, int q) {
    if (tokens[p].type == TK_LPAREN && tokens[q].type == TK_RPAREN) {
        int balance = 0;
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == TK_LPAREN) {
                balance++;
            }
            else if (tokens[i].type == TK_RPAREN) {
                balance--;
                if (balance < 0) {  // 括号不匹配
                    return false;
                }
                else if (balance == 0 && i != q) {  // 括号内还有其他内容，不是一对完整的括号
                    return false;
                }
            }
        }
        return balance == 0;
    }
    return false;
}

// --------------- 寻找主运算符 ---------------
static int find_main_operator(int p, int q) {
    int balance = 0;
    int tk_level_cur = 0, tk_level_last = 0;
    int op_pos = 0;

    for (int i = p; i <= q; i++) {
        if (tokens[i].type == TK_LPAREN)
            balance++;
        else if (tokens[i].type == TK_RPAREN)
            balance--;
        else {
            if (balance == 0 && tokens[i].type < TK_NONDATA) {
                // 当遍历到非数据token且不在括号内，比较优先级来决定当前符号是否为主运算符
                tk_level_cur = tokens[i].type / 10;
                op_pos = (tk_level_cur >= tk_level_last) ? i : op_pos;
                tk_level_last = tk_level_cur;
            }
        }
    }
    return op_pos;
}

// =============== 计算框架 ===============
int eval(int p, int q) {
    int op;
    // int val1, val2;
    if (p > q) {
        /* Bad expression */
        printf("p,q值有误，两者分别为%d,%d\n", p, q);
        assert(0);
    }
    else if (p == q) {
        /* Single token.
         * For now this token should be a number.
         * Return the value of the number.
         */
        //t: printf("tokens[p] = %s\n", tokens[p].str);
        return atoi(tokens[p].str);
    }
    else if (check_parentheses(p, q) == true) {
        /* The expression is surrounded by a matched pair of parentheses.
         * If that is the case, just throw away the parentheses.
         */
        return eval(p + 1, q - 1);
    }
    else {
        op = find_main_operator(p, q);
        // if (tokens[op].type == TK_DEREF ) {
        //   val = eval(op + 1, q);
        // }
        // else {
        // val1 = eval(p, op - 1);
        // val2 = eval(op + 1, q);
        // }
        // t:  printf("op=%d\n", op);

        int val;
        char str_tmp[32];
        unsigned long addr_long;
        uint32_t addr;

        switch (tokens[op].type) {
            case TK_PLUS:   return eval(p, op - 1) +  eval(op + 1, q);
            case TK_MINUS:  return eval(p, op - 1) -  eval(op + 1, q);
            case TK_TIMES:  return eval(p, op - 1) *  eval(op + 1, q);
            case TK_DIV:    return eval(p, op - 1) /  eval(op + 1, q);
            case TK_EQ:     return eval(p, op - 1) == eval(op + 1, q);
            case TK_NEQ:    return eval(p, op - 1) != eval(op + 1, q);
            case TK_LAND:   return eval(p, op - 1) && eval(op + 1, q);

//            case TK_DEREF:
//                val = eval(op + 1, q);
//                sprintf(str_tmp, "%x", val); // 将val以十六进制的字符串表示
//                addr_long = strtoul(str_tmp, NULL, 16); // 这里十六进制的字符串不以0x开头，所以要设定一下转换基数
//                addr = (uint32_t)addr_long;
//                return paddr_read(addr, 4);

            default: printf(ANSI_FG_RED_L "[%s:%d] 有未定义的操作符%d！请重新输入\n" ANSI_NONE, filename(__FILE__), __LINE__, op); printf("\n");
        }
        return 0;
    }
}

// =============== 计算主函数 ===============
uint32_t expr(char *e, bool *success) {
    if (!make_token(e)) {
        *success = false;
        return 0;
    }
    //t: printf("token_number = %d\n", nr_token);
// --------------- Token 转换 ---------------
    for (int i = 0; i < nr_token; i ++) {
        // ----------- 乘号与指针
        if (tokens[i].type == TK_TIMES && (i == 0 || (tokens[i - 1].type != TK_DEC
                                                      && tokens[i - 1].type != TK_HEX
                                                      && tokens[i - 1].type != TK_LPAREN
                                                      && tokens[i - 1].type != TK_RPAREN ))) {
            tokens[i].type = TK_DEREF;
            //t: printf("token[%d]的类型为：%d\n", i - 1, tokens[i - 1].type);
            //t: printf("token[%d]的最终类型变为了：TK_DEREF\n", i);
        }
    }
    int res = eval(0, nr_token - 1);
    printf(ANSI_PR_WN "res = %d(dec), %08x(hex)\n" ANSI_NONE, res, res);

    return res;
}
