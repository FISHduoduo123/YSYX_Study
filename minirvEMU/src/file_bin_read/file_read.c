#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../dbg.h"

#define mem_size (1 << 30)

uint32_t load_file_to_rom (uint8_t **mem, char *path)
{
    FILE *fp = NULL;
    fp = fopen(path, "rb");
    check(fp != NULL, "Failed to open file: %s", path);

    // 获取文件大小
    fseek(fp,0,SEEK_END);
    uint32_t file_size = ftell(fp);
    fseek(fp,0,SEEK_SET);

    // 分配一个可寻址30位大小内存空间
    *mem = malloc(mem_size);
    check(*mem != NULL, "Failed to allocate memory for ROM.");

    // 加载指令
    check(file_size < mem_size, "File too large for allocated memory\n");

    size_t read_size = fread(*mem, 1, file_size, fp);
    check(read_size == file_size, "Failed to read the file: %s", path);
    fclose(fp);
    return file_size;
    
    error:
        if(fp) fclose(fp);
        if (*mem) free(*mem);
        return 0;
}
