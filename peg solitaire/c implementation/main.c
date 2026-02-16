// solo_test_solver.c
// Complete, Pure C Implementation of Peg Solitaire Solver (7x7 Board)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_NODES 33
#define MAX_STACK 100000

typedef struct Node {
    char is_filled;
    struct Node* left;
    struct Node* right;
    struct Node* up;
    struct Node* down;
} Node;

typedef struct Board {
    Node* all_nodes[TOTAL_NODES];
} Board;

typedef struct Stack {
    Board* items[MAX_STACK];
    int top;
} Stack;

void push(Stack* s, Board* b) {
    if (s->top < MAX_STACK) {
        s->items[s->top++] = b;
    }
}

Board* pop(Stack* s) {
    return (s->top > 0) ? s->items[--s->top] : NULL;
}

int is_empty(Stack* s) {
    return s->top == 0;
}

int count_filled(Board* b) {
    int count = 0;
    for (int i = 0; i < TOTAL_NODES; i++) {
        if (b->all_nodes[i]->is_filled == 'F') count++;
    }
    return count;
}

int try_move(Board* board, Node* from, Node* over, Node* to) {
    
    if (from && over && to &&
        from->is_filled == 'F' &&
        over->is_filled == 'F' &&
        to->is_filled == 'E') {
            return 1;
    }
    return 0;
}

void move(Board* board, Node* from, Node* over, Node* to){
    from->is_filled = 'E';
        over->is_filled = 'E';
        to->is_filled = 'F';
}

Board* copy_board(Board* original) {
    Board* new_board = (Board*) malloc(sizeof(Board));
    Node* node_map[TOTAL_NODES];

    for (int i = 0; i < TOTAL_NODES; i++) {
        node_map[i] = (Node*) malloc(sizeof(Node));
        *node_map[i] = *original->all_nodes[i];
    }

    for (int i = 0; i < TOTAL_NODES; i++) {
        Node* orig = original->all_nodes[i];
        Node* copy = node_map[i];

        for (int j = 0; j < TOTAL_NODES; j++) {
            if (orig->left  == original->all_nodes[j]) copy->left  = node_map[j];
            if (orig->right == original->all_nodes[j]) copy->right = node_map[j];
            if (orig->up    == original->all_nodes[j]) copy->up    = node_map[j];
            if (orig->down  == original->all_nodes[j]) copy->down  = node_map[j];
        }

        new_board->all_nodes[i] = copy;
    }

    return new_board;
}

Board* init_board() {
    Board* b = (Board*) malloc(sizeof(Board));
    int index_map[7][7];
    int i, j, idx = 0;

    for (i = 0; i < 7; i++)
        for (j = 0; j < 7; j++)
            index_map[i][j] = -1;

    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            if ((i >= 2 && i <= 4) || (j >= 2 && j <= 4)) {
                Node* node = (Node*) malloc(sizeof(Node));
                node->is_filled = 'F';
                node->left = node->right = node->up = node->down = NULL;
                b->all_nodes[idx] = node;
                index_map[i][j] = idx++;
            }
        }
    }

    b->all_nodes[16]->is_filled = 'E'; // center empty

    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            int id = index_map[i][j];
            if (id == -1) continue;

            Node* n = b->all_nodes[id];
            if (i > 0 && index_map[i-1][j] != -1) n->up = b->all_nodes[index_map[i-1][j]];
            if (i < 6 && index_map[i+1][j] != -1) n->down = b->all_nodes[index_map[i+1][j]];
            if (j > 0 && index_map[i][j-1] != -1) n->left = b->all_nodes[index_map[i][j-1]];
            if (j < 6 && index_map[i][j+1] != -1) n->right = b->all_nodes[index_map[i][j+1]];
        }
    }

    return b;
}

void print_board(Board* b) {
    int i, j, index = 0;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            if ((i >= 2 && i <= 4) || (j >= 2 && j <= 4)) {
                printf(" %c ", b->all_nodes[index++]->is_filled);
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int get_neighbor_index(int current, int delta_row, int delta_col) {
    int map[7][7], i, j, idx = 0;
    for (i = 0; i < 7; i++)
        for (j = 0; j < 7; j++)
            map[i][j] = -1;

    for (i = 0; i < 7; i++)
        for (j = 0; j < 7; j++)
            if ((i >= 2 && i <= 4) || (j >= 2 && j <= 4))
                map[i][j] = idx++;

    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            if (map[i][j] == current) {
                int ni = i + delta_row;
                int nj = j + delta_col;
                if (ni >= 0 && ni < 7 && nj >= 0 && nj < 7)
                    return map[ni][nj];
            }
        }
    }
    return -1;
}

