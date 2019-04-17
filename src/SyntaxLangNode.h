#ifndef SYNTAXLANGNODE_H
#define SYNTAXLANGNODE_H

#include "common.h"
#include "graphUtils.h"
#include "SyntaxNode.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <map>
#include <list>
#include <Rcpp.h>
#include <cwchar>
#include <cstring>

using namespace Rcpp;
using namespace boost;
using namespace std;

class NodeProcessorWhile;

class SyntaxLangNode : public SyntaxNode
{
public:
    vector<unique_ptr<SyntaxNode>> Children;

    //te dwa pola sa potrzebne tylko dla argumentow w definicji funkcji
    //czyli w pairlist, znanej tez jako LISTSXP
    //to pierwsze pole jest potrzebne takze przy wywolaniu funkcji, gdy
    // mamy fun(x=5,y,z=length(x)), przechowujemy tu "x" i "z" na 1 i 3 indeksie
    vector<string> Arguments;

    static unique_ptr<SyntaxNode> ConvertLispToSyntaxNode(SEXP s);
    // SyntaxLangNode() {}
    // ~SyntaxLangNode() {}

    virtual string ToString();
    virtual unique_ptr<SyntaxNode> Copy();
    virtual void RepairTree();

    virtual Context ProcessWhile(NodeProcessorWhile& processor,
                                 const Context& context);
    virtual Context ProcessFor(NodeProcessorWhile& processor,
                                         const Context& context);
    virtual Context ProcessForPredicate(NodeProcessorFor& processor,
                                        const SyntaxLangNode& forNode,
                                        const Context& context);
    virtual Context ProcessIf(NodeProcessorIf& processor,
                                 const Context& context);
    virtual Context ProcessFunction(NodeProcessorFunction& processor,
                                 const Context& context);
    virtual Context ProcessBrace(NodeProcessorBrace& processor,
                                 const Context& context);
    virtual Context ProcessParenthesis(NodeProcessorParenthesis& processor,
                                 const Context& context);
};

#endif