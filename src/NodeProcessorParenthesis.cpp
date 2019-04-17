#include "NodeProcessorParenthesis.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorParenthesis::NodeProcessorParenthesis() : base()
// {}

NodeProcessorParenthesis::NodeProcessorParenthesis(CDGCreator& cdg, vertex_t* entry) : NodeProcessor(cdg), entry(entry)
{}

Context NodeProcessorParenthesis::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessParenthesis(*this, context);
}

Context NodeProcessorParenthesis::ProcessParenthesis(SyntaxLangNode* parenthesisNode, const Context& context)
{
    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false); // taki sam jak wyzej
    processor->Process(parenthesisNode->Children[0].get(), context);   
}