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
matrix_pointer mread(FILE* inputFile);				// [Program 4.23] file에서 matrix 자료를 읽어서 리스트에 저장한다.
void mwrite(matrix_pointer node);					// [Program 4.24] 행렬을 적절한 포맷(2차원 행렬형태)으로 출력한다.
void merase(matrix_pointer* pointer);				// [Program 4.25]  NULL parameter인 경우 적절히 처리한다.

void main()																// 4.7 절의 선언과 함수들 참고
{
	matrix_pointer a, b;

	FILE* inputA = fopen("A.txt", "r");
	FILE* inputB = fopen("B.txt", "r");

	a = mread(inputA);
	b = mread(inputB);


	mwrite(a);
	mwrite(b);

	//merase(&a);
	//merase(&b);

	//mwrite(a);
	//mwrite(b);
};

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
				last->right = hdnode[current_row];								//........		1 : 
				current_row = row;															//........		2 : 
				last = hdnode[row];															//........		3 : 행이 바뀌면 헤드노드를 last로 지정
			}
			temp = new_node(); temp->tag = entry;							//........		4 : 새로운 노드(temp)를 선언하고 값 할당
			temp->u.entry.row = row; temp->u.entry.col = col;		//.........		.
			temp->u.entry.value = value;												//........		. 

			last->right = temp;																//........		5 : 마지막 노드(last) 다음에 temp 연결
			last = temp;																			//........		6 : 연결한 temp 노드를 last로 지정
			hdnode[col]->u.next->down = temp;								//........		7 : 헤드노드 다음노드 down에 temp 연결
			hdnode[col]->u.next = temp;												//........		8 : temp를 헤드노드의 연결된 마지막 노드로 지정
		}
	}
	// close last row
	last->right = hdnode[current_row];											//........		9 : 
	// close all column lists
	for (i = 0; i < num_cols; i++)
		hdnode[i]->u.next->down = hdnode[i];								//........	  10 : 
	// link all head nodes together
	for (i = 0; i < num_heads - 1; i++)												
		hdnode[i]->u.next = hdnode[i + 1];										//........	  11 : 
	hdnode[num_heads - 1]->u.next = node;								//........	  12 : 
	node->right = hdnode[0];															//........	  13 : 
	return node;
}

void mwrite(matrix_pointer node)
{
	int i;
	matrix_pointer temp=new_node(), head = node->right;
	for (i = 0; i < node->u.entry.row; i++) {
		for (temp = head->right; temp != head; temp = temp->right)
			printf("%d %d %d\n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
		head = head->u.next;
	}
	printf("\n");
}
