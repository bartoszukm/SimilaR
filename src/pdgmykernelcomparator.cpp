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

#include "pdgmykernelcomparator.h"

using namespace Rcpp;
using namespace std;
using namespace boost;

void PDGMyKernelComparator::calculateResultsForOneIteration(
    double& len1,
    double& len2, double& mul, double& mul_sym)
{
    for(size_t i = 0; i<phi1.size(); ++i)
    {
        mul += min(phi1[i],phi2[i]);
        mul_sym += abs((double)phi1[i]-(double)phi2[i]);
        len1 += phi1[i];
        len2 += phi2[i];
    }

}

void PDGMyKernelComparator::computeLabels(
    GraphType& g,
    unsigned long long maxDepth,
    map<unsigned long long,
        list<vertex_t> >& gVerticesByDepth
    )
{
    bool ok = true;
    boost::graph_traits<GraphType>::out_edge_iterator out_e, out_e_end;
    GraphType::vertex_iterator it, itEnd;
    for(unsigned long long depth = maxDepth; ok; depth--)
    {
        const list<vertex_t>& verticesOnDepth = gVerticesByDepth.at(depth);
        for(vertex_t v : verticesOnDepth)
        {
            g[v].labels = vector<Vertex::labelWithImportance>(out_degree(v,g)+1);
            g[v].labels[0].label = g[v].label;
            g[v].labels[0].importance = g[v].importance;

            size_t i = 1;
            for (tie(out_e, out_e_end) = out_edges(v, g); out_e != out_e_end;
                 ++out_e)
            {
                vertex_t nei = target(*out_e,g);
                if(g[*out_e].color == color_data_dependency)
                {
                    g[v].labels[i].label = last_label_counter + g[nei].label;
                    g[v].labels[i++].importance = g[nei].importance;
                }
                else
                {
                    g[v].labels[i].label = g[nei].label;
                    g[v].labels[i++].importance = g[nei].importance;
                }
            }



            std::sort(g[v].labels.begin()+1, g[v].labels.end());
        }

        if(depth-1 > depth)
            ok = false;
    }
}

