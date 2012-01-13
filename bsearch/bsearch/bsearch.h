//
//  bsearch.h
//  bsearch
//
//  Created by Merck Hung on 1/13/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef bsearch_bsearch_h
#define bsearch_bsearch_h


#define TRUE    1
#define FALSE   0


// Structures
typedef struct _bSearchTree {
    
    int                     node;
    struct _bSearchTree     *leftChild;
    struct _bSearchTree     *rightChild;
    
} bSearchTree_t;


bSearchTree_t *insertNode( int node, bSearchTree_t **pBSearchTree );
bSearchTree_t *retrieveReplaceNode( bSearchTree_t *pBSearchTree );
bSearchTree_t *removeNode( int node, bSearchTree_t **pBSearchTree );
void preorderTraversal( bSearchTree_t *pBSearchTree );
void postorderTraversal( bSearchTree_t *pBSearchTree );
void inorderTraversal( bSearchTree_t *pBSearchTree );
bSearchTree_t *searchNode( int node, bSearchTree_t *pBSearchTree );
bSearchTree_t *searchPartnerNode( int node, bSearchTree_t *pBSearchTree );


#endif
