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
};

#endif