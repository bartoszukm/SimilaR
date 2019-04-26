#include "SyntaxNode.h"
#include "SyntaxConstantNode.h"
#include "SyntaxLangNode.h"
#include "SyntaxPairlistNode.h"
#include "SyntaxSymbolNode.h"

#include "NodeProcessorWhile.h"
#include "NodeProcessorWrong.h"
#include "NodeProcessorAssignment.h"
#include "NodeProcessorFunction.h"
#include "NodeProcessorFunctionParameters.h"
#include "NodeProcessorSymbolOrConstant.h"
#include "NodeProcessorIf.h"
#include "NodeProcessorFor.h"
#include "NodeProcessorNext.h"
#include "NodeProcessorBreak.h"
#include "NodeProcessorParenthesis.h"
#include "NodeProcessorBrace.h"
#include "NodeProcessorCall.h"
#include "NodeProcessorIf.h"

struct {
    const char *name;
    int token;
}

static keywords[] = {
    { "NULL",	    0 },
    { "NA",	    0  },
    { "TRUE",	    0  },
    { "FALSE",	    0  },
    { "Inf",	    0  },
    { "NaN",	    0  },
    { "NA_integer_", 0  },
    { "NA_real_",    0  },
    { "NA_character_", 0  },
    { "NA_complex_", 0  },
    { "function",   0   },
    { "while",	    0      },
    { "repeat",	    0     },
    { "for",	    0	       },
    { "if",	    0	       },
    { "in",	    0	       },
    { "else",	    0       },
    { "next",	    0       },
    { "break",	    0      },
    { "...",	    0     },
    { 0,	    0	       }
};

void SyntaxNode::RepairTree()
{
}


// SyntaxNode::~SyntaxNode()
// {
// }

unique_ptr<SyntaxNode> SyntaxNode::ConvertLispToSyntaxNode(SEXP s)
{
    if(TYPEOF(s)==SYMSXP)
    {
        return SyntaxSymbolNode::ConvertLispToSyntaxNode(s);
    }
    else if(TYPEOF(s)==LANGSXP)
    {
        return SyntaxLangNode::ConvertLispToSyntaxNode(s);
    }
    else if(TYPEOF(s)==LISTSXP)
    {
        return SyntaxPairlistNode::ConvertLispToSyntaxNode(s);
    }
    else
    {
        return SyntaxConstantNode::ConvertLispToSyntaxNode(s);
    }
}

void SyntaxNode::ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

string SyntaxNode::constantToString(SEXP s)
{
    if (TYPEOF(s) ==  LGLSXP)
    {
        Rcpp::LogicalVector vec = Rcpp::as<Rcpp::LogicalVector>(s);
        if(vec[0] == NA_LOGICAL)
            return "NA";
        else
            return (bool) vec[0] ? "TRUE" : "FALSE";
        //return string((bool)Rcpp::as<Rcpp::LogicalVector>(
        //                  s)[0] ? "TRUE" : "FALSE");
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
        string str = string((string) Rcpp::as<Rcpp::CharacterVector>(s)[0]);
        // cout << "str = " << str << endl;
        ReplaceStringInPlace(str, "\\","\\\\");
        ReplaceStringInPlace(str, "\"","\\\"");
        ReplaceStringInPlace(str, "\n","\\\n");
        // ReplaceStringInPlace(str, "\\x","\\\\x");
        ReplaceStringInPlace(str, "\'","\\\'");
        ReplaceStringInPlace(str, "\t","\\t");
        ReplaceStringInPlace(str, "\b","\\\b");
        ReplaceStringInPlace(str, "\r","\\r");
        ReplaceStringInPlace(str, "\f","\\\f");
        return "\""+str+"\"";
    }
    else if(TYPEOF(s) ==  LANGSXP)
    {
        Environment ns = Environment::namespace_env("SimilaR" ) ;
        Rcpp::Function myprint(ns["myprint"]);
        string str;
        CharacterVector cv = as<CharacterVector>(myprint(s));
        for (size_t i = 0; i < cv.size(); i++) {
            str += cv[i];
        }
        // cout << "str2 = " << str << endl;
        //return string(as<CharacterVector>(myprint(s))[0]);
        return str;
    }
    else if(s == R_NilValue)
    {
        return string("NULL");
    }
    else
    {
        // cout << "cos poszlo nie tak! nie znaleziono typu!" << endl;
        // return string("");
        Environment ns = Environment::namespace_env("SimilaR" ) ;
        Rcpp::Function myprint(ns["myprint"]);
        string str;
        CharacterVector cv = as<CharacterVector>(myprint(s));
        for (size_t i = 0; i < cv.size(); i++) {
            str += cv[i];
        }
        // cout << "str2 = " << str << endl;
        //return string(as<CharacterVector>(myprint(s))[0]);
        return str;
    }
}

