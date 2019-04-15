#include "NodeProcessorWhile.h"

// NodeProcessorWhile::NodeProcessorWhile() : base()
// {}

NodeProcessorWhile::NodeProcessorWhile(CDGCreator& cdg) : NodeProcessor(cdg)
{}

Context NodeProcessorWhile::Process(SyntaxNode* n, const Context& context)
{
    n->ProcessWhile(*this, context);
}

Context NodeProcessorWhile::ProcessWhile(SyntaxLangNode* n, const Context& context)
{
    int index = 0;
    list<string> uses;

    // vertex_t oldControlVertex = controlVertex;

    // for(SEXP s1 = s; s1 != R_NilValue; s1 = CDR(s1))
    // {
    //     if(index==0)
    //     {
    //         ;

    //     }
    //     else if(index == 1)
    //     {
    //         if(TYPEOF(CAR(s1)) == SYMSXP)
    //         {

    //             uses.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(CAR(s1))),
    //                                                variableName2variableName));
    //         }
    //         else if(TYPEOF(CAR(s1)) == LANGSXP)
    //         {
    //             makeCallNode(CAR(
    //                              s1), returnValueVariableName,
    //                          controlVertex,
    //                          flowVertex, uses, true, false,false, false, false);
    //         }

    //     }
    //     else if(index == 2)
    //     {
    //         vertex_t node;
    //         node = boost::add_vertex(g);
    //         g[node].color = color_header;
    //         g[node].name = "while";
    //         g[node].isLeftSideOfAssign = false;
    //         g[node].lastInstruction = lastInstruction;

    //         std::pair<edge_t, bool>  e = add_edge(oldControlVertex, node, g);
    //         g[e.first].color = color_control_dependency;

    //         e = add_edge(flowVertex, node, g);
    //         g[e.first].color = color_control_flow;
    //         g[node].uses = uses;

    //         flowVertex = node;
    //         list<pair<vertex_t*, vertex_t*> > structuredTransfersOfControl;
    //         makeCDG_rec_cpp_wrapper(s1, returnValueVariableName,
    //                                 node,flowVertex,NULL,
    //                                 &structuredTransfersOfControl,
    //                                 lastInstruction);
    //         makeStructuredTransfersOfControlForLoop(
    //             node, &structuredTransfersOfControl);
    //         e = add_edge(flowVertex, node, g);
    //         g[e.first].color = color_control_flow;
    //         flowVertex = node;
    //     }
    //     index++;
    // }
}