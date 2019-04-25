#include "SyntaxSymbolNode.h"
#include "NodeProcessorWhile.h"

unique_ptr<SyntaxNode> SyntaxSymbolNode::ConvertLispToSyntaxNode(SEXP s)
{
    SyntaxSymbolNode* node = new SyntaxSymbolNode();
    node->Name = CHAR(PRINTNAME(s));
    node->Parent = nullptr;
    return unique_ptr<SyntaxNode>(node);
}

string SyntaxSymbolNode::ToString()
{
    // to jest obsluga przypadku arg.dimnames[, i] <- dimnames(m), chodzi o pierwszy argument w [,i]
    return Name != "" ? formatName(Name) : Name;
}

unique_ptr<SyntaxNode> SyntaxSymbolNode::Copy()
{
    SyntaxSymbolNode* s = new SyntaxSymbolNode();
    s->Parent = nullptr;
    s->Name = Name;
    s->WhichChild = WhichChild;
    return unique_ptr<SyntaxNode>(s);
}

Context SyntaxSymbolNode::ProcessSymbolOrConstant(
    NodeProcessorSymbolOrConstant& processor,
    const Context& context)
{
    return processor.ProcessSymbol(this, context);
}

Context SyntaxSymbolNode::ProcessFirstAssignmentChild(NodeProcessorAssignment& processor,
                                              SyntaxLangNode* assignmentNode,
                                              SyntaxNode* right,
                                              const Context& context)
{
    return right->ProcessSecondAssignmentChild(processor, assignmentNode, this, context);
}

Context SyntaxSymbolNode::ProcessSecondAssignmentChild(NodeProcessorAssignment& processor,
                                              SyntaxLangNode* assignmentNode,
                                              SyntaxSymbolNode* left,
                                              const Context& context)
{
    return processor.MakeAlias(assignmentNode, left, this, context);
}

Context SyntaxSymbolNode::ProcessSecondAssignmentChild(NodeProcessorAssignment& processor,
                                              SyntaxLangNode* assignmentNode,
                                              SyntaxLangNode* left,
                                              const Context& context)
{
    return processor.MakeLeftCallAndAssignmentVertex(assignmentNode, left, this, context);
}