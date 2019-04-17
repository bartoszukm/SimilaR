#ifndef NODEPROCESSORFUNCTION_H
#define NODEPROCESSORFUNCTION_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxLangNode;

class NodeProcessorFunction : public NodeProcessor
{
// vertex_t node;
// string gen;
// SyntaxLangNode *forNode;
// unique_ptr<NodeProcessor> processor;
vertex_t* entry;

public:
// NodeProcessorFunction();
NodeProcessorFunction(CDGCreator& cdg, vertex_t* entry = nullptr);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessFunction(SyntaxLangNode* n,
                    const Context& context);                 
};


#endif