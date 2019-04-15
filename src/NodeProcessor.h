#ifndef NODEPROCESSOR_H
#define NODEPROCESSOR_H

#include "common.h"
#include "graphUtils.h"
#include "Context.h"
#include "SyntaxNode.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class CDGCreator;

class NodeProcessor
{
private:
CDGCreator& CDG;
unique_ptr<NodeProcessor> Next;

public:
// NodeProcessor();
NodeProcessor(CDGCreator& cdg);
virtual Context Process(SyntaxNode* n, const Context& context); 
NodeProcessor* SetNext(NodeProcessor* next);
Context ProcessNext(SyntaxNode* n, const Context& context);

};


#endif