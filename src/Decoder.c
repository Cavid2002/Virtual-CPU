#include "../include/Decoder.h"


Decoder decoder_init()
{
    Decoder decoder;
    decoder.cond = 0;
    decoder.flag_res = 0;
    decoder.func = 0;
    decoder.opcode = 0;
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