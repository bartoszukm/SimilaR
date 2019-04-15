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


public:
GraphType CreateCDG(SyntaxNode* s); 

unique_ptr<NodeProcessor> GetProcessors();

};


#endif