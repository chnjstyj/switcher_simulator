#include "include/package.h"

Package::Package(int* source,int length)
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
        char result = ((package[0] & 0x70) >> 4);
        return result;
    }
    else
    {
        return -1;
    }
}

char Package::get_length()
{
    if (is_available)
    {
        char result = ((package[0] & 0xFF00) >> 8);
        return result;
    }
    else
    {
        return -1;
    }
}

char Package::s_get_data(int data_c)
{
    char result = ((data_c & 0xFF00) >> 8);
    return result;
}

char Package::s_get_destination(int data_c)
{
    char result = (data_c & 0xF);
    return result;
}

char Package::s_get_level(int data_c)
{
    char result = ((data_c & 0x70) >> 4);
    return result;
}

int* Package::get_data()
{
    if (is_available)
    {
        return (package + 1);
    }

    return 0; // temp: just for avoiding gcc error
}