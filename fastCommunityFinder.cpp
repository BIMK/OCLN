#include "fastCommunityFinder.h"
#include <numeric>
#include <algorithm>

using namespace std;

bool desorder(const pair<int,int> &p1, const pair<int,int> &p2)
{
    return p1.second > p2.second;
}

void fastCommunityFinder_before(vector<vector<int> > &myCommunity_before, const double t, const double alpha)
{

    void initialCommunity(int core, vector<int> &initialC, const double t);
    void expandingCommunity_before(int core, vector<int> &initialC, vector<int> &finalCommunity_before,
                                const double t, const double alpha);

    int vertexNum = theGlobalgraph.vertex_count;

    vector<pair<int,int> > vertex_degree;
    for (int temp = 0; temp < theGlobalgraph.vcount(); temp++)
        vertex_degree.push_back(make_pair(temp, theGlobalgraph.degree[temp]));

    sort(vertex_degree.begin(),vertex_degree.end(),desorder);

    int *isVisited = new int [vertexNum];
    //��ʼ��ʱ��ÿ���ڵ㶼��ʼ��Ϊδ����
    for (int i = 0; i < vertexNum; i++)
        isVisited[i] = 0;

    //����ѡ��һ���������ĵ㣬���δ������
    int vertexdegree_Index = 0;
    while (vertexdegree_Index < vertexNum)
    {
        pair<int,int> pMaxdegree = vertex_degree[vertexdegree_Index];

        if (isVisited[pMaxdegree.first] == 0)
        {

            vector<int> finalCommunity_before;

            //δ������
            vector<int> initialC;
            initialCommunity(pMaxdegree.first, initialC, t);

            expandingCommunity_before(pMaxdegree.first,initialC, finalCommunity_before, t, alpha);

//            if (finalCommunity_before.size() > 2)
                myCommunity_before.push_back(finalCommunity_before);


            //��finalCommunity�еĽڵ���Ϊ�ѷ���
            for (vector<int>::const_iterator visitedit = finalCommunity_before.begin();
                    visitedit != finalCommunity_before.end(); visitedit++)
                isVisited[*visitedit] = 1;

        }

        vertexdegree_Index++;
    }
}


void fastCommunityFinder_after(vector<vector<int> > &myCommunity_after, const double t, const double alpha)
{

    void initialCommunity(int core, vector<int> &initialC, const double t);
    void expandingCommunity_after(int core, vector<int> &initialC, vector<int> &finalCommunity_after,
                                        const double t, const double alpha);

    int vertexNum = theGlobalgraph.vertex_count;

    vector<pair<int,int> > vertex_degree;
    for (int temp = 0; temp < theGlobalgraph.vcount(); temp++)
        vertex_degree.push_back(make_pair(temp, theGlobalgraph.degree[temp]));

    sort(vertex_degree.begin(),vertex_degree.end(),desorder);

    int *isVisited = new int [vertexNum];
    //��ʼ��ʱ��ÿ���ڵ㶼��ʼ��Ϊδ����
    for (int i = 0; i < vertexNum; i++)
        isVisited[i] = 0;

    //����ѡ��һ���������ĵ㣬���δ������
    int vertexdegree_Index = 0;
    while (vertexdegree_Index < vertexNum)
    {
        pair<int,int> pMaxdegree = vertex_degree[vertexdegree_Index];

        if (isVisited[pMaxdegree.first] == 0)
        {
            vector<int> finalCommunity_after;

            //δ������
            vector<int> initialC;
            initialCommunity(pMaxdegree.first, initialC, t);

//            cout<<"InitialCommunityΪ��"<<endl;
//            for (vector<int>::const_iterator init = initialC.begin(); init != initialC.end(); init++)
//                cout<<theGlobalgraph.node_index_to_name[*init]<<" ";
//            cout<<endl;

            expandingCommunity_after(pMaxdegree.first,initialC, finalCommunity_after, t, alpha);

            if (finalCommunity_after.size() > 2)
                myCommunity_after.push_back(finalCommunity_after);


            //��finalCommunity�еĽڵ���Ϊ�ѷ���
            for (vector<int>::const_iterator visitedit = finalCommunity_after.begin();
                    visitedit != finalCommunity_after.end(); visitedit++)
                isVisited[*visitedit] = 1;

        }

        vertexdegree_Index++;
    }
}




