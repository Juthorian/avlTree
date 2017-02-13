//Justin Braham
//0932115
//19/11/2016
//A4 avltree.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct for avlTree
struct avlTree
{
	char * key;
	int frequency;
	int height;
	struct avlTree * leftChild;
	struct avlTree * rightChild;
};

//Function Prototypes
struct avlTree * insertKey(char * key, struct avlTree * root);
void preOrderTraversal(struct avlTree * root, int * count);
int getHeight(struct avlTree * root);
int getBalance(struct avlTree * root);
struct avlTree * leftRotate(struct avlTree * root);
struct avlTree * rightRotate(struct avlTree * root);
struct avlTree * leftLeftRotate(struct avlTree * root);
struct avlTree * leftRightRotate(struct avlTree * root);
struct avlTree * rightRightRotate(struct avlTree * root);
struct avlTree * rightLeftRotate(struct avlTree * root);
void findFrequency(struct avlTree * root, int frequency);
struct avlTree * findNode(struct avlTree * root, char * key);
struct avlTree * deleteNode(struct avlTree * root, char * key);
void freeAVLTree(struct avlTree * root);

int main()
{
	FILE *fp;

	struct avlTree * root = NULL;

	//Menu banner
	printf("Welcome to avltree!\nBy: Justin Braham (0932115)\n\n");

	//Menu loop
	while (1)
	{
		int input = 0;
		char line[50];

		//Menu
		printf("1. Initialization\n2. Find\n3. Insert\n4. Remove\n5. Check Height and Size\n6. Find All (above a given frequency)\n7. Exit\navl\\> ");
		scanf("%d",&input);
		getchar();

		//User wants to initialize
		if (input == 1)
		{
			char fileName[50];
			printf("Please enter a file name: ");
			scanf("%s",fileName);
			getchar();

			//Opens file
			fp = fopen(fileName,"r");

			//Checks if file was successfully opened
			if(fp == NULL)
		    {
		        printf("Could not open file!\n\n");
		  	    continue;
		    }

		    //Parses file 1 word at a time while also adding that word to the tree
		    while(fgets(line, sizeof line, fp))
		    {
		    	char * key = strtok(line, " ");
		    	key = strtok(key, "\n");

		    	while (key != NULL)
		    	{
		    		if (key[0] != '\n')
		    		{
		    			//add to avl tree
		    			root = insertKey(key, root);
		    		}
		    		key = strtok(NULL, " ");
		    		key = strtok(key, "\n");
		    	}
		    }
		    
		    printf("File successfully opened!\n\n");
		    //Close file
		    fclose(fp);
		}
		//user wants to find
		else if (input == 2)
		{
			char key[50];

			printf("Please enter a key to search for: ");
			scanf("%s",key);
			printf("\n");

			//Attempts to find node, if node exists tempTree will not be NULL
			struct avlTree * tempTree = findNode(root, key);
			if (tempTree == NULL)
			{
				printf("No such key: %s found!\n\n", key);
			}
			else
			{
				printf("Node found with key: %s\nThe Node frequency is: %d\n\n", tempTree->key, tempTree->frequency);
			}
		}
		//User wants to insert
		else if (input == 3)
		{
			char key[50];

			printf("Please enter a key to add: ");
			scanf("%s",key);
			printf("\n");

			//Inserts node user entered
			root = insertKey(key,root);
		}
		//User wants to delete
		else if (input == 4)
		{
			char key[50];

			printf("Please enter a key to delete: ");
			scanf("%s",key);
			printf("\n");

			//delete node based on key user entered
			root = deleteNode(root, key);

			//find node and its new frequency
			//Attempts to find node, if node exists tempTree will not be NULL
			struct avlTree * tempTree = findNode(root, key);
			if (tempTree == NULL)
			{
				printf("No such key: %s found!\n\n", key);
			}
			else
			{
				printf("Node found with key: %s\nThe Node frequency is: %d\n\n", tempTree->key, tempTree->frequency);
			}
		}
		//User wants height and size
		else if (input == 5)
		{
			printf("\n");

			int size = 0;

			int height = getHeight(root);

			preOrderTraversal(root, &size);

			printf("Height: %d\nSize: %d\n\n",height,size);
		}
		//User wants to find frequency
		else if (input == 6)
		{
			int frequency;

			printf("Please enter a frequency: ");
			scanf("%d", &frequency);
			getchar();

			//Finds node with a frequency greater or equal to frequency user entered
			findFrequency(root,frequency);
		}
		//User wants to exit;
		else if (input == 7)
		{
			freeAVLTree(root);
			exit(0);
		}
		//Invalid user input
		else
		{
			printf("Invalid Input!\n");
		}
	}

	return 0;
}

