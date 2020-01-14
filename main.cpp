# include <stdio.h>
# include <iostream>
# include <fstream>
# include <sstream>
# include <ctime>
# include <string>
# include <vector>
# include <set>
# include <list>
# include <utility>
# include <algorithm>
# include <math.h>
#include <numeric>
#include <string>

#include "Graph.h"
#include "MeasureFunction.h"
#include "fastCommunityFinder.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

int main(int argc, char *argv[])
{
    clock_t start,finish;
    double duration;

    vector <vector<int> > file2TrueCommunity(const string & TrueCommunity_filename);
	double CalculateNMI(vector<vector<int> >TrueCommunity , vector<vector<int> > myCommunity);
	double CalculateEQ(vector<vector<int> >myCommunity,vector<vector<int> >tmp_node_to_neighbors,int vertex_num,int edges_num);
    double CalculateQov(vector<vector<int> > myCommunity);
    void pajek(string file, vector<vector<int> > myCommunity);



//    main有参数时：
//    Determine the number of parameters
//    if (argc != 6)
//    {
//        cout<<"Incorrect number of parameters! Usage: inputFileName, similarity, TrueCommunityFileName outputFileName"<<endl;
//        exit(0);
//    }
//    string edgeslist_filename = argv[1];
//    double t = atof(argv[2]);
//    double alpha = atof(argv[3]);
//    string TrueCommunity_filename = argv[4];
//    string outputFileName = argv[5];


//    double t = atof(argv[1]);
//    double alpha = atof(argv[2]);

//    double t     = 0.5;
//    double alpha = 0.2;



     string networkFileName1[13] = {
                                    "35_0.3_net.dat","0.1M.dat","0.2M.dat","0.3M.dat","0.4M.dat","0.5M.dat","0.6M.dat","0.7M.dat","0.8M.dat","0.9M.dat","1M.dat",
                                    "1.1M.dat","1.2M.dat"

                                };
    string TrueCommunityFileName1[13] = {
        "35_0.3_com.dat","0.1M_true.dat","0.2M_true.dat","0.3M_true.dat","0.4M_true.dat","0.5M_true.dat","0.6M_true.dat","0.7M_true.dat","0.8M_true.dat","0.9M_true.dat","1M_true.dat",
        "1.1M_true.dat","1.2M_true.dat"

    };





    double t = 4, alpha = 0.2;


//string networkFileName1[9] = {"0.1.dat","0.15.dat","0.2.dat","0.25.dat","0.3.dat","0.35.dat","0.4.dat","0.45.dat","0.5.dat"};

//string TrueCommunityFileName1[9] = {"0.1_true.dat","0.15_true.dat","0.2_true.dat","0.25_true.dat","0.3_true.dat","0.35_true.dat","0.4_true.dat","0.45_true.dat","0.5_true.dat"};

//string networkFileName1[6]={ "100net.dat","football.dat","shili.txt","com-amazon.ungraph.txt","com-dblp.ungraph.txt","com-youtube.ungraph.txt"};
//string TrueCommunityFileName1[4]={"100co.dat","com-amazon.all.cmty.txt","com-dblp.all.cmty.txt","com-youtube.all.cmty.txt"};

  ofstream outNMI("result_NMI.txt");
  //ofstream outTime("result_Time.txt");
  //ofstream outCommunity("result_Community.txt");

    for (int index = 1; index < 13; index++)
    {
        const char * outName = networkFileName1[index].data();
		char *str = new char[30];
		char *co = new char[30];
		sprintf(str, "%s_Result.txt", outName);
		sprintf(co, "%s_Community.txt", outName);
		ofstream outresult(str);
		ofstream outCommunity(co);
        theGlobalgraph.linklist.clear();
        theGlobalgraph.linkliststring.clear();
        theGlobalgraph.degree.clear();
        theGlobalgraph.neighbors.clear();
        theGlobalgraph.vertexNames.clear();
        theGlobalgraph.node_index_to_name.clear();
        theGlobalgraph.node_name_to_index.clear();

        string edgeslist_filename = networkFileName1[index];
        string TrueCommunity_filename = TrueCommunityFileName1[index];

        cout<<"正在处理第"<<index+1<<"个网络"<<": "<<edgeslist_filename<<endl;

        theGlobalgraph.loadingGraph(edgeslist_filename);

        cout<<"vertex_num:"<<theGlobalgraph.vcount()<<endl;
        cout<<"edges_num:"<<theGlobalgraph.ecount()<<endl;



        //算法开始
        //想法描述：
        //1.先找出全局度数最大的点，将该点的邻居标记为已访问，然后再依次找出未访问的度数最大的点...，
        //这样就会找出k个核心点，即k个初始社团
        //2.计算ppt中的两个指标，如果connect score>leave interest，则把这个点加入到社团中

    vector<vector<int> >myCommunity_before, myCommunity_after;
    //    _before代表未进行优化处理之前的结果，_after代表优化之后的结果
    //    所谓优化是指：扩充一个community之后，通过每个邻接节点的概率计算community中每个节点自身的概率，进行筛选
    //	start = clock();
    //    fastCommunityFinder_before(myCommunity_before, t, alpha);
    //	finish = clock();
    //	duration = (double)(finish-start)/CLOCKS_PER_SEC;
    //	cout<<"before optimization (Time Used): "<<duration<<endl;
    //	cout<<"myCommunity_before.size():"<<myCommunity_before.size()<<endl;
    //
    //	//compute the overlap modularity
    //    cout<<endl<<"正在计算Qov的值..."<<endl;
    //    double Qov=CalculateQov(myCommunity_before);
    //    cout<<Qov<<endl<<endl;
    //
//        //	//Calculate NMI
//    	cout<<endl<<"Calculating NMI..."<<endl;
//    	double NMI1,NMI2,NMI;
//    	vector <vector <int> > TrueCommunity;
//        TrueCommunity = file2TrueCommunity(TrueCommunity_filename);
//
//        vector <vector<int> > myCommunity;
//        for (signed int i = 0 ; i < myCommunity_before.size() ; i++)
//        {
//            vector <int> v = myCommunity_before[i];
//            vector <int> com;
//            for (unsigned int j = 0 ; j < v.size() ; j++)
//            {
//                com.push_back(atoi(theGlobalgraph.node_index_to_name[v[j]].c_str())); //if string
//                //com.push_back(theGlobalgraph.node_index_to_name[v[j]]); if int
//            }
//            sort(com.begin(),com.end());
//            myCommunity.push_back(com);
//        }
//    //    cout<<"myCommunity.size():"<<myCommunity.size()<<endl;
//    //
//    //    cout<<"vertex_num:"<<theGlobalgraph.vcount()<<"   "<<"edge_num:"<<theGlobalgraph.ecount()<<endl;
//    //
//    //    cout<<"TrueCommunity.size():"<<TrueCommunity.size()<<endl;
//
//    	NMI1 = CalculateNMI(TrueCommunity , myCommunity);
//    	NMI2 = CalculateNMI(myCommunity , TrueCommunity);
//    	NMI =1-(NMI1+NMI2)/2;
//    	cout<<"NMI："<<NMI<<endl<<endl;


    //	after optimization
        start = clock();

        cout<<"t: "<<t<<" alpha: "<<alpha<<endl;

        fastCommunityFinder_after(myCommunity_after, t, alpha);
        finish = clock();
        duration = (double)(finish-start)/CLOCKS_PER_SEC;
        cout<<"myCommunity_after.size():"<<myCommunity_after.size()<<endl;
        cout<<"Time Used: "<<duration<<endl;
        double rt=(double)myCommunity_after.size()/duration;
		cout << "c.size/time:" << rt << endl;
		set <int> a;
		vector <int> cc;
		int myCommunity_num = myCommunity_after.size();
		for (int i = 0; i < myCommunity_num; i++)
		{
			vector <int> my = myCommunity_after[i];
			int num = my.size();
			for (int j = 0; j < num; j++)
			{
				int aaa = my[j];
				string s=theGlobalgraph.node_index_to_name[aaa];
				int vv = atoi(s.c_str());
				cc.push_back(vv);
			}
			sort(cc.begin(),cc.end());
			for (int j = 0; j < num; j++)
			{
				int aaa = cc[j];
				outCommunity << aaa << "  ";
			}
			cc.clear();
			outCommunity << endl;

		}


            cout<<endl<<"Calculating NMI..."<<endl;
            double NMI1,NMI2,NMI;
            vector <vector <int> > TrueCommunity;
            TrueCommunity = file2TrueCommunity(TrueCommunity_filename);

            cout<<"myCommunity.size():"<<myCommunity_after.size()<<endl;
            cout<<"TrueCommunity.size():"<<TrueCommunity.size()<<endl;

            NMI1 = CalculateNMI(TrueCommunity, myCommunity_after);
            NMI2 = CalculateNMI(myCommunity_after, TrueCommunity);
            NMI =1-(NMI1+NMI2)/2;
            cout<<"NMI："<<NMI<<endl<<endl;

           outresult << "   NMI:" << NMI << "   time:" << duration << "   runtime:" <<rt<<endl;
           outNMI << networkFileName1[index] << "   NMI:" << NMI << "   time:" << duration << "   runtime:" <<rt<< endl;



            outresult.close();
			outCommunity.close();
    }

    outNMI.close();
}
