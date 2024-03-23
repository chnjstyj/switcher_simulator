#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============== 十六进制char转为十进制数 ===============
unsigned int HexToDec(char hex) {
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    else if (hex >= 'A' && hex <= 'F')
        return hex - 'A' + 10;
    else if (hex >= 'a' && hex <= 'f')
        return hex - 'a' + 10;
    else
        return -1;
}

unsigned int charHex_To_uint32Dec(const char* hexNumber) {
    const char* hexPtr;
    unsigned int decimalNumber = 0;

    if (hexNumber[0] == '0' && (hexNumber[1] == 'x' || hexNumber[1] == 'X'))
        hexPtr = hexNumber + 2;
    else
        hexPtr = hexNumber;

    while (*hexPtr != '\0') {
        unsigned int value = HexToDec(*hexPtr);
        if (value == -1) {
            printf("输入的十六进制数包含非法字符：%c\n", *hexPtr);
            return -1;
        }
        decimalNumber = decimalNumber * 16 + value;
        hexPtr++;
    }

    return decimalNumber;
}