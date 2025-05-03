#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::string line;
    bool header_printed = false;

    while (std::getline(std::cin, line)) {
        if (line.empty()) 
            continue;

        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "c") {
            continue;
        }
        else if (token == "p") {
            std::string edge_word;
            int n, m;
            iss >> edge_word >> n >> m;
            if (!header_printed) {
                std::cout << n << "\n";
                header_printed = true;
            }
        }
        else if (token == "e") {
            int u, v;
            iss >> u >> v;
            std::cout << u << " " << v << "\n";
        }
    }

    return 0;
}

