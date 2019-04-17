#include "NodeProcessorSymbolOrConstant.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorFunction::NodeProcessorFunction() : base()
// {}

NodeProcessorSymbolOrConstant::NodeProcessorSymbolOrConstant(CDGCreator& cdg, bool onlyAddToUses) : NodeProcessor(cdg), onlyAddToUses(onlyAddToUses)
{}

Context NodeProcessorSymbolOrConstant::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessSymbolOrConstant(*this, context);
}

Context ProcessSymbol(SyntaxNode* n,
                    const Context& context)
{
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;
    if(onlyAddToUses)
    {
        myContext.push_back(n->Name);
        return myContext;
    }


    GraphType& g = CDG.GetGraph();
    vertex_t node;
    node = boost::add_vertex(g);
    g[node].color = color_symbol;
    g[node].name = n->Name;
    g[node].gen = "";
    g[node].lastInstruction = lastInstruction; // ???
    g[node].isLeftSideOfAssign = false;

    std::pair<edge_t, bool> e = add_edge(myContext.FlowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(myContext.ControlVertex, node, g);
    g[e.first].color = color_control_dependency;

    myContext.FlowVertex = node;
    return myContext;
}

Context ProcessConstant(SyntaxNode* n,
                    const Context& context)
{
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;

    if(onlyAddToUses)
    {
        myContext.push_back(n->Name);
        return myContext;
    }
    GraphType& g = CDG.GetGraph();
    vertex_t node;
    node = boost::add_vertex(g);
    g[node].color = color_constant;

    g[node].functionName = n->Name;
    g[node].functionName = g[node].functionName.substr(4);
    g[node].name = g[node].functionName;

    g[node].gen = "";
    g[node].lastInstruction = true;
    g[node].isLeftSideOfAssign = false;

    std::pair<edge_t, bool> e = add_edge(myContext.FlowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(myContext.ControlVertex, node, g);
    g[e.first].color = color_control_dependency;

    myContext.FlowVertex = node;
    return myContext;
}