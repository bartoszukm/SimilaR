#ifndef NODEPROCESSORWRONG_H
#define NODEPROCESSORWRONG_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;


class NodeProcessorWrong : public NodeProcessor
{
public:
// NodeProcessorWrong();
NodeProcessorWrong(CDGCreator& cdg);
virtual Context Process(SyntaxNode* s, const Context& context); 
};


#endif