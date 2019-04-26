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

class SyntaxLangNode;
class SyntaxSymbolNode;
class NodeProcessorWhile;
class NodeProcessorFor;
class NodeProcessorIf;
class NodeProcessorFunction;
class NodeProcessorFunctionParameters;
class NodeProcessorBrace;
class NodeProcessorParenthesis;
class NodeProcessorSymbolOrConstant;
class NodeProcessorCall;
class NodeProcessorBreak;
class NodeProcessorNext;
class NodeProcessorAssignment;



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

    virtual string GetLeftName();
    virtual bool IsReturnBranch(size_t &branchSize);


    virtual Context ProcessWhile(NodeProcessorWhile& processor,
                                 const Context& context);
    virtual Context ProcessFor(NodeProcessorFor& processor,
                                 const Context& context);
    virtual Context ProcessForPredicate(NodeProcessorFor& processor,
                                        SyntaxLangNode* forNode,
                                        const Context& context);
    virtual Context ProcessIf(NodeProcessorIf& processor,
                                 const Context& context);
    virtual Context ProcessFunction(NodeProcessorFunction& processor,
                                 const Context& context);
    virtual Context ProcessFunctionParameters(NodeProcessorFunctionParameters& processor,
                                 const Context& context);
    virtual Context ProcessBrace(NodeProcessorBrace& processor,
                                 const Context& context);
    virtual Context ProcessParenthesis(NodeProcessorParenthesis& processor,
                                 const Context& context);
    virtual Context ProcessCall(NodeProcessorCall& processor,
                                 const Context& context);
    virtual Context ProcessSymbolOrConstant(NodeProcessorSymbolOrConstant& processor,
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
