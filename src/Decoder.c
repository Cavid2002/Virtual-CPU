#include "../include/Decoder.h"


Decoder decoder_init()
{
    Decoder decoder;
    decoder.cond = 0;
    decoder.flag_res = 0;
    decoder.func = 0;
    decoder.opcode = 0;

    decoder.op[0] = equal;
    decoder.op[1] = not_equal;
    decoder.op[2] = carry_set;
    decoder.op[3] = carry_clear;
    decoder.op[4] = minus;
    decoder.op[5] = plus;
    decoder.op[6] = overflow_set;
    decoder.op[7] = overflow_clear;
    decoder.op[8] = higher;
    decoder.op[9] = lower_same;
    decoder.op[10] = greater_equal;
    decoder.op[11] = less;
    decoder.op[12] = greater;
    decoder.op[13] = less_equal;
    decoder.op[14] = always;
    decoder.op[15] = NULL;
    return decoder;
}

RegFile registers_init()
{
    RegFile rfile;
    rfile.dest_index = 0;
    rfile.src1_index = 0;
    rfile.src2_index = 0;
    rfile.immediate = 0;
    
    for(int i = 0; i < 16; i++)
    {
        rfile.r[i] = 0;
    }

    return rfile;

}


uint8_t equal(uint8_t flag)
{
    return flag & 0x02;
}

uint8_t not_equal(uint8_t flag)
{
    return ~(flag & 0x02);
}

uint8_t carry_set(uint8_t flag)
{
    return flag & 0x03;
}

uint8_t carry_clear(uint8_t flag)
{
    return ~(flag & 0x03);
}

uint8_t minus(uint8_t flag)
{
    return flag & 0x01;
}

uint8_t plus(uint8_t flag)
{
    return ~(flag & 0x01);
}

uint8_t overflow_set(uint8_t flag)
{
    return flag & 0x04;
}

uint8_t overflow_clear(uint8_t flag)
{
    return ~(flag | 0x04);
}

uint8_t higher(uint8_t flag)
{
    return ~((flag & 0x02) >> 1) & ((flag & 0x03) >> 2);
}

uint8_t lower_same(uint8_t flag)
{
    return ((flag & 0x02) >> 1) | ~((flag & 0x03) >> 2);
}

uint8_t greater_equal(uint8_t flag)
{
    return ~((flag & 0x01) ^ ((flag & 0x04) >> 3));
}

uint8_t less(uint8_t flag)
{
    return ((flag & 0x01) ^ ((flag & 0x04) >> 3));
}

uint8_t greater(uint8_t flag)
{
    return (~(flag & 0x02 >> 1) & ~((flag & 0x01) ^ (flag & 0x04) >> 3));
}

uint8_t less_equal(uint8_t flag)
{
    return (flag & 0x02 >> 1) | ((flag & 0x01) ^ ((flag & 0x04) >> 3));
}

uint8_t always(uint8_t flag)
{
    return 1;
}