void initialCommunity(int core, vector<int> &initialC, const double t)
{
    set<int> tempNeighbor = theGlobalgraph.neighbors[core];
    set<int> original; original.insert(core);

    for (set<int>::const_iterator tempit = tempNeighbor.begin(); tempit != tempNeighbor.end(); tempit++)
        original.insert(*tempit);


    //��O(k^2)��ʱ�临�Ӷ�
    set<int> unadd;
    for (set<int>::const_iterator tempit = tempNeighbor.begin(); tempit != tempNeighbor.end(); tempit++)
    {
        int nei = *tempit;
        int kin = intersection(nei, original);
        if (kin < (theGlobalgraph.degree[nei] - kin))
            unadd.insert(nei);
    }

    addinitialC(initialC,original,unadd);

}

void expandingCommunity_before(int core, vector<int> &initialC, vector<int> &finalCommunity_before,
                               const double t, const double alpha)
{
    set<int> currentCommunity;
    for (vector<int>::const_iterator vit = initialC.begin(); vit != initialC.end(); vit++)
        currentCommunity.insert(*vit);

    set<int> newAdd;
    map<int,double> candidate;//candidate��һ��<�ھӽڵ��ţ��ýڵ���currentCommunity��kinֵ>;
    for (set<int>::const_iterator sit = currentCommunity.begin(); sit != currentCommunity.end(); sit++)
    {
        if (*sit != core)
            newAdd.insert(*sit);
    }


    while (!newAdd.empty())
    {
        candidate.clear();
        //��ʱ��O(k^2)
        for (set<int>::const_iterator sit = newAdd.begin(); sit != newAdd.end(); sit++)
        {
            int Node = *sit;
            set<int> pNeighbor = theGlobalgraph.neighbors[Node];

            for (set<int>::const_iterator pTemp = pNeighbor.begin(); pTemp != pNeighbor.end(); pTemp++)
            {
                if (*pTemp != core)
                    candidate[*pTemp]++;
            }
        }

        //������O(k)��ʱ�临�Ӷȼ���kin,��������һ�����Ա����䵽currentCommunity�ļ���
        set<int> singleResult = singleExpand(currentCommunity, candidate, t);
        Merge(currentCommunity, singleResult);

        newAdd.clear();
        newAdd = singleResult;

    }

    for (set<int>::const_iterator finalit = currentCommunity.begin(); finalit != currentCommunity.end(); finalit++)
        finalCommunity_before.push_back(*finalit);


}


