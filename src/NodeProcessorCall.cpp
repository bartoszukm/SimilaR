#include "NodeProcessorCall.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorFunction::NodeProcessorFunction() : base()
// {}

NodeProcessorCall::NodeProcessorCall(CDGCreator& cdg, string gen, bool isLastInstruction) : NodeProcessor(cdg), gen(gen), isLastInstruction(isLastInstruction)
{}

Context NodeProcessorCall::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessCall(*this, context);
}

Context NodeProcessorCall::ProcessCall(SyntaxLangNode* callNode, const Context& context)
{
    GraphType& g = CDG.GetGraph();
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;

    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false);

    for(size_t i = 0; i < callNode->Children.size(); i++)
    {
        Context childContext;
        childContext = processor->Process(callNode->Children[i].get(), myContext);
        myContext.FlowVertex = childContext.FlowVertex;
        // https://stackoverflow.com/questions/1449703/how-to-append-a-listt-object-to-another
        myContext.Uses.splice(myContext.Uses.end(), childContext.Uses);
    }


    // wszystkie dzieci przeprocessowac
    // zrobic node dla siebie samego
    // wrzucic takze symbole i stale do uses

    // jak ma sie do tego createNode? Kiedy nie?    
    createNode(callNode, myContext);
    return myContext;

}

void NodeProcessorCall::createNode(SyntaxLangNode* callNode, Context& myContext)
{
    GraphType& g = CDG.GetGraph();
    int& globalCallNumber = CDG.GetGlobalCallNumber();
    string functionName = concatenateStringList(myContext.Uses)+string("_")+std::to_string(globalCallNumber++);

    string variableGetLangName;
    if(callNode->Name == "+")
        variableGetLangName = "plus";
    else if(callNode->Name == "-")
        variableGetLangName = "minus";
    else if(callNode->Name == "*")
        variableGetLangName = "multiplication";
    else if(callNode->Name == "/")
        variableGetLangName = "division";
    else if(callNode->Name == "%%")
        variableGetLangName = "modulo";
    else if(callNode->Name == "^")
        variableGetLangName = "power";
    else if(callNode->Name == "**")
        variableGetLangName = "power";
    else if(callNode->Name == "%/%")
        variableGetLangName = "division2";
    else if(callNode->Name == "<")
        variableGetLangName = "lt";
    else if(callNode->Name == "<=")
        variableGetLangName = "le";
    else if(callNode->Name == "==")
        variableGetLangName = "eq";
    else if(callNode->Name == "!=")
        variableGetLangName = "neq";
    else if(callNode->Name == ">")
        variableGetLangName = "gt";
    else if(callNode->Name == ">=")
        variableGetLangName = "ge";
    else if(callNode->Name == "&")
        variableGetLangName = "andV";
    else if(callNode->Name == "&&")
        variableGetLangName = "and";
    else if(callNode->Name == "||")
        variableGetLangName = "or";
    else if(callNode->Name == "|")
        variableGetLangName = "orV";
    else if(callNode->Name == ":")
        variableGetLangName = "colon";
    else if(callNode->Name == "[")
        variableGetLangName = "bracket";
    else if(callNode->Name == "[[")
        variableGetLangName = "twoBrackets";
    else
        variableGetLangName = callNode->Name;
    functionName = variableGetLangName + string("_") + functionName;

    myContext.Uses.push_back(callNode->Name);

    // if(isLeftAssign && !isLeftSideOfAssign) // ???
    // {
    //     functionName = returnValueVariableName;
    // }

    // jesli to jest czesc assignment, to mamy wymuszona nazwe, ktora generujemy
    // np. x <- f(y), to wtedy wiemy, ze generujemy x
    if(gen != "")
        functionName = gen;

    vertex_t node;
    node = boost::add_vertex(g);

    if(callNode->Name == "[")
    {
        if(callNode->Children.size() == 2)
            g[node].color = color_oneBracketSingle;
        else if(callNode->Children.size() == 3)
            g[node].color = color_oneBracketDouble;
        else if(callNode->Children.size() >= 4)
            g[node].color = color_oneBracketTripleOrMore;
    }
    else if(callNode->Name == "[[")
    {
        g[node].color = color_twoBrackets;
    }
    else if(callNode->Name == "$")
    {
        g[node].color = color_dollar;
    }
    else if(callNode->Name == ":")
    {
        g[node].color = color_colon;
        // g[node].color = color_comparisonOperator;
    }
    else if(callNode->Name == "+"
            || callNode->Name == "-"
            || callNode->Name == "*"
            || callNode->Name == "/"
            || callNode->Name == "%%"
            || callNode->Name == "^"
            || callNode->Name == "**"
            || callNode->Name == "%/%")
    {
        g[node].color = color_arithmeticOperator;
    }
    else if(callNode->Name == "&&" ||
            callNode->Name == "&" ||
            callNode->Name == "|" ||
            callNode->Name == "||")
    {
        g[node].color = color_logicalOperator;
    }
    else if(callNode->Name == ">"
            || callNode->Name == ">="
            || callNode->Name == "<"
            || callNode->Name == "<="
            || callNode->Name == "=="
            || callNode->Name == "!=")
    {
        g[node].color = color_comparisonOperator;
    }
    else if(callNode->Name == "return")
    {
        g[node].color = color_return;
    }
    else
    {
        if(callNode->Children.size()==1 || callNode->Children.size()==0)
            g[node].color = color_functionZeroArgument;
        else if(callNode->Children.size() == 2)
            g[node].color = color_functionOneArgument;
        else if(callNode->Children.size() == 3)
            g[node].color = color_functionTwoArguments;
        else if(callNode->Children.size() == 4)
            g[node].color = color_functionThreeArguments;
        else if(callNode->Children.size() >= 5)
            g[node].color = color_functionFourOrMoreArguments;
        else
        {
          ;
        }
    }

    g[node].name = callNode->Name+string("()")+std::to_string(globalCallNumber++);
    g[node].uses = myContext.Uses;
    g[node].lastInstruction = isLastInstruction;


    g[node].gen = functionName;
    g[node].functionName = callNode->Name;
    g[node].originalFunctionName = CDG.GetCanonicalName(callNode->Name);

    std::pair<edge_t, bool> e = add_edge(myContext.FlowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(myContext.ControlVertex, node, g);
    g[e.first].color = color_control_dependency;

    myContext.Uses.push_back(CDG.GetCanonicalName(functionName)); // czy ten GetCanonicalName ma tu sens?

    myContext.FlowVertex = node;
}
