//
// AED, January 2022
//
// Solution of the second practical assignement (multi-ordered tree)
//
// Place your student numbers and names here
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AED_2021_A02.h"

//
// the custom tree node structure
//
// we want to maintain three ordered trees (using the same nodes!), so we need four left and four right pointers
// so, when inserting a new node we need to do it four times (one for each index), so we will end upo with 4 three roots
//

typedef struct tree_node_s
{
    char name[MAX_NAME_SIZE + 1];                         // index 0 data item
    char zip_code[MAX_ZIP_CODE_SIZE + 1];                 // index 1 data item
    char telephone_number[MAX_TELEPHONE_NUMBER_SIZE + 1]; // index 2 data item
    char cc[MAX_CC + 1];                                  // index 3 data item
    struct tree_node_s *left[4];                          // left pointers (one for each index) ---- left means smaller
    struct tree_node_s *right[4];                         // right pointers (one for each index) --- right means larger
} tree_node_t;

//
// the node comparison function (do not change this)
//

int compare_tree_nodes(tree_node_t *node1, tree_node_t *node2, int main_idx)
{
    int i, c;

    for (i = 0; i < 4; i++)
    {
        if (main_idx == 0)
            c = strcmp(node1->name, node2->name);
        else if (main_idx == 1)
            c = strcmp(node1->zip_code, node2->zip_code);
        else if (main_idx == 2)
            c = strcmp(node1->telephone_number, node2->telephone_number);
        else if (main_idx == 3)
            c = strcmp(node1->cc, node2->cc);
        if (c != 0)
            return c;                                  // different on this index, so return
        main_idx = (main_idx == 3) ? 0 : main_idx + 1; // advance to the next index
    }
    return 0;
}

//
// tree insertion routine (place your code here)
//

void tree_insert(tree_node_t **roots, tree_node_t *node, int main_index)
{
    /* If the tree is empty, the tree will be to equal the node and will not return any value */
    if (roots[main_index] == NULL)
    {
        roots[main_index] = node;
        return;
    }
    /* Otherwise, recur down the tree */
    if (compare_tree_nodes(roots[main_index], node, main_index) > 0)
        tree_insert(roots[main_index]->left, node, main_index);
    else if (compare_tree_nodes(roots[main_index], node, main_index) < 0)
        tree_insert(roots[main_index]->right, node, main_index);
    else
    {
        fprintf(stderr, "ERROR generating tree, two equal people!\n");
        fprintf(stderr, "Please chose another student number.\n");
        exit(1);
    }
}

//
// tree search routine (place your code here)
//

tree_node_t *find(tree_node_t *root, tree_node_t node, int main_index)
{
    if (root == NULL)
        return NULL;

    if (compare_tree_nodes(root, &node, main_index) > 0)
        return find(root->left[main_index], node, main_index);
    else if (compare_tree_nodes(root, &node, main_index) < 0)
        return find(root->right[main_index], node, main_index);
    else
        return root;
}

//
// tree depth
//

int tree_depth(tree_node_t *root, int main_index)
{
    if (root == NULL)
        return 0;

    int l = tree_depth(root->left[main_index], main_index);
    int r = tree_depth(root->right[main_index], main_index);

    if (l > r)
        return l + 1;

    return r + 1;
}

//
// list, i,e, traverse the tree (place your code here)
//

int c1 = 1; // global variable 
void list(tree_node_t *root, int main_index)
{
    if (root != NULL)
    {
        list(root->left[main_index], main_index);
        printf("Person #%d\n", c1++);
        printf("    name --------------- %s\n", root->name);
        printf("    zip code ----------- %s\n", root->zip_code);
        printf("    telephone number --- %s\n", root->telephone_number);
        printf("    cc ----------------- %s\n", root->cc);
        list(root->right[main_index], main_index);
    }
}

//
// list the people with a given zip code
//

int c2 = 1; // global variable
void findZipCode(tree_node_t *root, char *zip_code)
{
    if (root != NULL)
    {
        if (strcmp(root->zip_code, zip_code) == 0)
        {
            findZipCode(root->left[1], zip_code);
            printf("Person #%d\n", c2++);
            printf("    name --------------- %s\n", root->name);
            printf("    zip code ----------- %s\n", root->zip_code);
            printf("    telephone number --- %s\n", root->telephone_number);
            printf("    cc ----------------- %s\n", root->cc);
            findZipCode(root->right[1], zip_code);
        }
        else
        {
            findZipCode(root->left[1], zip_code);
            findZipCode(root->right[1], zip_code);
        }
    }
}

