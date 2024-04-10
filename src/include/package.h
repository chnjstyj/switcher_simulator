//
// Created by tang on 24-3-22.
//

#ifndef SIMULATOR_PACKAGE_H
#define SIMULATOR_PACKAGE_H

class Package
{
private :
    int package_length;
    bool is_available;
    int* package;

public :
    Package(int* package,int length);
    char get_destination();
    char get_level();
    int* get_data();
    char get_length();
    static char s_get_data(int data_c);
    static char s_get_destination(int data_c);
    static char s_get_level(int data_c);
    // length level des
};

#endif //SIMULATOR_PACKAGE_H
