#include "SyntaxTree.h"
#include "SyntaxLangNode.h"

// int SyntaxTree::numer = 0;

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

    root.reset(dynamic_cast<SyntaxLangNode*>(SyntaxLangNode::ConvertLispToSyntaxNode(s).get()));

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
    if (!root)
        Rf_error("Root is null");
    string ret = root->ToString();
    //cout << ret << endl;
    ret = "deparse(quote("+ret+"))";
    return ret;
}

void SyntaxTree::RepairTree()
{
    root->RepairTree();
}

// SyntaxTree::~SyntaxTree()
// {
//     // cout << "destruktor syntax tree for number" << nr << endl;
//     // if(root != nullptr)
//     //     delete root;
//     // root = nullptr;
// }

list<SyntaxNode*> SyntaxTree::GetListOfNodes()
{
    list<SyntaxNode*> ret;
    queue<SyntaxNode*> q;

    SyntaxNode* p = root.get();
    q.push(p);
    ret.push_back(p);
    while(!q.empty())
    {
        p = q.front();
        q.pop();
        SyntaxLangNode *p_lang = dynamic_cast<SyntaxLangNode*>(p);
        if(p_lang)
        {
            for(size_t i=0;i<p_lang->Children.size();++i)
            {
                q.push(p_lang->Children[i].get());
                ret.push_back(p_lang->Children[i].get());
            }
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
    t->root.reset(dynamic_cast<SyntaxLangNode*>(root->Copy().get()));
    return t;
}

string SyntaxTree::GetFunctionName()
{
    SyntaxLangNode *root_lang = dynamic_cast<SyntaxLangNode*>(root.get());
    return root_lang->Children[0]->Name;
}

SyntaxLangNode *SyntaxTree::GetRoot()
{
    return root.get();
}
