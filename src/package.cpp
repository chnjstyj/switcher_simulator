#include "include/package.h"

Package::Package(char* source,int length)
{
    package_length = length;
    package = source;
    is_available = true;
}

char Package::get_destination()
{
    if (is_available)
    {
        //int i;
        char result = (package[0] & 0xF);
        return result;
    }
    else
    {
        return -1;
    }
}

char Package::get_level()
{
    if (is_available)
    {
        char result = ((package[0] & 0xF0) >> 4);
        return result;
    }
    else
    {
        return -1;
    }
}

char* Package::get_data()
{
    if (is_available)
    {
        return (package + package_length / 8);
    }

    return 0; // temp: just for avoiding gcc error
}