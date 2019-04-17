#ifndef NODEPROCESSORCALL_H
#define NODEPROCESSORCALL_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxLangNode;

class NodeProcessorCall : public NodeProcessor
{
protected:
    void createNode(SyntaxLangNode* callNode, Context& myContext);

public:
// NodeProcessorCall();
NodeProcessorCall(CDGCreator& cdg);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessCall(SyntaxLangNode* n,
                    const Context& context);                 
};


#endif