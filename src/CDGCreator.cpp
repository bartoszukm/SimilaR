#include "CDGCreator.h"
#include "NodeProcessorWhile.h"
#include "NodeProcessorWrong.h"
#include "NodeProcessorAssignment.h"
#include "NodeProcessorFunction.h"
#include "NodeProcessorFunctionParameters.h"
#include "NodeProcessorSymbolOrConstant.h"
#include "NodeProcessorIf.h"
#include "NodeProcessorFor.h"
#include "NodeProcessorNext.h"
#include "NodeProcessorBreak.h"
#include "NodeProcessorParenthesis.h"
#include "NodeProcessorBrace.h"
#include "NodeProcessorCall.h"
#include "NodeProcessorIf.h"


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

unique_ptr<NodeProcessor> CDGCreator::GetProcessors(bool isLastInstruction, bool onlyAddToUses, string gen)
{
    NodeProcessor* functionProcessor = new NodeProcessorFunction(*this, gen);
    functionProcessor->SetNext(new NodeProcessorFunctionParameters(*this))
                    ->SetNext(new NodeProcessorSymbolOrConstant(*this, onlyAddToUses, isLastInstruction))
                    ->SetNext(new NodeProcessorIf(*this, isLastInstruction))
                    ->SetNext(new NodeProcessorFor(*this))
                    ->SetNext(new NodeProcessorWhile(*this))
                    ->SetNext(new NodeProcessorNext(*this))
                    ->SetNext(new NodeProcessorBreak(*this))
                    ->SetNext(new NodeProcessorParenthesis(*this, isLastInstruction))
                    ->SetNext(new NodeProcessorBrace(*this, isLastInstruction))
                    ->SetNext(new NodeProcessorCall(*this, gen, isLastInstruction))
                    ->SetNext(new NodeProcessorAssignment(*this, isLastInstruction))
                    ->SetNext(new NodeProcessorWrong(*this));

    return unique_ptr<NodeProcessor>(functionProcessor);
}

GraphType& CDGCreator::GetGraph()
{
    return g;
}

map<string, string>& CDGCreator::GetAliasesDictionary()
{
    return variableName2variableName;
}

string CDGCreator::GetCanonicalName(string s)
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
