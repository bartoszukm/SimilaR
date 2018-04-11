/*    SimilaR package for R
 *    Copyright (C) 2018 M. Bartoszuk, M. Gagolewski
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */    

#include "CDG.h"

#include "common.h"
#include <stdlib.h>
#include <stdio.h>


using namespace Rcpp;
using namespace std;
using namespace boost;

string CDGMaker::constantToString(SEXP s)
{
    if (TYPEOF(s) ==  LGLSXP)
    {
        return string((bool)Rcpp::as<Rcpp::LogicalVector>(
                          s)[0] ? "TRUE" : "FALSE");
    }
    else if (TYPEOF(s) ==  INTSXP)
    {
        return std::to_string(INTEGER(s)[0]);
    }
    else if (TYPEOF(s) ==  REALSXP)
    {
        double a = REAL(s)[0];
        if(a == floor(a))
            return std::to_string((int) a);
        else
            return std::to_string(a);
    }
    else if (TYPEOF(s) ==  CPLXSXP)
    {
        return std::to_string(COMPLEX(s)[0].r) + "+" +std::to_string(COMPLEX(
                                                                         s)[0].
                                                                     i);
    }
    else if (TYPEOF(s) ==  STRSXP)
    {
        return string((string) Rcpp::as<Rcpp::CharacterVector>(s)[0]);
    }
    else if(TYPEOF(s) ==  LANGSXP)
    {

        Environment ns = Environment::namespace_env("SimilaR" ) ;
        Rcpp::Function myprint(ns["myprint"]);


        return string(as<CharacterVector>(myprint(s))[0]);
    }
    else
    {
        return string("");
    }
}

string CDGMaker::concatenateStringList(list<string> l)
{
    string result;
    for (std::list<string>::iterator it = l.begin(); it != l.end(); it++)
        result = result + *it;
    return result;
}

const char* CDGMaker::getLangName(SEXP s)
{

    if(TYPEOF(PRINTNAME(s))==CHARSXP)
    {
        return CHAR(PRINTNAME(s));
    }
    if(TYPEOF(PRINTNAME(CAR(s)))==CHARSXP)
    {
        return CHAR(PRINTNAME(CAR(s)));
    }

    if(TYPEOF(CAR(s)) == SYMSXP || TYPEOF(CAR(s)) == LANGSXP ||
       TYPEOF(CAR(s)) == LISTSXP)
    {
        if(TYPEOF(CDR(CAR(s))) == SYMSXP || TYPEOF(CDR(CAR(s))) == LANGSXP ||
           TYPEOF(CDR(CAR(s))) == LISTSXP)
        {
            if(TYPEOF(CDR(CDR(CAR(s)))) == SYMSXP ||
               TYPEOF(CDR(CDR(CAR(s)))) == LANGSXP ||
               TYPEOF(CDR(CDR(CAR(s)))) ==
               LISTSXP)
            {
                if(TYPEOF(CAR(CDR(CDR(CAR(s))))) == SYMSXP ||
                   TYPEOF(CAR(CDR(CDR(CAR(s))))) == LANGSXP ||
                   TYPEOF(CAR(CDR(CDR(CAR(s))))) == LISTSXP)
                {
                    if(PRINTNAME(CAR(CDR(CDR(CAR(s))))) != NULL)
                    {
                        if(TYPEOF(PRINTNAME(CAR(CDR(CDR(CAR(s)))))) != 0)
                        {
                            return CHAR(PRINTNAME(CAR(CDR(CDR(CAR(s))))));
                        }
                    }
                }
            }
        }
    }

    string functionName = string("user_fun")+std::to_string(global_CallNumber++);
    char * cstr = new char [functionName.length()+1];
    std::strcpy (cstr, functionName.c_str());
    return cstr;

}

string CDGMaker::getLeftVariable(SEXP s)
{
    if (TYPEOF(s) == SYMSXP)
    {
        return graphUtils::getCanonicalName(string(CHAR(PRINTNAME(s))),variableName2variableName);
    }
    else if(TYPEOF(s) == LANGSXP)
    {
        return getLeftVariable(CAR(CDR(s)));
    }
    else
    {
      return graphUtils::getCanonicalName(constantToString(s), variableName2variableName);
    }

}

string CDGMaker::getLeftVariable_all(SEXP s,
                                     string returnValueVariableName,
                                     const vertex_t& controlVertex,
                                     vertex_t& flowVertex,
                                     list<string>& uses, bool createNode,
                                     bool lastInstruction)
{

    if (TYPEOF(s) == SYMSXP)
    {
        return string(CHAR(PRINTNAME(s)));
    }
    else if(TYPEOF(s) == LANGSXP)
    {
        list<string> uses;
        return getLeftVariable(CAR(CDR(s)));
    }
    else
    {
        return constantToString(s);
    }

}


bool CDGMaker::isSpecificFunction(SEXP s, const char* fname)
{
    if(TYPEOF(s)==LANGSXP)
    {
        string s1(graphUtils::getCanonicalName(getLangName(s),variableName2variableName));
        string f(fname);

        if(s1.size() != f.size())
            return false;
        for(size_t i = 0; i<s1.size(); ++i)
        {
            if(s1[i]!=f[i]) return false;
        }
        return true;
    }
    else
        return false;
}

bool CDGMaker::isApplyFunction(SEXP s)
{
    if(TYPEOF(s)==LANGSXP)
    {
        if(graphUtils::getCanonicalName(getLangName(s),
                               variableName2variableName) == "lapply"
           || graphUtils::getCanonicalName(getLangName(s),
                                  variableName2variableName) == "apply"
           || graphUtils::getCanonicalName(getLangName(s),
                                  variableName2variableName) == "sapply"
           || graphUtils::getCanonicalName(getLangName(s),
                                  variableName2variableName) == "mapply")
            return true;
        for (SEXP t = CDR(s); t != R_NilValue; t = CDR(t))
        {
            if(isSpecificFunction(CAR(t),"function"))
                return true;
        }
        return false;
    }
    return false;
}

