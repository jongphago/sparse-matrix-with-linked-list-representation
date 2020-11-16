#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 50

typedef enum tagfield { head, entry } tagfield;
typedef struct matrix_node* matrix_pointer;
typedef struct entry_node {
	int row;
	int col;
	int value;
}entry_node;
typedef struct matrix_node {
	matrix_pointer down;
	matrix_pointer right;
	tagfield tag;
	union u {
		matrix_pointer next;
		entry_node entry;
	} u;
}matrix_node;

matrix_pointer hdnode[MAX_SIZE];
matrix_pointer new_node();
matrix_pointer mread(FILE* inputFile);			
void mwrite(matrix_pointer node);					
void merase(matrix_pointer* pointer);			

void main()															
{
	matrix_pointer a, b;

	FILE* inputA = fopen("A.txt", "r");
	FILE* inputB = fopen("B.txt", "r");

	a = mread(inputA);
	b = mread(inputB);

	mwrite(a);
	mwrite(b);

	merase(&a);
	merase(&b);

	mwrite(a);
	mwrite(b);
}

matrix_pointer new_node()
{
	matrix_pointer tempPointer = malloc(sizeof(matrix_node));
	tempPointer->down = NULL;
	tempPointer->right = NULL;
	tempPointer->tag = 0;
	tempPointer->u.entry.col = NULL;
	tempPointer->u.entry.row = NULL;
	tempPointer->u.entry.value = NULL;
	tempPointer->u.next = NULL;
	return tempPointer;
}

matrix_pointer mread(FILE* inputFile)
{
	int num_rows, num_cols, num_terms, num_heads, i;
	int row, col, value, current_row;
	matrix_pointer temp, last=new_node(), node=new_node();

	fscanf(inputFile, "%d %d", &num_rows, &num_cols);
	num_heads = (num_cols > num_rows) ? num_cols : num_rows;
	node = new_node(); node->tag = entry;
	node->u.entry.row = num_rows;
	node->u.entry.col = num_cols;

	if (!num_heads) node->right = node;
	else 
	{
		for (i = 0; i < num_heads; i++) {
			temp = new_node();
			hdnode[i] = temp; hdnode[i]->tag = head;
			hdnode[i]->right = temp; hdnode[i]->u.next = temp;
		}
		current_row = 0; last = hdnode[0];
		for (i = 0; i < num_cols * num_rows; i++) {
			fscanf(inputFile, "%d", &value); row = i / num_cols; col = i % num_cols;
			if (value == 0)
				continue;
			if (row > current_row) {														 
				last->right = hdnode[current_row];								
				current_row = row;															
				last = hdnode[row];															
			}
			temp = new_node(); temp->tag = entry;							
			temp->u.entry.row = row; temp->u.entry.col = col;		
			temp->u.entry.value = value;												 

			last->right = temp;																 
			last = temp;																			 
			hdnode[col]->u.next->down = temp;								 
			hdnode[col]->u.next = temp;												 
		}
	}
	// close last row
	last->right = hdnode[current_row];											  
	// close all column lists
	for (i = 0; i < num_cols; i++)
		hdnode[i]->u.next->down = hdnode[i];								
	// link all head nodes together
	for (i = 0; i < num_heads - 1; i++)												
		hdnode[i]->u.next = hdnode[i + 1];									
	hdnode[num_heads - 1]->u.next = node;							
	node->right = hdnode[0];														
	return node;
}

void mwrite(matrix_pointer node)
{
	int i;
	if (node == NULL) {
		printf("empty\n");
		return;
	}
	matrix_pointer temp=new_node(), head = node->right;
	for (i = 0; i < node->u.entry.row; i++) {
		for (temp = head->right; temp != head; temp = temp->right)
			printf("%d %d %d\n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
		head = head->u.next;
	}
	printf("\n");
	return;
}

void merase(matrix_pointer* node)
{
	int i, num_heads;
	//matrix_pointer node = *nodePointer;
	matrix_pointer x, y, head = (*node)->right;

	for (i = 0; i < (*node)->u.entry.row; i++) {
		y = head->right;
		while (y != head) {
			x = y; y = y->right; free(x);
		}
		x = head; head = head->u.next; free(x);
	}
	// free remaining head nodes
	y = head;
	while (y != (*node)) {
		x = y; y = y->u.next; free(x);
	}
	free((*node)); (*node )= NULL;
	return;
}