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

#include "postprocessingPDG.h"

string PostprocessingPDG::sideEffectsFunctions[] = {
    "stopifnot", "stop", "warning",
    "return","break", "next",
    "abline", "arrows", "assocplot", "axis", "Axis",
    "axis.Date", "axis.POSIXct", "axTicks", "barplot",
    "barplot.default", "box", "boxplot", "boxplot.default",
    "boxplot.matrix", "bxp", "cdplot", "clip", "close.screen",
    "co.intervals", "contour", "contour.default", "coplot", "curve",
    "dotchart", "erase.screen", "filled.contour", "fourfoldplot", "frame",
    "grconvertX", "grconvertY", "grid", "hist", "hist.default", "identify",
    "image", "image.default", "layout", "layout.show", "lcm", "legend",
    "lines", "lines.default", "locator", "matlines", "matplot", "matpoints",
    "mosaicplot", "mtext", "pairs", "pairs.default", "panel.smooth",
    "par", "persp", "pie", "plot", "plot.default", "plot.design",
    "plot.function", "plot.new", "plot.window", "plot.xy",
    "points", "points.default", "polygon", "polypath", "rasterImage", "rect",
    "rug", "screen", "segments", "smoothScatter", "spineplot",
    "split.screen", "stars", "stem", "strheight", "stripchart",
    "strwidth", "sunflowerplot", "symbols", "text", "text.default", "title",
    "xinch", "xspline", "xyinch", "yinch",
    "read.csv", "read.csv2", "read.table", "write.csv", "write.csv2",
    "write.table", "saveRDS", "readRDS","save", "load", "serialize",
    "writeLines", "assertFunction", "assertCharacter", "assertString",
    "on.exit"
};

void PostprocessingPDG::deleteControlFlowEdges(GraphType& g)
{
    edge_remover er(g);
    remove_edge_if(er,g);
}

void PostprocessingPDG::changeWhileLoop(GraphType& g)
{
    graph_traits<GraphType>::vertex_iterator vi, vi_end, next;
    boost::graph_traits<GraphType>::in_edge_iterator in_e, in_e_end;
    boost::graph_traits<GraphType>::in_edge_iterator in_e2, in_e_end2;
    boost::graph_traits<GraphType>::out_edge_iterator out_e, out_e_end;
    boost::graph_traits<GraphType>::out_edge_iterator out_e2, out_e_end2;
    boost::graph_traits<GraphType>::out_edge_iterator out_e3, out_e_end3;
    tie(vi, vi_end) = vertices(g);
    set<string> uses;
    set<string> variablesChangedInLoop;
    map<string, vertex_t> initialVertices;
    for (next = vi; vi != vi_end; vi = next) {
        ++next;
        if(g[*vi].name == "while")
        {
            for (tie(in_e, in_e_end) = in_edges(*vi, g); in_e != in_e_end;
                 ++in_e)
            {
                if(g[*in_e].color != color_data_dependency) continue;
                vertex_t nei = source(*in_e,g);
                uses.insert(g[nei].uses.begin(), g[nei].uses.end());
                for (tie(in_e2, in_e_end2) = in_edges(nei, g);
                     in_e2 != in_e_end2; ++in_e2)
                {
                    vertex_t nei2 = source(*in_e2,g);
                    if(uses.find(g[nei2].gen) != uses.end())
                    {
                        initialVertices[g[nei2].gen] = nei2;
                    }
                }
            }



            for (tie(out_e, out_e_end) = out_edges(*vi, g); out_e != out_e_end;
                 ++out_e)
            {
                vertex_t nei = target(*out_e,g);
                if(uses.find(g[nei].gen) != uses.end())
                {
                    variablesChangedInLoop.emplace(g[nei].gen);
                    for (tie(out_e2, out_e_end2) = out_edges(nei, g);
                         out_e2 != out_e_end2; ++out_e2)
                    {
                        if(g[*out_e2].color != color_data_dependency) continue;
                        bool paired = false;
                        for (tie(out_e3, out_e_end3) = out_edges(*vi, g);
                             out_e3 != out_e_end3; ++out_e3)
                        {
                            if(g[*out_e3].color !=
                               color_data_dependency) continue;
                            if(target(*out_e2,g) == target(*out_e3,g))
                            {
                                paired = true;
                                break;
                            }
                        }
                        if(!paired)
                        {
                            std::pair<edge_t,
                                      bool> e = add_edge(*vi, target(*out_e2,
                                                                     g), g);
                            g[e.first].color = color_data_dependency;
                            remove_edge(initialVertices[g[nei].gen],
                                        target(*out_e2, g), g);
                        }
                    }
                    if(*next == nei)
                    {
                        ++next;
                    }
                    clear_vertex(nei, g);
                    remove_vertex(nei, g);
                }

            }

        }
    }

    for(auto it = initialVertices.begin(); it!=initialVertices.end(); ++it)
    {
        if(out_degree(it->second,g) == 1 && in_degree(it->second,g) == 1)
        {
            clear_vertex(it->second, g);
            remove_vertex(it->second, g);
        }
    }
}

bool PostprocessingPDG::isSideEffectFunction(string functionName)
{
    size_t n = (sizeof(sideEffectsFunctions)/sizeof(*sideEffectsFunctions));
    for(size_t i = 0; i< n; ++i)
    {
        if(functionName == sideEffectsFunctions[i])
            return true;
    }
    return false;
}

