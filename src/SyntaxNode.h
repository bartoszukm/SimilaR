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

class SyntaxNode
{
protected:
    string formatName(Rcpp::String x);
    int isValidName(const char *name);
    const char* getLangName(SEXP s);
    string constantToString(SEXP s);

public:
    weak_ptr<SyntaxNode> Parent;

    string Name;
    int WhichChild;

    static unique_ptr<SyntaxNode> ConvertLispToSyntaxNode(SEXP s);
    SyntaxNode() {}
    ~SyntaxNode();

    virtual string ToString() = 0;

    //polega na ustawieniu odpowiednio parent i whichchild
    virtual void RepairTree();
    virtual unique_ptr<SyntaxNode> Copy() = 0;
};