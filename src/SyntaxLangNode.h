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
};

#endif