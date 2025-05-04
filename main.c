#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROW 6
#define COL 4
#define MAX_ITEMS 4

int minsup = 2;


bool MTX[ROW][COL] = {
    {1,0,1,0},
    {0,1,0,1},
    {1,1,1,0},
    {0,0,1,0},
    {1,0,0,0},
    {1,1,1,0}
};

char obj[COL] = {'a','b','c','d'};

int count_support(int *items, int size) {
    int count = 0;
    for(int i = 0; i < ROW; i++) {
        bool found = true;
        for(int j = 0; j < size; j++) {
            if(MTX[i][items[j]] == 0) {
                found = false;
                break;
            }
        }
        if(found) count++;
    }
    return count;
}


void generate_L1(int *valid_items, int *valid_count) {
    printf("=== L1 (1-itemsets) ===\n");
    int count = 0;
    for(int i = 0; i < COL; i++) {
        int sup = count_support(&i, 1);
        if(sup >= minsup) {
            printf("{%c} sup=%d\n", obj[i], sup);
            valid_items[count++] = i;
        }
    }
    *valid_count = count;
}

void generate_L2(int *prev_items, int prev_count, int (*valid_pairs)[2], int *pair_count) {
    printf("\n=== L2 (2-itemsets) ===\n");
    int count = 0;
    for(int i = 0; i < prev_count; i++) {
        for(int j = i + 1; j < prev_count; j++) {
            int items[2] = {prev_items[i], prev_items[j]};
            int sup = count_support(items, 2);
            if(sup >= minsup) {
                printf("{%c,%c} sup=%d\n", obj[items[0]], obj[items[1]], sup);
                valid_pairs[count][0] = items[0];
                valid_pairs[count][1] = items[1];
                count++;
            }
        }
    }
    *pair_count = count;
}


void generate_L3(int pairs[][2], int pair_count) {
    printf("\n=== L3 (3-itemsets) ===\n");
    for(int i = 0; i < pair_count; i++) {
        for(int j = i + 1; j < pair_count; j++) {
            int a1 = pairs[i][0], b1 = pairs[i][1];
            int a2 = pairs[j][0], b2 = pairs[j][1];
            int common = -1, unique1 = -1, unique2 = -1;

            if(a1 == a2) { common = a1; unique1 = b1; unique2 = b2; }
            else if(a1 == b2) { common = a1; unique1 = b1; unique2 = a2; }
            else if(b1 == a2) { common = b1; unique1 = a1; unique2 = b2; }
            else if(b1 == b2) { common = b1; unique1 = a1; unique2 = a2; }

            if(common != -1 && unique1 != unique2) {
                int items[3] = {common, unique1, unique2};
                if(items[0] != items[1] && items[0] != items[2] && items[1] != items[2]) {
                    int sup = count_support(items, 3);
                    if(sup >= minsup) {
                        printf("{%c,%c,%c} sup=%d\n", obj[items[0]], obj[items[1]], obj[items[2]], sup);
                    }
                }
            }
        }
    }
}

int main() {
    int L1_items[COL];
    int L1_count = 0;

    int L2_pairs[COL * COL][2];
    int L2_count = 0;

    generate_L1(L1_items, &L1_count);
    generate_L2(L1_items, L1_count, L2_pairs, &L2_count);
    generate_L3(L2_pairs, L2_count);

    return 0;
}