void PDGMyKernelComparator::compareTwoVertices(
    GraphType& g1,
    GraphType& g2,
    vertex_t v1,
    vertex_t v2,
    double median1,
    double median2,
    double penalty)
{
    bool paired = false;
    double difference1 = 0;
    double difference2 = 0;

    RCOUT(
        "                g1[v1].label = " << g1[v1].label<<
        ", g2[v2].label = " <<
        g2[v2].label, KERNELSUBTREE_VERBOSE);


    if(g1[v1].label != g2[v2].label)
        return;


    size_t index1 = 1;
    size_t index2 = 1;

#if VERBOSE >= KERNELSUBTREE_VERBOSE
    Rcout << "depth 1 = " << g1[v1].depth << endl;
    Rcout << "depth 2 = " << g2[v2].depth << endl;
    for(size_t i = 0; i<g1[v1].labels.size(); ++i)
    {
        Rcout << g1[v1].labels[i].label << ",";
    }
    Rcout << endl;
    for(size_t i = 0; i<g2[v2].labels.size(); ++i)
    {
        Rcout << g2[v2].labels[i].label << ",";
    }
    Rcout << endl;
#endif
    int howManyMissed = 0;
    while(index1 < g1[v1].labels.size() && index2 < g2[v2].labels.size())
    {
        if((int)g1[v1].labels[index1].label == (int)g2[v2].labels[index2].label)
        {
            index1++;
            index2++;
        }
        else if((int)g1[v1].labels[index1].label <
                (int)g2[v2].labels[index2].label)
        {
            difference1 += penalty * g1[v1].labels[index1].importance/
                           g1[graph_bundle].importanceSum;
            difference2 += penalty * g1[v1].labels[index1].importance/
                           g2[graph_bundle].importanceSum;
            index1++;
            howManyMissed++;
        }
        else
        {
            difference1 += penalty * g2[v2].labels[index2].importance/
                           g1[graph_bundle].importanceSum;
            difference2 += penalty * g2[v2].labels[index2].importance/
                           g2[graph_bundle].importanceSum;
            index2++;
            howManyMissed++;
        }

    }

    while(index1 < g1[v1].labels.size())
    {
        difference1 += penalty * g1[v1].labels[index1].importance/
                       g1[graph_bundle].importanceSum;
        difference2 += penalty * g1[v1].labels[index1].importance/
                       g2[graph_bundle].importanceSum;
        index1++;
        howManyMissed++;
    }

    while(index2 < g2[v2].labels.size())
    {
        difference1 += penalty * g2[v2].labels[index2].importance/
                       g1[graph_bundle].importanceSum;
        difference2 += penalty * g2[v2].labels[index2].importance/
                       g2[graph_bundle].importanceSum;
        index2++;
        howManyMissed++;
    }

    difference1 *= howManyMissed;
    difference2 *= howManyMissed;

    RCOUT("                difference1 = " << difference1,
          KERNELSUBTREE_VERBOSE);
    RCOUT("                difference2 = " << difference2,
          KERNELSUBTREE_VERBOSE);
    if(difference1 < median1 && difference2 < median2)
        paired = true;

    if(paired)
    {
        RCOUT("                paired = true", KERNELSUBTREE_VERBOSE);
        if(g2[v2].isPaired)
        {
            if(!g1[v1].isPaired)
            {
                RCOUT("                case 1", KERNELSUBTREE_VERBOSE);
                g1[v1].isPaired =  true;
                g1[v1].newlabel = g2[v2].newlabel;
            }
            else
            {
                if(g1[v1].newlabel != g2[v2].newlabel)
                {
                    RCOUT("                case 0", KERNELSUBTREE_VERBOSE);
                    GraphType::vertex_iterator it, itEnd;
                    for (boost::tie(it, itEnd) = boost::vertices(g1);
                         it != itEnd; it++)
                    {
                        if(g1[*it].newlabel == g1[v1].newlabel && *it != v1)
                        {
                            g1[*it].newlabel = g2[v2].newlabel;
                        }
                    }

                    for (boost::tie(it, itEnd) = boost::vertices(g2);
                         it != itEnd; it++)
                    {
                        if(g2[*it].newlabel == g1[v1].newlabel)
                        {
                            g2[*it].newlabel = g2[v2].newlabel;
                        }
                    }
                    g1[v1].newlabel = g2[v2].newlabel;
                }
            }
        }
        else if(!g2[v2].isPaired)
        {
            if(g1[v1].isPaired)
            {
                RCOUT("                case 2", KERNELSUBTREE_VERBOSE);
                g2[v2].newlabel = g1[v1].newlabel;
            }
            else
            {
                RCOUT("                case 3", KERNELSUBTREE_VERBOSE);
                g1[v1].isPaired =  true;
                g1[v1].newlabel = g2[v2].newlabel = label_counter++;
            }
            g2[v2].isPaired =  true;
        }
        RCOUT("                g1[v1].newlabel = " << g1[v1].newlabel,
              KERNELSUBTREE_VERBOSE);
        RCOUT("                g2[v2].newlabel = " << g2[v2].newlabel,
              KERNELSUBTREE_VERBOSE);
    }
}

void PDGMyKernelComparator::calculatePhi(GraphType& g,
                                         vector<unsigned long long>& phi)
{
    GraphType::vertex_iterator it, itEnd;

    for (boost::tie(it, itEnd) = boost::vertices(g); it != itEnd; it++)
    {
        phi[g[*it].newlabel]++;
    }
}

