#include "include/common.h"
#include "include/debug.h"

// MMU只管理虚拟地址到物理地址的映射
// 当出现拆包写入的情况时，拆包的长度由SGDMA决定，其将数据与相应的写入长度交给MMU写入
// MMU只负责处理跨片读写的这一种特殊情况

uint8_t pmem[CONFIG_SRAM_NUM][CONFIG_SRAM_SIZE] = {};

void pmem_init() {
    for (int i = 0; i < CONFIG_SRAM_NUM; i++) {
        for (int j = 0; j < CONFIG_SRAM_SIZE; j++) {
            pmem[i][j] = 0;
        }
    }
}

bool pmem_is_full(uint8_t block) {
    Assert(block < CONFIG_SRAM_NUM, "block index out of bound");
    for (int i = 0; i < CONFIG_SRAM_SIZE; i++) {
        if (pmem[block][i] == 0) {
            return false;
        }
    }
    return true; // 没有找到未使用的空间，该块已满
}

void pmem_write(uint32_t vaddr, const uint8_t* data, uint32_t length) {
    while (length > 0) {
        uint8_t block = vaddr / CONFIG_SRAM_SIZE;
        uint32_t block_addr = vaddr % CONFIG_SRAM_SIZE;
        uint32_t space_in_block = CONFIG_SRAM_SIZE - block_addr;
        uint32_t chunk = (length < space_in_block) ? length : space_in_block;

        Assert(block < CONFIG_SRAM_NUM, "vaddr out of bound");

        for (uint32_t i = 0; i < chunk; i++) {
            pmem[block][block_addr + i] = data[i];
        }

        vaddr += chunk;
        data += chunk;
        length -= chunk;
    }
}

void pmem_read(uint32_t vaddr, uint8_t* buffer, uint32_t length) {
    while (length > 0) {
        uint8_t block = vaddr / CONFIG_SRAM_SIZE;
        uint32_t block_addr = vaddr % CONFIG_SRAM_SIZE;
        uint32_t space_in_block = CONFIG_SRAM_SIZE - block_addr;
        uint32_t chunk = (length < space_in_block) ? length : space_in_block;

        Assert(block < CONFIG_SRAM_NUM, "vaddr out of bound");

        for (uint32_t i = 0; i < chunk; i++) {
            buffer[i] = pmem[block][block_addr + i];
        }

        vaddr += chunk;
        buffer += chunk;
        length -= chunk;
    }
}

