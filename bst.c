#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX(a, b) (a >= b ? a : b)

typedef struct Node {
  int value;
  struct Node *left;
  struct Node *right;
} Node;

Node *create_node(int value) {
  Node *new = malloc(sizeof(Node));
  new->value = value;
  new->left = NULL;
  new->right = NULL;

  return new;
}

Node* insert(Node *root, int value) {
  if (!root) {
    return create_node(value);
  }

  if (value < root->value) {
    root->left = insert(root->left, value);
  } else if (value > root->value) {
    root->right = insert(root->right, value);
  }

  return root;
}

int height(Node *root) {
  if (!root) {
    return 0;
  }

  int lh = height(root->left);
  int rh = height(root->right);

  return 1 + MAX(lh, rh);
}

void print(Node *root) {
  if (!root) {
    return;
  }

  print(root->left);
  printf("%d\n", root->value);
  print(root->right);
}

void delete_tree(Node *root) {
  if (!root) {
    return;
  }

  delete_tree(root->left);
  delete_tree(root->right);
  free(root);
}

int main(void) {
  Node *root = NULL;
  float avg_h;
  FILE *stats = NULL;

  srand(time(NULL));

  stats = fopen("bst_stats.dat", "w");

  for (int i = 10; i <= 10000; i++) {
    // i = number of nodes
    avg_h = 0.0;
    for (int j = 0; j < 10; j++) {
      // 10 samples
      for (int k = 1; k <= i; k++) {
        // build tree with i nodes
        root = insert(root, rand() % 100000);
      }
      avg_h = avg_h + height(root) / 10.0;
    }

    fprintf(stats, "%d\t%.4f\n", i, avg_h);
    delete_tree(root);
    root = NULL;
  }

  fclose(stats);

exit:
  return EXIT_SUCCESS;
}