void PDGMyKernelComparator::compareVerticesWithinTwoGraphs(
    GraphType& g1,
    GraphType& g2
    )
{
    bool ok = true;
    for(unsigned long long depth = max(maxDepthG1, maxDepthG2); ok; depth--)
    {
        if(g1VerticesByDepth.find(depth) == g1VerticesByDepth.end() ||
           g2VerticesByDepth.find(depth) == g2VerticesByDepth.end())
            continue;
        const list<vertex_t>& verticesOnDepthG1 = g1VerticesByDepth.at(depth);
        const list<vertex_t>& verticesOnDepthG2 = g2VerticesByDepth.at(depth);
        for(vertex_t v1 : verticesOnDepthG1)
        {
            RCOUT("            v1= " << v1 << ", name = " << g1[v1].name,
                  KERNELSUBTREE_VERBOSE);
            for(vertex_t v2 : verticesOnDepthG2)
            {
                RCOUT("            v2= " << v2 << ", name = " << g2[v2].name,
                      KERNELSUBTREE_VERBOSE);
                compareTwoVertices(g1, g2, v1, v2, median1, median2);
            }
        }

        if(depth-1 > depth)
            ok = false;
    }

    for(unsigned long long depth = max(maxDepthG1, maxDepthG2); ok; depth--)
    {
        if(g1VerticesByDepth.find(depth) == g1VerticesByDepth.end() ||
           g2VerticesByDepth.find(depth-1) == g2VerticesByDepth.end())
            continue;
        const list<vertex_t>& verticesOnDepthG1 = g1VerticesByDepth.at(depth);
        const list<vertex_t>& verticesOnDepthG2 = g2VerticesByDepth.at(depth-1);
        for(vertex_t v1 : verticesOnDepthG1)
        {
            RCOUT("            v1= " << v1 << ", name = " << g1[v1].name,
                  KERNELSUBTREE_VERBOSE);
            for(vertex_t v2 : verticesOnDepthG2)
            {
                RCOUT("            v2= " << v2 << ", name = " << g2[v2].name,
                      KERNELSUBTREE_VERBOSE);
                compareTwoVertices(g1, g2, v1, v2, median1, median2, 3.0);
            }
        }

        if(depth-1 > depth)
            ok = false;
    }

    for(unsigned long long depth = max(maxDepthG1, maxDepthG2); ok; depth--)
    {
        if(g1VerticesByDepth.find(depth-1) == g1VerticesByDepth.end() ||
           g2VerticesByDepth.find(depth) == g2VerticesByDepth.end())
            continue;
        const list<vertex_t>& verticesOnDepthG1 = g1VerticesByDepth.at(depth-1);
        const list<vertex_t>& verticesOnDepthG2 = g2VerticesByDepth.at(depth);
        for(vertex_t v1 : verticesOnDepthG1)
        {
            RCOUT("            v1= " << v1 << ", name = " << g1[v1].name,
                  KERNELSUBTREE_VERBOSE);
            for(vertex_t v2 : verticesOnDepthG2)
            {
                RCOUT("            v2= " << v2 << ", name = " << g2[v2].name,
                      KERNELSUBTREE_VERBOSE);
                compareTwoVertices(g1, g2, v1, v2, median1, median2, 3.0);
            }
        }

        if(depth-1 > depth)
            ok = false;
    }
}

void PDGMyKernelComparator::compareVerticesWithinOneGraph(
    GraphType& g,
    unsigned long long&
    label_counter,
    unsigned long long
    maxDepth,
    double median,
    map<unsigned long
        long,
        list<vertex_t> >&
    gVerticesByDepth
    )
{
    bool ok = true;
    for(unsigned long long depth = maxDepth; ok; depth--)
    {

        if(gVerticesByDepth.find(depth) == gVerticesByDepth.end())
            continue;

        const list<vertex_t>& verticesOnDepth = gVerticesByDepth.at(depth);
        for(vertex_t v1 : verticesOnDepth)
        {
            if(g[v1].isPaired)
                continue;
            for(vertex_t v2 : verticesOnDepth)
            {
                if(g[v2].isPaired || v1 == v2)
                    continue;

                compareTwoVertices(g, g, v1, v2, median, median);
            }
            if(!g[v1].isPaired)
            {
                g[v1].isPaired =  true;
                g[v1].newlabel = label_counter++;
            }
        }
        if(depth-1 > depth)
            ok = false;
    }
}

