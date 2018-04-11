
#pragma once
#include "common.h"

class PDGComparator {
protected:
   PDGComparator(){}
   virtual ~PDGComparator() { }
public:
   virtual vector<double> compare(GraphType& g1, GraphType& g2, bool isSymmetric) = 0;
};
