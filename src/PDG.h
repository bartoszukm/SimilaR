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
#include "CDG.h"
#include "DDG.h"
#include "postprocessingPDG.h"

using namespace Rcpp;
using namespace std;
using namespace boost;

class PDGMaker
{
private:
CDGMaker cdg;
DDGMaker ddg;
PostprocessingPDG post;

public:

GraphType MakePDG(SEXP s,
                  bool isDeleteControlFlowEdges = true,
                  bool executeRemoveSingleInstructions=true,
                  bool executeMergeTheSameInstructions=true,
                  bool executeChangeWhileLoop=true);
PDGMaker() : cdg(), ddg(){}
};