void CDGMaker::makeCDGfromFunction(SEXP obj,
                                   vertex_t*& entryp,
                                   string returnValueVariableName,
                                   vertex_t& flowVertex) {
    if(entryp == NULL)
    {
        entryp = new vertex_t;
        *entryp = (boost::add_vertex(g));
        g[*entryp].color = color_entry;
        g[*entryp].name = "Entry";
        g[*entryp].lastInstruction = false;
        g[*entryp].isLeftSideOfAssign = false;
        g[*entryp].functionName = returnValueVariableName;
    }
    flowVertex = *entryp;
    int index1 = 0;
    for (SEXP s = CDR(obj); s != R_NilValue; s = CDR(s))
    {
        if(index1==0)
        {
            for (SEXP s1 = CAR(s); s1 != R_NilValue; s1 = CDR(s1))
            {



                vertex_t arg = boost::add_vertex(g);
                g[arg].color = color_parameter;

                g[arg].name = CHAR(PRINTNAME(TAG(s1)));
                g[arg].gen = CHAR(PRINTNAME(TAG(s1)));
                g[arg].kill = list<string>();
                g[arg].kill.push_back(CHAR(PRINTNAME(TAG(s1))));
                g[arg].lastInstruction = false;
                g[arg].isLeftSideOfAssign = false;
                g[arg].arguments.push_back(constantToString(CAR(s1)));

                if(TYPEOF(CAR(s1)) == LANGSXP)
                {
                 makeCDG_rec_cpp_wrapper(s1, returnValueVariableName,
                                         *entryp, flowVertex,NULL, NULL,
                                         true);

                }


                std::pair<edge_t, bool> e = add_edge(*entryp, arg, g);
                g[e.first].color = color_control_dependency;

                e = add_edge(flowVertex, arg, g);
                g[e.first].color = color_control_flow;
                flowVertex = arg;
            }
        }
        else if(index1==1)
        {

            makeCDG_rec_cpp_wrapper(s, returnValueVariableName,
                                    *entryp, flowVertex,NULL, NULL,
                                    true);
        }
        index1++;
    }


}

void
CDGMaker::
makeStructuredTransfersOfControlForLoop(const vertex_t& loop,
                                        list<std::pair<vertex_t*,
                                                       vertex_t*> >
                                        *structuredTransfersOfControl)
{
    for (std::list<std::pair<vertex_t*, vertex_t*> >::iterator it =
             structuredTransfersOfControl->begin();
         it != structuredTransfersOfControl->end(); it++)
    {

        std::pair<edge_t, bool> e;

        if((it->first) != NULL)
        {
            if(!edge(*(it->first),loop,g).second)
            {
                e = add_edge(*(it->first), loop, g);
                g[e.first].color = color_control_dependency;
            }

            e = edge(*(it->second),*(it->first),g);
            if(e.second)
            {
                remove_edge(e.first, g);
            }
        }

        e = add_edge(*(it->second), loop, g);
        g[e.first].color = color_control_flow;

    }

    for (std::list<std::pair<vertex_t*, vertex_t*> >::iterator it =
             structuredTransfersOfControl->begin();
         it != structuredTransfersOfControl->end(); it++)
    {
        delete it->first;
        delete it->second;
    }

}

void CDGMaker::makeConstantNode(SEXP s,
                                string returnValueVariableName,
                                const vertex_t& controlVertex,
                                vertex_t& flowVertex)
{

    vertex_t node;
    node = boost::add_vertex(g);
    g[node].color = color_constant;

    Environment ns = Environment::namespace_env("SimilaR" ) ;
    Rcpp::Function myprint(ns["myprint"]);


    g[node].functionName = string(as<CharacterVector>(myprint(s))[0]);
    g[node].functionName = g[node].functionName.substr(4);
    g[node].name = g[node].functionName;

    g[node].gen = "";
    g[node].lastInstruction = true;
    g[node].isLeftSideOfAssign = false;

    std::pair<edge_t, bool> e = add_edge(flowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(controlVertex, node, g);
    g[e.first].color = color_control_dependency;
}

void CDGMaker::makeIfNode(SEXP s,
                          string returnValueVariableName,
                          const vertex_t& controlVertex,
                          vertex_t& flowVertex,
                          list<std::pair<vertex_t*,
                                         vertex_t*> >*
                          structuredTransfersOfControl,
                          bool lastInstruction) {
    int index = 0;
    list<string> uses;
    bool klamra_if = false;
    bool klamra_else = false;


    vertex_t node;
    vertex_t oldControlVertex = controlVertex;
    std::pair<edge_t, bool>  e;

    for(SEXP s1 = s; s1 != R_NilValue; s1 = CDR(s1))
    {
        if(index==0)
        {
            ;

        }
        else if(index == 1)
        {
            if(TYPEOF(CAR(s1)) == SYMSXP)
            {

                uses.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(CAR(s1))),
                                                   variableName2variableName));
            }
            else if(TYPEOF(CAR(s1)) == LANGSXP)
            {
                makeCallNode(CAR(
                                 s1), returnValueVariableName,
                             controlVertex,
                             flowVertex, uses, true, false,false, false, false);
            }


            node = boost::add_vertex(g);
            g[node].color = color_if;
            g[node].name = "if";
            g[node].lastInstruction = lastInstruction;
            g[node].isLeftSideOfAssign = false;

            e = add_edge(oldControlVertex, node, g);
            g[e.first].color = color_control_dependency;

            e = add_edge(flowVertex, node, g);
            g[e.first].color = color_control_flow;
            g[node].uses = uses;

        }
        else if(index == 2)
        {
            if(TYPEOF(CAR(s1))==LANGSXP)
            {

                if (TYPEOF(PRINTNAME(CAR(CAR(s1)))) == CHARSXP && !strcmp(CHAR(PRINTNAME(CAR(CAR(s1)))), "{"))
                {
                    klamra_if = true;
                }
            }

            vertex_t node2 = boost::add_vertex(g);
            g[node2].color = color_if_part;
            g[node2].name = "if_part";
            g[node2].lastInstruction = lastInstruction;
            g[node2].isLeftSideOfAssign = false;

            e = add_edge(node, node2, g);
            g[e.first].color = color_control_dependency;

            e = add_edge(node, node2, g);
            g[e.first].color = color_control_flow;
            g[node2].uses = uses;

            flowVertex = node2;
            vertex_t* pnode = new vertex_t;
            *pnode = node2;


            if(TYPEOF(CAR(s1)) != SYMSXP && TYPEOF(CAR(s1))!=LANGSXP)
            {
                makeConstantNode(CAR(
                                     s1),
                                 returnValueVariableName,node,flowVertex);
            }
            else
                makeCDG_rec_cpp_wrapper(klamra_if ? s1 : ((TYPEOF(s1)==
                                                           SYMSXP ||
                                                           TYPEOF(s1)==
                                                           LANGSXP ||
                                                           TYPEOF(s1)==
                                                           LISTSXP) ? (TYPEOF(
                                                                           CAR(
                                                                               s1))
                                                                       ==
                                                                       SYMSXP
                                                                       ||
                                                                       TYPEOF(
                                                                           CAR(
                                                                               s1))
                                                                       ==
                                                                       LANGSXP
                                                                       ? CAR(
                                                                           s1)
                                                                       : s1) :
                                                          s1),
                                        returnValueVariableName,node2,
                                        flowVertex, pnode,
                                        structuredTransfersOfControl,
                                        lastInstruction);
            e = add_edge(flowVertex, node, g);
            g[e.first].color = color_control_flow;
            flowVertex = node;
        }
        else if(index == 3)
        {

            if(TYPEOF(CAR(s1))==LANGSXP)
            {

                if (TYPEOF(PRINTNAME(CAR(CAR(s1)))) == CHARSXP && !strcmp(CHAR(PRINTNAME(CAR(CAR(s1)))), "{"))
                {
                    klamra_else = true;
                }
            }

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

            flowVertex = node2;
            vertex_t* pnode = new vertex_t;
            *pnode = node2;

            if(TYPEOF(CAR(s1)) != SYMSXP && TYPEOF(CAR(s1))!=LANGSXP)
            {
                makeConstantNode(CAR(
                                     s1),
                                 returnValueVariableName,node,flowVertex);
            }
            else
                makeCDG_rec_cpp_wrapper(klamra_else ? s1 : ((TYPEOF(s1)==
                                                             SYMSXP ||
                                                             TYPEOF(s1)==
                                                             LANGSXP ||
                                                             TYPEOF(s1)==
                                                             LISTSXP) ? (TYPEOF(
                                                                             CAR(
                                                                                 s1))
                                                                         ==
                                                                         SYMSXP
                                                                         ||
                                                                         TYPEOF(
                                                                             CAR(
                                                                                 s1))
                                                                         ==
                                                                         LANGSXP
                                                                         ? CAR(
                                                                             s1)
                                                                         : s1)
                                                            : s1),
                                        returnValueVariableName,node2,
                                        flowVertex, pnode,
                                        structuredTransfersOfControl,
                                        lastInstruction);
            e = add_edge(flowVertex, node, g);
            g[e.first].color = color_control_flow;
            flowVertex = node;

        }
        index++;
    }
}

