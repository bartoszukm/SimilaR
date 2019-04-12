#include "SyntaxLangNode.h"

unique_ptr<SyntaxNode> SyntaxLangNode::ConvertLispToSyntaxNode(SEXP s)
{
    unique_ptr<SyntaxNode> node(new SyntaxLangNode());
    node->functionName = getLangName(s);
    
    int n = Rf_length(s)-1;
    if(node->functionName == "function")
    {
        n--;
    }
    node->children = vector<unique_ptr<SyntaxNode>>(n);
    node->argument = vector<string>(n);
    int index = 0;

    SEXP t = CDR(s);
    for (size_t i=0; i<n; ++i)
    {
        if(TAG(t)!=R_NilValue)
        {
            node->argument[index] = CHAR(PRINTNAME(TAG(t)));
        }
        node->children[index] = SyntaxNode::ConvertLispToSyntaxNode(CAR(t));
        node->children[index]->whichChild = index;
        node->children[index++]->parent = node;
        t = CDR(t);
    }
    return node;
}

string SyntaxLangNode::ToString()
{
    string result;
    if(Name == "function") //niestety definicje funkcji musimy potraktowac osobno
    {
        //przykladowo chcemy osiagnac
        //function(a=length(n),b)"{"("fun1"(x),"if"("length"(">"(x,0)),x,"-"(x)),"lapply"(x,function(y)"{"("<-"(z,sin(y)),z)))
        result = "function(";
        if(!(children[0]->type==type_constant && children[0]->Name=="NULL"))
            result += children[0]->ToString();
        result += ")";
        for(size_t i=1;i<children.size();++i)
        {
            result += children[i]->ToString();
            if(i < children.size()-1) //prawdopodobnie niepotrzebne, funkcja ma tylko jedno dziecko poza argumentami - cialo
                result += ",";
        }
    }
    else
    {
        result = "\""+Name+"\"(";
        for(size_t i=0;i<children.size();++i)
        {
            // cout << "argument[ " << i << "]" << endl;
            if(arguments[i]!="")
            {
                ReplaceStringInPlace(arguments[i], "\\","\\\\");
                result += formatName(arguments[i])+"=";
            }
            // cout << "children[ " << i << "]" << endl;
            if(children[i] != nullptr)
                result += children[i]->ToString();
            // cout << "po children[ " << i << "]" << endl;
            if(i < children.size()-1)
                result += ",";
        }
        result += ")";
    }
    
    return result;
}

unique_ptr<SyntaxNode> SyntaxLangNode::Copy()
{
    unique_ptr<SyntaxNode> s(new SyntaxLangNode());
    s->parent = nullptr;
    s->name = name;
    s->whichChild = whichChild;
    s->arguments = arguments;
    return s;

}

void SyntaxLangNode::RepairTree()
{
  for(size_t i=0; i<children.size();i++)
  {
    children[i]->whichChild = i;
    children[i]->parent = this;
    children[i]->RepairTree();
  }
}