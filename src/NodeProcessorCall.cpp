#include "NodeProcessorCall.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorFunction::NodeProcessorFunction() : base()
// {}

NodeProcessorCall::NodeProcessorCall(CDGCreator& cdg) : NodeProcessor(cdg)
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

    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;
    for(size_t i = 0; i < braceNode->Children.size(); i++)
    {
        Context childContext;
        childContext = processor->Process(braceNode->Children[i].get(), myContext);
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
    int& globalCallNumber = CDG.GetGlobalCallNumber();
    string functionName = concatenateStringList(myContext.Uses)+string("_")+std::to_string(globalCallNumber++);

    string variableGetLangName;
    if(callNode->Name == "+"))
        variableGetLangName = "plus";
    else if(callNode->Name == "-"))
        variableGetLangName = "minus";
    else if(callNode->Name == "*"))
        variableGetLangName = "multiplication";
    else if(callNode->Name == "/"))
        variableGetLangName = "division";
    else if(callNode->Name == "%%"))
        variableGetLangName = "modulo";
    else if(callNode->Name == "^"))
        variableGetLangName = "power";
    else if(callNode->Name == "**"))
        variableGetLangName = "power";
    else if(callNode->Name == "%/%"))
        variableGetLangName = "division2";
    else if(callNode->Name == "<"))
        variableGetLangName = "lt";
    else if(callNode->Name == "<="))
        variableGetLangName = "le";
    else if(callNode->Name == "=="))
        variableGetLangName = "eq";
    else if(callNode->Name == "!="))
        variableGetLangName = "neq";
    else if(callNode->Name == ">"))
        variableGetLangName = "gt";
    else if(callNode->Name == ">="))
        variableGetLangName = "ge";
    else if(callNode->Name == "&"))
        variableGetLangName = "andV";
    else if(callNode->Name == "&&"))
        variableGetLangName = "and";
    else if(callNode->Name == "||"))
        variableGetLangName = "or";
    else if(callNode->Name == "|"))
        variableGetLangName = "orV";
    else if(callNode->Name == ":"))
        variableGetLangName = "colon";
    else if(callNode->Name == "["))
        variableGetLangName = "bracket";
    else if(callNode->Name == "[["))
        variableGetLangName = "twoBrackets";
    else
        variableGetLangName = callNode->Name;
    functionName = variableGetLangName + string("_") + functionName;

    myContext.Uses.push_back(callNode->Name);

    if(isLeftAssign && !isLeftSideOfAssign) // ???
    {
        functionName = returnValueVariableName;
    }

    vertex_t node;
    node = boost::add_vertex(g);

    if(callNode->Name == "["))
    {
        if(usesAll == 2)
            g[node].color = color_oneBracketSingle;
        else if(usesAll == 3)
            g[node].color = color_oneBracketDouble;
        else if(usesAll >= 4)
            g[node].color = color_oneBracketTripleOrMore;
    }
    else if(callNode->Name == "[["))
    {
        g[node].color = color_twoBrackets;
    }
    else if(callNode->Name == "$"))
    {
        g[node].color = color_dollar;
    }
    else if(callNode->Name == ":"))
    {
        g[node].color = color_colon;
        // g[node].color = color_comparisonOperator;
    }
    else if(callNode->Name == "+")
            || callNode->Name == "-")
            || callNode->Name == "*")
            || callNode->Name == "/")
            || callNode->Name == "%%")
            || callNode->Name == "^")
            || callNode->Name == "**")
            || callNode->Name == "%/%"))
    {
        g[node].color = color_arithmeticOperator;
    }
    else if((callNode->Name == "&&") ||
            (callNode->Name == "&") ||
            (callNode->Name == "|") || 
            (callNode->Name == "||"))
    {
        g[node].color = color_logicalOperator;
    }
    else if(callNode->Name == ">")
            || (callNode->Name == ">=")
            || (callNode->Name == "<")
            || (callNode->Name == "<=")
            || (callNode->Name == "==")
            || (callNode->Name == "!="))
    {
        g[node].color = color_comparisonOperator;
    }
    else if(callNode->Name == "return"))
    {
        g[node].color = color_return;
    }
    else
    {
        if(usesAll==1 || usesAll==0)
            g[node].color = color_functionZeroArgument;
        else if(usesAll == 2)
            g[node].color = color_functionOneArgument;
        else if(usesAll == 3)
            g[node].color = color_functionTwoArguments;
        else if(usesAll == 4)
            g[node].color = color_functionThreeArguments;
        else if(usesAll >= 5)
            g[node].color = color_functionFourOrMoreArguments;
        else
        {
          ;
        }
    }
    g[node].name = callNode->Name+string("()")+std::to_string(global_CallNumber++);
    g[node].uses = myContext.Uses;
    g[node].lastInstruction = lastInstruction;


    g[node].gen = functionName;
    g[node].functionName = callNode->Name;
    g[node].originalFunctionName = graphUtils::getCanonicalName(getLangName(s), variableName2variableName); //@TODO
    g[node].arguments = arguments; // ???
    g[node].isLeftSideOfAssign = isLeftSideOfAssign;
    g[node].isLeftAssign = isLeftAssign;

    std::pair<edge_t, bool> e = add_edge(myContext.FlowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(myContext.ControlVertex, node, g);
    g[e.first].color = color_control_dependency;

    myContext.Uses.push_back(graphUtils::getCanonicalName(functionName,variableName2variableName)); //@TODO

    myContext.FlowVertex = node;
}