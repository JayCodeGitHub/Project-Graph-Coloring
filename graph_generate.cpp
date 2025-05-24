#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    if (!(std::cin >> n)) {
        std::cerr << "Podaj liczbę wierzchołków!\n";
        return 1;
    }

    // Stopień wypełnienia grafu
    const double density = 0.4;

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << n << '\n';

    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            double r = std::rand() / (RAND_MAX + 1.0);
            if (r < density) {
                std::cout << i << ' ' << j << '\n';
            }
        }
    }

    return 0;
}