void CDGMaker::makeForNode(SEXP s,
                           string returnValueVariableName,
                           const vertex_t& controlVertex,
                           vertex_t& flowVertex,
                           bool lastInstruction) {
    int index = 0;
    list<string> uses;
    string gen;

    vertex_t oldControlVertex = controlVertex;

    for(SEXP s1 = s; s1 != R_NilValue; s1 = CDR(s1))
    {
        if(index==0)
        {
            ;

        }
        else if(index == 1)
        {

            gen = CHAR(PRINTNAME(CAR(s1)));
        }
        else if(index == 2)
        {
            if(TYPEOF(CAR(s1)) == SYMSXP)
            {

                uses.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(CAR(s1))),
                                                   variableName2variableName));
            }
            else if(TYPEOF(CAR(s1)) == LANGSXP)
            {
                makeCallNode(CAR(
                                 s1), returnValueVariableName,
                             controlVertex,
                             flowVertex, uses, true, false,false, false, false
                         );
            }

        }
        else if(index == 3)
        {
            vertex_t node;
            node = boost::add_vertex(g);
            g[node].color = color_header;
            g[node].name = "for";
            g[node].gen = gen;
            g[node].isLeftSideOfAssign = false;
            g[node].lastInstruction = lastInstruction;
            std::pair<edge_t, bool>  e;



            e = add_edge(oldControlVertex, node, g);
            g[e.first].color = color_control_dependency;

            e = add_edge(flowVertex, node, g);
            g[e.first].color = color_control_flow;
            g[node].uses = uses;

            flowVertex = node;
            list<pair<vertex_t*, vertex_t*> > structuredTransfersOfControl;
            makeCDG_rec_cpp_wrapper(s1, returnValueVariableName,
                                    node,flowVertex,NULL,
                                    &structuredTransfersOfControl,
                                    lastInstruction);
            makeStructuredTransfersOfControlForLoop(
                node, &structuredTransfersOfControl);
            e = add_edge(flowVertex, node, g);
            g[e.first].color = color_control_flow;
            flowVertex = node;
        }
        index++;
    }

}

void CDGMaker::makeWhileNode(SEXP s,
                             string returnValueVariableName,
                             const vertex_t& controlVertex,
                             vertex_t& flowVertex,
                             bool lastInstruction) {
    int index = 0;
    list<string> uses;

    vertex_t oldControlVertex = controlVertex;

    for(SEXP s1 = s; s1 != R_NilValue; s1 = CDR(s1))
    {
        if(index==0)
        {
            ;

        }
        else if(index == 1)
        {
            if(TYPEOF(CAR(s1)) == SYMSXP)
            {

                uses.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(CAR(s1))),
                                                   variableName2variableName));
            }
            else if(TYPEOF(CAR(s1)) == LANGSXP)
            {
                makeCallNode(CAR(
                                 s1), returnValueVariableName,
                             controlVertex,
                             flowVertex, uses, true, false,false, false, false);
            }

        }
        else if(index == 2)
        {
            vertex_t node;
            node = boost::add_vertex(g);
            g[node].color = color_header;
            g[node].name = "while";
            g[node].isLeftSideOfAssign = false;
            g[node].lastInstruction = lastInstruction;

            std::pair<edge_t, bool>  e = add_edge(oldControlVertex, node, g);
            g[e.first].color = color_control_dependency;

            e = add_edge(flowVertex, node, g);
            g[e.first].color = color_control_flow;
            g[node].uses = uses;

            flowVertex = node;
            list<pair<vertex_t*, vertex_t*> > structuredTransfersOfControl;
            makeCDG_rec_cpp_wrapper(s1, returnValueVariableName,
                                    node,flowVertex,NULL,
                                    &structuredTransfersOfControl,
                                    lastInstruction);
            makeStructuredTransfersOfControlForLoop(
                node, &structuredTransfersOfControl);
            e = add_edge(flowVertex, node, g);
            g[e.first].color = color_control_flow;
            flowVertex = node;
        }
        index++;
    }
}