const char* SyntaxNode::getLangName(SEXP s)
{
    //cout << "getLangName - poczatek" << endl;

    if(TYPEOF(PRINTNAME(s))==CHARSXP)
    {
        return CHAR(PRINTNAME(s));
    }

    if(TYPEOF(PRINTNAME(CAR(s)))==CHARSXP)
    {
        //cout << "wszedlem w tego dziwnego ifa 1" << endl;
        return CHAR(PRINTNAME(CAR(s)));
    }
    //cout << "przed dziwnym ifem 2" << endl;

    if(TYPEOF(CAR(s)) == SYMSXP || TYPEOF(CAR(s)) == LANGSXP ||
       TYPEOF(CAR(s)) == LISTSXP)
    {     //cout << "if 1" << endl;
        if(TYPEOF(CDR(CAR(s))) == SYMSXP || TYPEOF(CDR(CAR(s))) == LANGSXP ||
           TYPEOF(CDR(CAR(s))) == LISTSXP)
        {         //cout << "if 2" << endl;
            if(TYPEOF(CDR(CDR(CAR(s)))) == SYMSXP ||
               TYPEOF(CDR(CDR(CAR(s)))) == LANGSXP ||
               TYPEOF(CDR(CDR(CAR(s)))) ==
               LISTSXP)
            {             //cout << "if 3" << endl;
                if(TYPEOF(CAR(CDR(CDR(CAR(s))))) == SYMSXP ||
                   TYPEOF(CAR(CDR(CDR(CAR(s))))) == LANGSXP ||
                   TYPEOF(CAR(CDR(CDR(CAR(s))))) == LISTSXP)
                {                 //cout << "if 4" << endl;
                    if(PRINTNAME(CAR(CDR(CDR(CAR(s))))) != NULL)
                    {                     //cout << "if 5" << endl;
                        if(TYPEOF(PRINTNAME(CAR(CDR(CDR(CAR(s)))))) != NULL)
                        {
                            //cout << "wszedlem w tego dziwnego ifa 2" << endl;
                            return CHAR(PRINTNAME(CAR(CDR(CDR(CAR(s))))));                             // wilcox.test (prawa strona)
                            //cout << CHAR(PRINTNAME(CAR(CAR(s))))<<endl;   //::
                            //cout << CHAR(PRINTNAME(CAR(CDR(CAR(s)))))<<endl;   //stats (lewa strona)
                        }
                    }
                }
            }
        }
    }


    string functionName = string("user_fun")+boost::lexical_cast<string>(1);
    char * cstr = new char [functionName.length()+1];
    std::strcpy (cstr, functionName.c_str());
    return cstr;

}

int SyntaxNode::isValidName(const char *name)
{
    const char *p = name;
    int i;

//     if(mbcslocale) {
// 	/* the only way to establish which chars are alpha etc is to
// 	   use the wchar variants */
// 	size_t n = strlen(name), used;
// 	wchar_t wc;
// 	used = mbrtowc(&wc, p, n, NULL); p += used; n -= used;
// 	if(used == 0) return 0;
// 	if (wc != L'.' && !iswalpha(wc) ) return 0;
// 	if (wc == L'.') {
// 	    /* We don't care about other than ASCII digits */
// 	    if(isdigit(0xff & (int)*p)) return 0;
// 	    /* Mbrtowc(&wc, p, n, NULL); if(iswdigit(wc)) return 0; */
// 	}
// 	while((used = mbrtowc(&wc, p, n, NULL))) {
// 	    if (!(iswalnum(wc) || wc == L'.' || wc == L'_')) break;
// 	    p += used; n -= used;
// 	}
// 	if (*p != '\0') return 0;
//     } else {
	int c = 0xff & *p++;
	if (c != '.' && !isalpha(c) ) return 0;
	if (c == '.' && isdigit(0xff & (int)*p)) return 0;
	while ( c = 0xff & *p++, (isalnum(c) || c == '.' || c == '_') ) ;
	if (c != '\0') return 0;
    // }

    if (strcmp(name, "...") == 0) return 1;

    for (i = 0; keywords[i].name != NULL; i++)
	if (strcmp(keywords[i].name, name) == 0) return 0;

    return 1;
}

string SyntaxNode::formatName(Rcpp::String x) {
   char buf[256];
   const char* xs = x.get_cstring ();
   if (isValidName(xs))
      snprintf(buf, 256, "%s", xs);
   else
      snprintf(buf, 256, "`%s`", xs);
   return string(buf);
}

string SyntaxNode::GetLeftName()
{
    return Name;
}

bool SyntaxNode::IsReturnBranch(size_t &branchSize)
{
    // @TODO
    branchSize = 1;
    return false;
}

Context SyntaxNode::ProcessWhile(NodeProcessorWhile& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessFor(NodeProcessorFor& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessForPredicate(NodeProcessorFor& processor,
                                        SyntaxLangNode *forNode,
                                        const Context& context)
{
    return processor.ProcessForeach(forNode, context);
}

Context SyntaxNode::ProcessIf(NodeProcessorIf& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessFunction(NodeProcessorFunction& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessFunctionParameters(NodeProcessorFunctionParameters& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessBrace(NodeProcessorBrace& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessParenthesis(NodeProcessorParenthesis& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessCall(NodeProcessorCall& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessSymbolOrConstant(NodeProcessorSymbolOrConstant& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessBreak(NodeProcessorBreak& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessNext(NodeProcessorNext& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessAssignment(NodeProcessorAssignment& processor,
                                 const Context& context)
{
    return processor.ProcessNext(this, context);
}

Context SyntaxNode::ProcessFirstAssignmentChild(NodeProcessorAssignment& processor,
                                              SyntaxLangNode* assignmentNode,
                                              SyntaxNode* right,
                                              const Context& context)
{
    Rf_error("Assignment that should not exist.");
}

Context SyntaxNode::ProcessSecondAssignmentChild(NodeProcessorAssignment& processor,
                                              SyntaxLangNode* assignmentNode,
                                              SyntaxSymbolNode* left,
                                              const Context& context)
{
    Rf_error("Assignment that should not exist.");
}

Context SyntaxNode::ProcessSecondAssignmentChild(NodeProcessorAssignment& processor,
                                              SyntaxLangNode* assignmentNode,
                                              SyntaxLangNode* left,
                                              const Context& context)
{
    Rf_error("Assignment that should not exist.");
}
