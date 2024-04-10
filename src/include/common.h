#ifndef __COMMON_H__
#define __COMMON_H__

#include <cassert>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

typedef struct mem_point
{
    uint32_t data;
    bool is_used;
    mem_point* next;
}mem_point;

#define mem_size 1000

#define CONFIG_SRAM_SIZE 0x40000
#define CONFIG_SRAM_NUM 32

#endif

