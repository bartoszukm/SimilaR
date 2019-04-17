#include "NodeProcessorIf.h"
#include "CDGCreator.h"
#include "SyntaxLangNode.h"

// NodeProcessorIf::NodeProcessorIf() : base()
// {}

NodeProcessorIf::NodeProcessorIf(CDGCreator& cdg) : NodeProcessor(cdg)
{}

Context NodeProcessorIf::Process(SyntaxNode* n, const Context& context)
{
    n->ProcessIf(*this, context);
}

Context NodeProcessorIf::ProcessIf(SyntaxLangNode* ifNode, const Context& context)
{
    GraphType& g = CDG.GetGraph();
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;

    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false);

    size_t branchSizeIf = 0;
    size_t branchSizeElse = 0;

    bool isIfReturnBranch = ifNode->Children[1]->IsReturnBranch(branchSizeIf) || lastInstruction;
    bool isElseReturnBranch = false;
    
    if(ifNode->Children.size() == 3)
        ifNode->Children[2]->IsReturnBranch(branchSizeElse) || lastInstruction;

    if(isIfReturnBranch && isElseReturnBranch && branchSizeIf == branchSizeElse)
    {
      branchSizeIf += makeLexicalComparison(if_child, else_child);
    }

    Context predicateContext = processor->Process(ifNode->Children[0], myContext);
    myContext.FlowVertex = predicateContext.FlowVertex;

    node = boost::add_vertex(g);
    g[node].color = color_if;
    g[node].name = "if";
    g[node].lastInstruction = lastInstruction;
    g[node].isLeftSideOfAssign = false;

    e = add_edge(context.ControlVertex, node, g);
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
        g[node2].lastInstruction = lastInstruction;
        g[node2].isLeftSideOfAssign = false;

        e = add_edge(node, node2, g);
        g[e.first].color = color_control_dependency;

        e = add_edge(node, node2, g);
        g[e.first].color = color_control_flow;
        g[node2].uses = predicateContext.Uses;

        // flowVertex = node2;
        // vertex_t* pnode = new vertex_t;
        // *pnode = node2;

        myContext.ControlVertex = node2;
        Context ifContext = processor->Process(ifNode->Children[1], myContext);
        e = add_edge(ifContext.FlowVertex;, node, g);
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
        g[node2].lastInstruction = lastInstruction;
        g[node2].isLeftSideOfAssign = false;

        e = add_edge(node, node2, g);
        g[e.first].color = color_control_dependency;

        e = add_edge(node, node2, g);
        g[e.first].color = color_control_flow;
        g[node2].uses = uses;

        // flowVertex = node2;
        // vertex_t* pnode = new vertex_t;
        // *pnode = node2;

        myContext.ControlVertex = node2;
        Context elseContext = processor->Process(ifNode->Children[2], myContext);
        e = add_edge(elseContext.FlowVertex;, node, g);
        g[e.first].color = color_control_flow;
        myContext.FlowVertex = node;

    }
    else if(ifNode->Children.size() == 3)
    {
        myContext.ControlVertex = context.ControlVertex;
        Context elseContext = processor->Process(ifNode->Children[2], myContext);
    }

    if(remember_if)
    {
        myContext.ControlVertex = context.ControlVertex;
        Context elseContext = processor->Process(ifNode->Children[1], myContext);
    }
}