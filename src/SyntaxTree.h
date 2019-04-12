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

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxTree
{
private:
    unique_ptr<SyntaxNode> root;

    const char* getLangName(SEXP s);
    SyntaxNode* processFunction(SEXP s);
    string constantToString(SEXP s);
public:
    // int nr;
    // static int numer;

    SyntaxTree();
    SyntaxTree(SEXP g);
    
    ~SyntaxTree();

    string ToString();
    void RepairTree();

    list<shared_ptr<SyntaxNode>> GetListOfNodes();
    int GetNodesCount();

    unique_ptr<SyntaxTree> Copy();
    string GetFunctionName();
};