void PDGMyKernelComparator::pairGraphs(GraphType& g1,
                                       GraphType& g2
                                       )
{
    RCOUT("        compareVerticesWithinTwoGraphs", KERNELSUBTREE_VERBOSE);
    compareVerticesWithinTwoGraphs(g1, g2);

    GraphType::vertex_iterator it, itEnd;
    int paired1 = 0;
    for (boost::tie(it, itEnd) = boost::vertices(g1); it != itEnd; it++)
    {
        if(g1[*it].isPaired) paired1++;
    }
    RCOUT("paired1 = " << paired1, KERNELSUBTREE_VERBOSE);

    int paired2 = 0;
    for (boost::tie(it, itEnd) = boost::vertices(g2); it != itEnd; it++)
    {
        if(g2[*it].isPaired) paired2++;
    }
    RCOUT("paired2 = " << paired2, KERNELSUBTREE_VERBOSE);

    RCOUT("        compareVerticesWithinOneGraph1", KERNELSUBTREE_VERBOSE);
    compareVerticesWithinOneGraph(g1,label_counter,maxDepthG1, median1,
                                  g1VerticesByDepth);
    RCOUT("        compareVerticesWithinTwoGraph2", KERNELSUBTREE_VERBOSE);
    compareVerticesWithinOneGraph(g2,label_counter,maxDepthG2, median2,
                                  g2VerticesByDepth);
}

void PDGMyKernelComparator::makeIterationForGraphs(
    GraphType& g1,
    GraphType& g2
    )
{

    computeLabels(g2,maxDepthG2,g2VerticesByDepth);
    computeLabels(g1,maxDepthG1,g1VerticesByDepth);
    pairGraphs(g1,g2);
    calculatePhi(g1,phi1);
    calculatePhi(g2,phi2);
}

void PDGMyKernelComparator::iterationZero(
    GraphType& g,
    vector<unsigned long long>& phi
    )
{
    GraphType::vertex_iterator it, itEnd;


    for (boost::tie(it, itEnd) = boost::vertices(g); it != itEnd; it++)
    {
        if(label_lookup_first.find(g[*it].color) == label_lookup_first.end())
        {
            label_lookup_first[g[*it].color] = label_counter_first;
            g[*it].newlabel = g[*it].label = label_counter_first;
            label_counter_first++;
        }
        else
        {
            g[*it].newlabel = g[*it].label = label_lookup_first[g[*it].color];
        }
        phi[g[*it].newlabel]++;
    }
}


unsigned long long PDGMyKernelComparator::assignDepthRec(
    GraphType& g, vertex_t v,
    unsigned long long
    depth,
    map<unsigned long long,
        list<vertex_t> >
    & gVerticesByDepth)
{
    boost::graph_traits<GraphType>::out_edge_iterator out_e, out_e_end;
    unsigned long long max_depth = depth;

    g[v].depth = depth;

    if(gVerticesByDepth.find(depth) == gVerticesByDepth.end())
    {
        gVerticesByDepth[depth] = list<vertex_t>();
    }
    gVerticesByDepth[depth].push_back(v);

    for (tie(out_e, out_e_end) = out_edges(v, g); out_e != out_e_end; ++out_e)
    {
        if(g[*out_e].color == color_data_dependency || g[*out_e].color ==
           color_control_flow)
        {
            continue;
        }
        vertex_t nei = target(*out_e,g);
        max_depth =
            max(max_depth,
                assignDepthRec(g, nei,g[nei].color ==
                               color_if_part ? depth : depth+1,
                               gVerticesByDepth));
    }

    return max_depth;
}

unsigned long long PDGMyKernelComparator::calculateDepthOfVerticesInTree(
    GraphType& g, map<unsigned long long, list<vertex_t> >& gVerticesByDepth)
{
    vertex_t entry = 0;
    GraphType::vertex_iterator it, itEnd;

    for (boost::tie(it, itEnd) = boost::vertices(g); it != itEnd; it++)
    {
        if(g[*it].color == color_entry)
        {
            entry = *it;
            break;
        }
    }

    return assignDepthRec(g, entry, 0, gVerticesByDepth);
}

