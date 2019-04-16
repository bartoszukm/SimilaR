#ifndef CONTEXT_H
#define CONTEXT_H

#include "common.h"

struct Context
{
    vertex_t FlowVertex;
    vertex_t ControlVertex;
    list<string> Uses;
};


#endif