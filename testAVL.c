#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long* Item1;
typedef int Info;
#include "AVLTree.h"

// -----------------------------------------------------------------------------

#define ASSERT(cond, msg) if (!(cond)) { failed(msg); return 0; }

void passed(char* s, float score) {
	printf("Testul %s a fost trecut cu succes!\t Puncte: %.2f\n", s, score);
}

void passed2(char* s, float score) {
	printf("Testul %s a fost trecut cu succes!\t\t Puncte: %.2f\n", s, score);
}

void failed(char* s) {
	printf("Testul %s NU a fost trecut!\n", s);
}

int compare(Item1 a, Item1 b){
	if(a == NULL || b == NULL) return -2;
	if( (*a) > (*b)) return 1;
	if( (*a) < (*b)) return -1;
	return 0;
}

Item1 allocElem(long elem){
	Item1 heapElem =  (Item1)malloc(sizeof(long));
	*heapElem = elem;
	return heapElem;
}

void destroyElem(Item1 elem){
	free(elem);
}
void destroyInfo(Info info){}

int abs(int value){
	return (value >= 0)?value:-value;
}

int checkBalance(AVLTree* tree, AVLNode* x){
	// The tree null is balanced
	if (x == tree->nil)
		return 1;

	// check if left sub-tree is balanced
	int leftSubtreeHeight = checkBalance(tree,x->l);
	if (leftSubtreeHeight == -1) return 0;

	// check if right sub-tree is balanced
	int rightSubtreeHeight = checkBalance(tree,x->r);
	if (rightSubtreeHeight == -1) return 0;

	// if both sub-trees are balanced, check the difference of heights
	// should be less than or equal to 1
	if (abs(leftSubtreeHeight - rightSubtreeHeight) > 1)
		return 0;


	// if tree rooted at this node is balanced, return height if tree rooted at this this node
	return (max(leftSubtreeHeight, rightSubtreeHeight) + 1);
}

int isBalanced(AVLTree* tree, AVLNode* x){
	return (checkBalance(tree, x) != 0);
}

// -----------------------------------------------------------------------------

int testInit(AVLTree** tree,  float score) {                   // tests init


	(*tree) = avlCreateTree(compare,destroyElem, destroyInfo);
	ASSERT((*tree)->root != NULL, "Init-01");
	ASSERT((*tree)->nil != NULL, "Init-02");
	ASSERT((*tree)->root->l == (*tree)->nil, "Init-03");
	ASSERT((*tree)->root->r == (*tree)->nil, "Init-04");
	ASSERT(((*tree)->nil->l == (*tree)->nil), "Init-05");
	ASSERT(((*tree)->nil->r == (*tree)->nil), "Init-06");
	ASSERT(avlIsEmpty((*tree)) == 1, "IsEmpty-01");
	printf(". ");

	passed2("Init&IsEmpty", score);
	return 1;
}

int testInsert(AVLTree** tree,  float score) {                   // tests insert

	avlInsert(*(tree),allocElem(5l),0);
	ASSERT(avlIsEmpty(*tree) != 1, "Insert-01");
	ASSERT((*tree)->root->l->l == (*tree)->nil, "Insert-02");
	ASSERT((*tree)->root->l->r == (*tree)->nil, "Insert-03");

	avlInsert((*tree), allocElem(3l),1);
	avlInsert((*tree),  allocElem(2l),2);
	ASSERT(*((*tree)->root->l->l->elem) == 2l, "Insert-04");
	ASSERT(*((*tree)->root->l->r->elem) == 5l, "Insert-05");

	avlInsert((*tree), allocElem(4l),5);
	avlInsert((*tree), allocElem(7l),6);
	avlInsert((*tree), allocElem(6l),7);
	avlInsert((*tree), allocElem(8l),8);
	ASSERT(*((*tree)->root->l->r->elem) == 7l,"Insert-06");
	ASSERT(*((*tree)->root->l->r->l->elem) == 6l,"Insert-07");
	ASSERT(*((*tree)->root->l->r->r->elem) == 8l,"Insert-08");


	avlInsert((*tree),  allocElem(2l),3);
	avlInsert((*tree),  allocElem(5l),4);
	ASSERT((*tree)->size == 9l, "Insert-09");
	ASSERT(*((*tree)->root->l->next->elem) == 5l,"Insert-10");
	ASSERT(*((*tree)->root->l->next->prev->elem) == 5l,"Insert-11");
	ASSERT(*((*tree)->root->l->prev->elem) == 4l,"Insert-12");
	ASSERT(*((*tree)->root->l->next->next->elem) == 6l,"Insert-13");
	ASSERT(isBalanced(*tree,(*tree)->root->l) ,"Insert-14");

	printf(". ");

	passed2("Insert", score);
	return 1;
}