int solve(Board* board) {
    Stack stack;
    stack.top = 0;

    int best_score = TOTAL_NODES;
    int remaining = count_filled(board);
    int iteration = 0;
    int target_pin = 1;
    push(&stack, copy_board(board));

    while (!is_empty(&stack)) {
        Board* current = pop(&stack);
        remaining = count_filled(current);
        iteration++;
        printf("Iteration %i: There are %i pins remaining.\n", iteration, remaining);

        if (remaining <= target_pin)
        {
            print_board(current);
            for (int i = 0; i < TOTAL_NODES; i++) {
                free(current->all_nodes[i]);
            }
            free(current);

            break;
        }

        for (int i = 0; i < TOTAL_NODES; i++)
        {
            Node * from  = current->all_nodes[i];
            
            if (from->is_filled != 'F' || !from)
            {
                continue;
            }
            
            

            if (from->up && from->up->up)
            {
               if( from->up->is_filled == 'F' && from->up->up->is_filled == 'E'){
                    if (try_move(current,from,from->up,from->up->up))
                    {   
                        Board * copy = copy_board(current);
                        
                        move(copy,copy->all_nodes[i],
                            copy->all_nodes[i]->up,
                            copy->all_nodes[i]->up->up);
                        push(&stack,copy);
                          
                    }
                }
            }

            if (from->down && from->down->down)
            {
               if( from->down->is_filled == 'F' && from->down->down->is_filled == 'E'){
                    if (try_move(current,from,from->down,from->down->down))
                    {
                        Board * copy = copy_board(current);
    
                        move(copy,copy->all_nodes[i],
                           copy->all_nodes[i]->down,copy->all_nodes[i]->down->down);
                        push(&stack,copy);
                         
                           
                    }
                }
            }
            if (from->right && from->right->right)
            {
               if( from->right->is_filled == 'F' && from->right->right->is_filled == 'E'){
                    if (try_move(current,from,from->right,from->right->right))
                    {
                        Board * copy = copy_board(current);
                        
                        move(copy,copy->all_nodes[i],
                           copy->all_nodes[i]->right,
                           copy->all_nodes[i]->right->right);
                        push(&stack,copy);
                         
                                    
                            
                    }
                }
            }
            if (from->left && from->left->left)
            {
               if( from->left->is_filled == 'F' && from->left->left->is_filled == 'E'){
                    if (try_move(current,from,from->left,from->left->left))
                    {
                        Board * copy = copy_board(current);
                        
                        move(copy,copy->all_nodes[i],
                            copy->all_nodes[i]->left,copy->all_nodes[i]->left->left);
                        push(&stack,copy);
                         
                            
                    }
                }
            }
            
           
           
        }
        
        for (int i = 0; i < TOTAL_NODES; i++) {
            free(current->all_nodes[i]);

           }

           free(current);
    }
        
         
		
    
   
	
    return iteration;
}


void print_board_indices() {
    int i, j, index = 0;
    printf("\nBoard Indexes:\n");
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            if ((i >= 2 && i <= 4) || (j >= 2 && j <= 4)) {
                printf("%2d ", index++);
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
    printf("\n");
}


int main() {
    Board* board = init_board();

    printf("=== SOLO TEST ===\n");
    printf("Select a game mode:\n");
    printf("1 - Play! \n");
    printf("2 - Solve the p/roblem\n");
    printf("Selection (1/2): ");

    int choice;
    scanf("%d", &choice);

    if (choice == 2) {
        printf("\nComputer is trying to solve the game...\n\n");

        int iteration_count = solve(board);
        
        printf("The computer solved the problem in %d iterations!\n", iteration_count);
        
        print_board_indices();
        
        return 0;
    }
    else if (choice == 1)
    {
    // Game mode
    printf("\nThe indexes on the board are between 0-32.\n");
    printf("To move: Enter three indexes representing FROM/OVER/TO.\n");
    printf("Enter -1 to exit.\n\n");


    while (1) {
        print_board(board);
        print_board_indices();

        printf("Remaining peg count: %d\n", count_filled(board));

        if (count_filled(board) == 1) {
            printf("Congrats! You are a genius!\n");
            break;
        }

        int from, over, to;
        printf("Enter three indexes FROM/OVER/TO: ");
        scanf("%d", &from);
        if (from == -1) {
            printf("Exiting the game...\n");
            break;
        }
        scanf("%d %d", &over, &to);

        if (from < 0 || from >= TOTAL_NODES || over < 0 || over >= TOTAL_NODES || to < 0 || to >= TOTAL_NODES) {
            printf("Index error!\n");
            continue;
        }

        if (!try_move(board, board->all_nodes[from], board->all_nodes[over], board->all_nodes[to])) {
            printf("Invalid move.\n");
        }
        else{
            move(board, board->all_nodes[from], board->all_nodes[over], board->all_nodes[to]);
        }
    }
    return 0;
    }
    else
        return -1;


}