double PDGMyKernelComparator::calculateImportanceOfVertices(
    GraphType& g,
    const map<unsigned
              long long,
              list<
                  vertex_t> >&
    gVerticesByDepth,
    unsigned long long
    maxDepth)
{
    const double importance = 0.1;
    const double data_dependency_multiplier = 1.1;
    boost::graph_traits<GraphType>::out_edge_iterator out_e, out_e_end;
    double sumImportance = 0.0;
    GraphType::vertex_iterator it, itEnd;
    bool notAllAssigned = true;
    while(notAllAssigned)
    {
        notAllAssigned = false;
        for (boost::tie(it, itEnd) = boost::vertices(g); it != itEnd; it++)
        {
            if(g[*it].importanceAssigned) continue;
            bool notReadyVertex = false;
            int notReadyNeighbors = 0;
            for (tie(out_e, out_e_end) = out_edges(*it, g); out_e != out_e_end;
                 ++out_e)
            {
                vertex_t nei = target(*out_e,g);
                if(nei == *it) continue;
                if(*it==8) RCOUT(
                        "nei = " << nei << ", g[nei].name = " << g[nei].name,
                        KERNELSUBTREE_VERBOSE);
                if(*it==8) RCOUT(
                        "g[nei].importanceAssigned = " <<
                        g[nei].importanceAssigned,
                        KERNELSUBTREE_VERBOSE)
                    if(!g[nei].importanceAssigned) {notReadyVertex = true;
                                                    notReadyNeighbors++; break; }
            }
            if(notReadyVertex)
            {
                if(*it==8) RCOUT(
                        "not all neighbours assigned, notReadyNeighbors="<<
                        notReadyNeighbors, KERNELSUBTREE_VERBOSE);
                notAllAssigned = true;
                continue;
            }

            RCOUT(
                endl<< endl<<"*it = " << *it << ", g[*it].name = " <<
                g[*it].name <<
                endl, KERNELSUBTREE_VERBOSE);

            if(graphUtils::getCanonicalName(g[*it].originalFunctionName,
                                   g[graph_bundle].variableName2variableName)
               == "stopifnot")
            {
                g[*it].importance = importance/3.0;
            }
            else
                g[*it].importance = importance;

            for (tie(out_e, out_e_end) = out_edges(*it, g); out_e != out_e_end;
                 ++out_e)
            {
                vertex_t nei = target(*out_e,g);
                if(nei == *it) continue;
                double multiplier = 1.0;
                if(g[*out_e].color == color_data_dependency)
                {
                    multiplier = data_dependency_multiplier;
                }
                RCOUT("nei = " << nei << ", g[nei].name = " << g[nei].name,
                      KERNELSUBTREE_VERBOSE);
                RCOUT("g[nei].importance = "<<g[nei].importance,
                      KERNELSUBTREE_VERBOSE)

                g[*it].importance += multiplier * g[nei].importance;
            }
            RCOUT(endl<<"g[*it].importance = "<<g[*it].importance,
                  KERNELSUBTREE_VERBOSE)
            sumImportance += g[*it].importance;
            g[*it].importanceAssigned = true;
        }
    }

    g[graph_bundle].importanceSum = sumImportance;
    vector<double> importances(num_vertices(g));
    size_t i = 0;

    for (boost::tie(it, itEnd) = boost::vertices(g); it != itEnd; it++)
    {
        g[*it].importanceNormalized = g[*it].importance/sumImportance;
        importances[i++] = g[*it].importanceNormalized;
    }

    std::sort(importances.begin(), importances.end());

#if VERBOSE >= KERNELSUBTREE_VERBOSE
    for(size_t i = 0; i<importances.size(); ++i)
    {
        Rcout<< importances[i] << ",";
    }
    Rcout << endl;
#endif

    int index1 = 11;
    int index2 = 20;

    double median = importances[index1*(importances.size()-1)/index2];
    if(importances[index1*(importances.size()-1)/index2-1] == median)
    {
        size_t index = index1*(importances.size()-1)/index2+1;
        while(importances[index] == median)
            index++;
        median = importances[index];
    }
    return median;
}

