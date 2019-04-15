#ifndef NODEPROCESSORWHILE_H
#define NODEPROCESSORWHILE_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxLangNode;

class NodeProcessorWhile : public NodeProcessor
{
public:
// NodeProcessorWhile();
NodeProcessorWhile(CDGCreator& cdg);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessWhile(SyntaxLangNode* n,
                    const Context& context);
};


#endif