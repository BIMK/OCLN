//Graph.h
#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include <math.h>
#include <algorithm>

using namespace std;

template<class VertexType>
class Graph
{
public:
    set <VertexType> vertexNames;//a container of contain all vertex name;
    set<pair<VertexType,VertexType> > linkliststring;
    set<pair<int,int> > linklist;
    map<int,VertexType> node_index_to_name;//a map of nodes about index and its name;
    map<VertexType,int> node_name_to_index;
    vector <int> degree;
    vector<set<int> > neighbors;
public:
    int vertex_count;//the number of vertex in the graph;
    int edge_count;

public:
    Graph();

    long int vcount(){return this->vertex_count;}
    long int ecount(){return this->edge_count;}

    int degree_of_node(int v){return degree[v];}

    string name_of_node_asString(int v){return node_index_to_name[v];}

    void add_neighbor(int i,int j, vector<set<int> > &neighborsCopy)
    {
        //j add as a neighbor of i
        neighborsCopy[i].insert(j);
        neighborsCopy[j].insert(i);
    }

    bool isConnect(int i, int j)
    {
        if (find(neighbors[i].begin(),neighbors[i].end(),j) != neighbors[i].end())
            return true;
        else
            return false;

    }
    void loadingGraph(string fileName);
    void Creat_Neighborschart( vector<set<int> > &neighborsCopy );

    ~Graph();
};

template<class VertexType>
Graph<VertexType>::Graph()
{
    vertex_count=0;
    edge_count=0;
}

template<class VertexType>
Graph<VertexType>::~Graph()
{

}

template<class VertexType>
void Graph<VertexType>::loadingGraph(string fileName)
{
    std::ifstream myfile(fileName.c_str());
    std::string line;
    VertexType source, dest, t;
    if (myfile.is_open())
    {
        while (!myfile.eof())
        {
            getline(myfile, line);   // Read a line from input into a string.
            if (!line.empty())
            {
                std::istringstream is(line);  // Extract data using a stringstream.
                if ((is >> source) && (is >> dest));
                else
                {
                   std::cerr<<"Error reading "<<fileName<<".txt"<< std::endl;
                }

                if (source > dest)
                {
                    VertexType temp = source;
                    source = dest;
                    dest = temp;
                }
                linkliststring.insert(make_pair(source,dest));

//                //if the network is directed, we need add another edge
                linkliststring.insert(make_pair(dest,source));

                vertexNames.insert(source);
                vertexNames.insert(dest);
            }
        }
        myfile.close();
    }

    vertex_count = vertexNames.size();
    edge_count = linkliststring.size()/2;

    //next,we get the map of node index and node name
    int index = 0;
    for (typename set<VertexType>::iterator it=vertexNames.begin();it!=vertexNames.end();it++)
    {
        node_index_to_name[index]=*it;
        node_name_to_index[*it]=index++;
    }

    //将linkliststring转化为linklist
    for (typename set<pair<VertexType,VertexType> >::const_iterator linkit = linkliststring.begin();
            linkit != linkliststring.end(); linkit++)
    {
        pair<VertexType,VertexType> link = *linkit;
        int nodefirst = node_name_to_index[link.first], nodesecond = node_name_to_index[link.second];
        if (nodefirst > nodesecond)
        {
            int nodetemp = nodefirst;
            nodefirst = nodesecond;
            nodesecond = nodetemp;
        }
        linklist.insert(make_pair(nodefirst,nodesecond));
    }

    //Create Graph neighbors chart
    vector<set<int> > neighborsCopy(vertex_count);
    Creat_Neighborschart( neighborsCopy );
    neighbors = neighborsCopy;

    //calculate the degrees of the each vertex in the graph
    for (int i=0;i<neighbors.size();i++)
        degree.push_back(neighbors[i].size());
}

template<class VertexType>
void Graph<VertexType>::Creat_Neighborschart( vector<set<int> > &neighborsCopy )
{
    for(set<pair<int,int> >::iterator it=linklist.begin();it!=linklist.end();it++)
    {
        pair<int,int> edge=*it;

        //find the index of vertex which name is edge.first and edge.second;
        add_neighbor(edge.first, edge.second, neighborsCopy);
    }
}

extern Graph<string> theGlobalgraph;

#endif
