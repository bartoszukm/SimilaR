#ifndef NODEPROCESSORBRACE_H
#define NODEPROCESSORBRACE_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxLangNode;

class NodeProcessorBrace : public NodeProcessor
{
// vertex_t node;
// string gen;
// SyntaxLangNode *forNode;
// unique_ptr<NodeProcessor> processor;

public:
// NodeProcessorFunctionParameters();
NodeProcessorFunctionParameters(CDGCreator& cdg);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessBrace(SyntaxLangNode* n,
                    const Context& context);                 
};


#endif