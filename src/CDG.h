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

#include "common.h"
#include "graphUtils.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Rcpp;
using namespace std;
using namespace boost;


class CDGMaker
{
private:
GraphType g;

const unsigned long long int global_CallNumber_startValue;
unsigned long long int global_CallNumber;
map<string, string> variableName2variableName;
SEXP stopifnotSEXP;
bool shouldBreakIf;

string constantToString(SEXP s);
string concatenateStringList(list<string> l);
string getLeftVariable(SEXP s);
const char* getLangName(SEXP s);
bool isSpecificFunction(SEXP s, const char* fname);
bool isApplyFunction(SEXP s);
string getLeftVariable_all(SEXP s,
                           string returnValueVariableName,
                           const vertex_t& controlVertex,
                           vertex_t& flowVertex,
                           list<string>& uses,
                           bool createNode = true,
                           bool lastInstruction = false);
void makeCDGfromFunction(SEXP obj,
                         vertex_t*& entryp,
                         string returnValueVariableName,
                         vertex_t& flowVertex);
void makeStructuredTransfersOfControlForLoop(const vertex_t& loop,
                                             list<
                                                 std::pair<vertex_t*,
                                                           vertex_t*> >
                                             *structuredTransfersOfControl);
void makeConstantNode(SEXP s,
                      string returnValueVariableName,
                      const vertex_t& controlVertex,
                      vertex_t& flowVertex);
void makeIfNode(SEXP s,
                string returnValueVariableName,
                const vertex_t& controlVertex,
                vertex_t& flowVertex,
                list<std::pair<vertex_t*,
                               vertex_t*> >* structuredTransfersOfControl,
                bool lastInstruction);
void makeForNode(SEXP s,
                 string returnValueVariableName,
                 const vertex_t& controlVertex,
                 vertex_t& flowVertex,
                 bool lastInstruction);
void makeWhileNode(SEXP s,
                   string returnValueVariableName,
                   const vertex_t& controlVertex,
                   vertex_t& flowVertex,
                   bool lastInstruction);
void makeApplyNode(SEXP s,
                   string returnValueVariableName,
                   const vertex_t& controlVertex,
                   vertex_t& flowVertex,
                   list<string>& uses,
                   bool createNode = true,
                   bool lastInstruction = false,
                   bool isLeftAssign = false,
                   list<string>* additional_uses = nullptr);
void makeNameSymbolNode(SEXP s,
                        string returnValueVariableName,
                        const vertex_t& controlVertex,
                        vertex_t& flowVertex,
                        bool lastInstruction);
void createNodeForCallNode(SEXP s,
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
                           );
void makeStopIfNotNodesIfNecessary(SEXP s,
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
                                   );
void makeCallNode(SEXP s,
                  string returnValueVariableName,
                  const vertex_t& controlVertex,
                  vertex_t& flowVertex,
                  list<string>& uses,
                  bool createNode = true,
                  bool lastInstruction = false,
                  bool isLeftAssign = false,
                  bool isStopifnotCall = false,
                  bool isLeftSideOfAssign = false);
void makeAssignmentNode(SEXP s,
                        string returnValueVariableName,
                        const vertex_t& controlVertex,
                        vertex_t& flowVertex,
                        list<string>& uses,
                        bool createNode = true,
                        bool lastInstruction = false,
                        bool isLeftAssign = false,
                        bool isStopifnotCall = false,
                        bool isLeftSideOfAssign = false);
void makeNormalFunctionCallNode(SEXP s,
                                string returnValueVariableName,
                                const vertex_t& controlVertex,
                                vertex_t& flowVertex,
                                list<string>& uses,
                                bool createNode = true,
                                bool lastInstruction = false,
                                bool isLeftAssign = false,
                                bool isStopifnotCall = false,
                                bool isLeftSideOfAssign = false);
void makeDplyrNode(SEXP s,
                                string returnValueVariableName,
                                const vertex_t& controlVertex,
                                vertex_t& flowVertex,
                                list<string>& uses,
                                bool createNode = true,
                                bool lastInstruction = false,
                                bool isLeftAssign = false,
                                bool isStopifnotCall = false,
                                bool isLeftSideOfAssign = false);
void makeDplyrSymbolNode(SEXP s,
                   string returnValueVariableName,
                   const vertex_t& controlVertex,
                   vertex_t& flowVertex,
                   list<string>& uses,
                   bool createNode = true,
                   bool lastInstruction = false,
                   bool isLeftAssign = false,
                   bool isStopifnotCall = false,
                   bool isLeftSideOfAssign = false);
void makeNextNode(SEXP s,
                  string returnValueVariableName,
                  const vertex_t& controlVertex,
                  vertex_t& flowVertex,
                  vertex_t* if_node,
                  list<pair<vertex_t*,
                            vertex_t*> >* structuredTransfersOfControl);
void makeBreakNode(SEXP s,
                   string returnValueVariableName,
                   const vertex_t& controlVertex,
                   vertex_t& flowVertex, vertex_t* if_node,
                   list<pair<vertex_t*,
                             vertex_t*> >* structuredTransfersOfControl);
void makeCDG_rec_cpp(SEXP t,
                     string returnValueVariableName,
                     const vertex_t& controlVertex,
                     vertex_t& flowVertex,
                     vertex_t* if_node,
                     list<pair<vertex_t*,
                               vertex_t*> >* structuredTransfersOfControl,
                     bool lastInstruction);
void makeCDG_rec_cpp_wrapper(SEXP s,
                             string returnValueVariableName,
                             const vertex_t& controlVertex,
                             vertex_t& flowVertex,
                             vertex_t* if_node,
                             list<pair<vertex_t*,
                                       vertex_t*> >
                             *structuredTransfersOfControl,
                             bool lastInstruction);

bool isReturnBranch(SEXP s, int& branchSize);
int makeLexicalComparison(SEXP s1, SEXP s2);

public:
CDGMaker() :
    global_CallNumber_startValue(0),
    global_CallNumber(global_CallNumber_startValue),
    shouldBreakIf(true){
    }

GraphType makeCDG_cpp(SEXP obj, vertex_t*& entry);   //zwracany do R graf to lista 3 rzeczy: macierz sasiedztwa, kolory i texty wierzcholkow
};
