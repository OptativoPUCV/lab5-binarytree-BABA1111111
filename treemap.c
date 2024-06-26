#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

// struct TreeNode {
//     Pair* pair;
//     TreeNode * left;
//     TreeNode * right;
//     TreeNode * parent;
// };

// struct TreeMap {
//     TreeNode * root;
//     TreeNode * current;
//     int (*lower_than) (void* key1, void* key2);
// };

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* tree = (TreeMap*) malloc(1 * sizeof(TreeMap));

    if (tree == NULL) return NULL;

    tree->root = NULL;
    tree->current = NULL;
    tree->lower_than = lower_than;

    return tree;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode* newNode = createTreeNode(key,value);
    TreeNode* current = tree->root;
    TreeNode* parent = NULL;

    while(current != NULL){
        parent = current;
        if (is_equal(tree, current->pair->key, key)){
            return;
        } else {
            if (tree->lower_than(key, current->pair->key)){
                current = current->left;
            }
            else{
                current = current->right;
            }
        }
    }

    newNode->parent = parent;
    if (parent == NULL){
        tree->root = newNode;
    }
    else if (tree->lower_than(key, parent->pair->key)){
        parent->left = newNode;
        tree->current = newNode;
    }
    else{
        parent->right = newNode;
        tree->current = newNode;
    }
}

TreeNode * minimum(TreeNode * x){
    TreeNode* auxCurrent = x;
    while (auxCurrent->left != NULL){
        auxCurrent = auxCurrent->left;
    }
    return auxCurrent;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode* auxTemp = tree->root;
    TreeNode* auxParent = NULL;

    while( auxTemp != NULL && auxTemp != node){
        auxParent = auxTemp;
        if (tree->lower_than(node->pair->key, auxTemp->pair->key)){
            auxTemp = auxTemp->left;
        } else {
            auxTemp = auxTemp->right;
        }
    }

    if (auxTemp == NULL){
        return;
    }

    if (auxTemp->left == NULL && auxTemp->right == NULL){
        if (auxTemp != tree->root){
            if (auxParent->left == auxTemp){
                auxParent->left = NULL;
            }
            else{
                auxParent->right = NULL;
            }
        }
        else{
            tree->root = NULL;
        }
        free(auxTemp);
    }
        
    else if (auxTemp->left == NULL || auxTemp->right == NULL){
        TreeNode* child;

        if (auxTemp->left != NULL){
            child = auxTemp->left;
        }
        else{
            child = auxTemp->right;
        }
            
        if (!is_equal(tree, auxTemp->pair->key, auxParent->pair->key)){
            if (tree->lower_than(auxTemp->pair->key, auxParent->pair->key)){
                auxParent->left = child;
            }
            else{
                auxParent->right = child;
            }
        }
        else{
            tree->root = child;
        }
        free(auxTemp);
    }
    else{
        TreeNode* succesor = minimum(auxTemp->right);
        removeNode(tree, succesor);

        succesor->parent = auxTemp->parent;

        if (auxTemp->parent == NULL){
            tree->root = succesor;
        } else if (auxTemp == auxTemp->parent->left){
            auxTemp->parent->left = succesor;
        } else {
            auxTemp->parent->right = succesor;
        }

        if (succesor->right != NULL){
            succesor->right->parent = succesor;
        }
        
        auxTemp->pair->key = succesor->pair->key;
        auxTemp->pair->value = succesor->pair->value;
    } 
}

    
    // TreeNode* auxTemp = tree->root;
    // TreeNode* auxParent = NULL;

    // while( auxTemp != NULL && auxTemp != node){
    //     auxParent = auxTemp;
    //     if (node->pair->key < auxTemp->pair->key){
    //         auxTemp = auxTemp->left;
    //     } else {
    //         auxTemp = auxTemp->right;
    //     }
    // }

    // if (auxTemp == NULL){
    //     return;
    // }

    // if (auxTemp->left == NULL && auxTemp->right == NULL){
    //     if (auxTemp != tree->root){
    //         if (auxParent->left == auxTemp){
    //             auxParent->left = NULL;
    //         }
    //         else{
    //             auxParent->right = NULL;
    //         }
    //     }
    //     else{
    //         tree->root = NULL;
    //     }
    //     free(auxTemp);
    // }

    // else if (auxTemp->left == NULL || auxTemp->right == NULL){
    //     TreeNode* child;

    //     if (auxTemp->left != NULL){
    //         child = auxTemp->left;
    //     }
    //     else{
    //         child = auxTemp->right;
    //     }
    //     if (auxTemp != NULL){
    //         if (auxParent->left == auxTemp){
    //             auxParent->left = child;
    //         }
    //         else{
    //             auxParent->right = child;
    //         }
    //     }
    //     else{
    //         tree->root = child;
    //     }
    //     free(auxTemp);
    // }
    // else{
    //     TreeNode* succesor = minimum(auxTemp->right);
    //     eraseTreeMap(tree, succesor->pair->key);
    //     auxTemp->pair->key = succesor->pair->key;
    //     auxTemp->pair->value = succesor->pair->value;
    // }     
//}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* auxCurrent = tree->root;
    while (auxCurrent != NULL && !is_equal(tree, auxCurrent->pair->key, key)){
        if (tree->lower_than(auxCurrent->pair->key, key)==0){
            auxCurrent = auxCurrent->left;
        } else {
            auxCurrent = auxCurrent->right;
        }
    }
    if (auxCurrent != NULL){
        tree->current = auxCurrent;
        return auxCurrent->pair;
    } else {
        return  NULL;
    }  
}


Pair* upperBound(TreeMap* tree, void* key) {
    TreeNode* ub_node = NULL;
    
    if (tree == NULL) {
        return NULL;
    }

    TreeNode* current = tree->root;
    
    while (current != NULL) {
        if (tree->lower_than(key, current->pair->key) || is_equal(tree, key, current->pair->key)) {
            ub_node = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (ub_node != NULL) {
        return ub_node->pair;
    }

    return NULL;
}


Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* auxFirst = minimum(tree->root);
    return auxFirst->pair;
}

Pair* nextTreeMap(TreeMap* tree) {
    if (tree == NULL) {
        return NULL;
    }

    if (tree->root == NULL) {
        return NULL;
    }

    if (tree->current == NULL) {
        tree->current->pair = firstTreeMap(tree);
        return tree->current->pair;
    }

    if (tree->current->right != NULL) {
        tree->current = minimum(tree->current->right);
        return tree->current->pair;
    }

    TreeNode* parent = tree->current->parent;
    while (parent != NULL && tree->current == parent->right) {
        tree->current = parent;
        parent = parent->parent;
    }

    if (parent != NULL) {
        tree->current = parent;
        return parent->pair;
    }
    return NULL;
}

