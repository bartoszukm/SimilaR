#include "NodeProcessorFunctionParameters.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorFunction::NodeProcessorFunction() : base()
// {}

NodeProcessorFunctionParameters::NodeProcessorFunctionParameters(CDGCreator& cdg) : NodeProcessor(cdg)
{}

Context NodeProcessorFunctionParameters::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessFunctionParameters(*this, context);
}

Context NodeProcessorFunctionParameters::ProcessFunctionParameters(SyntaxPairlistNode* parametersNode, const Context& context)
{
    GraphType& g = CDG.GetGraph();
    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false); // przekazac ze jak symbol to tworzyc nowy node
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;

    for(size_t i = 0; i < parametersNode->Arguments.size(); i++)
    {
        vertex_t arg = boost::add_vertex(g);
        g[arg].color = color_parameter;

        g[arg].name = parametersNode->Arguments[i];
        g[arg].gen = parametersNode->Arguments[i];
        g[arg].kill = list<string>();
        g[arg].kill.push_back(parametersNode->Arguments[i]);
        g[arg].lastInstruction = false;
        g[arg].isLeftSideOfAssign = false;
        g[arg].arguments.push_back(node->DefaultValues[i]);

        // @TODO: czy obslugujemy funkcje, np. x=length(y)?
        std::pair<edge_t, bool> e = add_edge(myContext.ControlVertex, arg, g);
        g[e.first].color = color_control_dependency;

        e = add_edge(myContext.FlowVertex, arg, g);
        g[e.first].color = color_control_flow;
        myContext.FlowVertex = arg;
    }
}