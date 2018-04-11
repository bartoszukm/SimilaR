
#pragma once
#include "pdgcomparator.h"
#include "common.h"
#include <vector>
#include <map>
#include <list>
#include "defs.h"
#include "graphUtils.h"
#include <unordered_map>

class PDGKernelComparator : public PDGComparator
{
protected:
    int h;
    PDGKernelComparator(int h) : h(h){
    }

    double calculateK(double mul, double len1, double len2)
    {
      return mul/len1;
    }

public:
    virtual vector<double> compare(GraphType& g1, GraphType& g2,
                                   bool isSymmetric) = 0;
};
