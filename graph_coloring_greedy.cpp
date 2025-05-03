#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 1;

    // wczytaj listę sąsiedztwa
    vector<vector<int>> adj(n + 1);
    int u, v;
    while (cin >> u >> v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // implementacja
    vector<int> color(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        vector<bool> used(n + 1, false);
        for (int to : adj[i]) {
            if (color[to] != 0)
                used[color[to]] = true;
        }
        int c = 1;
        while (used[c]) ++c;
        color[i] = c;
    }

    // wynik
    for (int i = 1; i <= n; ++i) {
        cout << "Wierzchołek: " << i
             << ", kolor: " << color[i]
             << "\n";
    }
    cout << "\n";
    int max_color = 0;
    for (int i = 1; i <= n; ++i) {
        if (color[i] > max_color)
            max_color = color[i];
    }
    cout << "Liczba wierzchołków: " << n
         << ", liczba kolorów: " << max_color
         << "\n";

    return 0;
}

// // wypisanie listy sąsiedztwa
// for (int i = 1; i <= n; ++i) {
//     cout << i << ":";
//     for (int to : adj[i]) {
//         cout << " " << to;
//     }
//     cout << "\n";
// }