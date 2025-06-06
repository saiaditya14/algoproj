#include <bits/stdc++.h>
using namespace std;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx,avx2,fma")

const int N = 1e7 + 5, M = 4e7 + 5;
const int LOG_N = 24;
vector<pair<int, int> > g[N];
vector<pair<int, int> > tree_edges[N], non_tree_edges[N];
bool vis[N], vis_edges[M];
int preorder[N], parent[N], parent_edge_number[N];
int lca[LOG_N][N], depth[N];
int min_depth_type_c_edges[N];
int tim = 1;

vector<int> components[M];
vector<pair<int, int> > edges(M);

int dsu_parent[M];

int find(int x) {
    if(dsu_parent[x] != x) {
        dsu_parent[x] = find(dsu_parent[x]);
    }
    return dsu_parent[x];
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if(a != b) {
        dsu_parent[a] = b;
    }
}

void get_spanning_tree(int node, int par) {
    lca[0][node] = par;
    vis[node] = true;
    preorder[node] = tim++;
    parent[node] = par;
    for(auto [to, edge_num]: g[node]) {
        if(!vis[to]) {
            vis_edges[edge_num] = true;
            tree_edges[node].push_back({to, edge_num});
            tree_edges[to].push_back({node, edge_num});
            // cout << "tree edge: " << node << " " << to << " with edge number = " << edge_num << endl;
            parent_edge_number[to] = edge_num;
            depth[to] = depth[node] + 1;
            get_spanning_tree(to, node);
        }
        else {
            if(to != par && !vis_edges[edge_num]) {
                vis_edges[edge_num] = true;
                non_tree_edges[node].push_back({to, edge_num});
                non_tree_edges[to].push_back({node, edge_num});
                // cout << "non-tree edge: " << node << " " << to << " with edge number = " << edge_num << endl;
            }
        }
    }
}

int find_ancestor(int u, int v) {
    if(depth[u] < depth[v]) {
        swap(u, v);
    }
    int diff = depth[u] - depth[v];
    for(int i = LOG_N - 1; i >= 0; i--) {
        if((1 << i) & diff) {
            u = lca[i][u];
        }
    }
    if(u == v) {
        return u;
    }
    for(int i = LOG_N - 1; i >= 0; i--) {
        if(lca[i][u] != lca[i][v]) {
            u = lca[i][u];
            v = lca[i][v];
        }
    }
    return lca[0][u];
}

void find_min_depth_in_subtree(int node, int par) {
    vis[node] = true;
    for(auto [to, edge_num]: tree_edges[node]) {
        if(!vis[to]) {
            find_min_depth_in_subtree(to, node);
            min_depth_type_c_edges[node] = min(min_depth_type_c_edges[node], min_depth_type_c_edges[to]);
        }
    }
    
}

int main() {
    int n, m;
    cin >> n >> m;
    iota(dsu_parent, dsu_parent + m + 1, 0);
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back({v, i + 1});
        g[v].push_back({u, i + 1});
        edges[i + 1] = {u, v};
    }

    auto begin = std::chrono::high_resolution_clock::now();
    for(int i = 1; i <= n; i++) {
        min_depth_type_c_edges[i] = INT_MAX;
        if(!vis[i]) {
            get_spanning_tree(i, -1);
        }
    }

    for(int i = 1; i < LOG_N; i++) {
        for(int j = 1; j <= n; j++) {
            lca[i][j] = (lca[i - 1][j] == -1) ? -1 : lca[i - 1][lca[i - 1][j]];
        }
    }  

    for(int i = 1; i <= n; i++) {
        vis[i] = false;
        for(auto [to, edge_num]: non_tree_edges[i]) {
            // add edges of type (a) -- (u, v) to (w, v) such that u = par[v] and pre[w] < pre[v]
            if(preorder[to] < preorder[i]) {
                unite(parent_edge_number[i], edge_num);
                // edge_graph[parent_edge_number[i]].push_back(edge_num);
                // edge_graph[edge_num].push_back(parent_edge_number[i]);
                // cout << "Adding edge of type (a): " << parent_edge_number[i] << " " << edge_num << endl;
            }
            // add edges of type (b) -- (u, v) to (x, y) such that u = par[v], x = par[y] and (v, y) is a non-tree edge while they are not in an ancestor-descendant relationship
            int LCA = find_ancestor(i, to);
            if(LCA != i && LCA != to) {
                unite(parent_edge_number[i], parent_edge_number[to]);
                // edge_graph[parent_edge_number[to]].push_back(parent_edge_number[i]);
                // edge_graph[parent_edge_number[i]].push_back(parent_edge_number[to]);
                // cout << "Adding edge of type (b): " << parent_edge_number[to] << " " << parent_edge_number[i] << endl;
            }
            // min_depth_type_c_edges[i] tells us how far we need to go up to add type c edges
            min_depth_type_c_edges[i] = min(min_depth_type_c_edges[i], depth[LCA]);
            // while(depth[LCA] < depth[current_node]) {
            //     // cout << "i = " << i << ", current_node = " << current_node << ", LCA = " << LCA << endl;
            //     edge_graph[parent_edge_number[current_node]].push_back(parent_edge_number[i]);
            //     edge_graph[parent_edge_number[i]].push_back(parent_edge_number[current_node]);
            //     // cout << "Adding edge of type (c): " << parent_edge_number[current_node] << " " << parent_edge_number[i] << endl;
            //     current_node = parent[current_node];
            // }
        }
    }

    for(int i = 1; i <= n; i++) {
        if(!vis[i]) {
            find_min_depth_in_subtree(i, -1);
        }
    }

    for(int i = 1; i <= n; i++) {
        if(min_depth_type_c_edges[i] < depth[i] - 1) {
            int parent_of_i = parent[i];
            unite(parent_edge_number[i], parent_edge_number[parent_of_i]);
            // edge_graph[parent_edge_number[i]].push_back(parent_edge_number[parent_of_i]);
            // edge_graph[parent_edge_number[parent_of_i]].push_back(parent_edge_number[i]);
            // cout << "Adding edge of type (c): " << parent_edge_number[parent_of_i] << " " << parent_edge_number[i] << endl;
        }
    }

    int components_count = 0;
    map<int, int> component_map;
    for(int i = 1; i <= m; i++) {
        if(dsu_parent[i] == i) {
            components_count++;
            component_map[i] = components_count;
        }
    }

    for(int i = 1; i <= m; i++) {
        int root = find(i);
        components[component_map[root]].push_back(i);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    cout << "There are " << components_count << " biconnected components.\n";
    // for(int i = 1; i <= components_count; i++) {
    //     cout << "Component " << i << ": ";
    //     for(auto edge_num: components[i]) {
    //         cout << "(" << edges[edge_num].first << ", " << edges[edge_num].second << ") ";
    //     }
    //     cout << "\n";
    // }
    cout << "Time measured: " << elapsed.count() * 1e-9 << " seconds.\n";
}
