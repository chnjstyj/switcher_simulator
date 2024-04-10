//
// Created by tang on 24-3-22.
//

#ifndef SIMULATOR_CHANNEL_H
#define SIMULATOR_CHANNEL_H

#include "package.h"
#include "common.h"


typedef struct package_in_channel
{
    char destination;
    char level;
    char length;
    char pos;
}package_in_channel;

class PQueue
{
public:
    int capacity;
    int size;
    package_in_channel elements[16];
    bool compare(package_in_channel* a,package_in_channel* b);
    PQueue(int cap)
    {
        capacity = cap;
    };
    bool push(package_in_channel p);
    bool pop(package_in_channel* p);

};

class Channel
{
private:
    //package_in_channel channel[16];
    PQueue* queue;
    bool is_input = false;
public:
    Channel(bool input);
    ~Channel();
    bool add(Package p);
    bool add(package_in_channel p);
    bool add(mem_point* mem,int* data,Channel* channels);
    //Package show_top(int length);
    bool output(mem_point *mem,int* package);
    static bool trans(Channel* src,Channel* des);
};

#endif //SIMULATOR_CHANNEL_H