/**
* insertKey
* Inserts a given key into the avlTree , if key alreadt exists, increases the frequency of that node instead
* IN: char * key, struct avlTree * root
* OUT: struct avlTree *
* POST: Node is added based on key
* ERROR: None
*/
struct avlTree * insertKey(char * key, struct avlTree * root)
{
	//Checks if root is null and therefore has not been initialized yet
	if (root == NULL)
	{
		root = (struct avlTree *)malloc(sizeof(struct avlTree));
		root->key = malloc(sizeof(char)*50);
		strcpy(root->key, key);
		root->frequency = 1;
		root->leftChild = NULL;
		root->rightChild = NULL;
	}
	//Node key matches node key already in tree, increase frequency, do not add node
	else if (strcmp(key,root->key) == 0)
	{
		root->frequency++;
		root->height = getHeight(root);

		return root;
	}
	//key less then root->key
	else if (strcmp(key,root->key) < 0)
	{
		root->leftChild = insertKey(key, root->leftChild);
		if (getBalance(root) == 2)
		{
			if (strcmp(key, root->leftChild->key) < 0)
			{
				root = leftLeftRotate(root);
			}
			else
			{
				root = leftRightRotate(root);
			}
		}
	}
	//Key greater then root->key
	else if (strcmp(key,root->key) > 0)
	{
		root->rightChild = insertKey(key, root->rightChild);
		if (getBalance(root) == -2)
		{
			if (strcmp(key, root->rightChild->key) > 0)
			{
				root = rightRightRotate(root);
			}
			else
			{
				root = rightLeftRotate(root);
			}
		}
	}
	root->height = getHeight(root);

	return root;
}

/**
* getHeight
* Calculates the height of a given node
* IN: struct avlTree * root
* OUT: int
* POST: Height is calculated and returned
* ERROR: None
*/
int getHeight(struct avlTree * root)
{
	int leftHeight;
	int rightHeight;

	if (root == NULL)
	{
		return 0;
	}
	
	if (root->leftChild == NULL)
	{
		leftHeight = 0;
	}
	else
	{
		leftHeight = 1 + root->leftChild->height;
	}

	if (root->rightChild == NULL)
	{
		rightHeight = 0;
	}
	else
	{
		rightHeight = 1 + root->rightChild->height;
	}

	if (leftHeight > rightHeight)
	{
		return leftHeight;
	}
	else
	{
		return rightHeight;
	}
}

/**
* getBalance
* Gets the balance of a given node
* IN: struct avlTree * root
* OUT: int 
* POST: Balance is calculated and returned
* ERROR: None
*/
int getBalance(struct avlTree * root)
{
	int leftHeight;
	int rightHeight;

	if (root == NULL)
	{
		return 0;
	}
	
	if (root->leftChild == NULL)
	{
		leftHeight = 0;
	}
	else
	{
		leftHeight = 1 + root->leftChild->height;
	}

	if (root->rightChild == NULL)
	{
		rightHeight = 0;
	}
	else
	{
		rightHeight = 1 + root->rightChild->height;
	}
	return (leftHeight-rightHeight);
}

/**
* leftRotate
* Performs a generic left rotate on a node
* IN: struct avlTree * root
* OUT: struct avlTree *
* POST: Node given is left rotated
* ERROR: None
*/
struct avlTree * leftRotate(struct avlTree * root)
{
	struct avlTree * temp;
	
	temp = root->rightChild;

	root->rightChild = temp->leftChild;
	temp->leftChild = root;

	root->height = getHeight(root);
	temp->height = getHeight(temp);

	return temp;
}

/**
* rightRotate
* Performs a generic right rotate on a node
* IN: struct avlTree * root
* OUT: struct avlTree *
* POST: Node given is right rotated
* ERROR: None
*/
struct avlTree * rightRotate(struct avlTree * root)
{
	struct avlTree * temp;
	
	temp = root->leftChild;

	root->leftChild = temp->rightChild;
	temp->rightChild = root;
	
	root->height = getHeight(root);
	temp->height = getHeight(temp);

	return temp;
}

/**
* leftLeftRotate
* Performs a left left rotate on a node
* IN: struct avlTree * root
* OUT: struct avlTree *
* POST: Node given is left left rotated
* ERROR: None
*/
struct avlTree * leftLeftRotate(struct avlTree * root)
{
	root = rightRotate(root);

	return root;
}

/**
* leftRightRotate
* Performs a left right rotate on a node
* IN: struct avlTree * root
* OUT: struct avlTree *
* POST: Node given is left right rotated
* ERROR: None
*/
struct avlTree * leftRightRotate(struct avlTree * root)
{
	root->leftChild = leftRotate(root->leftChild);
	root = rightRotate(root);
	
	return root;
}

/**
* rightRightRotate
* Performs a right right rotate on a node
* IN: struct avlTree * root
* OUT: struct avlTree *
* POST: Node given is right right rotated
* ERROR: None
*/
struct avlTree * rightRightRotate(struct avlTree * root)
{
	root = leftRotate(root);
	