void CDGMaker::makeApplyNode(SEXP s,
                             string returnValueVariableName,
                             const vertex_t& controlVertex,
                             vertex_t& flowVertex,
                             list<string>& uses,
                             bool createNode,
                             bool lastInstruction,
                             bool isLeftAssign,
                             list<string>* additional_uses) {
    vertex_t* entry = NULL;

    vertex_t oldControlVertex = controlVertex;
    list<string> arguments;
    list<string> my_uses;
    if(additional_uses != nullptr)
      my_uses = *additional_uses;
    for (SEXP t = CDR(s); t != R_NilValue; t = CDR(t))
    {
        if(isSpecificFunction(CAR(t),"function"))
        {
            continue;
        }
        if(TYPEOF(CAR(t))==LANGSXP)
        {
            size_t my_uses_size_before = uses.size();
            makeCallNode(CAR(
                             t), returnValueVariableName,
                         controlVertex, flowVertex,
                         my_uses, true, false,false, false, false);
            size_t my_uses_size_after = uses.size();

            auto it = uses.begin();
            for(size_t i = 0; i < my_uses_size_before; ++i)
                ++it;

            for(size_t i = my_uses_size_before; i < my_uses_size_after; ++i)
            {
                arguments.push_back(*it);
                ++it;
            }
        }
        else if(TYPEOF(CAR(t))==SYMSXP)
        {

            my_uses.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(CAR(t))),
                                                  variableName2variableName));
            arguments.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(CAR(t))),
                                                    variableName2variableName));
        }
        else
        {
            my_uses.push_back(constantToString(CAR(t)));
            arguments.push_back(constantToString(CAR(t)));
        }

    }

    vertex_t node;
    node = boost::add_vertex(g);
    g[node].color = color_header;
    g[node].name = "apply";
    g[node].isLeftSideOfAssign = false;

    std::pair<edge_t, bool>  e = add_edge(oldControlVertex, node, g);
    g[e.first].color = color_control_dependency;



    e = add_edge(flowVertex, node, g);
    g[e.first].color = color_control_flow;
    flowVertex = node;
    int functionIndex = 0;
    for (SEXP t = CDR(s); t != R_NilValue; t = CDR(t))
    {
        if(isSpecificFunction(CAR(t),"function"))
        {
            flowVertex = node;
            entry = &node;
            makeCDGfromFunction(CAR(
                                    t), entry,
                                returnValueVariableName, flowVertex);
            g[node].functionPosition = functionIndex;
        }
        functionIndex++;
    }
    g[node].uses = my_uses;
    g[node].lastInstruction = lastInstruction;
    string functionName = concatenateStringList(my_uses);
    functionName = getLangName(s) + functionName;
    uses.push_back(graphUtils::getCanonicalName(functionName,variableName2variableName));
    if(isLeftAssign)
    {
        functionName = returnValueVariableName;
    }
    g[node].gen = functionName;
    g[node].functionName = getLangName(s);
    g[node].originalFunctionName = graphUtils::getCanonicalName(getLangName(
                                                           s),
                                                       variableName2variableName);
    g[node].arguments = arguments;
    e = add_edge(flowVertex, node, g);
    g[e.first].color = color_control_flow;
    flowVertex = node;
}

