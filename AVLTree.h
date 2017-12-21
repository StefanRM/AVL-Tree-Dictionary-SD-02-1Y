/*
 * AVL.h
 *
 *  Created on: Apr 3, 2016
 *      Author: dan
 */

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include<stdlib.h>

// An AVL tree node
typedef struct AVLNode{
	Item1 elem;
	Info info;
	int height;
	struct AVLNode *l;
	struct AVLNode *r;
	struct AVLNode *p;
	struct AVLNode* next;
	struct AVLNode* prev;
	struct AVLNode* end;
}AVLNode;

typedef struct AVLTree{
	long size;
	AVLNode* root;
	AVLNode* nil;
	int (*comp)(Item1 a, Item1 b);
	void (*destroyElem)(Item1 elem);
	void (*destroyInfo)(Info info);
}AVLTree;


// A utility function to get maximum of two integers
int max(int a, int b){
	return (a > b)? a : b;
}

/* TODO: Cerinta 1 */
AVLTree* avlCreateTree(int (*comp) (Item1,Item1), 	void (*destroyElem)(Item1 elem), void (*destroyInfo)(Info info))
{
	AVLTree* tree = (AVLTree*) malloc(sizeof(AVLTree)); // our tree
	tree->nil = (AVLNode*) malloc(sizeof(AVLNode)); // nil dummy
	tree->nil->prev = tree->nil->next = tree->nil->p = tree->nil->l = tree->nil->r = tree->nil; // initial nil appearance
	tree->nil->end = tree->nil; // end points to node itself

	tree->nil->height = 0; // initial nil height
	tree->root = (AVLNode*) malloc(sizeof(AVLNode)); // root dummy
	tree->root->prev = tree->root->next = tree->root->p = tree->root->l = tree->root->r = tree->nil; // initial root appearance
	tree->root->end = tree->root; // end points to node itself

	tree->root->height = 0; // initial root height
	tree->size = 0; // initial size

	// Assigning functions to the tree
	tree->comp = comp;
	tree->destroyElem = destroyElem;
	tree->destroyInfo = destroyInfo;

	return tree;
}

int avlIsEmpty(AVLTree* tree)
{
	return (tree->root->l == tree->nil);
}

AVLNode* avlNewNode(AVLTree* tree)
{
	AVLNode* node = (AVLNode*) malloc(sizeof(AVLNode));
	// Initialize the new node to be used in the tree
	node->p = node->r = node->l = tree->nil;
	node->height = 1;

	node->prev = node->next = tree->nil;
	node->end = node;

	return node;
}

void avlDeleteNode(AVLTree *tree, AVLNode* node)
{
	tree->destroyElem(node->elem);
	tree->destroyInfo(node->info);
	free(node);
}

void avlRightRotate(AVLTree *tree,  AVLNode *y)
{
	// Setting things up
	AVLNode* x = y->l;
	AVLNode* beta = x->r;

	// do rotation
	y->l = beta;
	if (beta != tree->nil) // tree->nil is its own parent
	{
		beta->p = y;
	}

	x->p = y->p;
	if (y->p->r == y) // root of subtree
	{
		y->p->r = x;
	}
	else
	{
		y->p->l = x;
	}

	x->r = y;
	y->p = x;

	// at least we have to calculate the height
	x->height = max(x->l->height, x->r->height) + 1;
	y->height = max(y->l->height, y->r->height) + 1;
}

void avlLeftRotate(AVLTree *tree, AVLNode *x)
{
	// Setting things up
	AVLNode* y = x->r;
	AVLNode* beta = y->l;

	// do rotation
	x->r = beta;
	if (beta != tree->nil) // tree->nil is its own parent
	{
		beta->p = x;
	}

	y->p = x->p;
	if (x->p->l == x) // root of subtree
	{
		x->p->l = y;
	}
	else
	{
		x->p->r = y;
	}

	x->p = y;
	y->l = x;

	// at least we have to calculate the height
	x->height = max(x->l->height, x->r->height) + 1;
	y->height = max(y->l->height, y->r->height) + 1;
}

int avlGetBalance(AVLNode *x)
{
	if (x == NULL)
		return 0;
	return x->l->height - x->r->height;
}

AVLNode * avlMinimum(AVLTree* tree, AVLNode* x)
{
	while (x->l != tree->nil)
		x = x->l;
	return x;
}

AVLNode* avlMaximum(AVLTree* tree, AVLNode* x)
{
	while(x->r != tree->nil){
		x = x->r;
	}
	return x;
}