	return root;
}

/**
* rightLeftRotate
* Performs a right left rotate on a node
* IN: struct avlTree * root
* OUT: struct avlTree *
* POST: Node given is right lcceft rotated
* ERROR: None
*/
struct avlTree * rightLeftRotate(struct avlTree * root)
{
	root->rightChild = rightRotate(root->rightChild);
	root = leftRotate(root);
	
	return root;
}

/**
* preOrderTraversal
* Performs a preOrder traversal to count the number of nodes
* IN: struct avlTree * root, int * count
* OUT: None
* POST: Number of nodes is counter and returned by reference
* ERROR: None
*/
void preOrderTraversal(struct avlTree * root, int * count)
{
	if (root != NULL)
	{
		*count = *count + 1;
		preOrderTraversal(root->leftChild, count);
		preOrderTraversal(root->rightChild, count);
	}
}

/**
* findFrequency
* Performs a preOrder traversal while also checking if the frequency of a node is greater 
* or equal to the frequency passed in, if it is display that node, otherwise continue traversal
* IN: struct avlTree * root, int frequency
* OUT: None
* POST: Nodes with frequency greater or equal to frequency passed in displayed
* ERROR: None
*/
void findFrequency(struct avlTree * root, int frequency)
{
	if (root != NULL)
	{
		if (root->frequency >= frequency)
		{
			printf("Node Found:\nKey: %s\nFrequency: %d\n\n", root->key, root->frequency);
		}
		findFrequency(root->leftChild, frequency);
		findFrequency(root->rightChild, frequency);
	}
}

/**
* findNode
* Searches for a node in the avlTree who's key matches the key passed in, returns node if found otherwise returns null
* IN: struct avlTree * root, char * key
* OUT: struct avlTree *
* POST: If node found, that node returned otherwise null is returned
* ERROR: None
*/
struct avlTree * findNode(struct avlTree * root, char * key)
{
	struct avlTree * tempRoot;

	if (root == NULL)
	{
		return NULL;
	}
	if (strcmp(root->key, key) == 0)
	{
		return root;
	}
	if ((tempRoot=findNode(root->leftChild, key)) != NULL)
	{
		return tempRoot;
	}
	if ((tempRoot=findNode(root->rightChild, key)) != NULL)
	{
		return tempRoot;
	}
	return NULL;
}

/**
* deleteNode
* Searches for a node in the avlTree who's key matches the key passed in, deletes the node if found, or
* reduces its frquency if its greater then 1
* IN: struct avlTree * root, char * key
* OUT: struct avlTree *
* POST: If node found, that node deleted or frequency decreased
* ERROR: None
*/
struct avlTree * deleteNode(struct avlTree * root, char * key)
{
	struct avlTree * tempNode = NULL;

	//Node dosnt exist
	if (root == NULL)
	{
		return NULL;
	}
	//right side of tree
	else if (strcmp(key, root->key) > 0)
	{
		root->rightChild = deleteNode(root->rightChild,key);
		if (getBalance(root) == 2)
		{
			if (getBalance(root->leftChild) >= 0)
			{
				root = leftLeftRotate(root);
			}
			else
			{
				root = leftRightRotate(root);
			}
		}
	}
	//left side of tree
	else if (strcmp(key, root->key) < 0)
	{
		root->leftChild = deleteNode(root->leftChild,key);
		if (getBalance(root) == -2)
		{
			if (getBalance(root->rightChild) <= 0)
			{
				root = rightRightRotate(root);
			}
			else
			{
				root = rightLeftRotate(root);
			}
		}
	}
	else if (root->rightChild != NULL)
	{
		tempNode = root->rightChild;

		while (tempNode->leftChild != NULL)
		{
			tempNode = tempNode->leftChild;
		}

		//Dont delete node but instead reduce frequency
		if (root->frequency > 1)
		{
			root->frequency -= 1;
			root->height = getHeight(root);
			return root;
		}
		else
		{
			strcpy(root->key, tempNode->key);
		}

		root->rightChild = deleteNode(root->rightChild, tempNode->key);

		if (getBalance(root) == 2)
		{
			if (getBalance(root->leftChild) >= 0)
			{
				root = leftLeftRotate(root);
			}
			else
			{
				root = leftRightRotate(root);
			}
		}
	}
	else
	{
		return (root->leftChild);
	}

	root->height = getHeight(root);
	return root;
}

/**
* freeAVLTree
* Frees avlTree nodes and keys memory
* IN: struct avlTree * root
* OUT: None
* POST: Memory for avl tree freed
* ERROR: None
*/
void freeAVLTree(struct avlTree * root)
{
	if (root != NULL)
	{
		freeAVLTree(root->leftChild);
		freeAVLTree(root->rightChild);
		free(root->key);
		free(root);
	}
}