void CDGMaker::makeNameSymbolNode(SEXP s,

                                  string returnValueVariableName,
                                  const vertex_t& controlVertex,
                                  vertex_t& flowVertex,
                                  bool lastInstruction)
{
    list<string> uses;
    vertex_t node;
    node = boost::add_vertex(g);
    g[node].color = color_symbol;
    g[node].name = getLangName(s);
    uses.push_back(getLangName(s));
    g[node].uses = uses;
    g[node].gen = "";
    g[node].lastInstruction = lastInstruction;
    g[node].isLeftSideOfAssign = false;

    std::pair<edge_t, bool> e = add_edge(flowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(controlVertex, node, g);
    g[e.first].color = color_control_dependency;
}

void CDGMaker::createNodeForCallNode(SEXP s,
                                     string returnValueVariableName,
                                     const vertex_t& controlVertex,
                                     vertex_t& flowVertex,
                                     list<string>& my_uses,
                                     list<string>& uses,
                                     bool lastInstruction,
                                     bool isLeftAssign,
                                     int usesAll,
                                     list<string>& arguments,
                                     bool isLeftSideOfAssign
                                     )
{
    string functionName = concatenateStringList(my_uses)+string("_")+std::to_string(global_CallNumber++);

    string variableGetLangName;
    if(!strcmp(getLangName(s), "+"))
        variableGetLangName = "plus";
    else if(!strcmp(getLangName(s), "-"))
        variableGetLangName = "minus";
    else if(!strcmp(getLangName(s), "*"))
        variableGetLangName = "multiplication";
    else if(!strcmp(getLangName(s), "/"))
        variableGetLangName = "division";
    else if(!strcmp(getLangName(s), "%%"))
        variableGetLangName = "modulo";
    else if(!strcmp(getLangName(s), "^"))
        variableGetLangName = "power";
    else if(!strcmp(getLangName(s), "**"))
        variableGetLangName = "power";
    else if(!strcmp(getLangName(s), "%/%"))
        variableGetLangName = "division2";
    else if(!strcmp(getLangName(s), "<"))
        variableGetLangName = "lt";
    else if(!strcmp(getLangName(s), "<="))
        variableGetLangName = "le";
    else if(!strcmp(getLangName(s), "=="))
        variableGetLangName = "eq";
    else if(!strcmp(getLangName(s), "!="))
        variableGetLangName = "neq";
    else if(!strcmp(getLangName(s), ">"))
        variableGetLangName = "gt";
    else if(!strcmp(getLangName(s), ">="))
        variableGetLangName = "ge";
    else if(!strcmp(getLangName(s), "&"))
        variableGetLangName = "andV";
    else if(!strcmp(getLangName(s), "&&"))
        variableGetLangName = "and";
    else if(!strcmp(getLangName(s), "||"))
        variableGetLangName = "or";
    else if(!strcmp(getLangName(s), "|"))
        variableGetLangName = "orV";
    else if(!strcmp(getLangName(s), ":"))
        variableGetLangName = "colon";
    else if(!strcmp(getLangName(s), "["))
        variableGetLangName = "bracket";
    else if(!strcmp(getLangName(s), "[["))
        variableGetLangName = "twoBrackets";
    else
        variableGetLangName = getLangName(s);
    functionName = variableGetLangName + string("_") + functionName;

    my_uses.push_back(getLangName(s));

    if(isLeftAssign && !isLeftSideOfAssign)
    {
        functionName = returnValueVariableName;
    }

    vertex_t node;
    node = boost::add_vertex(g);

    if(!strcmp(getLangName(s), "["))
    {
        if(usesAll == 2)
            g[node].color = color_oneBracketSingle;
        else if(usesAll == 3)
            g[node].color = color_oneBracketDouble;
        else if(usesAll >= 4)
            g[node].color = color_oneBracketTripleOrMore;
    }
    else if(!strcmp(getLangName(s), "[["))
    {
        g[node].color = color_twoBrackets;
    }
    else if(!strcmp(getLangName(s), "$"))
    {
        g[node].color = color_dollar;
    }
    else if(!strcmp(getLangName(s), ":"))
    {
        g[node].color = color_colon;
    }
    else if(!strcmp(getLangName(s), "+")
            || !strcmp(getLangName(s), "-")
            || !strcmp(getLangName(s), "*")
            || !strcmp(getLangName(s), "/")
            || !strcmp(getLangName(s), "%%")
            || !strcmp(getLangName(s), "^")
            || !strcmp(getLangName(s), "**")
            || !strcmp(getLangName(s), "%/%"))
    {
        g[node].color = color_arithmeticOperator;
    }
    else if(!strcmp(getLangName(s),
                    "&&") ||
            !strcmp(getLangName(s),
                    "&") ||
            !strcmp(getLangName(s), "|") || !strcmp(getLangName(s), "||"))
    {
        g[node].color = color_logicalOperator;
    }
    else if(!strcmp(getLangName(s), ">")
            || !strcmp(getLangName(s), ">=")
            || !strcmp(getLangName(s), "<")
            || !strcmp(getLangName(s), "<=")
            || !strcmp(getLangName(s), "==")
            || !strcmp(getLangName(s), "!="))
    {
        g[node].color = color_comparisonOperator;
    }
    else if(!strcmp(getLangName(s), "return"))
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
    g[node].name = getLangName(s)+string("()")+std::to_string(global_CallNumber++);
    g[node].uses = my_uses;
    g[node].lastInstruction = lastInstruction;


    g[node].gen = functionName;
    g[node].functionName = getLangName(s);
    g[node].originalFunctionName = graphUtils::getCanonicalName(getLangName(
                                                           s),
                                                       variableName2variableName);
    g[node].arguments = arguments;
    g[node].isLeftSideOfAssign = isLeftSideOfAssign;
    g[node].isLeftAssign = isLeftAssign;

    std::pair<edge_t, bool> e = add_edge(flowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(controlVertex, node, g);
    g[e.first].color = color_control_dependency;

    uses.push_back(graphUtils::getCanonicalName(functionName,variableName2variableName));

    flowVertex = node;
}

void CDGMaker::makeStopIfNotNodesIfNecessary(SEXP s,
                                             SEXP t,
                                             string returnValueVariableName,
                                             const vertex_t& controlVertex,
                                             vertex_t& flowVertex,
                                             list<string>& my_uses,
                                             list<string>& uses,
                                             list<string>& arguments,
                                             bool lastInstruction,
                                             bool isLeftAssign,
                                             bool isStopifnotCall
                                             )
{
    if(isSpecificFunction(s,
                          "stopifnot") &&
       !isSpecificFunction(CAR(t), "&&") && !isSpecificFunction(CAR(t), "&"))
    {
        createNodeForCallNode(s,returnValueVariableName, controlVertex,
                              flowVertex, my_uses, uses,
                              lastInstruction,
                              isLeftAssign,
                              1,
                              arguments,
                              false
                          );
        g[flowVertex].color = color_stopifnot;
    }
    if(isSpecificFunction(s, "stopifnot"))
    {
        my_uses.clear();
        arguments.clear();
    }
    else if(isStopifnotCall &&
            (isSpecificFunction(s,
                                "&&") ||
             isSpecificFunction(s,
                                "&"))&&
            !(isSpecificFunction(CAR(t),
                                 "&&") || isSpecificFunction(CAR(t), "&")))
    {
        createNodeForCallNode(stopifnotSEXP,returnValueVariableName,
                              controlVertex,flowVertex, my_uses, uses,
                              lastInstruction, isLeftAssign,
                              1, arguments,
                              false);
        my_uses.clear();
        arguments.clear();
        g[flowVertex].color = color_stopifnot;
    }
    if(isStopifnotCall &&
       (isSpecificFunction(s, "&&") || isSpecificFunction(s, "&")))
    {
        my_uses.clear();
        arguments.clear();
    }
}

void CDGMaker::makeAssignmentNode(SEXP s,
                                  string returnValueVariableName,
                                  const vertex_t& controlVertex,
                                  vertex_t& flowVertex,
                                  list<string>& uses,
                                  bool createNode,
                                  bool lastInstruction,
                                  bool isLeftAssign,
                                  bool isStopifnotCall,
                                  bool isLeftSideOfAssign
                              )
{
    list<string> arguments;
    SEXP left = CAR(CDR(s));
    if(TYPEOF(left)==LANGSXP)
    {
        size_t my_uses_size_before = uses.size();
        makeCallNode(left,
                     getLeftVariable(CAR(CDR(s))),
                     controlVertex,
                     flowVertex, uses,
                     true,
                     true,
                     true,
                     isStopifnotCall,
                     true
                 );
        size_t my_uses_size_after = uses.size();

        auto it = uses.begin();
        for(size_t i = 0; i < my_uses_size_before; ++i)
            ++it;

        for(size_t i = my_uses_size_before; i < my_uses_size_after; ++i)
        {
            arguments.push_back(*it);
            ++it;
        }
    }

    SEXP right = CAR(CDR(CDR(s)));
    if(TYPEOF(right)==LANGSXP)
    {
        if(TYPEOF(left) == SYMSXP && isSpecificFunction(right, "$"))
        {

          string ss = getLeftVariable(right);
          bool shouldEmplace = true;
          while(variableName2variableName.find(ss) !=
                variableName2variableName.end())
          {
              ss = variableName2variableName[ss];
              if(ss == getLeftVariable(CAR(CDR(s))))
                  shouldEmplace = false;
          }

          if(shouldEmplace && getLeftVariable(CAR(CDR(s))) != ss)
          {
            variableName2variableName.emplace(getLeftVariable(CAR(CDR(s))), ss);
          }
        }

        size_t my_uses_size_before = uses.size();
        makeCallNode(right,
                     getLeftVariable(CAR(CDR(s))),
                     controlVertex, flowVertex, uses,
                     true,
                     lastInstruction,
                     true,
                     isStopifnotCall);
        size_t my_uses_size_after = uses.size();

        auto it = uses.begin();
        for(size_t i = 0; i < my_uses_size_before; ++i)
            ++it;

        for(size_t i = my_uses_size_before; i < my_uses_size_after; ++i)
        {
            arguments.push_back(*it);
            ++it;
        }
        if(TYPEOF(left)!=LANGSXP)
          return;
    }
    else if(TYPEOF(right)==SYMSXP)
    {



        string ss = CHAR(PRINTNAME(right));
        bool shouldEmplace = true;
        while(variableName2variableName.find(ss) !=
              variableName2variableName.end())
        {
            ss = variableName2variableName[ss];
            if(ss == getLeftVariable(CAR(CDR(s))))
                shouldEmplace = false;
        }

        if(TYPEOF(left) == SYMSXP)
        {
        if(shouldEmplace && getLeftVariable(CAR(CDR(s))) != CHAR(PRINTNAME(right)))
            variableName2variableName.emplace(getLeftVariable(CAR(CDR(s))), CHAR(PRINTNAME(right)));

          return;
          }
        uses.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(right)),
                                           variableName2variableName));
        arguments.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(right)),
                                                variableName2variableName));
    }
    else
    {
        ;
        uses.push_back(constantToString(right));
        arguments.push_back(constantToString(right));
    }



    string leftVariableName =
        isLeftAssign ? getLeftVariable(CAR(CDR(s))) : getLeftVariable_all(
            CAR(CDR(s)),"",controlVertex,flowVertex,uses,
            false
        );


    vertex_t node;
    node = boost::add_vertex(g);
    g[node].color = color_assignment;
    g[node].name = leftVariableName+string("<-")+std::to_string(global_CallNumber++);
    g[node].uses = uses;
    g[node].arguments = arguments;
    g[node].lastInstruction = lastInstruction;
    g[node].isLeftSideOfAssign = isLeftSideOfAssign;

    g[node].gen = leftVariableName;

    std::pair<edge_t, bool> e = add_edge(flowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(controlVertex, node, g);
    g[e.first].color = color_control_dependency;

    uses = list<string>();
    uses.push_back(graphUtils::getCanonicalName(leftVariableName,
                                       variableName2variableName));
    flowVertex = node;
}

