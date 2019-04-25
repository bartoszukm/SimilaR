#ifndef SYNTAXSYMBOLNODE_H
#define SYNTAXSYMBOLNODE_H

#include "SyntaxNode.h"
#include "common.h"
#include "graphUtils.h"
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
using namespace std;
using namespace boost;

// class SyntaxNode;

class SyntaxSymbolNode : public SyntaxNode
{
public:
    static unique_ptr<SyntaxNode> ConvertLispToSyntaxNode(SEXP s);
    SyntaxSymbolNode() {}
    ~SyntaxSymbolNode() {};

    virtual string ToString();
    virtual unique_ptr<SyntaxNode> Copy();

    virtual Context ProcessSymbolOrConstant(NodeProcessorSymbolOrConstant& processor,
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