#include "NodeProcessorFunction.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorIf::NodeProcessorWhile() : base()
// {}

NodeProcessorFunction::NodeProcessorFunction(CDGCreator& cdg, string gen, vertex_t* entry) : NodeProcessor(cdg), entry(entry), gen(gen)
{}

Context NodeProcessorFunction::Process(SyntaxNode* n, const Context& context)
{
    spdlog::debug("NodeProcessorFunction::Process()");
    spdlog::debug("Name: {0}", n->Name);
    return n->ProcessFunction(*this, context);
}

Context NodeProcessorFunction::ProcessFunction(SyntaxLangNode* functionNode, const Context& context)
{
    spdlog::debug("{:<30}", "ProcessFunction()");
    GraphType& g = CDG.GetGraph();

    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false); // przekazac: ze w ciele ostatnia to return value
    vertex_t myEntry;
    if(entry == nullptr)
    {

        entry = &myEntry;
        *entry = (boost::add_vertex(g));
        g[*entry].color = color_entry;
        g[*entry].name = "Entry";
        g[*entry].lastInstruction = false;
        g[*entry].functionName = gen; // jesli x <- apply(function(a)) to tutaj wstawimy x. Dla funkcji jako takiej wstawiamy glupote

        if(num_vertices(g)==1)
            CDG.SetEntry(*entry);
    }

    Context myContext;
    myContext.ControlVertex = *entry;
    myContext.FlowVertex = *entry;
    spdlog::debug("functionNode->Children.size(): {0}", functionNode->Children.size());
    spdlog::debug("{:<30}", "Process parametry");
    Context parametersContext = processor->Process(functionNode->Children[0].get(), myContext); // parametry funkcji
    myContext.FlowVertex = parametersContext.FlowVertex;
    spdlog::debug("{:<30}", "Process cialo funkcji");
    spdlog::debug("cialo->Name: {0}", functionNode->Children[1]->Name);
    processor->Process(functionNode->Children[1].get(), myContext); // cialo funkcji
    spdlog::debug("{:<30}", "~ProcessFunction()");
    return myContext;
}