void CDGMaker::makeNormalFunctionCallNode(SEXP s,
                                          string returnValueVariableName,
                                          const vertex_t& controlVertex,
                                          vertex_t& flowVertex,
                                          list<string>& uses,
                                          bool createNode,
                                          bool lastInstruction,
                                          bool isLeftAssign,
                                          bool isStopifnotCall,
                                          bool isLeftSideOfAssign
                                      )
{
    int usesAll = 0;
    list<string> my_uses;
    list<string> arguments;
    for (SEXP t = CDR(s); t != R_NilValue; t = CDR(t))
    {
        if(TYPEOF(CAR(t))==LANGSXP)
        {
            if(isSpecificFunction(s, "stopifnot"))
                stopifnotSEXP = s;

            size_t my_uses_size_before = my_uses.size();
            makeCallNode(CAR(
                             t), returnValueVariableName,
                         controlVertex,
                         flowVertex, my_uses,
                         true,
                         false,
                         false,
                         isSpecificFunction(s,"stopifnot")||isStopifnotCall,
                         false
                     );
            size_t my_uses_size_after = my_uses.size();

            auto it = my_uses.begin();
            for(size_t i = 0; i < my_uses_size_before; ++i)
                ++it;

            for(size_t i = my_uses_size_before; i < my_uses_size_after;
                ++i)
            {
                arguments.push_back(*it);
                ++it;
            }

            makeStopIfNotNodesIfNecessary(s,t,returnValueVariableName,
                                          controlVertex, flowVertex,
                                          my_uses, uses, arguments,
                                          lastInstruction, isLeftAssign,
                                          isStopifnotCall);
        }
        else if(TYPEOF(CAR(t))==SYMSXP)
        {
            if(isSpecificFunction(s, "stopifnot"))
                stopifnotSEXP = s;

            my_uses.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(CAR(t))),
                                                  variableName2variableName));
            arguments.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(CAR(
                                                                       t))),
                                                    variableName2variableName));
            makeStopIfNotNodesIfNecessary(s,t,returnValueVariableName,
                                          controlVertex, flowVertex,
                                          my_uses, uses, arguments,
                                          lastInstruction, isLeftAssign,
                                          isStopifnotCall);
        }
        else
        {
            my_uses.push_back(constantToString(CAR(t)));
            arguments.push_back(constantToString(CAR(t)));
        }
        usesAll++;
    }

    if(createNode &&
       !isSpecificFunction(s,
                           "stopifnot") &&
       !(isStopifnotCall && isSpecificFunction(s, "&&")))
    {
        createNodeForCallNode(s,returnValueVariableName,
                              controlVertex,flowVertex, my_uses, uses,
                              lastInstruction,
                              isLeftAssign, usesAll, arguments,
                              isLeftSideOfAssign);
    }
    else
    {
        uses.insert(uses.end(), my_uses.begin(), my_uses.end());
    }
}


void CDGMaker::makeDplyrNode(SEXP s,
                                          string returnValueVariableName,
                                          const vertex_t& controlVertex,
                                          vertex_t& flowVertex,
                                          list<string>& uses,
                                          bool createNode,
                                          bool lastInstruction,
                                          bool isLeftAssign,
                                          bool isStopifnotCall,
                                          bool isLeftSideOfAssign
                                      )
{
    int usesAll = 0;
    list<string> my_uses;
    list<string> arguments;
    int whichArgument = 0;

    SEXP leftArgument = CDR(s);
    s = CAR(CDR(CDR(s)));

    for (SEXP t = CDR(s); t != R_NilValue || whichArgument == 0; t = CDR(t))
    {
        whichArgument++;
        if(whichArgument == 1)
        {
          t = leftArgument;
        }

        if(TYPEOF(CAR(t))==LANGSXP)
        {
            if(isSpecificFunction(s, "stopifnot"))
                stopifnotSEXP = s;

            size_t my_uses_size_before = my_uses.size();
            makeCallNode(CAR(
                             t), returnValueVariableName,
                         controlVertex,
                         flowVertex, my_uses,
                         true,
                         isSpecificFunction(s,"return") && whichArgument == 1,
                         false,
                         isSpecificFunction(s,"stopifnot")||isStopifnotCall,
                         false
                     );
            size_t my_uses_size_after = my_uses.size();

            auto it = my_uses.begin();
            for(size_t i = 0; i < my_uses_size_before; ++i)
                ++it;

            for(size_t i = my_uses_size_before; i < my_uses_size_after;
                ++i)
            {
                arguments.push_back(*it);
                ++it;
            }

            makeStopIfNotNodesIfNecessary(s,t,returnValueVariableName,
                                          controlVertex, flowVertex,
                                          my_uses, uses, arguments,
                                          lastInstruction, isLeftAssign,
                                          isStopifnotCall);
        }
        else if(TYPEOF(CAR(t))==SYMSXP)
        {
            if(isSpecificFunction(s, "stopifnot"))
                stopifnotSEXP = s;

            my_uses.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(CAR(t))),
                                                  variableName2variableName));
            arguments.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(CAR(
                                                                       t))),
                                                    variableName2variableName));
            makeStopIfNotNodesIfNecessary(s,t,returnValueVariableName,
                                          controlVertex, flowVertex,
                                          my_uses, uses, arguments,
                                          lastInstruction, isLeftAssign,
                                          isStopifnotCall);
        }
        else
        {
            my_uses.push_back(constantToString(CAR(t)));
            arguments.push_back(constantToString(CAR(t)));
        }
        usesAll++;
        if(whichArgument == 1)
        {
          t = s;

          if(isApplyFunction(s))
          {
            break;
          }
        }
    }


    if(isApplyFunction(s))
    {
        makeApplyNode(s, returnValueVariableName,
                      controlVertex, flowVertex, my_uses, createNode,
                      lastInstruction,
                      isLeftAssign,
                      &my_uses);
    }
    else if(createNode &&
       !isSpecificFunction(s,
                           "stopifnot") &&
       !(isStopifnotCall && isSpecificFunction(s, "&&")) && !isSpecificFunction(s,
                           "return"))
    {
        createNodeForCallNode(s,returnValueVariableName,
                              controlVertex,flowVertex, my_uses, uses,
                              lastInstruction,
                              isLeftAssign, usesAll, arguments,
                              isLeftSideOfAssign);
    }
    else if(graphUtils::getCanonicalName(getLangName(s),
                           variableName2variableName) == "return")
    {

      if(TYPEOF(CAR(leftArgument)) == SYMSXP)
      {
          makeNameSymbolNode(CAR(leftArgument),
                             returnValueVariableName,controlVertex,flowVertex,
                             true);
      }
      else if(TYPEOF(CAR(leftArgument)) != LANGSXP)
      {
          makeConstantNode(CAR(leftArgument), returnValueVariableName,
                           controlVertex,flowVertex);
      }

    }
    else
    {
        uses.insert(uses.end(), my_uses.begin(), my_uses.end());
    }
}

