#include "NodeProcessorWrong.h"

// NodeProcessorWrong::NodeProcessorWrong() : base()
// {}

NodeProcessorWrong::NodeProcessorWrong(CDGCreator& cdg) : NodeProcessor(cdg)
{}

Context NodeProcessorWrong::Process(SyntaxNode* s, const Context& context)
{
    Rf_error("No processor got an expression.");
}