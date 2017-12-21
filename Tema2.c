/*
 ============================================================================
 Name        : Tema2.c
 Author      : Dan Novischi
 Version     : 007
 Copyright   : Have fun!
 Description : It's not that hard! 
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFLEN 1024
#define ELEMLEN 3

typedef char* Item1;
typedef int Info;
#include "AVLTree.h"
//#include "RBTree.h"

typedef struct Range{
	int *index;
	int size;
	int capacity;
}Range;


int compare(Item1 str1, Item1 str2);
void destroyElem(Item1 elem);
void destroyInfo(Info info);

void inOrderPrint(AVLTree* tree, AVLNode* node);
void preOrderPrint(AVLTree* tree, AVLNode* node);
void postOrderPrint(AVLTree* tree, AVLNode* node);

void printFile(char* fileName);


AVLTree* buildTreeFromFile(char* fileName)
{/*TODO: Cerinta 2 */
	
	AVLTree* tree = avlCreateTree(compare, destroyElem, destroyInfo);

	FILE *fp;
	char* text = (char*) malloc(BUFLEN * sizeof(char)); // file line
	char* word = (char*) malloc(BUFLEN * sizeof(char)); // words in text

	fp = fopen(fileName, "rt"); // file opening in 'read-only' mode

	if (fgets(text, BUFLEN, fp) != NULL) // obtaining the file line (we know that it ends with '\n')
	{
		int i = 0, j = 0, pos; // pos - position of characters

		// range insertion
		while(text[i] != '\0')
		{
			if (text[i] == ' ' || text[i] == '\n') // if we found a word
			{
				word[j] = '\0'; // we assign the end of the string

				if (word[0] != '\0') // if we have a word (not empty string)
				{
					char* fin = (char*) malloc((ELEMLEN + 1) * sizeof(char)); // initializing a string for elem in tree

					fin = strncpy(fin, word, ELEMLEN); // we need only the first tree chars from word
					pos = i - strlen(word); // obtaining position

					avlInsert(tree, fin, pos); // insertion in range
				}
				
				j = 0; // here we go again
			}
			else // the word is still building
			{
				word[j] = text[i];
				j = j + 1; // advance in word position
			}

			i = i + 1; // advance in file
		}
	}

	fclose(fp); // file closing

	// deallocate memory for what we allocated and we don't need it anymore
	free(text);
	free(word);

	return tree;
} //0.05p

// Initialize a range
Range* makeRange(int capacity)
{
	Range* range = (Range*) malloc(sizeof(Range));

	range->capacity = capacity; // initial capacity assigned
	range->size = 0; // we don't have elements (yet)
	range->index = (int*) malloc(capacity * sizeof(int)); // allocate array of indexes

	return range;
}

// insert in a range
void insertRange(Range* range, int x)
{
	if (range->size == range->capacity) // reallocation (resizing)
	{
		range->capacity = 2 * range->capacity; // doubling is better
		range->index = (int*) realloc(range->index, range->capacity * sizeof(int));
	}

	range->index[range->size] = x; // add element
	range->size = range->size + 1; // sizze increases
}

Range* singleKeyRangeQuery(AVLTree* tree, Item1 q) // for this function we use the list from tree
{ /* TODO: Cerinta 3 */	

	AVLNode* node = avlMinimum(tree, tree->root->l); // the head of list
	Range* range = makeRange(1); // 1 is initial capacity
	int position; // position of the substring q
	char* result; // for using strstr()

	if (q != NULL)
	{
		while( node != tree->nil)
		{
			result = strstr(node->elem, q); // substring
			if (result != NULL) // if a substring was found
			{
				position = result - node->elem; // get position
			}
			else
			{
				position = -1; // substring not found
			}
			if (position == 0) // if the substring is the beginning of elem
			{
				insertRange(range, node->info); // insert in range
			}

			node = node->next; // advance
		}
	}

	return range;
} //1p

