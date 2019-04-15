#ifndef SYNTAXNODE_H
#define SYNTAXNODE_H


#include "common.h"
#include "graphUtils.h"
#include "boost/lexical_cast.hpp"
#include "Context.h"
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

class NodeProcessorWhile;

class SyntaxNode
{
protected:
    static string formatName(Rcpp::String x);
    static int isValidName(const char *name);
    static const char* getLangName(SEXP s);
    static string constantToString(SEXP s);
    static void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace);

public:
    SyntaxNode* Parent;

    string Name;
    int WhichChild;

    static unique_ptr<SyntaxNode> ConvertLispToSyntaxNode(SEXP s);
    // SyntaxNode() {}
    // ~SyntaxNode();

    virtual string ToString() = 0;

    //polega na ustawieniu odpowiednio parent i whichchild
    virtual void RepairTree();
    virtual unique_ptr<SyntaxNode> Copy() = 0;


    virtual Context ProcessWhile(NodeProcessorWhile& processor,
                                 const Context& context) = 0;
};

#endif