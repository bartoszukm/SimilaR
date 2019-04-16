#include "NodeProcessor.h"

// NodeProcessor::NodeProcessor() : Next(nullptr)
// {}

NodeProcessor::NodeProcessor(CDGCreator& cdg) : Next(nullptr), CDG(cdg)
{}

Context NodeProcessor::Process(SyntaxNode* n, const Context& context)
{
    if(Next)
        return Next->Process(n, context);
    Rf_error("No Next pointer");
}

NodeProcessor* NodeProcessor::SetNext(NodeProcessor* next)
{
    Next.reset(next);
    return next;
}

Context NodeProcessor::ProcessNext(SyntaxNode* n, const Context& context)
{
    return NodeProcessor::Process(n, context);
}

string NodeProcessor::concatenateStringList(const list<string>& l)
{
    string result;
    for (std::list<string>::const_iterator it = l.begin(); it != l.end(); it++)
        result = result + *it;
    return result;
}