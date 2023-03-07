#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    if(root == NULL) return true;
    // Add your code below
    int leftDepth = 0, rightDepth = 0;
    Node* r = root->right;
    Node* l = root->left;
    if(r && (r->left || r->right)) rightDepth = 1;
    if(l && (l->left || l->right)) leftDepth =1;
    if(leftDepth != rightDepth) return false;
    
    return equalPaths(r) && equalPaths(l);
}

