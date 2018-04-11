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
#include "DDG.h"


using namespace Rcpp;
using namespace std;
using namespace boost;

void DDGMaker::makeDDG_cpp(GraphType& g, const vertex_t& entry)
{
    FilteredGraphType gCF(g, edge_predicate_c(g, color_control_flow));
    FilteredGraphType gControl(g, edge_predicate_c(g, color_control_dependency));
    FilteredGraphType gData(g, edge_predicate_c(g, color_data_dependency));
    bool changes = false;
    int iterations = 0;
    ddg_visitor vis(gCF,gData,g,changes);
    do
    {
        changes = false;
        breadth_first_search(gControl, entry, visitor(vis));
        iterations++;
    }
    while(changes && iterations < 10);
}


vector<edge_t> DDGMaker::makeCacheOfEdges(pair<out_edge_iterator_t,
                                               out_edge_iterator_t> p,
                                          GraphType g)
{
    std::vector<edge_t> edges_cache;
    out_edge_iterator_t fi, fi_end;
    for(tie(fi,fi_end) = p; fi != fi_end; ++fi)
    {
        edge_t e(*fi);
        edges_cache.push_back(e);
    }
    return edges_cache;
}

void DDGMaker::makeAdditionalDataDependencyEdges(GraphType& g)
{
    bool changes = true;

    while(changes)
    {
        changes = false;
        GraphType::vertex_iterator v, vEnd;

        for (boost::tie(v, vEnd) = boost::vertices(g); v != vEnd; v++)
        {
            vector< edge_t>::iterator e_data, e_data_end;
            vector<edge_t> edges_cache1 = makeCacheOfEdges(out_edges(*v,g), g);

            for (e_data = edges_cache1.begin(); e_data != edges_cache1.end();
                 ++e_data)
            {
                if(g[*e_data].color != color_data_dependency) continue;

                GraphType::vertex_descriptor nei = target(*e_data,g);
                vector< edge_t>::iterator e_data2, e_data_end2;
                vector<edge_t> edges_cache2 = makeCacheOfEdges(out_edges(nei,
                                                                         g), g);
                for (e_data2 = edges_cache2.begin();
                     e_data2 != edges_cache2.end(); ++e_data2)
                {
                    if(g[*e_data2].color != color_data_dependency) continue;
                    vector< edge_t>::iterator e_data3, e_data_end3;
                    vector<edge_t> edges_cache3 =
                        makeCacheOfEdges(out_edges(*v,g), g);
                    bool isDataEdge = false;
                    for (e_data3 = edges_cache3.begin();
                         e_data3 != edges_cache3.end(); ++e_data3)
                    {
                        if(g[*e_data3].color != color_data_dependency) continue;
                        if(target(*e_data3,g)==target(*e_data2,g))
                        {
                            isDataEdge = true;
                        }
                    }
                    if(!isDataEdge)
                    {
                        pair<edge_t, bool> eDataDependency;
                        eDataDependency = add_edge(*v, target(*e_data2,g), g);
                        if(eDataDependency.second)
                        {
                            g[eDataDependency.first].color =
                                color_data_dependency;
                            changes = true;
                        }
                    }
                }

            }
        }
    }
}
