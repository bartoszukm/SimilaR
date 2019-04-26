#include "NodeProcessorNext.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorNext::NodeProcessorNext() : base()
// {}

NodeProcessorNext::NodeProcessorNext(CDGCreator& cdg) : NodeProcessor(cdg)
{}

Context NodeProcessorNext::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessNext(*this, context);
}

Context NodeProcessorNext::ProcessNextNode(SyntaxNode* n,
                    const Context& context)
{
    GraphType& g = CDG.GetGraph();
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;

    vertex_t node;
    node = boost::add_vertex(g);
    g[node].color = color_next;
    g[node].name = "next";
    g[node].isLeftSideOfAssign = false;
    g[node].lastInstruction = false;

    std::pair<edge_t, bool> e = add_edge(myContext.FlowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(myContext.ControlVertex, node, g);
    g[e.first].color = color_control_dependency;

    // ???
//    vertex_t* pnode = new vertex_t;
//    *pnode = node;
//    if(structuredTransfersOfControl != NULL)
//        structuredTransfersOfControl->push_back(make_pair(if_node,pnode));

    myContext.FlowVertex = node;
    return myContext;
}
