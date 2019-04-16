#ifndef NODEPROCESSORFOR_H
#define NODEPROCESSORFOR_H

#include "common.h"
#include "graphUtils.h"
#include "NodeProcessor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;

class SyntaxLangNode;

class NodeProcessorIf : public NodeProcessor
{
vertex_t node;
string gen;
SyntaxLangNode *forNode;
unique_ptr<NodeProcessor> processor;

void MakeArtificialForeachNodes(const Context& context,
                                Context& myContext,
                                Context& predicateContext);
std::tuple<size_t, size_t> MakeForBody(const Context& context,
                                  Context& myContext,
                                  Context& predicateContext);
void MakeBracketsInForeach(const Context& context,
                           Context& myContext,
                           Context& predicateContext,
                           size_t vertices_count_before,
                           size_t vertices_count_after);

public:
// NodeProcessorIf();
NodeProcessorIf(CDGCreator& cdg);
virtual Context Process(SyntaxNode* s,
                        const Context& context); 

Context ProcessIf(SyntaxLangNode* n,
                    const Context& context);                 
};


#endif