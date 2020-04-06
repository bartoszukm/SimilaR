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

#include <iostream>
#include <Rcpp.h>
#define USE_RINTERNALS
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>
#include <Rdefines.h>
#include <R_ext/Rdynload.h>
#include <string>
#include <algorithm>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/filtered_graph.hpp>


#ifdef _OPENMP
#include <omp.h>
#endif

using namespace Rcpp;
using namespace std;
using namespace boost;

#ifdef _OPENMP
#define MASTER_OR_SINGLE_THREAD (omp_get_thread_num() == 0)
#else
#define MASTER_OR_SINGLE_THREAD (1)
#endif

struct Vertex;
struct Edge;
struct GraphPro;

typedef boost::adjacency_list<vecS, vecS, bidirectionalS,
                              // vertex properties
                              Vertex,
                              // edge properties
                              Edge,
                              GraphPro>  GraphType;

enum vertices_colors {
    color_header, color_next, color_break,
    color_if, color_if_part, color_assignment, color_parameter, color_entry,
    color_oneBracketSingle, color_oneBracketDouble, color_twoBrackets,
    color_oneBracketTripleOrMore, color_dollar,
    color_functionZeroArgument, color_functionOneArgument,
    color_functionTwoArguments, color_functionThreeArguments,
    color_functionFourOrMoreArguments,
    color_stopifnot,
    color_logicalOperator, color_arithmeticOperator, color_comparisonOperator,
    color_return,
    color_colon,
    color_symbol,
    color_constant,
    verticesColorsNumber
};

enum edges_colors {
    color_control_dependency, color_data_dependency, color_control_flow,
    edgesColorsNumber
};

typedef boost::graph_traits<GraphType>::vertex_descriptor vertex_t;
typedef boost::graph_traits<GraphType>::edge_descriptor edge_t;
typedef map<string, set<vertex_t> > ddg_map;

struct Vertex
{
    //http://stackoverflow.com/questions/11336723/bgl-dijkstra-shortest-path-algorithm-method-does-not-accept-my-color-map-exterio/19717509#19717509
    unsigned int id;     //bedzie potrzebne, jak bedziemy chcieli uzyc tej struktury do wbudowanej funkcji do znajdywania izomorfizmow
    int color;
    string name;
    string gen;

    list<string> gens;
    list<string> kill;
    list<string> uses;
    list<string> arguments;
    ddg_map in;
    ddg_map out;
    ddg_map out_old;
    int functionPosition;     // potrzebne dla apply, ktory argument jest funkcja

    bool lastInstruction;
    bool isLeftSideOfAssign;
    string functionName;
    string originalFunctionName;
    bool isLeftAssign; // czy dane wywolanie funkcji jest przypisywane do prawdziwej zmiennej
                       // potrzebne dla graph2code, aby wywolania funkcji byly jak wczesniej
                       // czy tez moze do zmiennej sztucznej, utworzonej w grafie (gdy false)
                       // pamietac nalezy, ze false moze oznaczac, ze wywolanie funkcji jest
                       // ostatnia funkcja kodzie i zwraca wartosc

    // unsigned long long label; // for WL kernel
    // unsigned long long newlabel; // for WL kernel
    double label;     // for WL kernel
    double newlabel;     // for WL kernel
    double weightLabel;
    double newWeightLabel;

    unsigned long long depth;     // for WL2 kernel
    double importance;
    double importanceNormalized;
    bool isImportant;

    struct labelWithImportance
    {
        double label;
        double importance;


        bool operator<(const Vertex::labelWithImportance &b) const
        { return label < b.label; }

    };

    vector<labelWithImportance> labels;

    bool isPaired;
    bool importanceAssigned;
    
    Vertex()
    {
      isImportant = false;
      isLeftAssign = false;
      isPaired = false;
      importanceAssigned = false;
      lastInstruction = false;
      isLeftSideOfAssign = false;
      
    }
};

struct Edge {

  int color;

  Edge()
  {
    color = 0;
  }

};

struct GraphPro
{
    vector<int> verticesHistogram;
    int verticesSum;
    vector<int> edgesHistogram;
    int edgesSum;
    double importanceSum;
    map<string, string> variableName2variableName;
};
