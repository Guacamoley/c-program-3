/*
Michael Ha
ICS 365-50
Professor Carlson
03/17/2022

-- breadth first traversal was based on pseudocode from https://www.geeksforgeeks.org/level-order-tree-traversal/ --
*/

#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *leftValue;
    struct Node *rightValue;
};

// creates a new node with left & right values NULL, returns the newNode
struct Node *newNode(int value)
{
    // new node is created with malloc to dynamically allocate memory and space
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->leftValue = newNode->rightValue = NULL;
    return newNode;
}

// inserts a value into the root
struct Node *insert(struct Node *root, int data)
{
    // if root is NULL then returns a new node allocated with the value
    if (root == NULL)
        return newNode(data);

    // else if the value being added is less than root, then it'll be inserted into the left leaf
    else if (data <= root->data)
    {
        root->leftValue = insert(root->leftValue, data);
    }
    // same as above but since its not less than the root, then it means its bigger and will be put into right leaf
    else
    {
        root->rightValue = insert(root->rightValue, data);
    }
    return root;
}

// helper function to print tree in depth first traverse
void printLevel(struct Node *root, int level)
{
    // if root is null then it returns nothing.
    if (root == NULL)
        return;

    // if the level is 1, then it just prints out the root which is always at the top
    if (level == 1)
        printf("%d -> ", root->data);

    // if the level is bigger than 1, then it recursively visits each left and right value and prints those out
    else if (level > 1)
    {
        printLevel(root->leftValue, level - 1);
        printLevel(root->rightValue, level - 1);
    }
}

// gets the height of the tree
int getHeight(struct Node *root)
{
    // if root is NULL then returns 0 since the tree is not allocated
    if (root == NULL)
        return 0;
    else
    {
        // recursively gets the left and right height of tree by traversing until the last leaf
        int leftHeight = getHeight(root->leftValue);
        int rightHeight = getHeight(root->rightValue);

        // compares which is bigger and then returns the biggest one as the height
        if (leftHeight > rightHeight)
            return (leftHeight + 1);
        else
            return (rightHeight + 1);
    }
}

// finds the level of element thats requested, will always start from level 1 or the root
int findLevel(struct Node *root, int value, int level)
{
    // temporary integer that will hold the level of a value and then be returned.
    int traverseLevel;
    // if root is null then returns 0 for the level
    if (root == NULL)
    {
        return 0;
    }
    // if the data is found to be at the root, then the level is 1
    if (root->data == value)
    {
        return level;
    }

    // traverses the left side of the tree to try and find the value
    // each time a traversal is made, the level is incremented
    traverseLevel = findLevel(root->leftValue, value, level + 1);

    // only if the level found is not 0 will it be returned for error catching
    if (traverseLevel != 0)
        return traverseLevel;

    // same thing as with traversing the left side of the tree
    traverseLevel = findLevel(root->rightValue, value, level + 1);

    // finally returns the level
    return traverseLevel;
}

// simple call to find the level of a value
int getLevel(struct Node *root, int value)
{
    return findLevel(root, value, 1);
}

// prints out tree in breadth first fashion
void breadthFirst(struct Node *root)
{
    // gets the height of the tree for use in looping through
    int h = getHeight(root);
    int i;

    printf("\n");
    // loops through the height of the tree to print each level the loop visits
    for (i = 1; i <= h; i++)
    {
        printLevel(root, i);
    }
}

// check file algorithm after user enters a file name before tree gets made
int checkFile(FILE *fileptr)
{
    // if file is not null, then it'll check if the file is empty
    if (fileptr != NULL)
    {
        // seeks to the end of the file and returns the size
        fseek(fileptr, 0, SEEK_END);
        int size = ftell(fileptr);

        // if size is 0 then the process aborts and gives user feedback
        if (size == 0)
        {
            // returns if the file is empty also telling the user
            printf("The file is empty\n");
            return 0;
        }
        else
        {
            // else it returns 1, rewinds the pointer back to the beginning of the file and gives user feedback
            printf("File is not empty\n");
            rewind(fileptr);
            return 1;
        }
    }
    // else the file simply does not exist and aborts with user feedback
    else
    {
        printf("The file does not exist\n");
        return 0;
    }
}

// gets the amount new line characters in the file indicating that each new line contains a value
// size will be use for array allocation
int getSize(FILE *fileptr)
{
    int count = 0;
    int i;
    // loops until the end of the file
    for (i = getc(fileptr); i != EOF; i = getc(fileptr))
    {
        // if getc finds a newline character, then count is incremented
        if (i == '\n')
        {
            count = count + 1;
        }
    }
    // returns the pointer back to the beginning of the file
    rewind(fileptr);
    return count;
}

// prints tree in depth first traversal
// traverses the nodes post order form
void depthFirst(struct Node *root)
{
    if (root == NULL)
        return;
    depthFirst(root->leftValue);
    depthFirst(root->rightValue);
    printf("%d -> ", root->data);
}

int main()
{
    // creates a new tree with a null root
    struct Node *root = NULL;
    // used to store filename that user inputs
    char fileName[20];
    printf("Enter the name of your file: ");
    scanf("%s", &fileName);

    // opens file
    FILE *FP = fopen(fileName, "r");

    // choice used to hold user input for switch
    int choice;
    // val for user value input
    int val;

    // checks the file first for null, empty file etc
    // if file returns 1, then program proceeds
    if (checkFile(FP) == 1)
    {
        // gets size of file for array
        int count = getSize(FP);

        // creates an array with the size of the file
        int array[count];

        // loops through the array and allocates with the data inside the file
        for (int i = 0; i < count; i++)
        {
            fscanf(FP, "%d", &array[i]);
        }
        // closes the file
        fclose(FP);

        // allocates the root with index 0 of array
        root = insert(root, array[0]);

        // temp val for looping
        int temp = 1;

        // while temp is less than the size of the array
        while (temp < count)
        {
            // inserts each value of array into the tree
            insert(root, array[temp]);
            temp++;
        }

        // user input options
        printf("\nPick a operation: ");
        printf("\n1: Search for an integer and remove it and everything below it");
        printf("\n2: Print out tree in breadth first traversal");
        printf("\n3: Print out tree in depth first traversal");
        printf("\n4: Exit program");

        // infinite while loop until user exits
        while (1)
        {
            printf("\nEnter a command: ");
            scanf("%d", &choice);

            switch (choice)
            {

            // did not have enough time to finish delete function
            // does return the level that the value is on though
            case 1:
                printf("\nEnter the element you want to search for: ");
                scanf("%d", &val);
                printf("\nThe value %d is in level %d\n", val, getLevel(root, val));
                break;

            // prints out tree in breadth first
            case 2:
                breadthFirst(root);
                break;

            // prints out tree in depth first
            case 3:
                depthFirst(root);
                break;

            // exits loop and program
            case 4:
                exit(0);
            }
        }
    }
    // if file could not pass, then the program aborts
    else
        abort();
}
