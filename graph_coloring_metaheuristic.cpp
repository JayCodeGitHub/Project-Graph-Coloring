#include <iostream>
#include <vector>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <climits>
#include <cstdlib>

using namespace std;

struct pair_hash {
    size_t operator()(const pair<int, int>& pair) const {
        return hash<int>()(pair.first) ^ (hash<int>()(pair.second) << 1);
    }
};

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

    int maxColors = n;
    int minColors = 1;
    int tabuTenure = 8;
    int maxIter = 10000;
    int maxTime = 180;
    int noImprovementLimit = 10000;

    vector<int> color(n + 1);
    vector<int> bestColor(n + 1);
    int bestColorCount = n + 1;

    srand(time(0));
    clock_t startTime = clock();

    while (maxColors >= minColors && ((double)(clock() - startTime) / CLOCKS_PER_SEC) < maxTime) {
        for (int i = 1; i <= n; ++i)
            color[i] = (color[i] > maxColors) ? (rand() % maxColors + 1) : color[i];

        vector<vector<int>> tabu(n + 1, vector<int>(maxColors + 2, 0));
        int iter = 0;
        int globalBestConflicts = INT_MAX;
        int noImprovement = 0;

        auto countConflicts = [&](const vector<int>& col) {
            int conf = 0;
            for (int u = 1; u <= n; ++u)
                for (int v : adj[u])
                    if (u < v && col[u] == col[v])
                        conf++;
            return conf;
        };

        int conflicts = countConflicts(color);
        if (conflicts < globalBestConflicts) globalBestConflicts = conflicts;

        while (conflicts > 0 && iter < maxIter &&
               ((double)(clock() - startTime) / CLOCKS_PER_SEC) < maxTime &&
               noImprovement < noImprovementLimit) {
            vector<int> conflictNodes;
            for (int u = 1; u <= n; ++u)
                for (int v : adj[u])
                    if (color[u] == color[v]) {
                        conflictNodes.push_back(u);
                        break;
                    }

            int u = conflictNodes[rand() % conflictNodes.size()];
            int oldColor = color[u];
            int bestDelta = INT_MAX, bestColorChoice = oldColor;

            for (int c = 1; c <= maxColors; ++c) {
                if (c == oldColor) continue;
                int delta = 0;
                for (int v : adj[u]) {
                    if (color[v] == oldColor) delta--;
                    if (color[v] == c) delta++;
                }
                bool tabuMove = (tabu[u][c] > iter);
                if ((!tabuMove && delta < bestDelta) ||
                    (tabuMove && (conflicts + delta < globalBestConflicts))) {
                    bestDelta = delta;
                    bestColorChoice = c;
                }
            }

            color[u] = bestColorChoice;
            tabu[u][oldColor] = iter + tabuTenure;
            int newConflicts = countConflicts(color);

            if (newConflicts < conflicts) {
                noImprovement = 0;
            } else {
                noImprovement++;
            }

            conflicts = newConflicts;
            if (conflicts < globalBestConflicts) globalBestConflicts = conflicts;

            iter++;
        }

        if (conflicts == 0) {
            bestColor = color;
            bestColorCount = maxColors;
            maxColors--;
        } else {
            break;
        }
    }

    if (bestColorCount <= n) {
        for (int i = 1; i <= n; ++i)
            cout << "Wierzchołek: " << i << ", kolor: " << bestColor[i] << "\n";
        cout << "Liczba wierzchołków: " << n
             << ", liczba kolorów: " << bestColorCount << "\n";
    } else {
        cout << "Nie udało się znaleźć poprawnego kolorowania bez konfliktów.\n";
    }
    return 0;
}