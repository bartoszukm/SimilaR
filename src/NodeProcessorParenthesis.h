#ifndef NODEPROCESSORPARENTHESIS_H
#define NODEPROCESSORPARENTHESIS_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxLangNode;

class NodeProcessorParenthesis : public NodeProcessor
{

public:
// NodeProcessorParenthesis();
NodeProcessorParenthesis(CDGCreator& cdg);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessParenthesis(SyntaxLangNode* n,
                    const Context& context);                 
};


#endif