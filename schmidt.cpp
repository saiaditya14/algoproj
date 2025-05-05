#include <bits/stdc++.h>
using namespace std;

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
const int N = 1e6 + 5;

int n, m, tin[N];
bool vis[N];
vi g[N], euler;
vector<pi> bicomponents[N];
vector<pi> edges[N];
vector<bool> proc[N];
vector<vector<pi>> comps;

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

void dfs(int u, int p) {
    euler.pb(u);
    tin[u] = euler.size();
    vis[u] = true;
    
    for(auto v : g[u]) {
        if(!vis[v]) {
            edges[v].pb(mp(u, 0));
            dfs(v, u);
        }
        else if(vis[v] && v != p && tin[u] > tin[v]) {
            edges[v].pb(mp(u, 1));
        }
    }
}

void add_edge(int u, int v, int id) {
    if(u > v) swap(u, v);
    
    while(proc[u].size() <= v - u)
        proc[u].pb(false);
    proc[u][v - u] = true;
    
    bicomponents[u].pb(mp(v, id));
    bicomponents[v].pb(mp(u, id));
}

bool is_proc(int u, int v) {
    if(u > v) swap(u, v);
    
    if(proc[u].size() <= v - u)
        return false;
    return proc[u][v - u];
}

void biconn() {
    for(int i = 1; i <= n; ++i) {
        proc[i].clear();
    }
    
    for(int st = 1; st <= n; ++st) {
        if(!vis[st]) {
            euler.clear();
            dfs(st, 0);
            
            for(int i = 1; i <= n; ++i) {
                vis[i] = false;
            }
            
            for(auto u : euler) {
                for(auto e : edges[u]) {
                    if(!e.se) continue;
                    
                    int v = e.fi;
                    
                    if(is_proc(u, v)) continue;
                    
                    vector<pi> chain;
                    chain.pb(mp(u, v));
                    
                    if(vis[v]) {
                        comps.pb(chain);
                        int id = comps.size() - 1;
                        add_edge(u, v, id);
                        continue;
                    }
                    
                    vis[u] = true;
                    
                    while(v != u) {
                        if(vis[v]) break;
                        
                        vis[v] = true;
                        bool found = false;
                        
                        for(auto nx : edges[v]) {
                            if(!vis[nx.fi] && nx.se == 0) {
                                chain.pb(mp(v, nx.fi));
                                v = nx.fi;
                                found = true;
                                break;
                            }
                        }
                        
                        if(!found && !edges[v].empty()) {
                            chain.pb(mp(v, edges[v][0].fi));
                            v = edges[v][0].fi;
                        }
                    }
                    
                    if(u == v) {
                        comps.pb(chain);
                        int id = comps.size() - 1;
                        
                        for(auto& e : chain) {
                            add_edge(e.fi, e.se, id);
                        }
                    } else {
                        bool added = false;
                        int target = -1;
                        
                        for(auto comp : bicomponents[u]) {
                            target = comp.se;
                            added = true;
                            break;
                        }
                        
                        if(!added) {
                            for(auto comp : bicomponents[v]) {
                                target = comp.se;
                                added = true;
                                break;
                            }
                        }
                        
                        if(added) {
                            for(auto& e : chain) {
                                comps[target].pb(e);
                                add_edge(e.fi, e.se, target);
                            }
                        } else {
                            comps.pb(chain);
                            int id = comps.size() - 1;
                            
                            for(auto& e : chain) {
                                add_edge(e.fi, e.se, id);
                            }
                        }
                    }
                }
            }
        }
    }
    
    for(int u = 1; u <= n; ++u) {
        for(auto v : g[u]) {
            if(u < v) {
                if(!is_proc(u, v)) {
                    vector<pi> single;
                    single.pb(mp(u, v));
                    
                    comps.pb(single);
                    int id = comps.size() - 1;
                    add_edge(u, v, id);
                }
            }
        }
    }
}

void print() {
    cout << "Number of biconnected components: " << comps.size() << endl;
    
    for(int i = 0; i < comps.size(); ++i) {
        cout << "Component " << i+1 << ": ";
        for(auto& e : comps[i]) {
            cout << "(" << e.fi << "," << e.se << ") ";
        }
        cout << endl;
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
    biconn();
    print();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Time measured in solve: " << elapsed.count() * 1e-9 << " seconds.\n";
}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    //cin >> t;
    auto begin = std::chrono::high_resolution_clock::now();

    while (t--) {
        solve();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Total time measured: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    return 0;
}