void CDGMaker::makeCallNode(SEXP s,
                            string returnValueVariableName,
                            const vertex_t& controlVertex,
                            vertex_t& flowVertex,
                            list<string>& uses,
                            bool createNode,
                            bool lastInstruction,
                            bool isLeftAssign,
                            bool isStopifnotCall,
                            bool isLeftSideOfAssign)
{


    if(TYPEOF(s)!=LANGSXP && TYPEOF(s)!=SYMSXP)
        return;
    if(TYPEOF(s)==SYMSXP)
    {

        uses.push_back(graphUtils::getCanonicalName(CHAR(PRINTNAME(s)),
                                           variableName2variableName));
        return;
    }
    if(!strcmp(getLangName(s),
               "(") ||
       graphUtils::getCanonicalName(getLangName(s),
                           variableName2variableName) == "return")
    {
        if(graphUtils::getCanonicalName(getLangName(s),
                               variableName2variableName) == "return" &&
           TYPEOF(CAR(CDR(s))) == SYMSXP)
        {
            makeNameSymbolNode(CAR(CDR(
                                       s)),
                               returnValueVariableName,controlVertex,flowVertex,
                               true);
        }
        else if(graphUtils::getCanonicalName(getLangName(s),
                               variableName2variableName) == "return" &&
           TYPEOF(CAR(CDR(s))) != LANGSXP)
        {
            makeConstantNode(CAR(CDR(s)), returnValueVariableName,
                             controlVertex,flowVertex);
        }
        else if(graphUtils::getCanonicalName(getLangName(s),
                               variableName2variableName) == "return" &&
           TYPEOF(CAR(CDR(s))) == LANGSXP)
           {
             makeCallNode(CAR(CDR(
                                  s)),returnValueVariableName,
                          controlVertex, flowVertex,
                          uses, createNode, true, isLeftAssign,
                          isStopifnotCall);

           }
        else
        {
            makeCallNode(CAR(CDR(
                                 s)),returnValueVariableName,
                         controlVertex, flowVertex,
                         uses, createNode, lastInstruction, isLeftAssign,
                         isStopifnotCall);
        }
    }
    else if(!strcmp(getLangName(s), "<-") || !strcmp(getLangName(s), "="))
    {
        makeAssignmentNode(s, returnValueVariableName, controlVertex,
                           flowVertex,
                           uses, createNode, lastInstruction, isLeftAssign,
                           isStopifnotCall,
                           isLeftSideOfAssign);
    }
    else
    {
        if(isApplyFunction(s))
        {
            makeApplyNode(s, returnValueVariableName,
                          controlVertex, flowVertex, uses, createNode,
                          lastInstruction,
                          isLeftAssign);
        }
        else if(isSpecificFunction(s,"%>%"))
        {


          if(TYPEOF(CAR(CDR(CDR(s)))) == LANGSXP && graphUtils::getCanonicalName(getLangName(CAR(CDR(CDR(s)))),
                                 variableName2variableName) != "{")
          {
            makeDplyrNode(s, returnValueVariableName,
                                     controlVertex,
                                     flowVertex,
                                     uses, createNode, lastInstruction,
                                     isLeftAssign,
                                     isStopifnotCall,
                                     isLeftSideOfAssign);
           }
           else
           {

                 makeNormalFunctionCallNode(s, returnValueVariableName,
                                            controlVertex,
                                            flowVertex,
                                            uses, createNode, lastInstruction,
                                            isLeftAssign,
                                            isStopifnotCall,
                                            isLeftSideOfAssign);
           }
        }
        else if(graphUtils::getCanonicalName(getLangName(s),
                               variableName2variableName) == "{")
        {
          makeCDG_rec_cpp_wrapper(CDR(s), returnValueVariableName,
                                         controlVertex, flowVertex,NULL, NULL,
                                         true);
        }
        else
        {

            makeNormalFunctionCallNode(s, returnValueVariableName,
                                       controlVertex,
                                       flowVertex,
                                       uses, createNode, lastInstruction,
                                       isLeftAssign,
                                       isStopifnotCall,
                                       isLeftSideOfAssign);
        }
    }
}

