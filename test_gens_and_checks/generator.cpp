#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;
 
int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	int n = opt<int>(1);
	int m = opt<int>(2);
    set<pair<int, int> > st;
	vector<pair<int, int> > edges;
    for(int i = 0; i < m; i++) {
        int u = rnd.next(1, n);
        int v = rnd.next(1, n);
        if(u == v) {
            i--;
            continue;
        }
        if(st.count({u, v}) || st.count({v, u})) {
            i--;
            continue;
        }
        st.insert({u, v});
        edges.push_back({u, v});
    }
    shuffle(edges.begin(), edges.end());
    string string_n = to_string(n);
    string string_m = to_string(m);
    assert(edges.size() == m);
    const string file_name = "../tests/test_" + string_n + "_" + string_m + ".in";
    ofstream f(file_name);
    f << n << " " << m << endl;
    for(auto &[u, v]: edges) {
        f << u << " " << v << endl;
    }
    f.close();
    return 0;
}
