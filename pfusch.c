#include <stdio.h>
#include <assert.h>

#define KSI  "\x1b[0m"
#define YLW  "\x1b[93m"
#define BLU  "\x1b[94m"
#define RED  "\x1b[91m"
#define BLK  "\x1b[90m"

void print_matrix(int size, int matrix[size][size]) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int const value = matrix[i][j];
            if (value > 0) printf(BLU);
            if (value < 0) printf(RED);
            if (value == 0) printf(BLK);

            if (i == j) printf(YLW);

            printf("%3d " KSI, value);
        }
        printf("\n");
    }
}

#define NOF_VERTICES 10

typedef struct Edge {
    int from;
    int to;
    int weight;
} Edge;

void generate_adjacency_matrix(int nov, Edge const *edges, int adjmx[nov][nov]) {
    assert(nov > 1); // at least 2 vertices
    assert(edges != NULL && edges[0].weight != -1); // non-empty edge list

    for (int i = 0; edges[i].weight != -1; ++i) {
        Edge e = edges[i];
        printf("Inserting edge #%d from %d to %d with weight %d\n", i + 1, e.from, e.to, e.weight);
        assert(e.from >= 0 && e.from < nov); // valid start node
        assert(e.to >= 0 && e.to < nov);     // valid end node
        assert(e.weight > 0);                // valid weight
        assert(e.from != e.to);              // start != end
        assert(adjmx[e.from][e.to] == 0);    // not yet filled

        adjmx[e.from][e.to] = e.weight;
        adjmx[e.to][e.from] = -e.weight;
    }
}

int main() {
    int adjmx[NOF_VERTICES][NOF_VERTICES] = { 0 };

    Edge const edges[] = {
        {0, 1, 10},
        {1, 2, 20},
        {2, 3, 30},
        {3, 4, 40},
        {4, 5, 50},
        {5, 6, 60},
        {6, 7, 70},
        {7, 8, 80},
        {8, 9, 90},
        {-1, -1, -1} // sentinel
    };

    generate_adjacency_matrix(NOF_VERTICES, edges, adjmx);
    print_matrix(NOF_VERTICES, adjmx);

    return 0;
}