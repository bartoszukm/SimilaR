#include "SyntaxSymbolNode.h"

unique_ptr<SyntaxNode> SyntaxSymbolNode::ConvertLispToSyntaxNode(SEXP s)
{
    unique_ptr<SyntaxNode> node(new SyntaxSymbolNode());
    node->Name = CHAR(PRINTNAME(s));
    return node;
}

string SyntaxSymbolNode::ToString()
{
    // to jest obsluga przypadku arg.dimnames[, i] <- dimnames(m), chodzi o pierwszy argument w [,i]
    return functionName != "" ? formatName(functionName) : functionName;
}

unique_ptr<SyntaxNode> SyntaxSymbolNode::Copy()
{
    unique_ptr<SyntaxNode> s(new SyntaxSymbolNode());
    s->parent = nullptr;
    s->name = name;
    s->whichChild = whichChild;
    return s;
}