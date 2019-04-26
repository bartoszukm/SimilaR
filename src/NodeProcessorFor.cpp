#include "NodeProcessorFor.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorWhile::NodeProcessorWhile() : base()
// {}

NodeProcessorFor::NodeProcessorFor(CDGCreator& cdg) : NodeProcessor(cdg)
{}

Context NodeProcessorFor::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessFor(*this, context);
}

Context NodeProcessorFor::ProcessFor(SyntaxLangNode* forNode, const Context& context)
{
    this->forNode = forNode;
    GraphType& g = CDG.GetGraph();
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;
    string gen = forNode->Children[0]->Name;

    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false);
    Context predicateContext = processor->Process(forNode->Children[1].get(), myContext);
    myContext.FlowVertex = predicateContext.FlowVertex;

    // normalne cialo for
    auto [ vertices_count_before, vertices_count_after ] = MakeForBody(context, myContext, predicateContext);

    std::pair<edge_t, bool> e = add_edge(myContext.FlowVertex, node, g);
    g[e.first].color = color_control_flow;
    myContext.FlowVertex = node;
    return myContext;
}

Context NodeProcessorFor::ProcessForeach(SyntaxLangNode* forNode, const Context& context)
{
    this->forNode = forNode;
    GraphType& g = CDG.GetGraph();
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;
    string gen = forNode->Children[0]->Name;

    processor = CDG.GetProcessors(false);
    Context predicateContext = processor->Process(forNode->Children[2].get(), myContext);
    myContext.FlowVertex = predicateContext.FlowVertex;
    MakeArtificialForeachNodes(context, myContext, predicateContext);
    

    // normalne cialo for
    auto [ vertices_count_before, vertices_count_after ] = MakeForBody(context, myContext, predicateContext);

    MakeBracketsInForeach(context, myContext, predicateContext, vertices_count_before, vertices_count_after);

    std::pair<edge_t, bool> e = add_edge(myContext.FlowVertex, node, g);
    g[e.first].color = color_control_flow;
    myContext.FlowVertex = node;
    return myContext;
}

void NodeProcessorFor::MakeArtificialForeachNodes(const Context& context,
                                                  Context& myContext,
                                                  Context& predicateContext)
{
    GraphType& g = CDG.GetGraph();
    // Rcout << "jest Foreach, tworze sztuczne wierzcholki" << endl;

    // jesli to foreach, to dodaj 1:length(to co wczesniej)
    
    // length(x)
    
    int& globalCallNumber = CDG.GetGlobalCallNumber();
    string functionName_length = string("length_")+std::to_string(globalCallNumber++);
    list<string> length_uses, length_arguments;
    length_uses.push_back(predicateContext.Uses.front());
    length_arguments.push_back(predicateContext.Uses.front());
    vertex_t node_length;
    node_length = boost::add_vertex(g);
    g[node_length].color = color_functionZeroArgument;
    g[node_length].name = string("length()")+std::to_string(globalCallNumber++);
    g[node_length].uses = length_uses;
    g[node_length].lastInstruction = false;
    g[node_length].gen = functionName_length;
    g[node_length].functionName = "length";
    g[node_length].originalFunctionName = "length";
    g[node_length].arguments = length_arguments;
    g[node_length].isLeftSideOfAssign = false;
    g[node_length].isLeftAssign = false;
    
    std::pair<edge_t, bool> e = add_edge(myContext.FlowVertex, node_length, g);
    g[e.first].color = color_control_flow;
    
    e = add_edge(context.ControlVertex, node_length, g);
    g[e.first].color = color_control_dependency;
    myContext.FlowVertex = node_length;
    
    // :
    list<string> colon_uses, colon_arguments;
    colon_uses.push_back("1");
    colon_uses.push_back(functionName_length);
    colon_arguments.push_back("1");
    colon_arguments.push_back(functionName_length);
    string functionName_colon = string("colon_")+concatenateStringList(colon_uses)+string("_")+std::to_string(globalCallNumber++);
    vertex_t node_colon;
    node_colon = boost::add_vertex(g);
    g[node_colon].color = color_colon;
    g[node_colon].name = string(":()")+std::to_string(globalCallNumber++);
    g[node_colon].uses = colon_uses;
    g[node_colon].lastInstruction = false;
    g[node_colon].gen = functionName_colon; 
    g[node_colon].functionName = "colon";
    g[node_colon].originalFunctionName = "colon";
    g[node_colon].arguments = colon_arguments;
    g[node_colon].isLeftSideOfAssign = false;
    g[node_colon].isLeftAssign = false;
    
    e = add_edge(myContext.FlowVertex, node_colon, g);
    g[e.first].color = color_control_flow;
    
    e = add_edge(context.ControlVertex, node_colon, g);
    g[e.first].color = color_control_dependency;
    myContext.FlowVertex = node_colon;
    
    predicateContext.Uses.pop_front(); //upewnic sie !!!!!
    predicateContext.Uses.push_back(g[node_colon].gen);
}

