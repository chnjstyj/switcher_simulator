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
    char* package;

public :
    Package(char* package,int length);
    char get_destination();
    char get_level();
    char* get_data();
};

#endif //SIMULATOR_PACKAGE_H
