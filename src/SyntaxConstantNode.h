#ifndef SYNTAXCONSTANTNODE_H
#define SYNTAXCONSTANTNODE_H

#include "SyntaxNode.h"
#include "NodeProcessorWhile.h"
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
using namespace boost;
using namespace std;

class SyntaxNode;

class SyntaxConstantNode : public SyntaxNode
{
public:
    static unique_ptr<SyntaxNode> ConvertLispToSyntaxNode(SEXP s);
    // SyntaxConstantNode() {}
    // ~SyntaxConstantNode();

    virtual string ToString();
    virtual unique_ptr<SyntaxNode> Copy();
};

#endif