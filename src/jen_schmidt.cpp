#include <bits/stdc++.h>
using namespace std;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx,avx2,fma")

#define int long long
#define pb push_back
#define fi first
#define se second
#define ld long double
#define pi pair<int, int>
#define mp make_pair
#define vi vector<int>
#define vvi vector<vector<int>>

const int INF = (int)4e18;
const int MODa = 1e9 + 7;
const int MOD = 998244353;
const int N = 1e7 + 5;

int n, m, tin[N], ans = 0;
bool vis[N], chain_vis[N];
vi g[N], euler;
vector<pair<int, bool>> dir[N];
unordered_map<int, unordered_set<int>> processed_edges;

int bin_exp(int x, int y) {
    x %= MOD;
    int res = 1;
    while (y > 0) {
        if (y % 2) {
            res = (res * x) % MOD;
        }
        x = (x * x) % MOD;
        y /= 2;
    }
    return res;
}

int closestpow(int x) {
    if ((x & (x - 1)) == 0) return x;
    int power = 1;
    while (power < x) power <<= 1;
    return power;
}

int mi(int x) {
    return bin_exp(x, MOD-2);
}

int add(int a, int b) {
    return (a+b)%MOD;
}

int mult(int a, int b) {
    return (a*b)%MOD;
}

void mark_edge_processed(int u, int v) {
    processed_edges[u].insert(v);
    processed_edges[v].insert(u);
}

bool is_edge_processed(int u, int v) {
    return processed_edges[u].find(v) != processed_edges[u].end();
}

void dfs(int node, int par) {
    euler.pb(node);
    tin[node] = euler.size();
    vis[node] = true;
    for(auto k : g[node]) {
        if(!vis[k]) {
            dir[k].pb({node, 0});
            dfs(k, node);
        }
        else if(vis[k] && k != par && tin[node] > tin[k])
            dir[k].pb({node, 1});
    }
}

void solve() {
    cin >> n >> m;
    for(int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].pb(v);
        g[v].pb(u);
    }
    
    auto begin = std::chrono::high_resolution_clock::now();
    
    for(int i = 1; i <= n; ++i) {
        if(!vis[i]) {
            dfs(i, 0);
        }
    }
    
    for(int i = 1; i <= n; ++i) {
        vis[i] = false;
    }
    
    for(auto k : euler) {
        for(auto p : dir[k]) {
            if(!p.se) continue;
            
            int v = p.fi;
            if(is_edge_processed(k, v)) continue;
            
            vector<int> chain;
            chain.pb(k);
            
            bool dont = false;
            if(chain_vis[v]) {
                chain.pb(v);
                dont = true;
            }
            
            chain_vis[k] = true;
            mark_edge_processed(k, v);
            
            while(v != k) {
                if(chain_vis[v]) break;
                chain.pb(v);
                chain_vis[v] = true;
                
                bool moved = false;
                for(auto dest : dir[v]) {
                    if(!chain_vis[dest.fi] && dest.se == 0 && !is_edge_processed(v, dest.fi)) {
                        mark_edge_processed(v, dest.fi);
                        v = dest.fi;
                        moved = true;
                        break;
                    }
                }
                
                if(!moved && !dir[v].empty()) {
                    int next_v = dir[v][0].fi;
                    if(!is_edge_processed(v, next_v)) {
                        mark_edge_processed(v, next_v);
                        v = next_v;
                        moved = true;
                    }
                }
                
                if(!moved) break;
            }
            
            if(!dont) {
                chain.pb(v);
            }
            
            if(chain[0] == chain[chain.size() - 1]) {
                ans++;
            }
        }
    }
    
    for(int u = 1; u <= n; ++u) {
        for(auto v : g[u]) {
            if(u < v && !is_edge_processed(u, v)) {
                ans++;
                mark_edge_processed(u, v);
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout << "There are " << ans << " biconnected components.\n";
    cout << "Time measured in solve: " << elapsed.count() * 1e-9 << " seconds.\n";
}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    while (t--) {
        solve();
    }
    return 0;
}
