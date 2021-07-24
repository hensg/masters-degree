//https://codeforces.com/gym/100287/attachments
//circular cellular automata
/*
References:
Cellular automaton: https://en.wikipedia.org/wiki/Cellular_automaton
Worfman Code: https://en.wikipedia.org/wiki/Wolfram_code
: https://www.youtube.com/watch?v=DKGodqDs9sA
*/
#include <vector>
#include <cmath>
#include <stdio.h>

int sum_neighbors(const std::vector<int>& cells_state, const int n, const int m, const int d, const int i) {
    int sum = 0;
    int j = (n - d + i) % n;
    for (int count = -d; count <= d; count++) {
        sum += cells_state[j++];
        if (j == n) j = 0;
    }
    return sum % m;
}

std::vector<int> run_cellular_automaton(const std::vector<int>& initial_cells_state,
                                        const int n, const int m, const int k, const int d) {
    int limit = k % ((int)std::pow(m, d+1)-1);

    std::vector<int> current_cell_state = initial_cells_state;
    std::vector<int> next_cells_state(n);
    for (int step = 0; step < limit; step++) {
        for (int i = 0; i < n; i++) {
            next_cells_state[i] = sum_neighbors(current_cell_state, n, m, d, i);
        }
        current_cell_state = next_cells_state;
    }
    return current_cell_state;
}

int main(int, char**) {
    // codeforces
    FILE* infile, *outfile;
    infile = fopen("cell.in", "r");
    outfile = fopen("cell.out", "w");

    int n, m, d, k;
    while (fscanf(infile, "%d %d %d %d\n", &n, &m, &d, &k) == 4) {

        std::vector<int> cells_state(n);
        int tmp, i = 0;
        while (i < n && fscanf(infile, "%d", &tmp))
            cells_state[i++] = tmp;

        std::vector<int> final_cells_state = run_cellular_automaton(cells_state, n, m, k, d);
        
        fprintf(outfile, "%d", final_cells_state[0]);
        for (i = 1; i < n; i++) {
            fprintf(outfile, " %d", final_cells_state[i]);
        }
        fprintf(outfile, "\n");
    }
    return 0;
}