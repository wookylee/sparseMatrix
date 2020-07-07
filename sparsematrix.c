#include <stdio.h>
#include <malloc.h>
#define MAX_SIZE 50
#define MAX_INDEX 64

typedef enum{head,entry} tagfield;
typedef struct matrixNode *matrixPointer;
typedef struct entryNode {
	int row;
	int col;
	int value;
} entryNode;
typedef struct matrixNode {
	matrixPointer down;
	matrixPointer right;
	tagfield tag;
	union {
		matrixPointer next;
		entryNode entry;
		} u;
};
matrixPointer hdnode[MAX_SIZE];
matrixPointer matrix[MAX_INDEX];

int compare(int a, int b) 
{
	if (a > b)
		return -1;
	else if (a == b)
		return 0;
	else
		return 1;


}

matrixPointer newNode(void) {

	matrixPointer temp;
	temp = (matrixPointer)malloc(sizeof(*temp));
	return temp;

}


matrixPointer mread(void)
{
	int numRows, numCols, numTerms, numHeads, i;
	int row, col, value, currentRow;
	matrixPointer temp, last, node;
	

	printf("Enter the number of rows, columns and number of nonzero terms: ");
	scanf_s("%d %d %d", &numRows, &numCols, &numTerms);
	if (numTerms > numRows * numCols) {
		printf("There are too many terms");
		return 0;
	}
		
	numHeads = (numCols > numRows) ? numCols : numRows;
	node = newNode();
	node->tag = entry;
	node->u.entry.row = numRows;
	node->u.entry.col = numCols;

	if (!numHeads)
		node->right = node;
	else {
		for (i = 0; i < numHeads; i++) {
			temp = newNode();
			hdnode[i] = temp;
			hdnode[i]->tag = head;
			hdnode[i]->right = temp;
			hdnode[i]->u.next = temp;
		}
		currentRow = 0;
		last = hdnode[0];
		for (i = 0; i < numTerms; i++) {
			printf("Enter row, column and value: ");
			scanf_s("%d%d%d", &row, &col, &value);
			if (row >= numRows || col >= numCols || row < 0 || col < 0) {
				printf("Insert the right row or col for the matrix\n");
				return 0;
			}
		
			if (row > currentRow) {
				last->right = hdnode[currentRow];
				currentRow = row;
				last = hdnode[row];
			}
			temp = newNode();
			temp->tag = entry;
			temp->u.entry.row = row;
			temp->u.entry.col = col;
			temp->u.entry.value = value;
			last->right = temp;
			last = temp;
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
		}
		last->right = hdnode[currentRow];
		for (i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		for (i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = node;
		node->right = hdnode[0];
	}
	return node;
}

void mwrite(matrixPointer node)
{
	int i;
	matrixPointer temp, head;
	head = node->right;
	printf("\n numRows = %d, num Cols = %d\n", node->u.entry.row, node->u.entry.col);
	printf("The matrix by row, column, and value: \n\n");
	for (i = 0; i < node->u.entry.row; i++) {
		for (temp = head->right; temp != head; temp = temp->right) {
			printf("%d %d %d \n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
		}
		head = head->u.next;		
	}
}

void merase(matrixPointer *node)
{
	matrixPointer x, y, head;
	int i;
	if (*node == NULL)
		return 0;
	else {
		head = (*node)->right;
		for (i = 0; i < (*node)->u.entry.row; i++) {
			y = head->right;
			while (y != head) {
				x = y;
				y = y->right;
				free(x);
			}
			x = head;
			head = head->u.next;
			free(x);
		}
		y = head;
		while (y != *node) {
			x = y;
			y = y->u.next;
			free(x);
		}
		free(*node);
		*node = NULL;
	}
}

matrixPointer madd(matrixPointer m1, matrixPointer m2) {
	
	int i, numHeads, currentRow;
	matrixPointer temp1, temp2, temp3, saved = NULL;
	matrixPointer head1, head2, node, temp;
	matrixPointer savehdnode[MAX_SIZE], last;

	head1 = m1->right;
	head2 = m2->right;

	if (m1->u.entry.col != m2->u.entry.col || m1->u.entry.row != m2->u.entry.row) {
		printf("The two matrix can not be added\n");
		return 0;
	}
	
	saved = newNode();
	saved->tag = entry;
	saved->u.entry.row = m1->u.entry.row;
	saved->u.entry.col = m1->u.entry.col;
	if (saved->u.entry.col > saved->u.entry.row)
		numHeads = saved->u.entry.col;
	else
		numHeads = saved->u.entry.row;
	

	if (!numHeads)
		saved->right = saved;
	else {
		for (i = 0; i < numHeads; i++) {
			temp3 = newNode();
			savehdnode[i] = temp3;
			savehdnode[i]->tag = head;
			savehdnode[i]->right = temp3;
			savehdnode[i]->u.next = temp3;
		}
		currentRow = 0;
		last = savehdnode[0];

		temp1 = m1->right;
		temp2 = m2->right;

		while(!(temp1 == m1 && temp2 == m2)) {
			if (temp1->tag == head) {
				if (temp1->right == temp1) {
					temp1 = temp1->u.next;
					continue;
				}
				temp1 = temp1->right;
			}
			if (temp2->tag == head) {
				if (temp2->right == temp2) {
					temp2 = temp2->u.next;
					continue;
				}
				temp2 = temp2->right;
			}

			if (temp1->u.entry.row < temp2->u.entry.row) {

				temp = temp1;
				temp1 = temp1->right;
			}
			else if (temp1->u.entry.row == temp2->u.entry.row) {

				if (temp1->u.entry.col < temp2->u.entry.col) {

					temp = temp1;
					temp1 = temp1->right;
				}

				else if (temp1->u.entry.col == temp2->u.entry.col) {
					temp = NULL;
					if (temp1->u.entry.row > currentRow) {
						last->right = savehdnode[currentRow];
						currentRow = temp1->u.entry.row;
						last = savehdnode[temp1->u.entry.row];
					}

					node = newNode();
					node->tag = entry;
					node->u.entry.row = temp1->u.entry.row;
					node->u.entry.col = temp1->u.entry.col;
					node->u.entry.value = temp1->u.entry.value + temp2->u.entry.value;
					last->right = node;
					last = node;

					savehdnode[temp1->u.entry.col]->u.next->down = node;
					savehdnode[temp1->u.entry.col]->u.next = node;

					temp1 = temp1->right;
					temp2 = temp2->right;
				}

				else {
					temp = temp2;
					temp2 = temp2->right;
				}
			}
			else {
				temp = temp2;
				temp2 = temp2->right;
			}

			if ( temp != NULL) {
				if (temp->u.entry.row > currentRow) {
					last->right = savehdnode[currentRow];
					currentRow = temp->u.entry.row;
					last = savehdnode[temp->u.entry.row];
				}

				node = newNode();
				node->tag = entry;
				node->u.entry.row = temp->u.entry.row;
				node->u.entry.col = temp->u.entry.col;
				node->u.entry.value = temp->u.entry.value;
				last->right = node;
				last = node;
				savehdnode[temp->u.entry.col]->u.next->down = node;
				savehdnode[temp->u.entry.col]->u.next = node;

			}

			if (temp1->tag == head)
				temp1 = temp1->u.next;
			if (temp2->tag == head)
				temp2 = temp2->u.next;
		}

		last->right = savehdnode[currentRow];
		
		for (i = 0; i < saved->u.entry.col; i++)
			savehdnode[i]->u.next->down = savehdnode[i];
		for (i = 0; i < numHeads - 1; i++) 
			savehdnode[i]->u.next = savehdnode[i + 1];
		savehdnode[numHeads - 1]->u.next = saved;
		saved->right = savehdnode[0];
	}

	return saved;

}

matrixPointer mmult(matrixPointer m1, matrixPointer m2) {
	int numrows, numcols, numheads;
	int i, j;
	int done = 0;
	int row, col, value, currentrow;
	matrixPointer temp, last, findnode, multnode;
	matrixPointer multhdnode[MAX_SIZE];
	matrixPointer m1head, m2head, m1temp, m2temp;

	multnode = NULL;
	
	m1temp = NULL;
	m2temp = NULL;
	
	if (m1->u.entry.col == m2->u.entry.row) {
		m1head = m1->right;
		m2head = m2->right;

		numrows = m1->u.entry.row;
		numcols = m2->u.entry.col;
		if (numcols > numrows)
			numheads = numcols;
		else
			numheads = numrows;

		multnode = newNode();
		multnode->tag = entry;
		multnode->u.entry.row = numrows;
		multnode->u.entry.col = numcols;

		if (!numheads)
			multnode->right = multnode;
		else {
			for (i = 0; i < numheads; i++) {
				temp = newNode();
				multhdnode[i] = temp;
				multhdnode[i]->tag = head;
				multhdnode[i]->right = temp;
				multhdnode[i]->u.next = temp;
			}
			currentrow = 0;
			last = multhdnode[0];

			for (i = 0; i < m1->u.entry.row; i++) {
				row = 0;
				col = 0;
				value = 0;
				for (j = 0; j < m2->u.entry.col; ) {
					if (j == 0 && done == 0) {
						m1temp = m1head->right;
						m2temp = m2head->down;

					}
					if (m2temp->tag == head || m2temp->down == m2temp) {
						done = 0;
						m2head = m2head->u.next;
						m2temp = m2head->down;
						m1temp = m1head->right;
						row = 0;
						col = 0;
						value = 0;
						j++;

					}
					if (m1temp->tag == head) {
						if (m1temp->right == m1temp) {
							done = 0;
							break;
						}
						if (m2head->u.next != m2) {
							m1temp = m1head->right;
							row = 0;
							col = 0;
							value = 0;
							m2head = m2head->u.next;
							m2temp = m2head->down;
							done = 0;
							j++;
						}
						else
							j++;
					}
					if (j >= m2->u.entry.col)
						break;
					if (m1temp->u.entry.col < m2temp->u.entry.row) {
						m1temp = m1temp->right;
						done = 1;
						continue;
					}
					else if (m1temp->u.entry.col == m2temp->u.entry.row) {
						done = 1;
						if (value != 0) {
							row = m1temp->u.entry.row;
							col = m2temp->u.entry.col;
							findnode = multhdnode[row]->right;
							while (1) {
								if (findnode->u.entry.col == col) {
									findnode->u.entry.value = findnode->u.entry.value + value;
									break;
								}
								else
									findnode = findnode->right;

								if (findnode->tag == head)
									break;
							}
							m1temp = m1temp->right;
							m2temp = m2temp->down;

						}
						else if (value == 0) {
							row = m1temp->u.entry.row;
							col = m2temp->u.entry.col;
							value = (m1temp->u.entry.value) * (m2temp->u.entry.value);
							if (row > currentrow) {
								last->right = multhdnode[currentrow];
								currentrow = row;
								last = multhdnode[row];
							}
							temp = newNode();
							temp->tag = entry;
							temp->u.entry.row = row;
							temp->u.entry.col = col;
							temp->u.entry.value = value;
							last->right = temp;
							last = temp;
							multhdnode[col]->u.next->down = temp;
							multhdnode[col]->u.next = temp;
							m1temp = m1temp->right;
							m2temp = m2temp->down;
						}
						continue;
					}
					else {
						done = 1;
						m2temp = m2temp->down;
						continue;
					}
				}
				m1head = m1head->u.next;
				m2head = m2->right;
				done = 0;						
			}

			last->right = multhdnode[currentrow];
				
			for (i = 0; i < numcols; i++)
				multhdnode[i]->u.next->down = multhdnode[i];
				
			for (i = 0; i < numheads-1; i++)				
				multhdnode[i]->u.next = multhdnode[i + 1];				
			multhdnode[numheads - 1]->u.next = multnode;
			multnode->right = multhdnode[0];
							
		}
	}
	else {
		printf("The matrix is unable to multiply\n");
		return NULL;
	}
	return multnode;
}
matrixPointer mtranspose(matrixPointer node) {
	int numheads, numrows, numcols;
	int i, j;
	int currentrow;
	int row, col, value;
	matrixPointer transposenode, temp, last;
	matrixPointer transhdnode[MAX_SIZE];
	matrixPointer nodehd, nodetemp;

	nodehd = node->right;

	numcols = node->u.entry.row;
	numrows = node->u.entry.col;
	if (numcols > numrows)
		numheads = numcols;
	else
		numheads = numrows;

	transposenode = newNode();
	transposenode->tag = entry;
	transposenode->u.entry.row = numrows;
	transposenode->u.entry.col = numcols;

	if (!numheads)
		transposenode->right = transposenode;
	else {
		for (i = 0; i < numheads; i++) {
			temp = newNode();
			transhdnode[i] = temp;
			transhdnode[i]->tag = head;
			transhdnode[i]->right = temp;
			transhdnode[i]->u.next = temp;

		}
		currentrow = 0;
		last = transhdnode[0];

		for (j = 0; j < node->u.entry.row; j++) {
			for (nodetemp = nodehd->right; nodetemp != nodehd; nodetemp = nodetemp->right) {
				row = nodetemp->u.entry.col;
				col = nodetemp->u.entry.row;
				value = nodetemp->u.entry.value;
				if (row > currentrow) {
					last->right = transhdnode[currentrow];
					currentrow = row;
					last = transhdnode[row];
				}
				temp = newNode();
				temp->tag = entry;
				temp->u.entry.row = row;
				temp->u.entry.col = col;
				temp->u.entry.value = value;
				last->right = temp;
				last = temp;
				transhdnode[col]->u.next->down = temp;
				transhdnode[col]->u.next = temp;

			}
			nodehd = nodehd->u.next;
		}
		last->right = transhdnode[currentrow];
		for (i = 0; i < numcols; i++) {
			transhdnode[i]->u.next = transhdnode[i + 1];
		}
			transhdnode[numheads - 1]->u.next = transposenode;
			transposenode->right = transhdnode[0];
		
	}
	return transposenode;
}


void main()
{
	int menuNum;
	int matrixNum;
	int mat1, mat2, save;
	for (int i = 0; i < MAX_INDEX; i++)
		matrix[i] = NULL;	

	while (1)
	{
		printf("-----------------\n");
		printf("Menu\n");
		printf("1. Read\n");
		printf("2. Write\n");
		printf("3. Erase\n");
		printf("4. Add\n");
		printf("5. Multiply\n");
		printf("6. Transpose\n");
		printf("7. Exit\n");
		printf("-----------------\n");
		printf("select a number of the Menu: ");
		scanf_s("%d", &menuNum);

		if (menuNum == 1) {
			printf("Save index: ");
			scanf_s("%d", &matrixNum);
			
			if (matrixNum <= 0 || matrixNum > 64) 
				printf("Index must be between 1 to 64\n");
				
			else	
				matrix[matrixNum] = mread();			
		}

		if (menuNum == 2) {
			printf("Print index: ");
			scanf_s("%d", &matrixNum);
			printf("\n");

			if (matrixNum <= 0 || matrixNum > 64)
				printf("Index must be between 1 to 64\n");

			else if (matrix[matrixNum] != NULL) {
				printf("The matrix of this index is: \n");
				
				mwrite(matrix[matrixNum]);
				printf("\n");			
			}

			else
				printf("Please input a matrix!\n");
		}

		if (menuNum == 3) {

			printf("Erase index: ");
			scanf_s("%d", &matrixNum);
			printf("\n");

			if (matrixNum <= 0 || matrixNum > 64)
				printf("Index must be between 1 to 64\n");

			else if (matrix[matrixNum] != NULL)
				merase(&matrix[matrixNum]);
			else
				printf("There is no matrix in index to erase\n");
		}

		if (menuNum == 4) {

			printf("First, second, save index(add): ");
			scanf_s("%d %d %d", &mat1, &mat2, &save);
			printf("\n");
			
			if (mat1 <= 0 || mat1 > 64 || mat2 <= 0 || mat2 > 64 || save <= 0 || save > 64)
				printf("Index must be between 1 to 64\n");

			else if (matrix[mat1] != NULL && matrix[mat2] != NULL) {
				merase(&matrix[save]);
				matrix[save] = madd(matrix[mat1], matrix[mat2]);
				printf("The result of add is: \n");
				mwrite(matrix[save]);
			}

			else
				printf("There is no matrix in index to add\n");
		}

		if (menuNum == 5) {
			
			printf("First, second, save index(mult): ");
			scanf_s("%d %d %d", &mat1, &mat2, &save);
			printf("\n");
			
			if (mat1 <= 0 || mat1 > 64 || mat2 <= 0 || mat2 > 64 || save <= 0 || save > 64)
				printf("Index must be between 1 to 64\n");

			else if (matrix[mat1] != NULL && matrix[mat2] != NULL) {
				merase(&matrix[save]);
				matrix[save] = mmult(matrix[mat1], matrix[mat2]);
				printf("The result of multiply is: \n");
				mwrite(matrix[save]);
			}

			else
				printf("There is no matrix in index to multiply\n");

		}

		if (menuNum == 6) {

			printf("Transpose index, save index(transpose): ");
			scanf_s("%d %d", &matrixNum, &save);
			printf("\n");

			if (matrixNum <= 0 || matrixNum > 64 || save <= 0 || save > 64)
				printf("Index must be between 1 to 64\n");

			else if (matrix[matrixNum] != NULL) {
				merase(&matrix[save]);
				matrix[save] = mtranspose(matrix[matrixNum]);
				mwrite(matrix[save]);
			}

			else
				printf("There is no matrix in index to transpose\n");				
			
		}
		
		if (menuNum == 7) 
			exit(0);

	}


	 
	return 0;
}