void expandingCommunity_after(int core, vector<int> &initialC, vector<int> &finalCommunity_after,
                               const double t, const double alpha)
{
    set<int> currentCommunity;
    for (vector<int>::const_iterator vit = initialC.begin(); vit != initialC.end(); vit++)
        currentCommunity.insert(*vit);

    set<int> newAdd;
    map<int,double> candidate;//candidate��һ��<�ھӽڵ��ţ��ýڵ���currentCommunity��kinֵ>;
    for (set<int>::const_iterator sit = currentCommunity.begin(); sit != currentCommunity.end(); sit++)
    {
        if (*sit != core)
            newAdd.insert(*sit);
    }


    while (!newAdd.empty())
    {
        candidate.clear();
        //��ʱ��O(k^2)
        for (set<int>::const_iterator sit = newAdd.begin(); sit != newAdd.end(); sit++)
        {
            int Node = *sit;
            set<int> pNeighbor = theGlobalgraph.neighbors[Node];

            for (set<int>::const_iterator pTemp = pNeighbor.begin(); pTemp != pNeighbor.end(); pTemp++)
            {
                if (*pTemp != core)
                    candidate[*pTemp]++;
            }
        }

        //������O(k)��ʱ�临�Ӷȼ���kin,��������һ�����Ա����䵽currentCommunity�ļ���
        set<int> singleResult = singleExpand(currentCommunity, candidate, t);
        Merge(currentCommunity, singleResult);

        newAdd.clear();
        newAdd = singleResult;

    }


//    cout<<"����֮���currentCommunityΪ��"<<endl;
//    for (set<int>::const_iterator init = currentCommunity.begin(); init != currentCommunity.end(); init++)
//        cout<<theGlobalgraph.node_index_to_name[*init]<<" ";
//    cout<<endl;



	//���������ڽڵ��������ڵ��ھӸ���
    map<int,int> map_isexist;
    map<int,set<int> > map_vertex_set;
    for (set<int>::const_iterator curit = currentCommunity.begin(); curit != currentCommunity.end(); curit++)
    {
        map_isexist[*curit] = 1;
    }

    for (set<int>::const_iterator curit = currentCommunity.begin(); curit != currentCommunity.end(); curit++)
    {
        int pNode = *curit;

        for (set<int>::const_iterator neiit = theGlobalgraph.neighbors[pNode].begin();
                neiit != theGlobalgraph.neighbors[pNode].end(); neiit++)
        {
            if (map_isexist[*neiit] == 1)
                map_vertex_set[pNode].insert(*neiit);
        }
    }



//    vector<double> belonging;
//    double minValue = 1.1;
    for (set<int>::const_iterator curit = currentCommunity.begin(); curit != currentCommunity.end(); curit++)
    {
        int pNode = *curit;
        set<int> sinTemp = map_vertex_set[pNode];

        double score = 0;
        for (set<int>::const_iterator sinit = sinTemp.begin(); sinit != sinTemp.end(); sinit++)
        {
            int tNode = *sinit;

            score = score + (double)map_vertex_set[tNode].size()/theGlobalgraph.degree[tNode];
        }
        score = score/theGlobalgraph.degree[pNode];

        if (score > alpha)
            finalCommunity_after.push_back(pNode);


//        belonging.push_back(score);
//        if (score < minValue)
//            minValue = score;
    }


//    double sum = std::accumulate(belonging.begin(), belonging.end(), 0.0);
//    double mean = sum/belonging.size();
//    double accum = 0.0;
//    for (vector<double>::const_iterator vit = belonging.begin(); vit != belonging.end(); vit++)
//    {
//        double d = *vit;
//        accum += (d-mean)*(d-mean);
//    }
//    double stdev = sqrt(accum/(belonging.size()-1));
//    //����minValue+stdev����alpha
//    int belongingIndex = 0;
//    int threashold = minValue + stdev;
//    for (set<int>::const_iterator curit = currentCommunity.begin(); curit != currentCommunity.end(); curit++)
//    {
//        int tNode = *curit;
//        double belongValue = belonging[belongingIndex];
//        if (belongValue > threashold)
//            finalCommunity_after.push_back(tNode);
//
//        belongingIndex++;
//    }



}



double commonNeighbors(int nodefirst, int nodesecond)
{
    double comm = 0;

    set<int>::const_iterator it1 = theGlobalgraph.neighbors[nodefirst].begin(),
                            it2 = theGlobalgraph.neighbors[nodesecond].begin();

    while (it1 != theGlobalgraph.neighbors[nodefirst].end() && it2 != theGlobalgraph.neighbors[nodesecond].end())
    {
        if (*it1 == *it2)
        {
            comm = comm + 1;
            it1++;
            it2++;
        }
        else if (*it1 > *it2)
            it2++;
        else
            it1++;
    }

    return comm;
}

/*
void LinkSimilarity(map<pair<int,int>, double> &map_link_similarity)
{
    for (set<pair<int,int> >::const_iterator linkit = theGlobalgraph.linklist.begin();
            linkit != theGlobalgraph.linklist.end(); linkit++)
    {
        pair<int,int> link = *linkit;
        int nodefirst = link.first, nodesecond = link.second;
        double common = commonNeighbors(nodefirst, nodesecond);

        map_link_similarity[link] = common/sqrt(theGlobalgraph.degree[nodefirst]*theGlobalgraph.degree[nodesecond]);
    }
}
*/
