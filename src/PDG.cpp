
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
