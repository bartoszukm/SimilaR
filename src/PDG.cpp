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

#include "PDG.h"

GraphType PDGMaker::MakePDG(SEXP s,
                  bool isDeleteControlFlowEdges,
                  bool executeRemoveSingleInstructions,
                  bool executeMergeTheSameInstructions,
                  bool executeChangeWhileLoop)
{
        vertex_t* entry=NULL;
        // Rcout << "Make CDG" << endl;
        GraphType pdg = cdg.makeCDG_cpp(s, entry);
        // Rcout << "Make DDG" << endl;
        ddg.makeDDG_cpp(pdg,*entry);
        // Rcout << "Make post" << endl;
        if(isDeleteControlFlowEdges) post.deleteControlFlowEdges(pdg);
        if(executeRemoveSingleInstructions) post.removeSingleInstructions(pdg);
        if(executeMergeTheSameInstructions) post.mergeTheSameInstructions(pdg);
        if(executeChangeWhileLoop) post.changeWhileLoop(pdg);
        post.colonToComparison(pdg);
        post.removeSymbolNodes(pdg);
        
        
        // graph_traits<GraphType>::vertex_iterator vi, vi_end, next;
        // tie(vi, vi_end) = vertices(pdg);
        // for (next = vi; vi != vi_end; vi = next) {
        //   ++next;
        // 
        //   Rcout << pdg[*vi].color << " " << pdg[*vi].name << ", " << pdg[*vi].functionName << ", gen:" << pdg[*vi].gen << endl;
        //   for(auto s : pdg[*vi].uses)
        //   {
        //     Rcout << s << ",";
        //   }
        //   Rcout << endl;
        // }
        
        post.memoryClean(pdg);
        
        return pdg;
}

GraphType PDGMaker::MakePDG_refactored(SEXP s, bool isDeleteControlFlowEdges, bool executeRemoveSingleInstructions, bool executeMergeTheSameInstructions, bool executeChangeWhileLoop)
{
    spdlog::set_level(spdlog::level::debug);
//    vertex_t* entry=NULL;
    // Rcout << "Make CDG" << endl;
    spdlog::debug("Tworze drzewo");
    SyntaxTree t(s);
    CDGCreator cdg;
    spdlog::debug("Tworze CDG");
    GraphType pdg = cdg.CreateCDG(t.GetRoot());
    // Rcout << "Make DDG" << endl;
    spdlog::debug("Tworze DDG");
    ddg.makeDDG_cpp(pdg,cdg.GetEntry());
    // Rcout << "Make post" << endl;
    if(isDeleteControlFlowEdges) post.deleteControlFlowEdges(pdg);
    if(executeRemoveSingleInstructions) post.removeSingleInstructions(pdg);
    if(executeMergeTheSameInstructions) post.mergeTheSameInstructions(pdg);
    if(executeChangeWhileLoop) post.changeWhileLoop(pdg);
    post.colonToComparison(pdg);
    post.removeSymbolNodes(pdg);


    // graph_traits<GraphType>::vertex_iterator vi, vi_end, next;
    // tie(vi, vi_end) = vertices(pdg);
    // for (next = vi; vi != vi_end; vi = next) {
    //   ++next;
    //
    //   Rcout << pdg[*vi].color << " " << pdg[*vi].name << ", " << pdg[*vi].functionName << ", gen:" << pdg[*vi].gen << endl;
    //   for(auto s : pdg[*vi].uses)
    //   {
    //     Rcout << s << ",";
    //   }
    //   Rcout << endl;
    // }

    post.memoryClean(pdg);

    return pdg;
}
