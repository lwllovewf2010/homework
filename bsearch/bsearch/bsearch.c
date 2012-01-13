//
//  bsearch.c
//  bsearch
//
//  Created by Merck Hung on 1/13/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#include "bsearch.h"


bSearchTree_t *insertNode( int node, bSearchTree_t **pBSearchTree ) {
    
    bSearchTree_t **ppBST;
    
    // Sanity check
    if( !pBSearchTree )
        return NULL;
    
    printf( "ADD NODE %d\n", node );
    
    // Search & insert
    for( ppBST = pBSearchTree ; ; ) {
        
        if( !(*ppBST) ) {
        
            *ppBST = (bSearchTree_t *)malloc( sizeof( bSearchTree_t ) );
            if( !*ppBST )
                return NULL;
            
            (*ppBST)->node = node;
            (*ppBST)->leftChild = NULL;
            (*ppBST)->rightChild = NULL;
            break;
        }
        
        if( node == (*ppBST)->node ) {
        
            // Redundant node
            break;
        }
        else if( node < (*ppBST)->node ) {
         
            //printf( "GO LEFT of Node %d\n", (*ppBST)->node );
            ppBST = &((*ppBST)->leftChild);
        }
        else {
            
            //printf( "GO RIGHT of Node %d\n", (*ppBST)->node );
            ppBST = &((*ppBST)->rightChild);
        }
    }
    
    return *ppBST;
}


bSearchTree_t *retrieveReplaceNode( bSearchTree_t *pBSearchTree ) {
    
    int left;
    
    // Sanity check
    if( !pBSearchTree )
        return NULL;
    
    // Decide left or right
    if( pBSearchTree->leftChild ) {
        
        left = TRUE;
        pBSearchTree = pBSearchTree->leftChild;
    }
    else if( pBSearchTree->rightChild ) {
     
        left = FALSE;
        pBSearchTree = pBSearchTree->rightChild;
    }
    else
        return NULL;
    
    for( ; ; ) {
        
        if( left == TRUE ) {
        
            // Retrieve largest node
            if( pBSearchTree->rightChild )
                pBSearchTree = pBSearchTree->rightChild;
            else
                break;
        }
        else {
        
            // Retrieve smallest node
            if( pBSearchTree->leftChild )
                pBSearchTree = pBSearchTree->leftChild;
            else
                break;      
        }
    }
    
    return pBSearchTree;
}


bSearchTree_t *removeNode( int node, bSearchTree_t **pBSearchTree ) {
    
    bSearchTree_t *pPartner = NULL, *pNode = NULL, *rNode = NULL;

    if( !pBSearchTree )
        return NULL;
    
    // Locate the node
    pNode = searchNode( node, *pBSearchTree );
    if( !pNode )
        return NULL;

    // Retrieve its partner
    pPartner = searchPartnerNode( node, *pBSearchTree );
    if( pPartner )
        printf( "Partner is %d\n", pPartner->node );
    else
        printf( "Partner is none\n" );
    
    // Locate replace node
    rNode = retrieveReplaceNode( pNode );
    if( rNode )
        printf( "Replace Node is %d\n", rNode->node );
    else
        printf( "Replace Node is none\n" );
    
    printf( "DEL NODE %d\n", node );
    
    // Do delete
    if( !rNode ) {
    
        if( !pPartner ) {
            
            *pBSearchTree = NULL;
            goto Exit;
        }
        
        if( pPartner->leftChild == pNode )
            pPartner->leftChild = NULL;
        else
            pPartner->rightChild = NULL;
    }
    else {
        
        // If the rnode has a child, need to recursive removal
        if( rNode->leftChild || rNode->rightChild ) {
        
            printf( "Replace node %d has a left or right child\n", rNode->node );
            if( !removeNode( rNode->node, pBSearchTree ) ) {
                
                printf( "Failed to remove node %d\n", rNode->node );
                return NULL;
            }
        }
        else {
        
            printf( "Replace node %d has no child\n", rNode->node );
        }
        
        // Let pnode's partner links to rnode
        if( pPartner ) {
        
            if( pPartner->leftChild == pNode )
                pPartner->leftChild = rNode;
            else
                pPartner->rightChild = rNode;
        }
        else
            *pBSearchTree = rNode;
        
        // Supposed the the childs of rnode have been cleared by recursive removal
        rNode->leftChild = pNode->leftChild;
        rNode->rightChild = pNode->rightChild;
        
        if( rNode->leftChild == rNode )
            rNode->leftChild = NULL;
        
        if( rNode->rightChild == rNode )
            rNode->rightChild = NULL;
    }
    
    
Exit:
    pNode->leftChild = NULL;
    pNode->rightChild = NULL;
    return pNode;
}


