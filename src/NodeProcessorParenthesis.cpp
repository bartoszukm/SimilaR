#include "NodeProcessorParenthesis.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorParenthesis::NodeProcessorParenthesis() : base()
// {}

NodeProcessorParenthesis::NodeProcessorParenthesis(CDGCreator& cdg, bool isLastInstruction) : NodeProcessor(cdg), isLastInstruction(isLastInstruction)
{}

Context NodeProcessorParenthesis::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessParenthesis(*this, context);
}

Context NodeProcessorParenthesis::ProcessParenthesis(SyntaxLangNode* parenthesisNode, const Context& context)
{
    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(isLastInstruction);
    processor->Process(parenthesisNode->Children[0].get(), context);   
}