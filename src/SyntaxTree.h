#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include "common.h"
#include "SyntaxNode.h"
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
#include <memory>

using namespace Rcpp;
using namespace boost;
using namespace std;


class SyntaxTree
{
private:
    unique_ptr<SyntaxNode> root;

    // const char* getLangName(SEXP s);
    // SyntaxNode* processFunction(SEXP s);
    // string constantToString(SEXP s);
public:
    // int nr;
    // static int numer;

    SyntaxTree();
    SyntaxTree(SEXP g);
    
    // ~SyntaxTree();

    string ToString();
    void RepairTree();

    list<SyntaxNode*> GetListOfNodes();
    int GetNodesCount();

    unique_ptr<SyntaxTree> Copy();
    string GetFunctionName();
};

#endif