#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX(a, b) (a >= b ? a : b)

typedef enum COLOR { RED = 0, BLACK } COLOR;

typedef struct Node {
  int value;
  struct Node *left;
  struct Node *right;
  COLOR color;
} Node;

Node *create_node(int value) {
  Node *new = malloc(sizeof(Node));
  new->value = value;
  new->left = NULL;
  new->right = NULL;
  new->color = RED;

  return new;
}

Node *left_rotate(Node *node) {
  Node *right = node->right;
  node->right = right->left;
  right->left = node;
  right->color = node->color;
  node->color = RED;

  return right;
}

Node *right_rotate(Node *node) {
  Node *left = node->left;
  node->left = left->right;
  left->right = node;
  left->color = node->color;
  node->color = RED;

  return left;
}

void flip_colors(Node *node) {
  node->color = RED;
  node->left->color = BLACK;
  node->right->color = BLACK;
}

bool is_red(Node *node) {
  return node != NULL && node->color == RED;
}

Node *insert(Node *root, int value) {
  if (!root) {
    return create_node(value);
  }

  if (value < root->value) {
    root->left = insert(root->left, value);
  } else if (value > root->value) {
    root->right = insert(root->right, value);
  }

  if (is_red(root->right) && !is_red(root->left)) {
    root = left_rotate(root);
  }
  if (is_red(root->left) && is_red(root->left->left)) {
    root = right_rotate(root);
  }
  if (is_red(root->left) && is_red(root->right)) {
    flip_colors(root); 
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

  stats = fopen("rbtree_stats.dat", "w");

  for (int i = 10; i <= 10000; i++) {
    // i = number of nodes
    avg_h = 0.0;
    for (int j = 0; j < 10; j++) {
      // 10 samples
      for (int k = 1; k <= i; k++) {
        // build tree with i nodes
        root = insert(root, rand() % 100000);
      }
      avg_h += height(root);
    }
    avg_h = avg_h / 10.0;

    fprintf(stats, "%d\t%.4f\n", i, avg_h);
    delete_tree(root);
    root = NULL;
  }

  fclose(stats);

exit:
  return EXIT_SUCCESS;
}