void PDGMyKernelComparator::copyNewLabels2Labels(GraphType& g)
{
    GraphType::vertex_iterator it, itEnd;
    for (boost::tie(it, itEnd) = boost::vertices(g); it != itEnd; it++)
    {
        g[*it].label = g[*it].newlabel;
    }
}

void PDGMyKernelComparator::removeDataCycles(GraphType& g)
{
    RCOUT("removing edges begin", KERNELSUBTREE_VERBOSE);
    graph_traits<GraphType>::edge_iterator vi, vi_end, next;
    tie(vi, vi_end) = edges(g);
    for (next = vi; vi != vi_end; vi = next) {
        ++next;
        if(source(*vi,g) > target(*vi,g))
        {
            RCOUT("removing edge from " <<
                  source(*vi,g) << " to "<< target(*vi,
                                                   g), KERNELSUBTREE_VERBOSE);
            remove_edge(*vi, g);
        }
    }
    RCOUT("removing edges end", KERNELSUBTREE_VERBOSE);

}

void PDGMyKernelComparator::resetDataStructures(GraphType& g1, GraphType& g2)
{
    label_lookup.clear();
    label_counter = 0;
    std::fill(phi1.begin(), phi1.end(), 0);
    std::fill(phi2.begin(), phi2.end(), 0);

    GraphType::vertex_iterator it, itEnd;
    for (boost::tie(it, itEnd) = boost::vertices(g1); it != itEnd; it++)
    {
        g1[*it].isPaired = false;
    }
    for (boost::tie(it, itEnd) = boost::vertices(g2); it != itEnd; it++)
    {
        g2[*it].isPaired = false;
    }
}

bool PDGMyKernelComparator::comparePhis()
{
    for(size_t i = 0; i<phi1.size(); ++i)
        if(phi1[i] != oldPhi1[i]) return false;
    for(size_t i = 0; i<phi2.size(); ++i)
        if(phi2[i] != oldPhi2[i]) return false;
    return true;
}

