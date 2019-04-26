#ifndef NODEPROCESSORNEXT_H
#define NODEPROCESSORNEXT_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxLangNode;

class NodeProcessorNext : public NodeProcessor
{
protected:
    void createNode(SyntaxLangNode* callNode, Context& myContext);

public:
// NodeProcessorNext();
NodeProcessorNext(CDGCreator& cdg);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessNextNode(SyntaxNode* n,
                    const Context& context);

};


#endif