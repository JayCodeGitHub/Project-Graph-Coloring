#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
using namespace std;

struct pair_hash {
    size_t operator()(const pair<int, int>& p) const {
        return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n))
        return 1;

    // Wczytanie grafu
    vector<vector<int>> adj(n + 1);
    int u, v;
    while (cin >> u >> v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    srand(time(0));

    // -------------------------
    // STAN 1: Pierwotne przypisanie n kolorów (0-indexowane)
    // Każdemu wierzchołkowi przypisujemy unikalny kolor od 0 do n-1.
    vector<int> color1(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        color1[i] = i - 1;
    }
    cout << "=== Stan 1: Pierwotne przypisanie " << n << " kolorów (0-indexowane) ===\n";
    for (int i = 1; i <= n; i++) {
        cout << "Wierzchołek: " << i << ", kolor: " << color1[i] << "\n";
    }
    cout << "\n";

    // -------------------------
    // STAN 2: Przypisanie 5 kolorów po usunięciu błędów.
    // Dopuszczalne kolory: 0,1,2,3,4.
    int fixedColorCount = 5;
    vector<int> color2 = color1;
    // Jeśli kolor jest spoza zakresu [0, fixedColorCount-1] to losujemy nowy (0-indexowany)
    for (int i = 1; i <= n; i++) {
        if (color2[i] >= fixedColorCount) {
            color2[i] = rand() % fixedColorCount;
        }
    }
    int tabuTenure = 8, maxIter = 10000, noImprovementLimit = 10000;
    clock_t startTime = clock();
    vector<vector<int>> tabu2(n + 1, vector<int>(fixedColorCount, 0));
    auto countConflicts = [&](const vector<int>& col) {
        int conf = 0;
        for (int i = 1; i <= n; i++) {
            for (int j : adj[i]) {
                if (i < j && col[i] == col[j])
                    conf++;
            }
        }
        return conf;
    };
    int conflicts = countConflicts(color2);
    int globalBestConflicts = conflicts;
    int iter = 0;
    int noImprovement = 0;
    while (conflicts > 0 && iter < maxIter &&
           ((double)(clock() - startTime) / CLOCKS_PER_SEC) < 180 &&
           noImprovement < noImprovementLimit) {
        vector<int> conflictNodes;
        for (int i = 1; i <= n; i++) {
            for (int j : adj[i]) {
                if (color2[i] == color2[j]) {
                    conflictNodes.push_back(i);
                    break;
                }
            }
        }
        if (conflictNodes.empty())
            break;
        int node = conflictNodes[rand() % conflictNodes.size()];
        int oldColor = color2[node];
        int bestDelta = INT_MAX, bestColorChoice = oldColor;
        for (int c = 0; c < fixedColorCount; c++) {
            if (c == oldColor)
                continue;
            int delta = 0;
            for (int j : adj[node]) {
                if (color2[j] == oldColor)
                    delta--;
                if (color2[j] == c)
                    delta++;
            }
            bool tabuMove = (tabu2[node][c] > iter);
            if ((!tabuMove && delta < bestDelta) ||
                (tabuMove && (conflicts + delta < globalBestConflicts))) {
                bestDelta = delta;
                bestColorChoice = c;
            }
        }
        color2[node] = bestColorChoice;
        tabu2[node][oldColor] = iter + tabuTenure;
        int newConflicts = countConflicts(color2);
        if (newConflicts < conflicts)
            noImprovement = 0;
        else
            noImprovement++;
        conflicts = newConflicts;
        if (conflicts < globalBestConflicts)
            globalBestConflicts = conflicts;
        iter++;
    }
    cout << "=== Stan 2: Przypisanie 5 kolorów po usunięciu błędów ===\n";
    for (int i = 1; i <= n; i++) {
        cout << "Wierzchołek: " << i << ", kolor: " << color2[i] << "\n";
    }
    cout << "\n";

    // -------------------------
    // STAN 3: Redukcja z 5 do 4 kolorów.
    // Dopuszczalne kolory: 0,1,2,3. Zmieniamy tylko te wierzchołki, które mają kolor niedozwolony (czyli >= 4).
    int newColorCount = 4;
    vector<int> color3 = color2;
    for (int i = 1; i <= n; i++) {
        if (color3[i] >= newColorCount) {
            color3[i] = rand() % newColorCount;
        }
    }
    cout << "=== Stan 3: Redukcja do 4 kolorów (zmiana tylko niedozwolonych) ===\n";
    for (int i = 1; i <= n; i++) {
        cout << "Wierzchołek: " << i << ", kolor: " << color3[i] << "\n";
    }
    cout << "\n";

    // -------------------------
    // STAN 4: Przypisanie 4 kolorów już bez błędów.
    // Uruchamiamy tabu search działającą z 4 kolorami (0..3), bazując na ustawieniu stanu 3.
    vector<int> color4 = color3;
    vector<vector<int>> tabu4(n + 1, vector<int>(newColorCount, 0));
    iter = 0;
    startTime = clock();
    conflicts = countConflicts(color4);
    globalBestConflicts = conflicts;
    noImprovement = 0;
    while (conflicts > 0 && iter < maxIter &&
           ((double)(clock() - startTime) / CLOCKS_PER_SEC) < 180 &&
           noImprovement < noImprovementLimit) {
        vector<int> conflictNodes;
        for (int i = 1; i <= n; i++) {
            for (int j : adj[i]) {
                if (color4[i] == color4[j]) {
                    conflictNodes.push_back(i);
                    break;
                }
            }
        }
        if (conflictNodes.empty())
            break;
        int node = conflictNodes[rand() % conflictNodes.size()];
        int oldColor = color4[node];
        int bestDelta = INT_MAX, bestColorChoice = oldColor;
        for (int c = 0; c < newColorCount; c++) {
            if (c == oldColor)
                continue;
            int delta = 0;
            for (int j : adj[node]) {
                if (color4[j] == oldColor)
                    delta--;
                if (color4[j] == c)
                    delta++;
            }
            bool tabuMove = (tabu4[node][c] > iter);
            if ((!tabuMove && delta < bestDelta) ||
                (tabuMove && (conflicts + delta < globalBestConflicts))) {
                bestDelta = delta;
                bestColorChoice = c;
            }
        }
        color4[node] = bestColorChoice;
        tabu4[node][oldColor] = iter + tabuTenure;
        int newConflicts = countConflicts(color4);
        if (newConflicts < conflicts)
            noImprovement = 0;
        else
            noImprovement++;
        conflicts = newConflicts;
        if (conflicts < globalBestConflicts)
            globalBestConflicts = conflicts;
        iter++;
    }
    cout << "=== Stan 4: Przypisanie 4 kolorów już bez błędów ===\n";
    for (int i = 1; i <= n; i++) {
        cout << "Wierzchołek: " << i << ", kolor: " << color4[i] << "\n";
    }
    cout << "\n";

    // -------------------------
    // STAN 5: Docelowe pokolorowanie – finalny algorytm.
    // Stopniowo redukujemy liczbę dostępnych kolorów.
    // Kolory są 0-indexowane, czyli dla maxColorsFinal dostępnych kolorów przyjmujemy zakres [0, maxColorsFinal-1].
    vector<int> bestColor(n + 1, 0);
    vector<int> currentColor(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        currentColor[i] = i - 1;
    }
    int bestColorCountFinal = n;
    int maxColorsFinal = n;
    clock_t finalStartTime = clock();
    while (maxColorsFinal >= 1 &&
           ((double)(clock() - finalStartTime) / CLOCKS_PER_SEC) < 180) {
        for (int i = 1; i <= n; i++) {
            if (currentColor[i] >= maxColorsFinal)
                currentColor[i] = rand() % maxColorsFinal;
        }
        vector<vector<int>> tabuFinal(n + 1, vector<int>(maxColorsFinal, 0));
        iter = 0;
        globalBestConflicts = INT_MAX;
        noImprovement = 0;
        int currentConflicts = countConflicts(currentColor);
        if (currentConflicts < globalBestConflicts)
            globalBestConflicts = currentConflicts;
        while (currentConflicts > 0 && iter < maxIter &&
               ((double)(clock() - finalStartTime) / CLOCKS_PER_SEC) < 180 &&
               noImprovement < noImprovementLimit) {
            vector<int> conflictNodes;
            for (int i = 1; i <= n; i++) {
                for (int j : adj[i]) {
                    if (currentColor[i] == currentColor[j]) {
                        conflictNodes.push_back(i);
                        break;
                    }
                }
            }
            if (conflictNodes.empty())
                break;
            int node = conflictNodes[rand() % conflictNodes.size()];
            int oldColor = currentColor[node];
            int bestDelta = INT_MAX, bestColorChoice = oldColor;
            for (int c = 0; c < maxColorsFinal; c++) {
                if (c == oldColor)
                    continue;
                int delta = 0;
                for (int j : adj[node]) {
                    if (currentColor[j] == oldColor)
                        delta--;
                    if (currentColor[j] == c)
                        delta++;
                }
                bool tabuMove = (tabuFinal[node][c] > iter);
                if ((!tabuMove && delta < bestDelta) ||
                    (tabuMove && (currentConflicts + delta < globalBestConflicts))) {
                    bestDelta = delta;
                    bestColorChoice = c;
                }
            }
            currentColor[node] = bestColorChoice;
            tabuFinal[node][oldColor] = iter + tabuTenure;
            int newConflicts = countConflicts(currentColor);
            if (newConflicts < currentConflicts)
                noImprovement = 0;
            else
                noImprovement++;
            currentConflicts = newConflicts;
            if (currentConflicts < globalBestConflicts)
                globalBestConflicts = currentConflicts;
            iter++;
        }
        if (currentConflicts == 0) {
            bestColor = currentColor;
            bestColorCountFinal = maxColorsFinal;
            maxColorsFinal--;
        } else {
            break;
        }
    }
    if (bestColorCountFinal <= n) {
        cout << "=== Stan 5: Docelowe pokolorowanie ===\n";
        for (int i = 1; i <= n; i++) {
            cout << "Wierzchołek: " << i << ", kolor: " << bestColor[i] << "\n";
        }
        cout << "Liczba wierzchołków: " << n 
             << ", liczba kolorów: " << bestColorCountFinal << "\n";
    } else {
        cout << "Nie udało się znaleźć poprawnego kolorowania bez konfliktów.\n";
    }
    return 0;
}