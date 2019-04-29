#include "NodeProcessorBrace.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorBrace::NodeProcessorBrace() : base()
// {}

NodeProcessorBrace::NodeProcessorBrace(CDGCreator& cdg, bool isLastInstruction) : NodeProcessor(cdg), isLastInstruction(isLastInstruction)
{}

Context NodeProcessorBrace::Process(SyntaxNode* n, const Context& context)
{
    spdlog::debug("NodeProcessorBrace::Process()");
    return n->ProcessBrace(*this, context);
}

Context NodeProcessorBrace::ProcessBrace(SyntaxLangNode* braceNode, const Context& context)
{
    spdlog::debug("ProcessBrace()");
    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false);
    unique_ptr<NodeProcessor> last_processor = CDG.GetProcessors(isLastInstruction);

    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;
    spdlog::debug("braceNode->Children.size(): {0}", braceNode->Children.size());
    for(size_t i = 0; i < braceNode->Children.size(); i++)
    {
        spdlog::debug("brace iteration");
        Context childContext;
        if(i < braceNode->Children.size()-1)
            childContext = processor->Process(braceNode->Children[i].get(), myContext);
        else
            childContext = last_processor->Process(braceNode->Children[i].get(), myContext);
        myContext.FlowVertex = childContext.FlowVertex;
    }
    spdlog::debug("~ProcessBrace()");
    return myContext;
}
