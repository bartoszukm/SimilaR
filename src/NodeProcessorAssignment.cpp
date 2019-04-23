#include "NodeProcessorAssignment.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorAssignment::NodeProcessorAssignment() : base()
// {}

NodeProcessorAssignment::NodeProcessorAssignment(CDGCreator& cdg) : NodeProcessor(cdg)
{}

Context NodeProcessorAssignment::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessAssignment(*this, context);
}

Context NodeProcessorAssignment::ProcessAssignment(SyntaxLangNode* assignmentNode,
                    const Context& context)
{
    // jak lewe lang to call
    // jak prawe lang:
    //    call
    //    jak lewe symbol a prawe dolar, to emplace
    //    jak lewe symbol to koniec
    // jak prawe sym:
    //    jak lewe sym to alias
    // wierzchołek tworzony jest gdy:
    //    lewe to lang
    //    prawe to numeryczne
    //     

    GraphType& g = CDG.GetGraph();
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;

    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false);
    Context leftContext = processor->Process(assignmentNode->Children[0].get(), myContext);
    myContext.FlowVertex = leftContext.FlowVertex;

       

    return myContext;
}