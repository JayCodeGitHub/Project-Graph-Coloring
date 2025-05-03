#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 1;

    vector<vector<int>> adj(n + 1);

    int u, v;
    while (cin >> u >> v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // wypisanie listy sąsiedztwa
    for (int i = 1; i <= n; ++i) {
        cout << i << ":";
        for (int to : adj[i]) {
            cout << " " << to;
        }
        cout << "\n";
    }

    return 0;
}