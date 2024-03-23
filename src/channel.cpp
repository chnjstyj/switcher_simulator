//
// Created by tang on 24-3-22.
//

#include "include/channel.h"
#include <stddef.h>

void swap_package_in_channel(package_in_channel* a, package_in_channel* b)
{
    package_in_channel temp = *a;
    *a = *b;
    *b = temp;
}

void Channel::add(Package p)
{
    auto x = new package_in_channel();
    x->data = p.get_data();
    x->destination = p.get_destination();
    x->level = p.get_level();
    queue->push(*x);
    delete x;
}

Channel::Channel(bool input)
{
    is_input = input;
    queue = new PQueue(16);
}

Channel::~Channel()
{
    delete queue;
}

bool PQueue::compare(package_in_channel *a, package_in_channel *b)
{
    if (a->level >= b->level) return true;
    else return false;
}

bool PQueue::push(package_in_channel p)
{
    bool r = false;
    int new_node = 0;
    int par_node = 0;

    if (elements != NULL && size < capacity)
    {
        new_node = size;
        par_node = (new_node - 1)/2;
        elements[new_node] = p;
        while (new_node != 0 && !compare(&elements[par_node], &elements[new_node]))
        {
            swap_package_in_channel(&elements[par_node], &elements[new_node]);
            new_node = par_node;
            par_node = (new_node - 1)/2;
        }
        size++;
        r = true;
    }

    return r;
}

bool PQueue::pop(package_in_channel *p)
{
    bool r = false;
    bool l_cond = false;
    bool r_cond = false;
    int pos = 0;
    int l_pos = 0;
    int r_pos = 0;

    if (elements != NULL && size > 0)
    {
        swap_package_in_channel(&elements[pos],&elements[size - 1]);
        size--;
        while (pos < size-1)
        {
            l_pos = pos * 2 + 1;
            r_pos = pos * 2 + 2;

            l_cond = (l_pos < size) ? compare(&elements[pos],&elements[l_pos]) : true;
            r_cond = (r_pos < size) ? compare(&elements[pos],&elements[r_pos]) : true;

            if (l_cond == true && r_cond == true)
            {
                break;
            }
            else if (l_cond == false && r_cond == true)
            {
                swap_package_in_channel(&elements[pos],&elements[l_pos]);
                pos = l_pos;
            }
            else if (r_cond == false && l_cond == true)
            {
                swap_package_in_channel(&elements[pos],&elements[r_pos]);
                pos = r_pos;
            }
            else
            {
                if (compare(&elements[l_pos],&elements[r_pos]) == true)
                {
                    swap_package_in_channel(&elements[pos],&elements[l_pos]);
                    pos = l_pos;
                }
                else
                {
                    swap_package_in_channel(&elements[pos],&elements[r_pos]);
                    pos = r_pos;
                }
            }

        }
        r = true;
    }

    return r;
}

Package Channel::show_top(int length)
{
    if (queue->size != 0)
    {
        auto temp = queue->elements;
        //Package r = Package(temp->data,length);
        char p[2] = {0};
        p[0] = temp->destination | (temp->level << 4);
        Package t = Package(p,length);
        return t;
    }
    else
    {
        Package t = Package(NULL,length);
        return t;
    }
}