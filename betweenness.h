#pragma once
#ifndef BETWEENNESS_H
#define BETWEENNESS_H

#include <list>
#include <numeric>
#include <algorithm>
#include "Graph.h"
#include <iostream>
#include <vector>
#include <set>
#include <queue>



using namespace std;

map<int, double> nodebetweenness(vector<vector<int> > myCommunity);

#endif // KSHELL_H_INCLUDED