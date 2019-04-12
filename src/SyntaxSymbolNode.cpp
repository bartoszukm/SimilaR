#include "SyntaxSymbolNode.h"

unique_ptr<SyntaxNode> SyntaxSymbolNode::ConvertLispToSyntaxNode(SEXP s)
{
    SyntaxSymbolNode* node = new SyntaxSymbolNode();
    node->Name = CHAR(PRINTNAME(s));
    return unique_ptr<SyntaxNode>(node);
}

string SyntaxSymbolNode::ToString()
{
    // to jest obsluga przypadku arg.dimnames[, i] <- dimnames(m), chodzi o pierwszy argument w [,i]
    return functionName != "" ? formatName(functionName) : functionName;
}

unique_ptr<SyntaxNode> SyntaxSymbolNode::Copy()
{
    SyntaxSymbolNode* s = new SyntaxSymbolNode();
    s->Parent = nullptr;
    s->Name = Name;
    s->WhichChild = WhichChild;
    return unique_ptr<SyntaxNode>(s);
}