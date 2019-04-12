#include "SyntaxLangNode.h"

unique_ptr<SyntaxNode> SyntaxLangNode::ConvertLispToSyntaxNode(SEXP s)
{
    SyntaxLangNode* node = new SyntaxLangNode();
    
    node->Name = getLangName(s);
    
    int n = Rf_length(s)-1;
    if(node->Name == "function")
    {
        n--;
    }
    node->Children = vector<unique_ptr<SyntaxNode>>(n);
    node->Arguments = vector<string>(n);
    int index = 0;

    SEXP t = CDR(s);
    for (size_t i=0; i<n; ++i)
    {
        if(TAG(t)!=R_NilValue)
        {
            node->Arguments[index] = CHAR(PRINTNAME(TAG(t)));
        }
        node->Children[index] = SyntaxNode::ConvertLispToSyntaxNode(CAR(t));
        node->Children[index]->WhichChild = index;
        node->Children[index++]->Parent.reset(node);
        t = CDR(t);
    }
    return unique_ptr<SyntaxNode>(node);
}

string SyntaxLangNode::ToString()
{
    string result;
    if(Name == "function") //niestety definicje funkcji musimy potraktowac osobno
    {
        //przykladowo chcemy osiagnac
        //function(a=length(n),b)"{"("fun1"(x),"if"("length"(">"(x,0)),x,"-"(x)),"lapply"(x,function(y)"{"("<-"(z,sin(y)),z)))
        result = "function(";
        if(!(Children[0]->Name=="NULL")) //Children[0]->type==type_constant &&
            result += Children[0]->ToString();
        result += ")";
        for(size_t i=1;i<Children.size();++i)
        {
            result += Children[i]->ToString();
            if(i < Children.size()-1) //prawdopodobnie niepotrzebne, funkcja ma tylko jedno dziecko poza argumentami - cialo
                result += ",";
        }
    }
    else
    {
        result = "\""+Name+"\"(";
        for(size_t i=0;i<Children.size();++i)
        {
            // cout << "argument[ " << i << "]" << endl;
            if(Arguments[i]!="")
            {
                ReplaceStringInPlace(Arguments[i], "\\","\\\\");
                result += formatName(Arguments[i])+"=";
            }
            // cout << "Children[ " << i << "]" << endl;
            if(Children[i] != nullptr)
                result += Children[i]->ToString();
            // cout << "po Children[ " << i << "]" << endl;
            if(i < Children.size()-1)
                result += ",";
        }
        result += ")";
    }
    
    return result;
}

unique_ptr<SyntaxNode> SyntaxLangNode::Copy()
{
    SyntaxLangNode* s = new SyntaxLangNode();
    // s->Parent = nullptr;
    s->Name = Name;
    s->WhichChild = WhichChild;
    s->Arguments = Arguments;
    s->Children = vector<unique_ptr<SyntaxNode>>(Children.size());
    for (size_t i = 0; i < Children.size(); i++) {
        s->Children[i] = Children[i]->Copy();
    }
    return unique_ptr<SyntaxNode>(s);

}

void SyntaxLangNode::RepairTree()
{
  for(size_t i=0; i<Children.size();i++)
  {
    Children[i]->WhichChild = i;
    Children[i]->Parent = this;
    Children[i]->RepairTree();
  }
}