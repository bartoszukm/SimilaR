
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
