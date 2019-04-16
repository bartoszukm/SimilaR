#include "NodeProcessorWhile.h"
#include "CDGCreator.h"

// NodeProcessorWhile::NodeProcessorWhile() : base()
// {}

NodeProcessorWhile::NodeProcessorWhile(CDGCreator& cdg) : NodeProcessor(cdg)
{}

Context NodeProcessorWhile::Process(SyntaxNode* n, const Context& context)
{
    n->ProcessWhile(*this, context);
}

Context NodeProcessorWhile::ProcessWhile(SyntaxLangNode* whileNode, const Context& context)
{
    GraphType& g = CDG.GetGraph();
    Context myContext;
    myContext.ControlVertex = context.ControlVertex;
    myContext.FlowVertex = context.FlowVertex;
    
    unique_ptr<NodeProcessor> processor = CDG.GetProcessors(false);
    Context predicateContext = processor->Process(whileNode->Children[0], myContext);
    
        // else if(index == 1)
        // {
        //     if(TYPEOF(CAR(s1)) == SYMSXP)
        //     {

        //         uses.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(CAR(s1))),
        //                                            variableName2variableName));
        //     }
        //     else if(TYPEOF(CAR(s1)) == LANGSXP)
        //     {
        //         makeCallNode(CAR(
        //                          s1), returnValueVariableName,
        //                      controlVertex,
        //                      flowVertex, uses, true, false,false, false, false);
        //     }

        // }

        //context update...

        vertex_t node;
        
        node = boost::add_vertex(g);
        g[node].color = color_header;
        g[node].name = "while";
        g[node].isLeftSideOfAssign = false;
        g[node].lastInstruction = lastInstruction; // ???

        std::pair<edge_t, bool>  e = add_edge(context.ControlVertex, node, g);
        g[e.first].color = color_control_dependency;

        e = add_edge(predicateContext.FlowVertex, node, g);
        g[e.first].color = color_control_flow;
        g[node].uses = predicateContext.Uses;

        myContext.FlowVertex = node;
        Context body_context = processor->Process(whileNode->Children[1], myContext);

        makeStructuredTransfersOfControlForLoop(node, &structuredTransfersOfControl); /// ???

        e = add_edge(body_context.FlowVertex, node, g);
        g[e.first].color = color_control_flow;
        return myContext;
        // else if(index == 2)
        // {
            

        //     flowVertex = node;
        //     list<pair<vertex_t*, vertex_t*> > structuredTransfersOfControl;
        //     makeCDG_rec_cpp_wrapper(s1, returnValueVariableName,
        //                             node,flowVertex,NULL,
        //                             &structuredTransfersOfControl,
        //                             lastInstruction);
            
        // }
    }
}