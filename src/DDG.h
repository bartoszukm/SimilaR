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

#include "common.h"

using namespace Rcpp;
using namespace std;
using namespace boost;

class edge_predicate_c {
public:
        edge_predicate_c() : graph_m(0), color_m(0) {
        }
        edge_predicate_c(GraphType& graph, int color) : graph_m(&graph), color_m(color) {
        }
        bool operator() (const edge_t &edge_id) const {
                int type = (*graph_m)[edge_id].color;
                return type==color_m;
        }
private:
        GraphType* graph_m;
        int color_m;
};

typedef filtered_graph<GraphType, edge_predicate_c> FilteredGraphType;
typedef typename graph_traits<GraphType>::out_edge_iterator out_edge_iterator_t;

class DDGMaker
{
private:

struct ddg_visitor : boost::default_bfs_visitor {

        ddg_visitor(FilteredGraphType& gCF, FilteredGraphType& gData, GraphType& g, bool& changes) : graphCF(gCF), graph(g), gData(gData), changes(changes){
        }

        void examine_vertex(const GraphType::vertex_descriptor &u, const FilteredGraphType &g) const {
                boost::graph_traits< FilteredGraphType >::in_edge_iterator e, e_end;


                for (tie(e, e_end) = in_edges(u, graphCF); e != e_end; ++e)
                {

                        for (ddg_map::iterator it=graph[source(*e,graph)].out.begin(); it!=graph[source(*e,graph)].out.end(); ++it)
                        {

                                if(graph[u].in.count(it->first)>0)
                                {
                                        if(graph[u].color == color_header || graph[u].color == color_if)
                                        {


                                                graph[u].in[it->first].insert(it->second.begin(), it->second.end());
                                        }
                                        else
                                        {
                                                graph[u].in[it->first]=it->second;
                                        }
                                }
                                else
                                {
                                        graph[u].in[it->first]=it->second;
                                }
                        }
                }


                ddg_map out = graph[u].in;
                if(graph[u].functionName == "return")
                {
                        out.clear();
                }
                else
                {
                        for (list<string>::iterator it=graph[u].kill.begin(); it!=graph[u].kill.end(); ++it)
                        {
                                out.erase(*it);
                        }
                }
                graph[u].out = out;

                for (list<string>::iterator variableName = graph[u].uses.begin(); variableName != graph[u].uses.end(); variableName++)
                {
                        if(graph[u].in.count(*variableName)>0)
                        {
                                for (set<vertex_t>::iterator v = graph[u].in[*variableName].begin(); v != graph[u].in[*variableName].end(); v++)
                                {


                                        pair<edge_t, bool> eDataDependency;
                                        boost::graph_traits< FilteredGraphType>::out_edge_iterator e_data, e_data_end;
                                        bool isDataEdge = false;
                                        for (tie(e_data, e_data_end) = out_edges(*v,gData); e_data != e_data_end; ++e_data)
                                        {
                                                if(target(*e_data,gData)==u)
                                                {
                                                        isDataEdge = true;
                                                }
                                        }

                                        if(!isDataEdge)
                                        {
                                                eDataDependency = add_edge(*v, u, graph);
                                                graph[eDataDependency.first].color = color_data_dependency;
                                        }
                                }
                        }
                }


                if(graph[u].gen != "")
                {
                        graph[u].out[graphCF[u].gen].insert(u);
                }

                if(graph[u].out != graph[u].out_old)
                {
                        graph[u].out_old = graph[u].out;
                        changes=true;
                }
        }
private:
        FilteredGraphType& graphCF;
        GraphType& graph;
        FilteredGraphType& gData;
        bool& changes;
};

vector<edge_t> makeCacheOfEdges(pair<out_edge_iterator_t,
                                     out_edge_iterator_t> p, GraphType g);
void makeAdditionalDataDependencyEdges(GraphType& g);
public:
void makeDDG_cpp(GraphType& g, const vertex_t& entry);

};
