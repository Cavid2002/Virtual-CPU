#include "./include/CPU.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "[ERROR] usage [main] [binary_file]");
        return 1;
    }

    FILE* file = fopen(argv[1], "rb");

    int c = fread(mem, sizeof(uint32_t), MEM_SIZE, file);

    for(int i = 0; mem[i] != 0; i++)
    {
        printf("%d : %d %X\n", i, mem[i], mem[i]);
    }
    
    fclose(file);

    cpu_init();
    cpu_loop();
    return 0;
}