void preorderTraversal( bSearchTree_t *pBSearchTree ) {
    
    if( pBSearchTree ) {
        
        // Node
        printf( "NODE => %d\n", pBSearchTree->node );
        
        // Left
        preorderTraversal( pBSearchTree->leftChild );
        
        // Right
        preorderTraversal( pBSearchTree->rightChild );
    }
}


void postorderTraversal( bSearchTree_t *pBSearchTree ) {
    
    if( pBSearchTree ) {
        
        // Left
        postorderTraversal( pBSearchTree->leftChild );
        
        // Right
        postorderTraversal( pBSearchTree->rightChild );
        
        // Node
        printf( "NODE => %d\n", pBSearchTree->node );
    }
}


void inorderTraversal( bSearchTree_t *pBSearchTree ) {
    
    if( pBSearchTree ) {
        
        // Left
        inorderTraversal( pBSearchTree->leftChild );
        
        // Node
        printf( "NODE => %d\n", pBSearchTree->node );
        
        // Right
        inorderTraversal( pBSearchTree->rightChild );
    }
}


bSearchTree_t *searchNode( int node, bSearchTree_t *pBSearchTree ) {
    
    for( ; pBSearchTree ; ) {

        if( pBSearchTree->node == node ) {
         
            //printf( "FOUND NODE: %d\n", pBSearchTree->node );
            return pBSearchTree;
        }
        else if( pBSearchTree->node < node ) {
           
            //printf( "GO RIGHT of NODE %d\n", pBSearchTree->node );
            pBSearchTree = pBSearchTree->rightChild;
        }
        else {
         
            //printf( "GO LEFT of NODE %d\n", pBSearchTree->node );
            pBSearchTree = pBSearchTree->leftChild;
        }
    }
    
    printf( "NO SUCH NODE %d\n", node );
    return NULL;
}


bSearchTree_t *searchPartnerNode( int node, bSearchTree_t *pBSearchTree ) {
    
    bSearchTree_t *partner = NULL;
    
    for( ; pBSearchTree ; ) {
        
        if( pBSearchTree->node == node ) {
            
            printf( "FOUND NODE: %d\n", pBSearchTree->node );
            return partner;
        }
        else if( pBSearchTree->node < node ) {
            
            printf( "GO RIGHT of NODE %d\n", pBSearchTree->node );
            partner = pBSearchTree;
            pBSearchTree = pBSearchTree->rightChild;
        }
        else {
            
            printf( "GO LEFT of NODE %d\n", pBSearchTree->node );
            partner = pBSearchTree;
            pBSearchTree = pBSearchTree->leftChild;
        }
    }
    
    printf( "NO SUCH NODE %d\n", node );
    return NULL;
}


int main (int argc, const char * argv[]) {

    bSearchTree_t *pBSearchTree = NULL;
    
    
    // Insert
    insertNode( 21, &pBSearchTree );
    insertNode( 9, &pBSearchTree );
    insertNode( 5, &pBSearchTree );
    insertNode( 7, &pBSearchTree );
    insertNode( 6, &pBSearchTree );
    insertNode( 10, &pBSearchTree );
    insertNode( 57, &pBSearchTree );
    insertNode( 34, &pBSearchTree );
    insertNode( 23, &pBSearchTree );
    
    
    // Delete
    removeNode( 9, &pBSearchTree );
    
    
    // Traversals
    printf( "Preorder Traversal\n" );
    preorderTraversal( pBSearchTree );
    
    printf( "Inorder Traversal\n" );
    inorderTraversal( pBSearchTree );
    
    printf( "Postorder Traversal\n" );
    postorderTraversal( pBSearchTree );
    
    
    // Search
    printf( "Search for Node 23\n" );
    searchNode( 23, pBSearchTree );
    
    printf( "Search for Node 100\n" );
    searchNode( 100, pBSearchTree );
    
    return 0;
}

