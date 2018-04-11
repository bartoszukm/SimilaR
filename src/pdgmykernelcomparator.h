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

#include "pdgkernelcomparator.h"

class PDGMyKernelComparator : public PDGKernelComparator
{
private:
    map<unsigned long long, list<vertex_t> > g1VerticesByDepth;
    map<unsigned long long, list<vertex_t> > g2VerticesByDepth;
    unsigned long long maxDepthG1;
    unsigned long long maxDepthG2;
    double median1;
    double median2;
    vector<unsigned long long> phi1; //phi is only a fragment of whole feature vector of a graph. Only one iteration fragment
    vector<unsigned long long> phi2;
    vector<unsigned long long> oldPhi1;
    vector<unsigned long long> oldPhi2;
    unsigned long long label_counter_first = 0;
    map<double, unsigned long long> label_lookup_first;
    unsigned long long last_label_counter;
    std::map<vector<unsigned long long>, unsigned long long> label_lookup;
    unsigned long long label_counter;

    void calculateResultsForOneIteration(
        double& len1,
        double& len2, double& mul, double& mul_sym);
    void computeLabels(
        GraphType& g,
        unsigned long long maxDepth,
        map<unsigned long long,
            list<vertex_t> >& gVerticesByDepth
        );
    void compareTwoVertices(
        GraphType& g1,
        GraphType& g2,
        vertex_t v1,
        vertex_t v2,
        double median1,
        double median2,
        double penalty = 1.0);
    void calculatePhi(GraphType& g,
                      vector<unsigned long long>& phi);
    void compareVerticesWithinTwoGraphs(
        GraphType& g1,
        GraphType& g2
        );
    void compareVerticesWithinOneGraph(
        GraphType& g,
        unsigned long long&
        label_counter,
        unsigned long long
        maxDepth,
        double median,
        map<unsigned long
            long,
            list<vertex_t> >&
        gVerticesByDepth
        );
    void pairGraphs(GraphType& g1,
                    GraphType& g2
                    );
    void makeIterationForGraphs(
        GraphType& g1,
        GraphType& g2
        );
    void iterationZero(
        GraphType& g,
        vector<unsigned long long>& phi
        );
    unsigned long long assignDepthRec(
        GraphType& g, vertex_t v,
        unsigned long long
        depth,
        map<unsigned long long,
            list<vertex_t> >
        & gVerticesByDepth);
    unsigned long long calculateDepthOfVerticesInTree(
        GraphType& g,
        map<unsigned long long, list<vertex_t> >& gVerticesByDepth);

    double calculateImportanceOfVertices(
        GraphType& g,
        const map<unsigned
                  long long,
                  list<
                      vertex_t> >&
        gVerticesByDepth,
        unsigned long long
        maxDepth);
    void copyNewLabels2Labels(GraphType& g);
    void removeDataCycles(GraphType& g);
    void resetDataStructures(GraphType& g1, GraphType& g2);
    bool comparePhis();

public:
    PDGMyKernelComparator(int h) : PDGKernelComparator(h){
    }
    virtual vector<double> compare(GraphType& g1, GraphType& g2,
                                   bool isSymmetric);
};
