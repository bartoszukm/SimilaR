#include "SyntaxTree.h"


int SyntaxTree::numer = 0;

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



void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

// [[Rcpp::export]]
CharacterVector syntaxTree_identity(SEXP parse)
{
    SyntaxTree st(parse);
    return CharacterVector(st.ToString());
}

SyntaxTree::SyntaxTree(SEXP s)
{
    if (TYPEOF(s) != LANGSXP)
        Rf_error("Not a `language` object");

    root = SyntaxNode::ConvertLispToSyntaxNode(s);
    root->parent = nullptr;

    // nr = ++numer;
    // cout << "tworze drzewo nr " << nr << endl;
}

SyntaxTree::SyntaxTree()
{
    // nr = ++numer;
    // cout << "tworze drzewo nr " << nr << endl;
    root = nullptr;
}


string SyntaxTree::ToString()
{
    string ret = root->ToString();
    //cout << ret << endl;
    ret = "deparse(quote("+ret+"))";
    return ret;
}

void SyntaxTree::RepairTree()
{
    root->RepairTree();
}

SyntaxTree::~SyntaxTree()
{
    // cout << "destruktor syntax tree for number" << nr << endl;
    // if(root != nullptr)
    //     delete root;
    // root = nullptr;
}

list<SyntaxNode*> SyntaxTree::GetListOfNodes()
{
    list<SyntaxNode*> ret;
    queue<SyntaxNode*> q;

    SyntaxNode* p = root;
    q.push(p);
    ret.push_back(p);
    while(!q.empty())
    {
        p = q.front();
        q.pop();
        for(size_t i=0;i<p->children.size();++i)
        {
            q.push(p->children[i]);
            ret.push_back(p->children[i]);
        }
    }
    return ret;
}

int SyntaxTree::GetNodesCount()
{
    list<SyntaxNode*> ret = GetListOfNodes();
    return ret.size();
}


unique_ptr<SyntaxTree> SyntaxTree::Copy()
{
    unique_ptr<SyntaxTree> t(new SyntaxTree());
    t->root = root->Copy();
    return t;
}

string SyntaxTree::GetFunctionName()
{
    return root->children[0]->functionName;
}
