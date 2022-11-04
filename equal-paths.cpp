#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int calculateHeight(Node* root) {
	int height = 0;
	if (root == nullptr) return height;
	return 1+max(calculateHeight(root->left), calculateHeight(root->right));
}

bool equalPaths(Node * root)
{
    // Add your code below
	if (root == nullptr) return true;
	int lH = calculateHeight(root->left);
  int rH = calculateHeight(root->right);
	if (root->left != NULL) {
    lH = 1 + calculateHeight(root->left);
	}
	else if (equalPaths(root->right)) return true;
	if (root->right != NULL) {
    rH = 1 + calculateHeight(root->right);
	}
	else if (equalPaths(root->left)) return true;
	if (lH == rH) return true;
	else return false;
}