vector<double> PDGMyKernelComparator::compare(GraphType& g1, GraphType& g2,
                                              bool isSymmetric)
{
    GraphType::vertex_iterator it, itEnd;
    size_t n_nodes = num_vertices(g1) + num_vertices(g2);

    phi1 = vector<unsigned long long>(n_nodes,0);
    phi2 = vector<unsigned long long>(n_nodes,0);
    oldPhi1 = vector<unsigned long long>(n_nodes,0);
    oldPhi2 = vector<unsigned long long>(n_nodes,0);

    double len1 = 0;
    double len2 = 0;
    double mul = 0;
    double mul_sym = 0;
    label_counter_first = 0;
    label_lookup_first.clear();

    double k12 = 0;
    double kAll12 = 0;
    double wAll12 = 0;

    double k21 = 0;
    double kAll21 = 0;
    double wAll21 = 0;

    double k_sym = 0;
    double kAll_sym = 0;

    for (boost::tie(it, itEnd) = boost::vertices(g1); it != itEnd; it++)
    {
        g1[*it].isPaired = false;
        g1[*it].importanceAssigned = false;
    }
    for (boost::tie(it, itEnd) = boost::vertices(g2); it != itEnd; it++)
    {
        g2[*it].isPaired = false;
        g2[*it].importanceAssigned = false;
    }


    removeDataCycles(g1);
    removeDataCycles(g2);

    g1VerticesByDepth.clear();
    g2VerticesByDepth.clear();

    maxDepthG1 = calculateDepthOfVerticesInTree(
        g1, g1VerticesByDepth);
    maxDepthG2 = calculateDepthOfVerticesInTree(
        g2, g2VerticesByDepth);
    median1 = calculateImportanceOfVertices(g1, g1VerticesByDepth,
                                                   maxDepthG1);
    median2 = calculateImportanceOfVertices(g2, g2VerticesByDepth,
                                                   maxDepthG2);

    RCOUT("num_vertices(g1) = " << num_vertices(g1), KERNELSUBTREE_VERBOSE);
    RCOUT("num_vertices(g2) = " << num_vertices(g2), KERNELSUBTREE_VERBOSE);
    RCOUT("maxDepthG1 = " << maxDepthG1, KERNELSUBTREE_VERBOSE);
    RCOUT("maxDepthG2 = " << maxDepthG2, KERNELSUBTREE_VERBOSE);
    RCOUT("median1 = " << median1, KERNELSUBTREE_VERBOSE);
    RCOUT("median2 = " << median2, KERNELSUBTREE_VERBOSE);

    iterationZero(g1, phi1);
    iterationZero(g2, phi2);

    RCOUT( "phi1:",KERNELSUBTREE_VERBOSE);
#if VERBOSE >= KERNELSUBTREE_VERBOSE
    for(size_t i = 0; i<phi1.size(); ++i)
    {
        Rcout << phi1[i] << ",";
    }
#endif
    RCOUT( "",KERNELSUBTREE_VERBOSE);
    RCOUT( "phi2:",KERNELSUBTREE_VERBOSE);
#if VERBOSE >= KERNELSUBTREE_VERBOSE
    for(size_t i = 0; i<phi2.size(); ++i)
    {
        Rcout << phi2[i] << ",";
    }
#endif
    RCOUT("",KERNELSUBTREE_VERBOSE);

    calculateResultsForOneIteration(len1, len2, mul, mul_sym);

    copy(phi1.begin(), phi1.end(), oldPhi1.begin());
    copy(phi2.begin(), phi2.end(), oldPhi2.begin());

    boost::graph_traits<GraphType>::out_edge_iterator e, e_end;

    k12 = calculateK(mul, len1, len2);
    k21 = calculateK(mul, len2, len1);
    k_sym = 1.0-mul_sym/(len1+len2);
    RCOUT("k12 = " << k12, KERNELSUBTREE_VERBOSE);
    RCOUT("k21 = " << k21, KERNELSUBTREE_VERBOSE);


    mul_sym = mul = len1 = len2 = 0;

    kAll12 += k12;
    kAll21 += k21;
    kAll_sym += k_sym;

    wAll12 += 1;
    wAll21 += 1;

    last_label_counter = label_lookup_first.size();
    label_lookup.clear();
    label_counter = 0;

    for(int i = 1; i<=h; ++i)
    {
        resetDataStructures(g1,g2);

        makeIterationForGraphs(g1, g2);

        if(comparePhis()) break;
        copy(phi1.begin(), phi1.end(), oldPhi1.begin());
        copy(phi2.begin(), phi2.end(), oldPhi2.begin());

        RCOUT( "phi1:",KERNELSUBTREE_VERBOSE);
#if VERBOSE >= KERNELSUBTREE_VERBOSE
        for(size_t i = 0; i<phi1.size(); ++i)
        {
            Rcout << phi1[i] << ",";
        }
#endif
        RCOUT( "",KERNELSUBTREE_VERBOSE);
        RCOUT( "phi2:",KERNELSUBTREE_VERBOSE);
#if VERBOSE >= KERNELSUBTREE_VERBOSE
        for(size_t i = 0; i<phi2.size(); ++i)
        {
            Rcout << phi2[i] << ",";
        }
#endif
        RCOUT("",KERNELSUBTREE_VERBOSE);


        calculateResultsForOneIteration(len1, len2, mul, mul_sym);
        copyNewLabels2Labels(g1);
        copyNewLabels2Labels(g2);
        last_label_counter = label_lookup.size();

        k12 = calculateK(mul, len1, len2);
        k21 = calculateK(mul, len2, len1);
        k_sym = 1.0-mul_sym/(len1+len2);

        RCOUT("k12 = " << k12, KERNELSUBTREE_VERBOSE);
        RCOUT("k21 = " << k21, KERNELSUBTREE_VERBOSE);
        mul_sym = mul = len1 = len2 = 0;
        kAll12 += k12;
        kAll21 += k21;
        kAll_sym += k_sym;
        wAll12 += 1;
        wAll21 += 1;
    }
    vector<double> ret(3);

    ret[0] = kAll12/wAll12;
    ret[1] = kAll21/wAll21;
    ret[2] = kAll_sym/wAll21;
    return ret;
}
