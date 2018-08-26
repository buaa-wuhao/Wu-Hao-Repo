#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>

using namespace std;
class Graph
{
    private:
        int numOfNodes;
        vector<vector<pair<int,int>>> graph;
    public:
        Graph(int n);
        void connect(int n1,int n2,int w);
        int findShortest(int from,int to);
};

Graph::Graph(int n)
{
    numOfNodes = n;
    graph.resize(n,vector<pair<int,int>>());
}

void Graph::connect(int n1,int n2,int w)
{
    graph[n1].push_back({w,n2});
    graph[n2].push_back({w,n1});
    return;
}

int Graph::findShortest(int from,int to)
{
    vector<int> dist(numOfNodes,INT_MAX);
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
    dist[from] = 0;
    pq.push({0,from});
    while(!pq.empty())
    {
        pair<int,int> cur = pq.top();
        pq.pop();
        int node = cur.second;
        int distance = cur.first;

        if(distance > dist[node]) continue;

        for(int i=0;i<graph[node].size();i++)
        {
            int peer = graph[node][i].second;
            int weight = graph[node][i].first;
            if(distance+weight < dist[peer])
            {
                dist[peer] = dist[node] + weight;
                pq.push({dist[peer],peer});
            }
        }
    }

    for(int i=0;i<dist.size();i++)
    {
        cout<<i << " " << dist[i] << endl;
    }
    return dist[to];
}

int main()
{

    int V = 9;
    Graph g(V);
    //  making above shown graph
    g.connect(0, 1, 4);
    g.connect(0, 7, 8);
    g.connect(1, 2, 8);
    g.connect(1, 7, 11);
    g.connect(2, 3, 7);
    g.connect(2, 8, 2);
    g.connect(2, 5, 4);
    g.connect(3, 4, 9);
    g.connect(3, 5, 14);
    g.connect(4, 5, 10);
    g.connect(5, 6, 2);
    g.connect(6, 7, 1);
    g.connect(6, 8, 6);
    g.connect(7, 8, 7);

    g.findShortest(0,8);
    return 0;
}




