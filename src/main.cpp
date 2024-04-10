#include <iostream>
//#include "include/sdb.h"
#include "include/package.h"
#include "include/channel.h"
#include "include/common.h"


mem_point mem[mem_size] = {0};


int main() {
    std::cout << "Hello, World!" << std::endl;
    //mem = (mem_point*)malloc(mem_size * sizeof(mem_point));
    int pos[mem_size] = {0};
    int head = 0;
    int tail = 0;


    //init_sdb();
    //sdb_mainloop();

    // length level des
    int p1data[2] = {0};
    p1data[0] = 0x23f;
    p1data[1] = 0x12;

    int p2data[2] = {0};
    p2data[0] = 0x235;
    p2data[1] = 0x13;

    int p3data[2] = {0};
    p3data[0] = 0x255;
    p3data[1] = 0x14;

    int p4data[2] = {0};
    p4data[0] = 0x256;
    p4data[1] = 0x15;

    Channel cs[16] { {true },{true },{true },{true },{true },{true },{true },{true },{true },{true },{true },{true },{true },{true },{true },{true }};

    //Channel c1 = Channel(true);
    cs[0].add(mem,p1data,cs);
    int output[32] = {0};

    cs[0].add(mem,p2data,cs);
    cs[0].add(mem,p3data,cs);
    cs[5].output(mem,output);
    cs[0].add(mem,p4data,cs);

    return 0;
}