std::tuple<size_t, size_t> NodeProcessorFor::MakeForBody(const Context& context, Context& myContext, Context& predicateContext)
{
    GraphType& g = CDG.GetGraph();
    node = boost::add_vertex(g);
    g[node].color = color_header;
    g[node].name = "for";
    g[node].gen = gen;
    g[node].lastInstruction = false; // ???
    std::pair<edge_t, bool>  e;

    e = add_edge(context.ControlVertex, node, g);
    g[e.first].color = color_control_dependency;

    e = add_edge(myContext.FlowVertex, node, g);
    g[e.first].color = color_control_flow;
    g[node].uses = predicateContext.Uses;

    myContext.FlowVertex = node;
    // list<pair<vertex_t*, vertex_t*> > structuredTransfersOfControl;
    size_t vertices_count_before = num_vertices(g);

    Context bodyContext = processor->Process(forNode->Children[2].get(), myContext);

    // makeCDG_rec_cpp_wrapper(s1, returnValueVariableName,
    //                         node,flowVertex,NULL,
    //                         &structuredTransfersOfControl,
    //                         lastInstruction);
    // makeStructuredTransfersOfControlForLoop(
    //     node, &structuredTransfersOfControl);
    size_t vertices_count_after = num_vertices(g);
    myContext.FlowVertex = bodyContext.FlowVertex;
    return make_tuple(vertices_count_before, vertices_count_after);

}

void NodeProcessorFor::MakeBracketsInForeach(const Context& context,
                           Context& myContext,
                           Context& predicateContext,
                           size_t vertices_count_before,
                           size_t vertices_count_after)
{
    int& globalCallNumber = CDG.GetGlobalCallNumber();
    GraphType& g = CDG.GetGraph();
    // przeksztalcenie ze wzgledu na foreach

    //  stworz bracket [[]]: korzysta z wektora wejsciowego, generuje cos, co podamy zamiast argumentu y (std:replace, jak w post)
    list<string> bracket_uses, bracket_arguments;
    string argument_name = predicateContext.Uses.front();
    bracket_uses.push_back(g[node].gen); // zmienna iterujaca
    bracket_uses.push_back(argument_name); //wektor z ktorego bierzemy
    bracket_arguments.push_back(g[node].gen);
    bracket_arguments.push_back(argument_name);
    string functionName_bracket = string("[[_")+concatenateStringList(bracket_uses)+string("_")+std::to_string(globalCallNumber++);
    vertex_t node_bracket;
    node_bracket = boost::add_vertex(g);
    g[node_bracket].color = color_twoBrackets;
    g[node_bracket].name = string("[[()")+std::to_string(globalCallNumber++);
    g[node_bracket].uses = bracket_uses;
    g[node_bracket].lastInstruction = false;
    g[node_bracket].gen = functionName_bracket; 
    g[node_bracket].functionName = "[[";
    g[node_bracket].originalFunctionName = "[[";
    g[node_bracket].arguments = bracket_arguments;
    g[node_bracket].isLeftSideOfAssign = false;
    g[node_bracket].isLeftAssign = false;
    
    std::pair<edge_t, bool> e = add_edge(myContext.FlowVertex, node_bracket, g); //troche watpliwe
    g[e.first].color = color_control_flow;
    
    
    myContext.FlowVertex = node_bracket;
    
    bool isFirstOccurenceFound = false;
    
    for(size_t j=vertices_count_before; j<vertices_count_after; ++j)
    {
        if(std::find(g[j].uses.begin(), g[j].uses.end() , gen) != g[j].uses.end() && !isFirstOccurenceFound)
        {
            isFirstOccurenceFound = true;
            
            boost::graph_traits<GraphType>::in_edge_iterator in_e, in_e_end;
            for (tie(in_e, in_e_end) = in_edges(j, g); in_e != in_e_end; ++in_e)
            {
                if(g[*in_e].color == color_control_dependency)
                {
                    e = add_edge(source(*in_e, g), node_bracket, g);
                    g[e.first].color = color_control_dependency;
                    break;
                }
            }
        }
        
        std::replace (
            g[j].uses.begin(),
            g[j].uses.end(), gen, g[node_bracket].gen);
    }

}
