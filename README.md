
#### Compile
```
g++ -std=c++11 graph_generate.cpp -o binaries/graph_generate
```
```
g++ -std=c++11 graph_coloring_greedy.cpp -o binaries/graph_coloring_greedy
```

```
g++ -std=c++11 format.cpp -o binaries/format
```

#### Run
```
echo 23 | ./binaries/graph_generate | ./binaries/graph_coloring_greedy
```

```
cat ./datasets/fpsol2.i.1.col | ./binaries/format | ./binaries/graph_coloring_greedy
```
