#ifndef CDGCREATOR_H
#define CDGCREATOR_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include "SyntaxNode.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;


class CDGCreator
{
private:
GraphType g;
map<string, string> variableName2variableName;
int globalCallNumber;

public:
GraphType CreateCDG(SyntaxNode* s); 

unique_ptr<NodeProcessor> GetProcessors(bool isLastInstruction=false, bool onlyAddToUses=true, string gen="");
GraphType& GetGraph();
map<string, string>& GetAliasesDictionary();
string GetCanonicalName(string s);
int& GetGlobalCallNumber();


};


#endif
