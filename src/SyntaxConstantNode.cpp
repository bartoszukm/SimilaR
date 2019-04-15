#include "SyntaxConstantNode.h"
#include "NodeProcessorWhile.h"

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


Context SyntaxConstantNode::ProcessWhile(NodeProcessorWhile& processor,
                                         const Context& context)
{
    return processor.ProcessNext(this, context);
}