int testSearch(AVLTree** tree, float score) {                   // tests contains
	long x = 4l;
	ASSERT((avlExactQuery(*tree,&x) != NULL) == 1,"Search-01");
	x = 10l;
	ASSERT((avlExactQuery(*tree,&x) != NULL) == 0,"Search-02");

	x = 0l;
	ASSERT(avlExactQuery((*tree),&x) == NULL,"Search-03");
	x = 5l;
	ASSERT(avlExactQuery((*tree),&x)->l == (*tree)->root->l->l,"Search-04");
	ASSERT(avlExactQuery((*tree),&x)->r == (*tree)->root->l->r,"Search-05");
	printf(". ");

	passed2("Search", score);

	return 1;
}

int testMinMax(AVLTree* (*tree),  float score) { // test min max
	ASSERT(*(avlMinimum((*tree),(*tree)->root->l)->elem) == 2l, "MinMax-01");
	ASSERT(avlMinimum((*tree),(*tree)->root->l)->l == (*tree)->nil, "MinMax-02");
	ASSERT(avlMinimum((*tree),(*tree)->root->l)->r == (*tree)->nil, "MinMax-03");

	ASSERT(*(avlMaximum((*tree),(*tree)->root->l)->elem) == 8l, "MinMax-04");
	ASSERT(avlMaximum((*tree),(*tree)->root->l)->l == (*tree)->nil, "MinMax-05");
	ASSERT(avlMaximum((*tree),(*tree)->root->l)->r == (*tree)->nil, "MinMax-06");
	printf(". ");

	passed("Minimum&Maximium", score);

	return 1;
}


int testSuccPred(AVLTree* (*tree),  float score) { 						// test successor predecessor

	long x;
	ASSERT(avlSucccessor((*tree),avlMaximum((*tree),(*tree)->root->l)) == (*tree)->nil,"Successor-01");
	x = 6l;
	ASSERT(*(avlSucccessor((*tree),avlExactQuery((*tree),&x))->elem) == 7l,"Successor-02");

	x = 2l;
	ASSERT(*(avlSucccessor((*tree),avlExactQuery((*tree),&x))->elem) == 3l,"Successor-03");

	x = 2l;
	ASSERT(avlPredecessor((*tree),avlMinimum((*tree),(*tree)->root->l)) == (*tree)->nil,"Predecesor-01")
	x = 5l;
	ASSERT(*(avlPredecessor((*tree),avlExactQuery((*tree),&x))->elem) == 4l,"Predecesor-02")
	x=7l;
	ASSERT(*(avlPredecessor((*tree),avlExactQuery((*tree),&x))->elem) == 6l,"Predecesor-03")

	printf(". ");
	passed("Successor&Predecessor", score);

	return 1;
}

int testDelete(AVLTree **tree, float score) { 						// test delete

	long x = 2l;
	avlDelete((*tree),&x);
	ASSERT(*((*tree)->root->l->elem) == 5l, "Delete-01");

	x = 7l;
	avlDelete((*tree),&x);
	ASSERT(*((*tree)->root->l->r->elem) == 8l, "Delete-02");

	x = 8l;
	avlDelete((*tree),&x);
	ASSERT(*((*tree)->root->l->r->elem) == 6l, "Delete-03");

	x = 5l;
	long size = (*tree)->size;
	avlDelete((*tree),&x);
	ASSERT(*(avlExactQuery(*tree,&x)->elem) == 5l, "Delete-04");
	ASSERT(*((avlExactQuery(*tree,&x)->next)->elem) == 6l,"Delete-05");
	ASSERT(*(avlExactQuery(*tree,&x)->prev->elem) == 4l,"Delete-06");
	ASSERT((*tree)->size == size-1,"Delete-07");

	avlDelete((*tree),&x);
	ASSERT(isBalanced(*tree,(*tree)->root->l) ,"Delete-08");
	x = 4l;
	ASSERT(*(avlExactQuery(*tree,&x)->prev->elem) == 3l,"Delete-09");
	ASSERT(*(avlExactQuery(*tree,&x)->next->elem) == 6l,"Delete-09");

	printf(". ");
	passed2("Delete", score);

	return 1;
}


int testFree(AVLTree **tree, float score) {
	avlDestroyTree(*tree);
	printf(". Testul Destroy: *Se va verifica cu valgrind*\t\t Puncte: %.2f.\n", score);
	return 1;
}


typedef struct Test {
	int (*testFunction)(AVLTree** tree, float);
	float score;
} Test;

int main(int argc, char* argv[]) {
	Test tests[] =                                    // Define the tests you want
	{
			{ &testInit, 0.00 },
			{ &testInsert, 0.2 },
			{ &testSearch, 0.05 },
			{ &testMinMax, 0.05 },
			{ &testSuccPred, 0.05 },
			{ &testDelete, 0.2 },
			{ &testFree, 0.05}
	};

	float totalScore = 0.0f, maxScore = 0.0f;
	long testsNo = sizeof(tests) / sizeof(Test);
	long testIdx;
	AVLTree* tree;
	for (testIdx = 0; testIdx < testsNo; testIdx++) {
		float score = tests[testIdx].score;
		if ((*(tests[testIdx].testFunction))(&tree, score)) {
			totalScore += score;
		}
		maxScore += score;
	}
	printf("\nScor total: %.2f / %.2f\n\n", totalScore, maxScore);

	return 0;
}
