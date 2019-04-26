#ifndef NODEPROCESSORSYMBOLORCONSTANT_H
#define NODEPROCESSORSYMBOLORCONSTANT_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxLangNode;

class NodeProcessorSymbolOrConstant : public NodeProcessor
{
protected:
    void createNode(SyntaxLangNode* callNode, Context& myContext);
    bool onlyAddToUses;
    bool isLastInstruction;

public:
// NodeProcessorSymbolOrConstant();
NodeProcessorSymbolOrConstant(CDGCreator& cdg, bool onlyAddToUses, bool isLastInstruction);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessSymbol(SyntaxNode* n,
                    const Context& context);

Context ProcessConstant(SyntaxNode* n,
                    const Context& context);                 
};


#endif