Range* multiKeyRangeQuery(AVLTree* tree, Item1 q, Item1 p) // for this function we use the list from tree
{/* TODO: Cerinta 4 */

	Range* range = makeRange(1); // 1 is initial capacity
	int position; // position of the substring q anp p
	char* result; // for using strstr()
	AVLNode* node_in = avlMinimum(tree, tree->root->l); // the head of list
	AVLNode* node_fin = avlMaximum(tree, tree->root->l); // the tail of list

	// obtain the first node in [q, p]
	if (q != NULL)
	{
		while( node_in != tree->nil)
		{
			result = strstr(node_in->elem, q); // substring
			if (result != NULL) // if a substring was found
			{
				position = result - node_in->elem; // get position
			}
			else
			{
				position = -1; // substring not found
			}
			if ((position == 0) || (1 == tree->comp(node_in->elem, q))) // if node_in->elem is >= q (greater or equal)
			{
				break; // we found it
			}

			node_in = node_in->next; // advance
		}
	}

	// obtain the last node in [q, p]
	if (p != NULL)
	{
		while( node_fin != tree->nil)
		{
			result = strstr(node_fin->elem, p); // substring
			if (result != NULL) // if a substring was found
			{
				position = result - node_fin->elem; // get position
			}
			else
			{
				position = -1; // substring not found
			}
			if ((position == 0) || (-1 == tree->comp(node_fin->elem, p))) // if node_fin->elem is <= p (less or equal)
			{
				break; // we found it
			}

			node_fin = node_fin->prev; // advance
		}
	}

	if ((-1 == tree->comp(node_in->elem, node_fin->elem)) || (0 == tree->comp(node_in->elem, node_fin->elem))) // if the [q, p] was correct (a.k.a. q <= p) and node_in->elem <= node_fin->elem
	{
		while( node_in != node_fin) // insert all elements between node_in and node_fin
		{
			insertRange(range, node_in->info);

			node_in = node_in->next; // advance
		}

		insertRange(range, node_fin->info); // insert node_fin as well
	}

	return range;
} //2p

void printWordsInRange(Range* range, char* fileName)
{/* TODO: Cerinta 5 */ 

	FILE* fp;
	int i, j;
	char* text = (char*) malloc(BUFLEN * sizeof(char)); // file line

	fp = fopen(fileName, "rt"); // file opening in 'read-only' mode

	if (fgets(text, BUFLEN, fp) != NULL) // obtaining the file line (we know that it ends with '\n')
	{
		for (i = 0; i < range->size; i++)
		{
			j = range->index[i]; // obtain index of word

			printf("%d. ", i+1); // print current number


			while((text[j] != ' ') && (text[j] != '\n')) // till it's a word
			{
				printf("%c", text[j]); // print character in text

				j = j + 1; // advance in text
			}
			printf("\n"); // new line
		}
	}

	fclose(fp); // close file

	// deallocate unneeded memory
	free(text);

} //0.05p


int main(void) {

	printf("The text file:\n");
	printFile("text.txt");

	AVLTree* tree = buildTreeFromFile("text.txt");
	printf("Tree In Order:\n");
	inOrderPrint(tree, tree->root->l);
	printf("\n\n");

	printf("Single search:\n");
	Range *range = singleKeyRangeQuery(tree,"vr");
	printWordsInRange(range,"text.txt");

	printf("Multi search:\n");
	Range *range2 = multiKeyRangeQuery(tree,"j","pr");
	printWordsInRange(range2,"text.txt");

	free(range->index);
	free(range);
	free(range2->index);
	free(range2);
	//rbDestroyTree(tree);
	avlDestroyTree(tree);
	return 0;
}

int compare(Item1 str1, Item1 str2){
	if(str1 == NULL) return -2;
	if(str2 == NULL) return 2;

	if(strcmp(str1,str2) > 0) return 1;
	else if(strcmp(str1,str2) < 0) return -1;
	return 0;
}

void destroyElem(Item1 elem){
	free(elem);
}
void destroyInfo(Info info){
}

void inOrderPrint(AVLTree* tree, AVLNode* node){
	if(node != tree->nil){
		inOrderPrint(tree, node->l);
//		RBNode* begin = node;
//		RBNode* end = node->end->next;
		AVLNode* begin = node;
		AVLNode* end = node->end->next;
		while(begin != end){
			printf("%d:%s  ",begin->info,begin->elem);
			begin = begin->next;
		}
		inOrderPrint(tree, node->r);
	}
}

void preOrderPrint(AVLTree* tree, AVLNode* node){
	if(node != tree->nil){
//		RBNode* begin = node;
//		RBNode* end = node->end->next;
		AVLNode* begin = node;
		AVLNode* end = node->end->next;
		while(begin != end){
			printf("%d %s \t",begin->info,begin->elem);
			begin = begin->next;
		}
		preOrderPrint(tree, node->l);
		preOrderPrint(tree, node->r);
	}
}

void postOrderPrint(AVLTree* tree, AVLNode* node){
	if(node != tree->nil){
		preOrderPrint(tree, node->l);
		preOrderPrint(tree, node->r);
//		RBNode* begin = node;
//		RBNode* end = node->end->next;
		AVLNode* begin = node;
		AVLNode* end = node->end->next;
		while(begin != end){
			printf("%d %s \t",begin->info,begin->elem);
			begin = begin->next;
		}
	}
}

void printFile(char* fileName){
	if(fileName == NULL) return;
	FILE * file = fopen(fileName,"r");
	if (file == NULL) return;
	char *buf = (char*) malloc(BUFLEN+1);
	while(fgets(buf,BUFLEN,file) != NULL){
		printf("%s",buf);
	}
	printf("\n");
	free(buf);
	fclose(file);
}
