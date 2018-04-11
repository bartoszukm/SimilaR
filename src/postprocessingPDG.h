/*    SimilaR package for R
 *    Copyright (C) 2018 M. Bartoszuk, M. Gagolewski
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

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