//
// main program
//

int main(int argc, char **argv)
{
    double dt;
    // process the command line arguments
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s student_number number_of_people [options ...]\n", argv[0]);
        fprintf(stderr, "Recognized options:\n");
        fprintf(stderr, "  -list[N]              # list the tree contents, sorted by key index N (the default is index 0)\n");
        // place a description of your own options here
        fprintf(stderr, "  -find 'zip code'      # list the people with a given zip code\n");
        return 1;
    }
    int student_number = atoi(argv[1]);
    if (student_number < 1 || student_number >= 1000000)
    {
        fprintf(stderr, "Bad student number (%d) --- must be an integer belonging to [1,1000000[\n", student_number);
        return 1;
    }
    int n_people = atoi(argv[2]);
    if (n_people < 3 || n_people > 10000000)
    {
        fprintf(stderr, "Bad number of people (%d) --- must be an integer belonging to [3,10000000]\n", n_people);
        return 1;
    }
    // generate all data
    tree_node_t *people = (tree_node_t *)calloc((size_t)n_people, sizeof(tree_node_t));
    if (people == NULL)
    {
        fprintf(stderr, "Output memory!\n");
        return 1;
    }
    aed_srandom(student_number);
    for (int i = 0; i < n_people; i++)
    {
        random_name(&(people[i].name[0]));
        random_zip_code(&(people[i].zip_code[0]));
        random_telephone_number(&(people[i].telephone_number[0]));
        random_cc(&(people[i].cc[0]));
        for (int j = 0; j < 4; j++)
            people[i].left[j] = people[i].right[j] = NULL; // make sure the pointers are initially NULL
    }
    // create the ordered binary trees
    dt = cpu_time();
    tree_node_t *roots[4]; // three indices, three roots
    for (int main_index = 0; main_index < 4; main_index++)
        roots[main_index] = NULL;
    for (int i = 0; i < n_people; i++)
        for (int main_index = 0; main_index < 4; main_index++)
            tree_insert(roots, &people[i], main_index); // place your code here to insert &(people[i]) in the tree with number main_index
    dt = cpu_time() - dt;
    printf("Tree creation time (%d people): %.3es\n", n_people, dt);
    // search the tree
    for (int main_index = 0; main_index < 4; main_index++)
    {
        dt = cpu_time();
        for (int i = 0; i < n_people; i++)
        {
            tree_node_t n = people[i];                                  // make a copy of the node data
            if (find(roots[main_index], n, main_index) != &(people[i])) // place your code here to find a given person, searching for it using the tree with number main_index
            {
                fprintf(stderr, "person %d not found using index %d\n", i, main_index);
                return 1;
            }
        }
        dt = cpu_time() - dt;
        printf("Tree search time (%d people, index %d): %.3es\n", n_people, main_index, dt);
    }
    for (int main_index = 0; main_index < 4; main_index++)
    {
        dt = cpu_time();
        int depth = tree_depth(roots[main_index], main_index); // place your code here to compute the depth of the tree with number main_index
        dt = cpu_time() - dt;
        printf("Tree depth for index %d: %d (done in %.3es)\n", main_index, depth, dt);
    }
    // process the command line optional arguments
    for (int i = 3; i < argc; i++)
    {
        if (strncmp(argv[i], "-list", 5) == 0)
        { // list all (optional)
            int main_index = atoi(&(argv[i][5]));
            if (main_index < 0)
                main_index = 0;
            if (main_index > 3)
                main_index = 3;
            printf("List of people:\n");
            list(roots[main_index], main_index); // place your code here to traverse, in order, the tree with number main_index
        }
        // place your own options here
        else if (strcmp(argv[i], "-find") == 0)
        {
            printf("List of people with the zip code: %s\n", argv[i + 1]);
            findZipCode(roots[1], argv[i + 1]);
        }
    }
    // clean up --- don't forget to test your program with valgrind, we don't want any memory leaks
    free(people);
    return 0;
}