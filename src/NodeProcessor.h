#ifndef NODEPROCESSOR_H
#define NODEPROCESSOR_H

#include "common.h"
#include "graphUtils.h"
#include "Context.h"
#include "SyntaxNode.h"
#include <stdlib.h>
#include <stdio.h>
#include <tuple>

using namespace Rcpp;
using namespace std;
using namespace boost;

class CDGCreator;

class NodeProcessor
{
protected:
CDGCreator& CDG;
unique_ptr<NodeProcessor> Next;

string concatenateStringList(const list<string>& l);

public:
// NodeProcessor();
NodeProcessor(CDGCreator& cdg);
virtual Context Process(SyntaxNode* n, const Context& context); 
NodeProcessor* SetNext(NodeProcessor* next);
Context ProcessNext(SyntaxNode* n, const Context& context);

};


#endif