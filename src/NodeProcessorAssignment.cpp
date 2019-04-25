#include "NodeProcessorAssignment.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorAssignment::NodeProcessorAssignment() : base()
// {}

NodeProcessorAssignment::NodeProcessorAssignment(CDGCreator& cdg) : NodeProcessor(cdg)
{}

Context NodeProcessorAssignment::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessAssignment(*this, context);
}

Context NodeProcessorAssignment::ProcessAssignment(SyntaxLangNode* assignmentNode,
                    const Context& context)
{
    assignmentNode->Children[0]->ProcessFirstAssignmentChild(*this,
                                                            assignmentNode, 
                                                            assignmentNode->Children[1].get(),
                                                            context);
    // jak lewe lang to call
    // jak prawe lang:
    //    call
    //    jak lewe symbol a prawe dolar, to emplace
    //    jak lewe symbol to koniec
    // jak prawe sym:
    //    jak lewe sym to alias
    // wierzchołek tworzony jest gdy:
    //    lewe to lang
    //    prawe to numeryczne
    //     

    // GraphType& g = CDG.GetGraph();
    // Context myContext;
    // myContext.ControlVertex = context.ControlVertex;
    // myContext.FlowVertex = context.FlowVertex;

    // unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false);
    // Context leftContext = processor->Process(assignmentNode->Children[0].get(), myContext);
    // myContext.FlowVertex = leftContext.FlowVertex;

       

    return myContext;
}

Context NodeProcessorAssignment::MakeAssignmentVertex(string leftVariableName, const Context& context)
{
    GraphType& g = CDG.GetGraph();
    int& globalCallNumber = CDG.GetGlobalCallNumber();

    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;

    vertex_t node;
    node = boost::add_vertex(g);
    g[node].color = color_assignment;
    g[node].name = leftVariableName+string("<-")+std::to_string(globalCallNumber++);
    g[node].uses = context.Uses;
    g[node].arguments = arguments; // ???
    g[node].lastInstruction = lastInstruction; // ???
    g[node].isLeftSideOfAssign = isLeftSideOfAssign; // ???

    g[node].gen = leftVariableName;

    std::pair<edge_t, bool> e = add_edge(myContext.FlowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(myContext.ControlVertex, node, g);
    g[e.first].color = color_control_dependency;


    myContext.Uses.push_back(CDG.GetCanonicalName(leftVariableName));
    myContext.FlowVertex = node;
    
    return myContext;
}

Context NodeProcessorAssignment::TryAddAlias(string left, string right)
{
    map<string, string>& variableName2variableName = CDG.GetAliasesDictionary();
    string ss = right; 
    bool shouldEmplace = true;
    // tutaj sprawdzenie, czy juz takiego aliasu nie bylo wczesniej
    while(variableName2variableName.find(ss) != variableName2variableName.end())
    {
        ss = variableName2variableName[ss];
        if(ss == left)
            shouldEmplace = false;
    }

    if(shouldEmplace && left != ss)
        variableName2variableName.emplace(left, ss);
}

Context NodeProcessorAssignment::MakeAssignmentVertex(SyntaxLangNode* assignmentNode,
                             SyntaxSymbolNode* left,
                             SyntaxConstantNode* right,
                             const Context& context
                             )
{
    return MakeAssignmentVertex(left->Name, context);
}

Context NodeProcessorAssignment::MakeLeftCallAndAssignmentVertex(SyntaxLangNode* assignmentNode,
                             SyntaxLangNode* left,
                             SyntaxConstantNode* right,
                             const Context& context
                             )
{
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;
    myContext.Uses = context.Uses;
    myContext.Uses.push_back(right->Name);

    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false); //leftSideOfAssign = true
    Context leftContext = processor->Process(left, myContext);
    // wstawic to z leftContext uses do mojego uses?
    myContext.Uses.splice(myContext.Uses.end(), leftContext.Uses);
    return MakeAssignmentVertex(left->GetLeftName(), myContext); // czy przepuszczac przez variable2variable?
}

Context NodeProcessorAssignment::MakeLeftCallAndAssignmentVertex(SyntaxLangNode* assignmentNode,
                             SyntaxLangNode* left,
                             SyntaxSymbolNode* right,
                             const Context& context
                             )
{
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;
    myContext.Uses = context.Uses;
    myContext.Uses.push_back(right->Name);

    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false, left->GetLeftName()); //leftSideOfAssign = true
    Context leftContext = processor->Process(left, myContext);
    // wstawic to z leftContext uses do mojego uses?
    myContext.Uses.splice(myContext.Uses.end(), leftContext.Uses);
    return MakeAssignmentVertex(left->GetLeftName(), myContext); // czy przepuszczac przez variable2variable?
}

Context NodeProcessorAssignment::MakeAlias(SyntaxLangNode* assignmentNode,
                             SyntaxSymbolNode* left,
                             SyntaxSymbolNode* right,
                             const Context& context
                             )
{
    // obsluga przypadku a <- b
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;
    TryAddAlias(left->Name, right->Name);
    return myContext;
}

Context NodeProcessorAssignment::MakeAliasAndRightCall(SyntaxLangNode* assignmentNode,
                             SyntaxSymbolNode* left,
                             SyntaxLangNode* right,
                             const Context& context
                             )
{
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;

    TryAddAlias(left->Name, right->GetLeftName());// wyciagamy x z x$a
    
    // tu trzeba zaznaczyc, ze generujemy left->Name
    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false, left->Name); //leftSideOfAssign = true
    Context leftContext = processor->Process(right, myContext);
    return leftContext;
}

Context NodeProcessorAssignment::MakeRightCall(SyntaxLangNode* assignmentNode,
                             SyntaxSymbolNode* left,
                             SyntaxLangNode* right,
                             const Context& context
                             )
{
    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false, left->Name); //leftSideOfAssign = true
    Context rightContext = processor->Process(right, context);
    return rightContext;
}

Context NodeProcessorAssignment::MakeLeftCallRightCallAssignmentVertex(SyntaxLangNode* assignmentNode,
                             SyntaxLangNode* left,
                             SyntaxLangNode* right,
                             const Context& context
                             )
{
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;
    myContext.Uses = context.Uses;

    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false, left->GetLeftName()); //leftSideOfAssign = true
    Context leftContext = processor->Process(left, myContext);
    // wstawic to z leftContext uses do mojego uses?
    myContext.Uses.splice(myContext.Uses.end(), leftContext.Uses);

    Context rightContext = processor->Process(right, myContext);
    myContext.Uses.splice(myContext.Uses.end(), rightContext.Uses); // ???

    return MakeAssignmentVertex(left->GetLeftName(), myContext);
}