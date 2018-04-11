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

        GraphType pdg = cdg.makeCDG_cpp(s, entry);

        ddg.makeDDG_cpp(pdg,*entry);

        if(isDeleteControlFlowEdges) post.deleteControlFlowEdges(pdg);
        if(executeRemoveSingleInstructions) post.removeSingleInstructions(pdg);
        if(executeMergeTheSameInstructions) post.mergeTheSameInstructions(pdg);
        if(executeChangeWhileLoop) post.changeWhileLoop(pdg);
        post.memoryClean(pdg);
        return pdg;
}
