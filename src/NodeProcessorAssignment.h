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
                        NodeProcessorAssignment& processor,
                        const Context& context); 

Context ProcessAssignment(SyntaxLangNode* n,
                    const Context& context);

Context MakeAssignmentVertex(string leftVariableName, const Context& context);
Context TryAddAlias(string left, string right);


Context MakeAssignmentVertex(SyntaxLangNode* assignmentNode,
                             SyntaxSymbolNode* left,
                             SyntaxConstantNode* right,
                             const Context& context
                             );

Context MakeLeftCallAndAssignmentVertex(SyntaxLangNode* assignmentNode,
                             SyntaxLangNode* left,
                             SyntaxConstantNode* right,
                             const Context& context
                             );

Context MakeLeftCallAndAssignmentVertex(SyntaxLangNode* assignmentNode,
                             SyntaxLangNode* left,
                             SyntaxSymbolNode* right,
                             const Context& context
                             );

Context MakeAlias(SyntaxLangNode* assignmentNode,
                             SyntaxSymbolNode* left,
                             SyntaxSymbolNode* right,
                             const Context& context
                             );

Context MakeAliasAndRightCall(SyntaxLangNode* assignmentNode,
                             SyntaxSymbolNode* left,
                             SyntaxLangNode* right,
                             const Context& context
                             );

Context MakeRightCall(SyntaxLangNode* assignmentNode,
                             SyntaxSymbolNode* left,
                             SyntaxLangNode* right,
                             const Context& context
                             );

Context MakeLeftCallRightCallAssignmentVertex(SyntaxLangNode* assignmentNode,
                             SyntaxLangNode* left,
                             SyntaxLangNode* right,
                             const Context& context
                             );


};


#endif