#### Compile

```
g++ -std=c++11 format.cpp -o binaries/format
```

```
g++ -std=c++11 graph_generate.cpp -o binaries/graph_generate
```

```
g++ -std=c++11 graph_coloring_greedy.cpp -o binaries/graph_coloring_greedy
```

```
g++ -std=c++11 graph_coloring_metaheuristic.cpp -o binaries/graph_coloring_metaheuristic
```

#### Run Greedy

```
echo 23 | ./binaries/graph_generate | ./binaries/graph_coloring_greedy
```

```
cat ./datasets/fpsol2.i.1.col | ./binaries/format | ./binaries/graph_coloring_greedy
```

#### Run Metaheuristic

```
echo 23 | ./binaries/graph_generate | ./binaries/graph_coloring_metaheuristic
```

```
cat ./datasets/fpsol2.i.1.col | ./binaries/format | ./binaries/graph_coloring_metaheuristic
```

#### Graph Coloring Instances

[mat.tepper.cmu.edu/COLOR/instances](https://mat.tepper.cmu.edu/COLOR/instances.html)

#### Tabu Search Algorithm assumptions:

| Założenie                                  | Wartość                                                           |
| ------------------------------------------ | ----------------------------------------------------------------- |
| Liczba wierzchołków                        | `n`                                                               |
| Liczba kolorów `maxColors`                 | Iteracyjnie w dół, Początkowa do ustalenia start(n)               |
| Rozmiar listy tabu `tabuTenure`            | Do ustalenia start(n/2)                                           |
| Maksymalna liczba iteracji `maxIterations` | Do ustalenia start(1000/10000) i schodzimy w dół                  |
| Limit braku poprawy `noImprovementLimit`   | Do ustalenia start(100)                                           |
| Rozmiar sąsiedztwa                         | `n * (maxColors - 1)`                                             |
|                                            |                                                                   |
| Kryterium aspiracji                        | `Pozwala na tabu ruch, jeśli prowadzi do najlepszego rozwiązania` |
| Strategia wyboru ruchu                     | `Best improvement`                                                |
| Rozwiązanie początkowe                     | `Losowe kolorowanie`                                              |
| Warunki zakończenia                        | `conflicts == 0` OR `maxTime 3m`                                  |
| Zapisujemy najlepsze wyniki                | `best-so-far`                                                     |
| Zakaz ponownej zmiany koloru wierzchołka   | `Vertex + Color`                                                  |
| Typ ruchu                                  | `Zmiana koloru jednego wierzchołka na inny`                       |
