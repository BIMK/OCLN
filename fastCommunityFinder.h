
#ifndef FASTCOMMUNITYFINDER_H
#define FASTCOMMUNITYFINDER_H

#include "Graph.h"
//#include "fastCommunityFunction.cpp"

using namespace std;

void fastCommunityFinder_before(vector<vector<int> > &myCommunity_before, const double t, const double alpha);

void fastCommunityFinder_after(vector<vector<int> > &myCommunity_after, const double t, const double alpha);

int intersection(int nei, set<int> &original);

void addinitialC(vector<int> &initialC, set<int> &original, set<int> &unadd);

set<int> singleExpand(set<int> &currentCommunity, map<int,double> &candidate, double t);

void Merge(set<int> &current, set<int> &result);

//void LinkSimilarity(map<pair<int,int>, double> &map_link_similarity);

#endif // FASTCOMMUNITYFINDER
