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
    string gen;
    bool isLastInstruction;
    void createNode(SyntaxLangNode* callNode, Context& myContext);

public:
// NodeProcessorCall();
NodeProcessorCall(CDGCreator& cdg, string gen, bool isLastInstruction);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessCall(SyntaxLangNode* n,
                    const Context& context);                 
};


#endif
