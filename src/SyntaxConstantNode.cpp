#include "SyntaxConstantNode.h"

unique_ptr<SyntaxNode> SyntaxConstantNode::ConvertLispToSyntaxNode(SEXP s)
{
    unique_ptr<SyntaxNode> node(new SyntaxConstantNode());
    node->Name = constantToString(s);

    if(node->Name == "-2147483648")
        node->Name = "Inf";
    if(node->Name == "nan")
        node->Name = "NaN";
    return node;
}

string SyntaxConstantNode::ToString()
{
    return name;
}

unique_ptr<SyntaxNode> SyntaxConstantNode::Copy()
{
    unique_ptr<SyntaxNode> s(new SyntaxConstantNode());
    s->parent = nullptr;
    s->name = name;
    s->whichChild = whichChild;
    return s;
}