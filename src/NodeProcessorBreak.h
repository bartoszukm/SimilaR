#ifndef NODEPROCESSORBREAK_H
#define NODEPROCESSORBREAK_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxLangNode;

class NodeProcessorBreak : public NodeProcessor
{
protected:
    void createNode(SyntaxLangNode* callNode, Context& myContext);

public:
// NodeProcessorBreak();
NodeProcessorBreak(CDGCreator& cdg);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessBreak(SyntaxNode* n,
                    const Context& context);

};


#endif