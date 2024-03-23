#include <iostream>
//#include "include/sdb.h"
#include "include/package.h"
#include "include/channel.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    //init_sdb();
    //sdb_mainloop();
    char p1data[2] = {0};
    p1data[0] = 0x3f;
    char p2data[2] = {0};
    p2data[0] = 0x5f;
    Channel c1 = Channel(true);
    Package p1 = Package(p1data,8);
    Package p2 = Package(p2data,8);
    printf("level %d\n",p1.get_level());
    c1.add(p1);
    c1.add(p2);
    Package t = c1.show_top(8);

    printf("level top %d\n",t.get_level());

    return 0;
}
