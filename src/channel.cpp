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

bool Channel::add(Package p)
{
    /*
    auto x = new package_in_channel();
    x->data = p.get_data();
    x->destination = p.get_destination();
    x->level = p.get_level();
    bool result = queue->push(*x);
    delete x;*/
    return false;
}

bool Channel::add(package_in_channel p)
{
    bool result = queue->push(p);
    return result;
}

int find_next_space(mem_point* mem)
{
    int i = 0;
    for (i = 0; i < mem_size; i ++)
    {
        if (mem[i].is_used == false)
            break;
    }
    return i;
}

bool Channel::add(mem_point *mem, int data[],Channel* channels)
{
    int i = find_next_space(mem);
    if (i == mem_size)
        return false;
    int j = i;
    int k = 0;
    int length = Package::s_get_data(data[0]);
    bool overflow = false;
    int temp_j;
    char des = Package::s_get_destination(data[0]);
    char level = Package::s_get_level(data[0]);
    package_in_channel p = {des,level,(char)length,(char)i};
    for (; k < length; k++)
    {
        if (j < mem_size)
        {
            (mem+j) -> is_used = true;
            (mem+j) -> data = data[k];
            j++;
            temp_j = j;
            if (k != mem_size && (mem+j) -> is_used == true && j != mem_size)
            {
                //not full
                j = find_next_space(mem);
                if (j == mem_size)
                {
                    overflow = true;
                    break;
                }
                else
                {
                    (mem+temp_j) -> next = (mem+j);
                }
            }
            else if (k != mem_size && (mem+j) -> is_used == false && j != mem_size)
            {
                (mem+j-1) -> next = (mem+j);
            }
            else
            {
                overflow = true;
            }

        }
        else
        {
            overflow = true;
            break;
        }
    }
    if (overflow)
    {
        while ((mem+i) -> next != NULL)
        {
            i = ((mem+i) -> next) - (mem+i) + i;
            (mem+i) -> next = NULL;
        }
        return false;
    }
    else
    {
        (mem + j - 1)->next = NULL;
        (channels + des) -> queue->push(p);
        return true;
    }

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
    if (a->level >= b->level) return false;
    else return true;
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
        p->level = elements[size - 1].level;
        p->destination = elements[size - 1].destination;
        p->length = elements[size -1].length;
        p->pos = elements[size -1].pos;
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

bool Channel::output(mem_point *mem,int package[])
{
    if (queue->size == 0)
        return false;
    package_in_channel p = {0};
    if (queue->pop(&p))
    {
        int data[32] = {0};
        mem_point* i = mem + p.pos;
        mem_point* temp_i = NULL;
        int j = 0;
        if (i->is_used == true)
        {
            do
            {
                data[j] = i -> data;
                j++;
                i = i -> next;
            } while (i != NULL);
            for (j = 0; j < 32; j++)
            {
                package[j] = data[j];
            }
            i = mem + p.pos;
            while (i -> next != NULL)
            {
                temp_i = i -> next;
                i -> next = NULL;
                i -> is_used = false;
                i = temp_i ;
            }
            i -> is_used = false;
        }
    }
    else
    {
        package = NULL;
        return false;
    }
    return true;
}

bool Channel::trans(Channel *src, Channel *des)
{
    bool r = false;
    /*
    if (src->is_input == false || des->is_input == true)
    {
        return r;
    }
    package_in_channel temp = {0};
    if(src->output(&temp))
    {
        if(des->add(temp))
        {
            r = true;
        }
    }*/
    return r;
}