#include <bits/stdc++.h>
using namespace std;
#define int long long

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

const int INF = 1e18;
const int N = 1e7 + 5;
vector<int> g[N];
int disc[N], low[N], timer = 1;
stack<pair<int, int> > processed_edges;
vector<vector<pair<int, int> > > biconnected_components_list;

void solve_tarjan(int node, int par) {
    disc[node] = timer++;
    low[node] = timer - 1;

    int children = 0;
    for(int to: g[node]) {
        if(to == par) {
            continue;
        }
        if(disc[to] != -1) {
            low[node] = min(low[node], disc[to]);
            if(disc[to] < disc[node]) {
                processed_edges.push({node, to});
            }
        }
        else {
            children++;
            processed_edges.push({node, to});
            solve_tarjan(to, node);
            low[node] = min(low[node], low[to]);

            if((disc[node] == 1 && children > 1) || (disc[node] != 1 && low[to] >= disc[node])) {
                vector<pair<int, int> > component_edge_list;
                while(processed_edges.top() != make_pair(node, to) && processed_edges.top() != make_pair(to, node)) {
                    component_edge_list.push_back(processed_edges.top());
                    processed_edges.pop();
                }
                component_edge_list.push_back(processed_edges.top());
                processed_edges.pop();
                biconnected_components_list.push_back(component_edge_list);
            }
        }
    }
}

void Solve() 
{
    int n, m;
    cin >> n >> m;
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for(int i = 1; i <= n; i++) {
        disc[i] = -1;
        low[i] = -1;
    }
    auto begin = std::chrono::high_resolution_clock::now();
    for(int i = 1; i <= n; i++) {
        if(disc[i] != -1) {
            continue;
        }
        solve_tarjan(i, -1);
        vector<pair<int, int> > component_edge_list;
        bool endline = false;
        while(!processed_edges.empty()) {
            endline = true;
            component_edge_list.push_back(processed_edges.top());
            processed_edges.pop();
        }
        if(endline) {
            biconnected_components_list.push_back(component_edge_list);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    cout << "There are " << biconnected_components_list.size() << " biconnected components.\n";
    cout << "Time measured: " << elapsed.count() * 1e-9 << " seconds.\n"; 
    // for(int i = 0; i < biconnected_components_list.size(); i++) {
    //     cout << "Component " << i + 1 << ": ";
    //     for(auto &edge: biconnected_components_list[i]) {
    //         cout << "(" << edge.first << ", " << edge.second << ") ";
    //     }
    //     cout << "\n";
    // }
}

int32_t main() 
{
    
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    // cin >> t;
    for(int i = 1; i <= t; i++) 
    {
        //cout << "Case #" << i << ": ";
        Solve();
    }
    return 0;
}