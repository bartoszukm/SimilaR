#ifndef NODEPROCESSORASSIGNMENT_H
#define NODEPROCESSORASSIGNMENT_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxLangNode;

class NodeProcessorAssignment : public NodeProcessor
{
protected:
    void createNode(SyntaxLangNode* callNode, Context& myContext);

public:
// NodeProcessorAssignment();
NodeProcessorAssignment(CDGCreator& cdg);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessAssignment(SyntaxLangNode* n,
                    const Context& context);

};


#endif