#include "NodeProcessorIf.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorIf::NodeProcessorIf() : base()
// {}

int NodeProcessorIf::makeLexicalComparison(SyntaxNode *n1, SyntaxNode *n2)
{
    // @TODO
    return 1;
}

NodeProcessorIf::NodeProcessorIf(CDGCreator& cdg, bool isLastInstruction) : NodeProcessor(cdg), isLastInstruction(isLastInstruction)
{}

Context NodeProcessorIf::Process(SyntaxNode* n, const Context& context)
{
    return n->ProcessIf(*this, context);
}

Context NodeProcessorIf::ProcessIf(SyntaxLangNode* ifNode, const Context& context)
{
    GraphType& g = CDG.GetGraph();
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;

    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(isLastInstruction);

    size_t branchSizeIf = 0;
    size_t branchSizeElse = 0;
    bool remember_if = false;

    bool isIfReturnBranch = ifNode->Children[1]->IsReturnBranch(branchSizeIf) || isLastInstruction;
    bool isElseReturnBranch = false;
    
    if(ifNode->Children.size() == 3)
        ifNode->Children[2]->IsReturnBranch(branchSizeElse) || isLastInstruction;

    if(isIfReturnBranch && isElseReturnBranch && branchSizeIf == branchSizeElse)
    {
      branchSizeIf += makeLexicalComparison(ifNode->Children[1].get(), ifNode->Children[2].get());
    }

    Context predicateContext = processor->Process(ifNode->Children[0].get(), myContext);
    myContext.FlowVertex = predicateContext.FlowVertex;

    vertex_t node = boost::add_vertex(g);
    g[node].color = color_if;
    g[node].name = "if";
    g[node].lastInstruction = false; // tutaj trudno powiedzieć, ale no raczej instrukcja pod ifem jest ostatnia, a nie if jako taki

    std::pair<edge_t, bool> e = add_edge(context.ControlVertex, node, g);
    g[e.first].color = color_control_dependency;

    e = add_edge(myContext.FlowVertex, node, g);
    g[e.first].color = color_control_flow;
    g[node].uses = predicateContext.Uses;

    myContext.FlowVertex = node;

    if((!isIfReturnBranch && !isElseReturnBranch) || (isIfReturnBranch && !isElseReturnBranch) || (isIfReturnBranch && isElseReturnBranch && branchSizeIf < branchSizeElse))
    {
        // Rcout << "Tworze ifpart" << endl;
        
        vertex_t node2 = boost::add_vertex(g);
        g[node2].color = color_if_part;
        g[node2].name = "if_part";
        g[node2].lastInstruction = false;

        std::pair<edge_t, bool> e = add_edge(node, node2, g);
        g[e.first].color = color_control_dependency;

        e = add_edge(node, node2, g);
        g[e.first].color = color_control_flow;
//        g[node2].uses = predicateContext.Uses;// mi sie wydaje, ze nie ma sensu zeby on cokolwiek uzywal

        // flowVertex = node2;
        // vertex_t* pnode = new vertex_t;
        // *pnode = node2;

        myContext.ControlVertex = node2;
        Context ifContext = processor->Process(ifNode->Children[1].get(), myContext);
        e = add_edge(ifContext.FlowVertex, node, g);
        g[e.first].color = color_control_flow;
        myContext.FlowVertex = node;
    }
    else
    {
        remember_if = true;
    }
        

    if(ifNode->Children.size() == 3 && (!isIfReturnBranch && !isElseReturnBranch) || (!isIfReturnBranch && isElseReturnBranch) || (isIfReturnBranch && isElseReturnBranch && branchSizeElse <= branchSizeIf)) // zamiast <= to zrobic porzadek liniowy porzadny!!!
    {
        // Rcout << "Tworze ifpart" << endl;
        vertex_t node2 = boost::add_vertex(g);
        g[node2].color = color_if_part;
        g[node2].name = "else_part";
        g[node2].lastInstruction = false;

        std::pair<edge_t, bool> e = add_edge(node, node2, g);
        g[e.first].color = color_control_dependency;

        e = add_edge(node, node2, g);
        g[e.first].color = color_control_flow;
//        g[node2].uses = uses; // mi sie wydaje, ze nie ma sensu zeby on cokolwiek uzywal

        // flowVertex = node2;
        // vertex_t* pnode = new vertex_t;
        // *pnode = node2;

        myContext.ControlVertex = node2;
        Context elseContext = processor->Process(ifNode->Children[2].get(), myContext);
        e = add_edge(elseContext.FlowVertex, node, g);
        g[e.first].color = color_control_flow;
        myContext.FlowVertex = node;

    }
    else if(ifNode->Children.size() == 3)
    {
        myContext.ControlVertex = context.ControlVertex;
        Context elseContext = processor->Process(ifNode->Children[2].get(), myContext);
    }

    if(remember_if)
    {
        myContext.ControlVertex = context.ControlVertex;
        Context elseContext = processor->Process(ifNode->Children[1].get(), myContext);
    }
}
