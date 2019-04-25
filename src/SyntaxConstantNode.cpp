#include "SyntaxConstantNode.h"
#include "NodeProcessorWhile.h"
#include "NodeProcessorSymbolOrConstant.h"

unique_ptr<SyntaxNode> SyntaxConstantNode::ConvertLispToSyntaxNode(SEXP s)
{
    SyntaxConstantNode* node = new SyntaxConstantNode();
    node->Name = SyntaxNode::constantToString(s);
    node->Parent = nullptr;
    
    if(node->Name == "-2147483648")
        node->Name = "Inf";
    if(node->Name == "nan")
        node->Name = "NaN";
    return unique_ptr<SyntaxNode>(node);
}

string SyntaxConstantNode::ToString()
{
    return Name;
}

unique_ptr<SyntaxNode> SyntaxConstantNode::Copy()
{
    SyntaxConstantNode* s = new SyntaxConstantNode();
    s->Parent = nullptr;
    s->Name = Name;
    s->WhichChild = WhichChild;
    return unique_ptr<SyntaxNode>(s);
}

Context SyntaxConstantNode::SyntaxSymbolNode::ProcessSymbolOrConstant(
    NodeProcessorSymbolOrConstant& processor,
    const Context& context)
{
    return processor->ProcessConstant(this, context);
}

Context SyntaxConstantNode::ProcessSecondAssignmentChild(NodeProcessorAssignment& processor,
                                              SyntaxLangNode* assignmentNode,
                                              SyntaxSymbolNode* left,
                                              const Context& context)
{
    return processor.MakeAssignmentVertex(assignmentNode, left, this, context);
}

Context SyntaxConstantNode::ProcessSecondAssignmentChild(NodeProcessorAssignment& processor,
                                              SyntaxLangNode* assignmentNode,
                                              SyntaxLangNode* left,
                                              const Context& context)
{
    return processor.MakeLeftCallAndAssignmentVertex(assignmentNode, left, this, context);
}