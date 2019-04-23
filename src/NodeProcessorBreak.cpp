#include "NodeProcessorBreak.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorBreak::NodeProcessorBreak() : base()
// {}

NodeProcessorBreak::NodeProcessorBreak(CDGCreator& cdg) : NodeProcessor(cdg)
{}

Context NodeProcessorBreak::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessBreak(*this, context);
}

Context NodeProcessorBreak::ProcessBreak(SyntaxNode* n,
                    const Context& context)
{
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;

    vertex_t node;
    node = boost::add_vertex(g);
    g[node].color = color_break;
    g[node].name = "break";
    g[node].isLeftSideOfAssign = false;
    g[node].lastInstruction = false;

    std::pair<edge_t, bool> e = add_edge(myContext.FlowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(myContext.ControlVertex, node, g);
    g[e.first].color = color_control_dependency;

    // ???
    vertex_t* pnode = new vertex_t;
    *pnode = node;
    if(structuredTransfersOfControl != NULL) 
        structuredTransfersOfControl->push_back(make_pair(if_node,pnode));

    myContext.FlowVertex = node;
    return myContext.FlowVertex;
}