AVLNode* avlSucccessor(AVLTree* tree, AVLNode* x)
{
	if (x->r != tree->nil) // Check if right tree exists
 	{
 		return avlMinimum(tree, x->r); // return the 'leftest' node
 	}

 	// Otherwise check for ancestor
 	AVLNode* parent = x->p;
 	while(parent != tree->root && x == parent->r)
 	{
 		x = parent;
 		parent = parent->p;
 	}
 	if (parent == tree->root) // if the node is the largest
 	{
 		return tree->nil;
 	}

 	return parent;
}

AVLNode* avlPredecessor(AVLTree* tree, AVLNode* x)
{
	if (x->l != tree->nil) // Check if left tree exists
 	{
 		return avlMaximum(tree, x->l); // return the 'rightest' node
 	}

 	// Otherwise check for ancestor
 	AVLNode* parent = x->p;
 	while(parent != tree->root && x == parent->l)
 	{
 		x = parent;
 		parent = parent->p;
 	}
 	if (parent == tree->root)
 	{
 		return tree->nil; // if the node is the smallest
 	}
 	
 	return parent;
}

void avlInsertFixUp(AVLTree* tree, AVLNode* y, Item1 elem) // Balancing stuff
{
	while (y != tree->root)
	{
		y->height = max(y->l->height, y->r->height) + 1;
		int balance = avlGetBalance(y);

		// unbalance cases
		if (balance > 1 && (-1 == tree->comp(elem, y->l->elem)))
		{
			avlRightRotate(tree, y);
		}

		if (balance < -1 && (1 == tree->comp(elem, y->r->elem)))
		{
			avlLeftRotate(tree, y);
		}

		if (balance > 1 && (1 == tree->comp(elem, y->l->elem)))
		{
			avlLeftRotate(tree, y->l);
			avlRightRotate(tree, y);
		}

		if (balance < -1 && (-1 == tree->comp(elem, y->r->elem)))
		{
			avlRightRotate(tree, y->r);
			avlLeftRotate(tree, y);
		}

		y = y->p;
	}
}

void avlInsert(struct AVLTree* tree, Item1 elem, Info info)
{
	AVLNode* y = tree->root; // parent to our new node
	AVLNode* x = tree->root->l; // node for iteration

	// Creating a new AVL node
	AVLNode* z = avlNewNode(tree);
	z->elem = elem; // elem is taken here
	z->info = info; // info is taken here

	tree->size++; // increase the size of the AVL (it will be always increased on insertion)

	// Finding place for our new node z in AVL
	while (x != tree->nil)
	{
		y = x; // keeping the parent

		// In case of duplicates in AVL
		if (0 == tree->comp(elem, x->elem))
		{
			AVLNode* p = x; // node for iteration
			AVLNode* successor = avlSucccessor(tree, x); // successor
			x->end = z; // link to the last item of duplicates

			// we want to stay in the list of duplicates
			while((p->next != p) && (0 == tree->comp(p->next->elem, elem)))
			{
				p = p->next;
			}
			p->next = z;
			z->prev = p;

			z->next = successor;

			if (successor != tree->nil) // tree->nil points only to itself
			{
				successor->prev = z;
			}

			return;
		}

		if (-1 == tree->comp(elem, x->elem))
		{
			x = x->l;
		}
		else if (1 == tree->comp(elem, x->elem))
		{
			x = x->r;
		}
	}

	z->p = y; // assign the new node's parent

	if (y == tree->root) // case for new root (actually root->l from the definition of our structure, root-r->nil :-) )
	{
		tree->root->l = z; // add new node in AVL
	}
	else if (-1 == tree->comp(elem, y->elem))
	{
		y->l = z; // add new node in AVL

		AVLNode* predecessor = avlPredecessor(tree, z);
		AVLNode* successor = avlSucccessor(tree, z);
		z->prev = predecessor;

		if (predecessor != tree->nil) // tree->nil points only to itself
		{
			predecessor->next = z;
		}

		z->next = successor;

		if (successor != tree->nil) // tree->nil points only to itself
		{
			successor->prev = z;
		}
	}
	else
	{
		y->r = z; // add new node in AVL

		AVLNode* predecessor = avlPredecessor(tree, z);
		AVLNode* successor = avlSucccessor(tree, z);
		z->prev = predecessor;

		if (predecessor != tree->nil) // tree->nil points only to itself
		{
			predecessor->next = z;
		}

		z->next = successor;

		if (successor != tree->nil) // tree->nil points only to itself
		{
			successor->prev = z;
		}
	}

	avlInsertFixUp(tree, y, elem); // things should be fixed up (balancing)
}

