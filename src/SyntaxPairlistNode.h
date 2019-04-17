#ifndef SYNTAXPAIRLISTNODE_H
#define SYNTAXPAIRLISTNODE_H

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
using namespace boost;
using namespace std;

class SyntaxPairlistNode : public SyntaxNode
{
public:
    //te dwa pola sa potrzebne tylko dla argumentow w definicji funkcji
    //czyli w pairlist, znanej tez jako LISTSXP
    //to pierwsze pole jest potrzebne takze przy wywolaniu funkcji, gdy
    // mamy fun(x=5,y,z=length(x)), przechowujemy tu "x" i "z" na 1 i 3 indeksie
    vector<string> Arguments;
    vector<string> DefaultValues;

    static unique_ptr<SyntaxNode> ConvertLispToSyntaxNode(SEXP s);
    // SyntaxPairlistNode() {}
    // ~SyntaxPairlistNode() {}

    virtual string ToString();
    virtual unique_ptr<SyntaxNode> Copy();

    virtual Context ProcessFunctionParameters(NodeProcessorFunctionParameters& processor,
                                 const Context& context);
};

#endif