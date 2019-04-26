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

    virtual string GetLeftName();

    virtual Context ProcessWhile(NodeProcessorWhile& processor,
                                 const Context& context);
    virtual Context ProcessFor(NodeProcessorFor &processor,
                                         const Context& context);
    virtual Context ProcessForPredicate(NodeProcessorFor& processor,
                                        SyntaxLangNode* forNode,
                                        const Context& context);
    virtual Context ProcessIf(NodeProcessorIf& processor,
                                 const Context& context);
    virtual Context ProcessFunction(NodeProcessorFunction& processor,
                                 const Context& context);
    virtual Context ProcessBrace(NodeProcessorBrace& processor,
                                 const Context& context);
    virtual Context ProcessParenthesis(NodeProcessorParenthesis& processor,
                                 const Context& context);
    virtual Context ProcessCall(NodeProcessorCall& processor,
                                 const Context& context);
    virtual Context ProcessBreak(NodeProcessorBreak& processor,
                                 const Context& context);
    virtual Context ProcessNext(NodeProcessorNext& processor,
                                 const Context& context);
    virtual Context ProcessAssignment(NodeProcessorAssignment& processor,
                                 const Context& context);
    virtual Context ProcessFirstAssignmentChild(NodeProcessorAssignment& processor,
                                              SyntaxLangNode* assignmentNode,
                                              SyntaxNode* right,
                                              const Context& context);
    virtual Context ProcessSecondAssignmentChild(NodeProcessorAssignment& processor,
                                              SyntaxLangNode* assignmentNode,
                                              SyntaxSymbolNode* left,
                                              const Context& context);
    virtual Context ProcessSecondAssignmentChild(NodeProcessorAssignment& processor,
                                              SyntaxLangNode* assignmentNode,
                                              SyntaxLangNode* left,
                                              const Context& context);
};

#endif
