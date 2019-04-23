#include "SyntaxLangNode.h"
#include "NodeProcessorWhile.h"

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
        node->Children[index++]->Parent = node;
        t = CDR(t);
    }
    node->Parent = nullptr;
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
    s->Parent = nullptr;
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


Context SyntaxLangNode::ProcessWhile(NodeProcessorWhile& processor,
                                         const Context& context)
{
    if(Name != "while")
        return processor.ProcessNext(this, context);

    return processor.ProcessWhile(this, context);
}

Context SyntaxLangNode::ProcessFor(NodeProcessorWhile& processor,
                                         const Context& context)
{
    if(Name != "for")
        return processor.ProcessNext(this, context);

    return children[1]->ProcessForPredicate(processor, *this, context);
}

Context SyntaxLangNode::ProcessForPredicate(NodeProcessorFor& processor,
                                        const SyntaxLangNode& forNode,
                                        const Context& context)
{
    //@TODO: Name przepuscic przez getCanonicalName
    if(Name != "seq" && Name != ":" && Name != "seq_len" && Name != "seq_along")
    {
        return processor.ProcessForeach(forNode, context);
    }
    else
    {
        return processor.ProcessFor(forNode, context);
    }
    
}

Context SyntaxLangNode::ProcessIf(NodeProcessorIf& processor,
                                         const Context& context)
{
    if(Name != "if")
        return processor.ProcessNext(this, context);

    return processor.ProcessIf(this, context);
}

Context SyntaxLangNode::ProcessFunction(NodeProcessorFunction& processor,
                                 const Context& context)
{
    if(Name != "function")
        return processor.ProcessNext(this, context);

    return processor.ProcessFunction(this, context);
}

Context SyntaxLangNode::ProcessBrace(NodeProcessorBrace& processor,
                                 const Context& context)
{
    if(Name != "{")
        return processor.ProcessNext(this, context);

    return processor.ProcessBrace(this, context);
}

Context SyntaxLangNode::ProcessParenthesis(NodeProcessorParenthesis& processor,
                                 const Context& context)
{
    if(Name != "(")
        return processor.ProcessNext(this, context);

    return processor.ProcessParenthesis(this, context);
}

Context SyntaxLangNode::ProcessCall(NodeProcessorCall& processor,
                                 const Context& context)
{
    return processor.ProcessCall(this, context);
}

Context ProcessBreak(NodeProcessorCall& processor,
                                 const Context& context)
{
    if(Name != "break")
        return processor.ProcessNext(this, context);

    return processor.ProcessBreak(this, context);
}

Context ProcessNext(NodeProcessorCall& processor,
                                 const Context& context)
{
    if(Name != "next")
        return processor.ProcessNext(this, context);

    return processor.ProcessNextNode(this, context);
}