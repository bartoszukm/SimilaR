#ifndef NODEPROCESSORFOR_H
#define NODEPROCESSORFOR_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxLangNode;

class NodeProcessorIf : public NodeProcessor
{
// vertex_t node;
// string gen;
// SyntaxLangNode *ifNode;
// unique_ptr<NodeProcessor> processor;

public:
// NodeProcessorIf();
NodeProcessorIf(CDGCreator& cdg);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessIf(SyntaxLangNode* n,
                    const Context& context);                 
};


#endif