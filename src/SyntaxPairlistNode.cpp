#include "SyntaxPairlistNode.h"

unique_ptr<SyntaxNode> SyntaxPairlistNode::ConvertLispToSyntaxNode(SEXP s)
{
    unique_ptr<SyntaxNode> node(new SyntaxPairlistNode());
    SEXP s1 = s;
    int n = Rf_length(s1);

    node->arguments = vector<string>(n);
    node->defaultValues = vector<string>(n);

    int index=0;
    for (; s1 != R_NilValue; s1 = CDR(s1))
    {
        node->argument[index] = CHAR(PRINTNAME(TAG(s1)));
        node->defaultValue[index++] = constantToString(CAR(s1));
    }
    return node;
}

string SyntaxPairlistNode::ToString()
{
    string result;
    for(size_t i=0;i<arguments.size();++i)
    {
        ReplaceStringInPlace(arguments[i], "\\","\\\\");
        result += formatName(arguments[i]);

        if(defaultValues[i] != "")
        {
            result += "="+defaultValues[i];
        }
        if(i < arguments.size()-1)
            result += ",";
    }
    return result;
}

unique_ptr<SyntaxNode> SyntaxPairlistNode::Copy()
{
    unique_ptr<SyntaxNode> s(new SyntaxPairlistNode());
    s->parent = nullptr;
    s->name = name;
    s->whichChild = whichChild;
    s->arguments = arguments;
    s->defaultValues = defaultValues;
    return s;

}