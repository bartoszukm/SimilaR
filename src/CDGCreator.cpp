#include "CDGCreator.h"
#include "NodeProcessorWhile.h"
#include "NodeProcessorWrong.h"

GraphType CDGCreator::CreateCDG(SyntaxNode* s)
{
    unique_ptr<NodeProcessor> processor = GetProcessors();
    g = GraphType(0);
    Context context;
    // context.ControlVertex = ...;
    // context.FlowVertex = ...;
    processor->Process(s, context);
    return g;
} 

unique_ptr<NodeProcessor> CDGCreator::GetProcessors(bool isLastInstruction)
{
    NodeProcessor* whileProcessor = new NodeProcessorWhile(*this);
    whileProcessor->SetNext(new NodeProcessorWrong(*this));

    return unique_ptr<NodeProcessor>(whileProcessor);
}

GraphType& CDGCreator::GetGraph()
{
    return g;
}

map<string, string>& CDGCreator::GetAliasesDictionary()
{
    return variableName2variableName;
}

string CDGCreator::getCanonicalName(string s)
{
    while(variableName2variableName.find(s) !=
            variableName2variableName.end())
    {
        auto it = variableName2variableName.find(s);
        if(s == it->second)
            break;
        s = it->second ;
    }
    return s;
}

int& CDGCreator::GetGlobalCallNumber()
{
    return globalCallNumber;
}