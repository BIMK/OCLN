#include <iostream>
#include <set>

#include "Graph.h"
#include "fastCommunityFinder.h"
using namespace std;

int intersection(int nei, set<int> &original)
{
    int kin = 0;

    set<int> neighbor = theGlobalgraph.neighbors[nei];

    set<int>::const_iterator sbegin = original.begin(), send = original.end();
    set<int>::const_iterator neibegin = neighbor.begin();

    while (neibegin != neighbor.end() && sbegin != send)
    {
        if (*neibegin == *sbegin)
        {
            kin++;
            neibegin++;
            sbegin++;
        }
        else if (*neibegin < *sbegin)
            neibegin++;
        else
            sbegin++;
    }

    return kin;
}


void addinitialC(vector<int> &initialC, set<int> &original, set<int> &unadd)
{
    set<int>::const_iterator oribegin = original.begin(), oriend = original.end();
    set<int>::const_iterator unaddbegin = unadd.begin(), unaddend = unadd.end();

    while (oribegin != oriend && unaddbegin != unaddend)
    {
        if (*oribegin == *unaddbegin)
        {
            oribegin++;
            unaddbegin++;
        }
        else if (*oribegin < *unaddbegin)
        {
            initialC.push_back(*oribegin);
            oribegin++;
        }
        else
        {
            initialC.push_back(*oribegin);
            unaddbegin++;
        }
    }

    while (oribegin != oriend)
        initialC.push_back(*oribegin++);

}

set<int> singleExpand(set<int> &currentCommunity, map<int,double> &candidate, double t)
{
    set<int> result;
    set<int>::const_iterator sitbegin = currentCommunity.begin();
    map<int,double>::const_iterator mitbegin = candidate.begin();

    while (mitbegin != candidate.end())
    {
        if (sitbegin != currentCommunity.end() && *sitbegin == mitbegin->first)
        {
            sitbegin++;
            mitbegin++;
        }
        else if (sitbegin != currentCommunity.end() && *sitbegin > mitbegin->first)
        {
            //¼ÆËãjoin interest
            double join = 0.0, leave = 0.0;
            join = mitbegin->second;
            leave = (theGlobalgraph.degree[mitbegin->first]-join)/t;

            if (join > leave)
                result.insert(mitbegin->first);

            mitbegin++;
        }
        else if (sitbegin != currentCommunity.end() && *sitbegin < mitbegin->first)
        {
            sitbegin++;
        }
        else if (sitbegin == currentCommunity.end())
        {
            //¼ÆËãjoin interest
            double join = 0.0, leave = 0.0;
            join = mitbegin->second;
            leave = (theGlobalgraph.degree[mitbegin->first]-join)/t;

            if (join > leave)
                result.insert(mitbegin->first);

            mitbegin++;
        }


    }

    return result;
}

void Merge(set<int> &current, set<int> &result)
{
    for (set<int>::const_iterator sit = result.begin(); sit != result.end(); sit++)
        current.insert(*sit);
}
