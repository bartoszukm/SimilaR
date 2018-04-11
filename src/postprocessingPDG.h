
#pragma once
#include "common.h"
#include "graphUtils.h"

using namespace Rcpp;
using namespace std;
using namespace boost;

class edge_remover
{
const GraphType& graph;
public:


edge_remover(const GraphType &g) : graph(g){
}
bool operator() (edge_t e) const
{
    if(graph[e].color==color_control_flow)
        return true;
    else
        return false;
}
};

class PostprocessingPDG
{
private:
    static string sideEffectsFunctions[];
public:
    void deleteControlFlowEdges(GraphType& g);
    void changeWhileLoop(GraphType& g);
    bool isSideEffectFunction(string functionName);
    void removeSingleInstructions(GraphType& g);
    void mergeTheSameInstructions(GraphType& g);
    void memoryClean(GraphType& g);
};
