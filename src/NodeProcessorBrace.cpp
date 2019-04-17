#include "NodeProcessorBrace.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorBrace::NodeProcessorBrace() : base()
// {}

NodeProcessorBrace::NodeProcessorBrace(CDGCreator& cdg, vertex_t* entry) : NodeProcessor(cdg), entry(entry)
{}

Context NodeProcessorBrace::Process(SyntaxNode* n, const Context& context)
{
    n->ProcessBrace(*this, context);
}

Context NodeProcessorBrace::ProcessBrace(SyntaxLangNode* braceNode, const Context& context)
{
    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false);
    unique_ptr<NodeProcessor> last_processor = CDG.GetProcessors(true);

    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;
    for(size_t i = 0; i < braceNode->Children.size(); i++)
    {
        Context childContext;
        if(i < Children.size()-1)
            childContext = processor->Process(braceNode->Children[i].get(), myContext);
        else
            childContext = last_processor->Process(braceNode->Children[i].get(), myContext);
        myContext.FlowVertex = childContext.FlowVertex;
    }
}