void PostprocessingPDG::removeSingleInstructions(GraphType& g)
{
    graph_traits<GraphType>::vertex_iterator vi, vi_end, next;
    tie(vi, vi_end) = vertices(g);

    g[num_vertices(g)-1].lastInstruction = true;
    bool changes = true;
    while(changes)
    {
        changes = false;
        tie(vi, vi_end) = vertices(g);
        for (next = vi; vi != vi_end; vi = next) {
            ++next;
            if(!g[*vi].lastInstruction &&
               out_degree(*vi,
                          g) == 0 &&
               !isSideEffectFunction(graphUtils::getCanonicalName(g[*vi].functionName,
                                                         g[graph_bundle].
                                                         variableName2variableName))
               &&
               !(g[*vi].color == color_break) && !(g[*vi].color == color_next)
               && !(g[*vi].color == color_dollar) && !(g[*vi].color == color_oneBracketSingle)
             && !(g[*vi].color == color_oneBracketDouble) && !(g[*vi].color == color_oneBracketTripleOrMore)
           && !(g[*vi].color == color_twoBrackets))
            {
                changes = true;
                clear_vertex(*vi, g);
                remove_vertex(*vi, g);
            }
        }
    }
}

void PostprocessingPDG::mergeTheSameInstructions(GraphType& g)
{
    graph_traits<GraphType>::vertex_iterator vi, vi_end, next;
    graph_traits<GraphType>::vertex_iterator vi2, vi_end2, next2;
    graph_traits<GraphType>::vertex_iterator vi3, vi_end3, next3;
    boost::graph_traits<GraphType>::in_edge_iterator in_e, in_e_end;
    boost::graph_traits<GraphType>::in_edge_iterator in_e2, in_e_end2;
    boost::graph_traits<GraphType>::out_edge_iterator out_e, out_e_end;
    boost::graph_traits<GraphType>::out_edge_iterator out_e2, out_e_end2;
    tie(vi, vi_end) = vertices(g);

    bool changes = true;
    while(changes)
    {
        changes = false;
        tie(vi, vi_end) = vertices(g);
        for (next = vi; vi != vi_end && !changes; vi = next) {
            ++next;
            if(g[*vi].color == color_functionZeroArgument || g[*vi].color ==
               color_functionOneArgument
               || g[*vi].color == color_functionTwoArguments || g[*vi].color ==
               color_functionThreeArguments
               || g[*vi].color == color_functionFourOrMoreArguments ||
               g[*vi].color == color_oneBracketSingle
               || g[*vi].color == color_oneBracketDouble || g[*vi].color ==
               color_twoBrackets)
            {
                tie(vi2, vi_end2) = vertices(g);
                vi2 = vi;
                ++vi2;
                for (next2 = vi2; vi2 != vi_end; vi2 = next2) {
                    ++next2;
                    if(g[*vi2].color != g[*vi].color) continue;
                    if(g[*vi2].functionName != g[*vi].functionName) continue;
                    bool vertexPaired = true;
                    for (tie(in_e, in_e_end) = in_edges(*vi, g);
                         in_e != in_e_end; ++in_e)
                    {
                        vertex_t nei = source(*in_e,g);
                        if(g[*in_e].color == color_data_dependency)
                        {
                            bool found = false;
                            for (tie(in_e2, in_e_end2) = in_edges(*vi2, g);
                                 in_e2 != in_e_end2; ++in_e2)
                            {
                                vertex_t nei2 = source(*in_e2,g);
                                if(nei==nei2)
                                {
                                    found = true;
                                    break;
                                }
                            }
                            if(!found)
                            {
                                vertexPaired = false;
                                break;
                            }

                        }
                    }
                    if(vertexPaired && g[*vi].uses == g[*vi2].uses)
                    {
                        for (tie(out_e, out_e_end) = out_edges(*vi2, g);
                             out_e != out_e_end; ++out_e)
                        {
                            if(g[*out_e].color !=
                               color_data_dependency) continue;
                            bool paired = false;
                            for (tie(out_e2, out_e_end2) = out_edges(*vi, g);
                                 out_e2 != out_e_end2; ++out_e2)
                            {
                                if(g[*out_e2].color !=
                                   color_data_dependency) continue;
                                if(target(*out_e,g) == target(*out_e2,g))
                                {
                                    paired = true;
                                    break;
                                }
                            }
                            if(!paired)
                            {
                                std::pair<edge_t, bool> e = add_edge(
                                    *vi, target(*out_e, g), g);
                                g[e.first].color = color_data_dependency;
                            }
                        }
                        if(*next == *vi2)
                        {
                            ++next;
                        }

                        tie(vi3, vi_end3) = vertices(g);
                        for (next3 = vi3; vi3 != vi_end3; vi3 = next3) {
                            ++next3;
                            std::replace (
                                g[*vi3].uses.begin(),
                                g[*vi3].uses.end(), g[*vi2].gen, g[*vi].gen);
                            std::replace (
                                g[*vi3].arguments.begin(),
                                g[*vi3].arguments.end(), g[*vi2].gen,
                                g[*vi].gen);
                        }

                        clear_vertex(*vi2, g);
                        remove_vertex(*vi2, g);
                        changes = true;
                        break;
                    }
                }
            }
        }
    }
}

void PostprocessingPDG::memoryClean(GraphType& g)
{
  GraphType::vertex_iterator it, itEnd;
  for (boost::tie(it, itEnd) = boost::vertices(g); it != itEnd; it++)
  {
    g[*it].gens.clear();
    g[*it].kill.clear();
    g[*it].uses.clear();
    g[*it].arguments.clear();
    g[*it].in.clear();
    g[*it].out.clear();
    g[*it].out_old.clear();
  }

}
