#include "SyntaxPairlistNode.h"
#include "NodeProcessorWhile.h"

unique_ptr<SyntaxNode> SyntaxPairlistNode::ConvertLispToSyntaxNode(SEXP s)
{
    SyntaxPairlistNode* node = new SyntaxPairlistNode();
    SEXP s1 = s;
    int n = Rf_length(s1);

    node->Arguments = vector<string>(n);
    node->DefaultValues = vector<string>(n);

    int index=0;
    for (; s1 != R_NilValue; s1 = CDR(s1))
    {
        node->Arguments[index] = CHAR(PRINTNAME(TAG(s1)));
        node->DefaultValues[index++] = constantToString(CAR(s1));
    }
    node->Parent = nullptr;
    return unique_ptr<SyntaxNode>(node);
}

string SyntaxPairlistNode::ToString()
{
    string result;
    for(size_t i=0;i<Arguments.size();++i)
    {
        ReplaceStringInPlace(Arguments[i], "\\","\\\\");
        result += formatName(Arguments[i]);

        if(DefaultValues[i] != "")
        {
            result += "="+DefaultValues[i];
        }
        if(i < Arguments.size()-1)
            result += ",";
    }
    return result;
}

unique_ptr<SyntaxNode> SyntaxPairlistNode::Copy()
{
    SyntaxPairlistNode* s = new SyntaxPairlistNode();
    s->Parent = nullptr;
    s->Name = Name;
    s->WhichChild = WhichChild;
    s->Arguments = Arguments;
    s->DefaultValues = DefaultValues;
    return unique_ptr<SyntaxNode>(s);

}

Context SyntaxPairlistNode::ProcessWhile(NodeProcessorWhile& processor,
                                         const Context& context)
{
    return processor.ProcessNext(this, context);
}