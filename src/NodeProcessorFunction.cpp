#include "NodeProcessorFunction.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorIf::NodeProcessorWhile() : base()
// {}

NodeProcessorFunction::NodeProcessorFunction(CDGCreator& cdg, vertex_t* entry) : NodeProcessor(cdg), entry(entry)
{}

Context NodeProcessorFunction::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessFunction(*this, context);
}

Context NodeProcessorFunction::ProcessFunction(SyntaxLangNode* functionNode, const Context& context)
{
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
        g[*entry].isLeftSideOfAssign = false;
        g[*entry].functionName = returnValueVariableName; // ???
    }

    Context myContext;
    myContext.ControlVertex = *entry;
    myContext.FlowVertex = *entry;
    processor.Process(functionNode->Children[0].get(), myContext); // parametry funkcji
    processor.Process(functionNode->Children[1].get(), myContext); // cialo funkcji
}