void CDGMaker::makeNextNode(SEXP s,
                            string returnValueVariableName,
                            const vertex_t& controlVertex,
                            vertex_t& flowVertex,
                            vertex_t* if_node,
                            list<pair<vertex_t*,
                                      vertex_t*> >*
                            structuredTransfersOfControl)
{
    vertex_t node;
    node = boost::add_vertex(g);
    g[node].color = color_next;
    g[node].name = "next";
    g[node].lastInstruction = false;
    g[node].isLeftSideOfAssign = false;

    std::pair<edge_t, bool> e = add_edge(flowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(controlVertex, node, g);
    g[e.first].color = color_control_dependency;

    vertex_t* pnode = new vertex_t;
    *pnode = node;
    if(structuredTransfersOfControl != NULL)
        structuredTransfersOfControl->push_back(make_pair(if_node,pnode));

    flowVertex = node;

}

void CDGMaker::makeBreakNode(SEXP s,
                             string returnValueVariableName,
                             const vertex_t& controlVertex,
                             vertex_t& flowVertex,
                             vertex_t* if_node,
                             list<pair<vertex_t*,
                                       vertex_t*> >*
                             structuredTransfersOfControl)
{
    vertex_t node;
    node = boost::add_vertex(g);
    g[node].color = color_break;
    g[node].name = "break";
    g[node].isLeftSideOfAssign = false;
    g[node].lastInstruction = false;

    std::pair<edge_t, bool> e = add_edge(flowVertex, node, g);
    g[e.first].color = color_control_flow;

    e = add_edge(controlVertex, node, g);
    g[e.first].color = color_control_dependency;

    vertex_t* pnode = new vertex_t;
    *pnode = node;
    if(structuredTransfersOfControl != NULL)
        structuredTransfersOfControl->push_back(make_pair(if_node,pnode));

    flowVertex = node;

}

void CDGMaker::makeCDG_rec_cpp(SEXP t,
                               string returnValueVariableName,
                               const vertex_t& controlVertex,
                               vertex_t& flowVertex,
                               vertex_t* if_node,
                               list<pair<vertex_t*,
                                         vertex_t*> >*
                               structuredTransfersOfControl,
                               bool lastInstruction) {

    if (t == R_NilValue) {
        return;
    }


    for(SEXP s1 = t; s1 != R_NilValue;
        s1 = TYPEOF(t)==LISTSXP ? CDR(s1) : R_NilValue)
    {
        SEXP s = s1;

        if(TYPEOF(s) != SYMSXP && TYPEOF(s) != LANGSXP)
            s = CAR(s1);

        if (TYPEOF(s) == SYMSXP)
        {
            makeNameSymbolNode(s, returnValueVariableName,
                               controlVertex,flowVertex,
                               !lastInstruction ? false : ((TYPEOF(
                                                                t)
                                                            ==LISTSXP ? CDR(s1)
                                                            : R_NilValue) ==
                                                           R_NilValue));
        }
        else if(TYPEOF(s) == LANGSXP)
        {
            if (!strcmp(getLangName(s), "for")) {
                makeForNode(s, returnValueVariableName,
                            controlVertex,flowVertex,
                            false);
            } else if (!strcmp(getLangName(s), "while")) {
                makeWhileNode(s, returnValueVariableName,
                              controlVertex,flowVertex,
                              false);
            } else if (!strcmp(getLangName(s), "repeat")) {
                ;
            } else if (!strcmp(getLangName(s), "next")) {
                makeNextNode(s, returnValueVariableName,
                             controlVertex, flowVertex, if_node,
                             structuredTransfersOfControl);

            }else if (!strcmp(getLangName(s), "break")) {
                makeBreakNode(s, returnValueVariableName,
                              controlVertex, flowVertex, if_node,
                              structuredTransfersOfControl);

            } else if (!strcmp(getLangName(s), "if")) {
                makeIfNode(s, returnValueVariableName,
                           controlVertex,flowVertex,
                           structuredTransfersOfControl,
                           !lastInstruction ? false : ((TYPEOF(
                                                            t)==
                                                        LISTSXP ? CDR(s1) :
                                                        R_NilValue) ==
                                                       R_NilValue));
            } else if (!strcmp(getLangName(s), "<-")
                       || !strcmp(getLangName(s), "=")) {
                list<string> uses;
                makeCallNode(s, returnValueVariableName,
                             controlVertex,flowVertex, uses,
                             true,
                             !lastInstruction ? false : ((TYPEOF(
                                                              t)==
                                                          LISTSXP ? CDR(s1) :
                                                          R_NilValue) ==
                                                         R_NilValue));
            } else if (!strcmp(getLangName(s), "(")) {
                makeCDG_rec_cpp(CDR(
                                    s), returnValueVariableName,
                                controlVertex, flowVertex,
                                if_node,structuredTransfersOfControl,
                                !lastInstruction ? false : ((TYPEOF(t)==
                                                             LISTSXP ? CDR(s1)
                                                             : R_NilValue) ==
                                                            R_NilValue));
                ;
            }
            else if (!strcmp(getLangName(s), "{")) {
                ;
                makeCDG_rec_cpp(CDR(
                                    s), returnValueVariableName,
                                controlVertex, flowVertex,
                                if_node,structuredTransfersOfControl,
                                !lastInstruction ? false : ((TYPEOF(t)==
                                                             LISTSXP ? CDR(s1)
                                                             : R_NilValue) ==
                                                            R_NilValue));

            }
            else
            {
                list<string> uses;
                makeCallNode(s, returnValueVariableName,
                             controlVertex,flowVertex,uses, true,
                             !lastInstruction ? false : ((TYPEOF(
                                                              t)==
                                                          LISTSXP ? CDR(s1) :
                                                          R_NilValue) ==
                                                         R_NilValue));
            }

        }
        else
        {
            if(!lastInstruction ? false : ((TYPEOF(t)==
                                            LISTSXP ? CDR(CDR(s1)) : R_NilValue)
                                           == R_NilValue))
                makeConstantNode(s, returnValueVariableName,
                                 controlVertex,
                                 flowVertex);
            ;
        }
    }
}

GraphType CDGMaker::makeCDG_cpp(SEXP obj,vertex_t*& entry)
{
    if (TYPEOF(obj) != LANGSXP)
        Rf_error("Not a `language` object");


    global_CallNumber = global_CallNumber_startValue;
    variableName2variableName.clear();
    vertex_t flow;
    g = GraphType(0);

    int index = 0;
    string functionName;


    for (SEXP t = CDR(obj); t != R_NilValue; t = CDR(t))
    {
        if(index == 0)
        {

            if(TYPEOF(CAR(t))==SYMSXP)
                functionName = CHAR(PRINTNAME(CAR(t)));
            else
                functionName = "ReplacementFunction3211823";

        }
        else if (index == 1)
        {
            makeCDGfromFunction(CAR(t), entry, functionName,flow);
            g[graph_bundle].variableName2variableName =
                variableName2variableName;
            return g;
        }
        index++;
    }

  return g;
}


void CDGMaker::makeCDG_rec_cpp_wrapper(
    SEXP s,
    string returnValueVariableName,
    const vertex_t& controlVertex,
    vertex_t& flowVertex,
    vertex_t* if_node,
    list<std::pair<vertex_t*,
                   vertex_t*> >* structuredTransfersOfControl,
    bool lastInstruction) {
    if(TYPEOF(CAR(s))==LANGSXP)
    {
        if (!strcmp(getLangName(CAR(s)), "{"))
        {
            makeCDG_rec_cpp(CDR(CAR(
                                    s)), returnValueVariableName,
                            controlVertex, flowVertex,
                            if_node,structuredTransfersOfControl,
                            true);
        }
        else
        {
            makeCDG_rec_cpp(s, returnValueVariableName,
                            controlVertex, flowVertex,if_node,
                            structuredTransfersOfControl,
                            lastInstruction);
        }
    }
    else if(TYPEOF(CAR(s))==SYMSXP)
    {
        makeCDG_rec_cpp(s, returnValueVariableName, controlVertex,
                        flowVertex,if_node,structuredTransfersOfControl,
                        lastInstruction);
    }
    else if(TYPEOF(s)==SYMSXP)
    {
        makeCDG_rec_cpp(s, returnValueVariableName, controlVertex,
                        flowVertex,if_node,structuredTransfersOfControl,
                        lastInstruction);
    }
    else if(TYPEOF(s)==SYMSXP)
    {
        makeCDG_rec_cpp(s, returnValueVariableName, controlVertex,
                        flowVertex,if_node,structuredTransfersOfControl,
                        lastInstruction);
    }
    else
    {
        makeCDG_rec_cpp(s, returnValueVariableName, controlVertex,
                        flowVertex,if_node,structuredTransfersOfControl,
                        lastInstruction);
    }


}