AVLNode* avlExactQuery(AVLTree* tree, Item1 elem)
{
	AVLNode* x = tree->root->l; // node for iteration

	// Finding node with elem in AVL
	while (x != tree->nil)
	{
		// Maybe we found it!
		if (0 == tree->comp(elem, x->elem))
		{
			return x;
		}

		if (-1 == tree->comp(elem, x->elem))
		{
			x = x->l;
		}
		else if (1 == tree->comp(elem, x->elem))
		{
			x = x->r;
		}
	}

	return NULL; // We didn't found it
}

void avlDeleteFixUp(AVLTree* tree, AVLNode* y)
{
	Item1 elem = y->elem; // to keep the fixing form avlInsertFixUp

	while (y != tree->root)
	{
		y->height = max(y->l->height, y->r->height) + 1;
		int balance = avlGetBalance(y);

		// unbalance cases
		if (balance > 1 && (-1 == tree->comp(elem, y->l->elem)))
		{
			avlRightRotate(tree, y);
		}

		if (balance < -1 && (1 == tree->comp(elem, y->r->elem)))
		{
			avlLeftRotate(tree, y);
		}

		if (balance > 1 && (1 == tree->comp(elem, y->l->elem)))
		{
			avlLeftRotate(tree, y->l);
			avlRightRotate(tree, y);
		}

		if (balance < -1 && (-1 == tree->comp(elem, y->r->elem)))
		{
			avlRightRotate(tree, y->r);
			avlLeftRotate(tree, y);
		}

		y = y->p;
	}
}

void avlDelete(AVLTree* tree, Item1 elem)
{
	AVLNode* z; // node to be deleted
	AVLNode* y; // Node that is spliced out
	AVLNode* x; // The child of the sliced out node

	// check if node with elem exists in tree
	if( (z = avlExactQuery(tree,elem)) == tree->nil)
	{
		return;
	}

	// we need these guys for the linked list links
	AVLNode* predecessor = avlPredecessor(tree, z);
	AVLNode* successor = avlSucccessor(tree, z);
	AVLNode* temp; // in case of deleting a duplicate

	// case of duplicate
	if (z->end != z)
	{
		temp = z->end;
		z->end->prev->next = z->end->next;

		if (successor != tree->nil) // tree->nil points only to itself
		{
			successor->prev = z->end->prev;
		}

		z->end = z;

		avlDeleteNode(tree, temp);

		tree->size--;

		return;
	}

	/*
	 * Note:
	 * Case 1: The node has no children
	 * Case 2: The node has one child
	 * Case 3: The node has two children
	 */

	// Are we in cases 1,2 or in case 3
	y = ( (z->l == tree->nil) || (z->r == tree->nil) ) ? z : avlSucccessor(tree, z);

	// Set x to the child of y
	x = (y->l == tree->nil) ? y->r : y->l;

	/*
	 * Note:
	 * There is no need to check if x is a valid pointer, we have the dummies!
	 */
	if (tree->root == (x->p = y->p)) // assignment of y->p to x->p is intentional
	{
		tree->root->l = x;
	}
	else if (y == y->p->l)
	{
		y->p->l = x;
	}
	else
	{
		y->p->r = x;
	}

	if (y != z)
	{

		/*
		 * Note:
		 * The data type might be a very big structure, thus copying the key
		 * from one node to another might be very inefficient. It is better to
		 * modify the actual links.
		 */

		// y is the node to splice out and x is its child
		y->l = z->l;
		y->r = z->r;
		y->p = z->p;
		z->l->p = z->r->p = y;
		if (z == z->p->l)
		{
			z->p->l = y;
		}
		else
		{
			z->p->r = y;
		}

		// resolving list links
		if (successor != tree->nil) // tree->nil points only to itself
		{
			successor->prev = z->prev;
		}

		if (predecessor != tree->nil) // tree->nil points only to itself
		{
			predecessor->next = z->next;
		}

		avlDeleteNode(tree, z);
	}
	else
	{
		// resolving list links
		if (successor != tree->nil) // tree->nil points only to itself
		{
			successor->prev = y->prev;
		}

		if (predecessor != tree->nil) // tree->nil points only to itself
		{
			predecessor->next = y->next;
		}

		avlDeleteNode(tree, y);
	}

	tree->size--;

	avlDeleteFixUp(tree, x->p);
}

void avlDestroyTreeHelper(AVLTree* tree, AVLNode* x)
{
	if (x != tree->nil) {
		avlDestroyTreeHelper(tree,x->l);
		avlDestroyTreeHelper(tree,x->r);
		while(x->end != x) // deallocate duplicates as well
		{
			avlDelete(tree, x->elem);
		}
		avlDeleteNode(tree,x);
	}
}

void avlDestroyTree(AVLTree* tree)
{
	avlDestroyTreeHelper(tree, tree->root->l);
	free(tree->root);
	free(tree->nil);
	free(tree);
}

#endif /* AVLTREE_H_ */
